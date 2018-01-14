#include <fcntl.h>
#include <signal.h>
#include <cstring>
#include <algorithm>
#include <unistd.h>
#include <thread>
#include <future>
#include <random>
#include <chrono>

#include <iostream>

#include "file_manager.h"
#include "file_exceptions.h"

std::string FileManager::getCurrentLine() const {
	return lineCache[currentLineInBlock];
}

bool FileManager::goToNextLine(int timeout, bool loop) {
    if (lineCache.empty() || currentLineInBlock+1 == LINES_IN_BLOCK ) {
        if (!loadLinesToCache(loop, timeout)) {
            return false;
        }
    }
	else {
		++currentLineInBlock;
	}

	return true;
}

struct ReadLine FileManager::getNextLine(int timeout, bool loop) {
	if( !goToNextLine( timeout, loop ) )
            return {false, std::string()};

	std::cout<<"Getting next line: "<<currentLineInBlock<<std::endl;
	std::cout<<"In block: "<<currentBlock<<std::endl;

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

    while ( !isEmpty( loadCurrentLine() ) ) {
        unlock(-1);
        findEmptyLine();
        lockExclusive(-1);
    }

    int lineOffset = currentBlock * BLOCK_SIZE + currentLineInBlock * LINE_SIZE;
    writeIn(lineOffset, line.c_str());
    unlock(-1);
}

void FileManager::setFile(const std::string &filepath) {

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
		std::cout<<"Iterating over"<<std::endl;
		std::cout<<"New blocks array: "<<std::endl;	
		for( auto b : nextBlocks )
			std::cout<<"Block : "<<b<<std::endl;
    }
    currentBlock = (int) nextBlocks.back();
    nextBlocks.pop_back();

    if (!lockShared(timeout)) {
        return false;
    }

	lineCache.clear();
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
    writeIn(lineOffset, EMPTY_LINE.c_str());
}


void FileManager::findEmptyLine() {
    while ( !isEmpty(getNextLine(-1, true).line ) ) {}
	std::cout<<"Finding next line: "<<currentLineInBlock<<std::endl;
	std::cout<<"In block: "<<currentBlock<<std::endl;
}

bool FileManager::isEmpty( const std::string& s ) const {
	return s.front() == '\0';
}

void FileManager::fillFileWithEmptyLines() {
    for ( currentBlock = 0; currentBlock < BLOCKS_IN_FILE; ++currentBlock) {
		lockExclusive(-1);
		for (currentLineInBlock = 0; currentLineInBlock < LINES_IN_BLOCK; ++currentLineInBlock) {
            int offset = currentBlock * BLOCK_SIZE + currentLineInBlock * LINE_SIZE;
			writeIn(offset, EMPTY_LINE.c_str());
		}
		unlock(-1);
    }
	currentBlock = 0;
    currentLineInBlock = 0;
}

void FileManager::fillNextBlocksArray() {
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

	nextBlocks.clear();
    for (size_t i = 0; i < BLOCKS_IN_FILE; ++i) {
        nextBlocks.push_back(i);
    }
    std::shuffle(nextBlocks.begin(), nextBlocks.end(), std::default_random_engine(seed));
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
