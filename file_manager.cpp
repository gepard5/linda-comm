#include <fcntl.h>
#include <signal.h>
#include <cstring>
#include <algorithm>
#include <unistd.h>
#include <thread>
#include <future>
#include <iostream> //delete this

#include "file_manager.h"
#include "file_exceptions.h"

struct ReadLine FileManager::getNextLine(int timeout, bool loop) {
	if(currentLineInBlock <= LINES_IN_BLOCK) ++currentLineInBlock;
    if (lineCache.empty() || currentLineInBlock >= LINES_IN_BLOCK) {
        if (!loadLinesToCache(loop, timeout)) {
            return {false, std::string()};
        }
    }
	std::cout<<"GetNextLine: "<<currentLineInBlock<<std::endl;
	std::cout<<"line: "<<lineCache[currentLineInBlock]<<std::endl;
    return {true, lineCache[currentLineInBlock]};
}

std::vector<std::string> FileManager::getAllLines() {
    auto currentBlockOriginal = currentBlock;
    auto currentLineInBlockOriginal = currentLineInBlock;
    std::vector<std::string> allLines = {};
    for (currentBlock = 0; currentBlock < BLOCKS_IN_FILE; currentBlock++) {
		lockShared(-1);
        for (currentLineInBlock = 0; currentLineInBlock < LINES_IN_BLOCK; currentLineInBlock++) {
            std::string line = readIn(currentBlock * BLOCK_SIZE + currentLineInBlock * LINE_SIZE);
            allLines.push_back(line);
        }
		unlock(-1);
    }
    currentBlock = currentBlockOriginal;
    currentLineInBlock = currentLineInBlockOriginal;
    return allLines;
}

bool FileManager::deleteLine(const std::string &line, int timeout) {
    if (!lockExclusive(timeout)) {
        return false;
    }
    std::string currentLine = loadCurrentLine();
    if (line != currentLine) {
        unlock(-1);
        return false;
    }
    deleteCurrentLine();
    return unlock(-1);
}

void FileManager::writeLine(const std::string &line) {
    if (line.length() > LINE_SIZE) {
        throw TooLongLineException(LINE_SIZE);
    }
    findEmptyLine();
    lockExclusive(-1);
    while (loadCurrentLine() != std::string(EMPTY_LINE)) {
        unlock(-1);
        findEmptyLine();
        lockExclusive(-1);
    }
    int lineOffset = currentBlock * BLOCK_SIZE + currentLineInBlock * LINE_SIZE;
	std::cout<<"Current block: "<<currentBlock<<std::endl;
	std::cout<<"Current line: "<<currentLineInBlock<<std::endl;
    writeIn(lineOffset, line.c_str());
    unlock(-1);
}

void FileManager::setFile(const std::string &filepath) {
	//std::srand(std::time(0));

    if (exists(filepath)) {
        fd = open(filepath.c_str(), O_RDWR);
    } else {
        creat(filepath.c_str(), S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
        fd = open(filepath.c_str(), O_RDWR);
        fillFileWithEmptyLines();
    }
	clear();
}

void FileManager::clear()
{
    lineCache.clear();
    fillNextBlocksArray();
}

bool FileManager::loadLinesToCache(bool loop, int timeout) {
    if (nextBlocks.empty()) {
        if (!loop) {
            throw EndOfFileException();
        }
        fillNextBlocksArray();
    }
    currentBlock = (int) nextBlocks.back();
    nextBlocks.pop_back();

    if (!lockShared(timeout)) {
        return false;
    }
    for (currentLineInBlock = 0; currentLineInBlock < LINES_IN_BLOCK; ++currentLineInBlock) {
        lineCache.push_back( loadCurrentLine() );
    }
    unlock(-1);
    currentLineInBlock = 0;
    return true;
}

std::string FileManager::loadCurrentLine() {
    int lineOffset = currentBlock * BLOCK_SIZE + currentLineInBlock * LINE_SIZE;
    return readIn(lineOffset);
}

void FileManager::deleteCurrentLine() {
    int lineOffset = currentBlock * BLOCK_SIZE + currentLineInBlock * LINE_SIZE;
    writeIn(lineOffset, EMPTY_LINE);
}


void FileManager::findEmptyLine() {
    while (getNextLine(-1, true).line != std::string(EMPTY_LINE)) {}
}

void FileManager::fillFileWithEmptyLines() {
    for ( currentBlock = 0; currentBlock < BLOCKS_IN_FILE; ++currentBlock) {
		lockExclusive(-1);
		for (currentLineInBlock = 0; currentLineInBlock < LINES_IN_BLOCK; ++currentLineInBlock) {
            int offset = currentBlock * BLOCK_SIZE + currentLineInBlock * LINE_SIZE;
			writeIn(offset, EMPTY_LINE);
		}
		unlock(-1);
    }
	currentBlock = 0;
    currentLineInBlock = 0;
}

void FileManager::fillNextBlocksArray() {
	nextBlocks.clear();
    for (size_t i = 1; i < BLOCKS_IN_FILE; ++i) {
        nextBlocks.push_back(i);
    }
    std::random_shuffle(nextBlocks.begin(), nextBlocks.end());
}

bool FileManager::lockShared(int timeout) {
    return setLock(F_RDLCK, timeout);
}

bool FileManager::lockExclusive(int timeout) {
    return setLock(F_WRLCK, timeout);
}

bool FileManager::unlock(int timeout) {
    return setLock(F_UNLCK, timeout);
}

std::string FileManager::readIn(int offset) {
	lseek(fd, offset, SEEK_SET);
	char buffer[LINE_SIZE + 1];
	if (read(fd, buffer, LINE_SIZE) == -1) {
		throw ReadingException();
	}
	return std::string(buffer);
}

void FileManager::writeIn(int offset, const char* data) {
	lseek(fd, offset, SEEK_SET);
	if (write(fd, data, LINE_SIZE) == -1) {
		throw WritingException();
	}
	lseek(fd, 0, LINE_SIZE);
}

bool FileManager::setLock(short lockType, int timeout) {
	std::promise<int> file_promise;
	std::future<int> file_future = file_promise.get_future();
	auto locker = [=](std::promise<int> p) {
		flock fl = {};
		fl.l_type = lockType;
		fl.l_whence = SEEK_SET;
		fl.l_start = currentBlock * BLOCK_SIZE;
		fl.l_len = BLOCK_SIZE;
		int result = fcntl(fd, F_SETLKW, &fl);

		p.set_value(result);
	};

	std::thread file_locker( locker, std::move(file_promise) );
	if( timeout >= 0 ) {
		auto status = file_future.wait_for( std::chrono::milliseconds(timeout) );
		if( status == std::future_status::timeout )
			pthread_kill( file_locker.native_handle(), SIGUSR1 );
	}

	int result = file_future.get();
	file_locker.join();
	if (result == -1) {
		if (errno == EINTR) {
			return false; //timeout
		}

		if (errno == EACCES || errno == EAGAIN) {
			throw BlockAlreadyLockedException();
		}
		throw UnexpectedException();
	}

	return true;
}

bool FileManager::exists(std::string filePath) {
	return access(filePath.c_str(), F_OK) != -1;
}
