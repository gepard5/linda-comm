#include <fcntl.h>
#include <cstring>
#include <algorithm>
#include "file_manager.h"
#include "file_utils.h"
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
    findEmptyLine();
    lockExclusive();
    while (loadCurrentLine() != std::string(EMPTY_LINE)) {
        unlock();
        findEmptyLine();
        lockExclusive();
    }
    int lineOffset = currentBlock * BLOCK_SIZE + currentLineInBlock * LINE_SIZE;
    file_utils::writeIn(fd, lineOffset, line.c_str(), LINE_SIZE);
    unlock();
}

void FileManager::setFile(const std::string &filepath) {
    if (file_utils::exists(filepath)) {
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
    return file_utils::readIn(fd, lineOffset, LINE_SIZE);
}

void FileManager::deleteCurrentLine() {
    int lineOffset = currentBlock * BLOCK_SIZE + currentLineInBlock * LINE_SIZE;
    file_utils::writeIn(fd, lineOffset, EMPTY_LINE, LINE_SIZE);
}


void FileManager::findEmptyLine() {
    while (getNextLine(-1, true) != std::string(EMPTY_LINE)) {}
}

void FileManager::fillFileWithEmptyLines() {
    for ( currentBlock = 0; currentBlock < BLOCKS_IN_FILE; ++currentBlock) {
		lockExclusive();
		for (currentLineInBlock = 0; currentLineInBlock < LINES_IN_BLOCK; ++currentLineInBlock) {
            int offset = currentBlock * BLOCK_SIZE + currentLineInBlock * LINE_SIZE;
			file_utils::writeIn(fd, offset, EMPTY_LINE, LINE_SIZE);
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
    file_utils::setLock(fd, F_RDLCK, currentBlock, BLOCK_SIZE);
}

void FileManager::lockExclusive() {
    file_utils::setLock(fd, F_WRLCK, currentBlock, BLOCK_SIZE);
}

void FileManager::unlock() {
    file_utils::setLock(fd, F_UNLCK, currentBlock, BLOCK_SIZE);
}
