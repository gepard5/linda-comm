#include <fcntl.h>
#include <cstring>
#include <algorithm>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>

#include <thread>
#include <chrono>
#include <future>
#include <functional>

#include "file_manager.h"
#include "file_exceptions.h"

std::string FileManager::getNextLine(int timeout, bool loop) {
    if (lineCache.empty() || currentLineInBlock >= LINES_IN_BLOCK) {
        loadLinesToCache(loop);
    }
    return lineCache[currentLineInBlock++];
}

bool FileManager::deleteLine(const std::string &line, int timeout) {
    lockExclusive();
    std::string currentLine = loadCurrentLine();
    if (line != currentLine) {
        unlock();
        return false;
    }
    deleteCurrentLine();
    unlock();
    return true;
}

void FileManager::writeLine(const std::string &line) {
    if (line.length() > LINE_SIZE) {
        throw TooLongLineException(LINE_SIZE);
    }
    findEmptyLine();
    lockExclusive();
    while (loadCurrentLine() != std::string(EMPTY_LINE)) {
        unlock();
        findEmptyLine();
        lockExclusive();
    }
    int lineOffset = currentBlock * BLOCK_SIZE + currentLineInBlock * LINE_SIZE;
    writeIn(lineOffset, line.c_str());
    unlock();
}

void FileManager::setFile(const std::string &filepath) {
    if (exists(filepath)) {
        fd = open(filepath.c_str(), O_RDWR);
    } else {
        creat(filepath.c_str(), S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
        fd = open(filepath.c_str(), O_RDWR);
        fillFileWithEmptyLines();
    }
    fillNextBlocksArray();
}

void FileManager::loadLinesToCache(bool loop) {
    if (nextBlocks.empty()) {
        if (!loop) {
            throw EndOfFileException();
        }
        fillNextBlocksArray();
    }
    currentBlock = (int) nextBlocks.back();
    nextBlocks.pop_back();

    lockShared();
    for (currentLineInBlock = 0; currentLineInBlock < LINES_IN_BLOCK; ++currentLineInBlock) {
        lineCache.push_back( loadCurrentLine() );
    }
    unlock();
    currentLineInBlock = 0;
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
    while (getNextLine(-1, true) != std::string(EMPTY_LINE)) {}
}

void FileManager::fillFileWithEmptyLines() {
    for ( currentBlock = 0; currentBlock < BLOCKS_IN_FILE; ++currentBlock) {
		lockExclusive();
		for (currentLineInBlock = 0; currentLineInBlock < LINES_IN_BLOCK; ++currentLineInBlock) {
            int offset = currentBlock * BLOCK_SIZE + currentLineInBlock * LINE_SIZE;
			writeIn(offset, EMPTY_LINE);
		}
		unlock();
    }
	currentBlock = 0;
    currentLineInBlock = 0;
}

void FileManager::fillNextBlocksArray() {
    for (size_t i = 1; i < BLOCKS_IN_FILE; ++i) {
        nextBlocks.push_back(i);
    }
    std::random_shuffle(nextBlocks.begin(), nextBlocks.end());
}

void FileManager::lockShared() {
    setLock(F_RDLCK,1);
}

void FileManager::lockExclusive() {
    setLock(F_WRLCK,1);
}

void FileManager::unlock() {
    setLock(F_UNLCK, 1);
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
