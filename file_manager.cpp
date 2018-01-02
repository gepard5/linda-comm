#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include "file_manager.h"

std::string FileManager::getNextLine(int timeout, bool loop) {
    if (lineCache.empty() || ++currentLineInBlock >= LINES_IN_BLOCK) {
        loadLinesToCache();
    }
    return lineCache[currentLineInBlock];
}

bool FileManager::deleteLine(const std::string &line, int timeout) {
    lockExclusive();
    std::string currentLine = loadCurrentLine();
    if (line.compare(currentLine) != 0) {
        unlock();
        return false;
    }
    unlock();
    deleteCurrentLine();
    return true;
}

void FileManager::writeLine(const std::string &line) {
    findEmptyLine();
    lockExclusive();
    if (loadCurrentLine().compare(std::string(EMPTY_LINE)) != 0) {
        unlock();
        findEmptyLine();
        lockExclusive();
    }
    int lineOffset = currentBlock * BLOCK_SIZE + currentLineInBlock * LINE_SIZE;
    lseek(fd, lineOffset, SEEK_SET);
    write(fd, line.c_str(), LINE_SIZE);
    lseek(fd, 0, SEEK_SET);
    unlock();
}

void FileManager::setFile(const std::string &filepath) {
    if (access(filepath.c_str(), F_OK) != -1) {
        fd = open(filepath.c_str(), O_RDWR);
    } else {
        creat(filepath.c_str(), S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
        fd = open(filepath.c_str(), O_RDWR);
        fillFileWithEmptyLines();
    }
}

void FileManager::loadLinesToCache() {
    currentBlock++;
    lockShared();
    char buffer[LINE_SIZE + 1];
    lseek(fd, currentBlock * BLOCK_SIZE, SEEK_SET);
    for (int i = 0; i < LINES_IN_BLOCK; i++) {
        read(fd, buffer, LINE_SIZE);
        lineCache.push_back(std::string(buffer));
    }
    unlock();
    currentLineInBlock = 0;
}

std::string FileManager::loadCurrentLine() {
    char buffer[LINE_SIZE + 1];
    int lineOffset = currentBlock * BLOCK_SIZE + currentLineInBlock * LINE_SIZE;
    lseek(fd, lineOffset, SEEK_SET);
    read(fd, buffer, LINE_SIZE);
    return std::string(buffer);
}

void FileManager::deleteCurrentLine() {

    int lineOffset = currentBlock * BLOCK_SIZE + currentLineInBlock * LINE_SIZE;
    lseek(fd, lineOffset, SEEK_SET);
    write(fd, EMPTY_LINE, LINE_SIZE);
}


void FileManager::findEmptyLine() {
    while (getNextLine(-1, true).compare(std::string(EMPTY_LINE)) != 0) {
    }
}

void FileManager::fillFileWithEmptyLines() {
    lockExclusive();
    for (int i = 0; i < BLOCKS_IN_FILE * LINES_IN_BLOCK; i++) {
        write(fd, EMPTY_LINE, LINE_SIZE);
    }
    unlock();
}

void FileManager::lockShared() {
    flock fl = {};
    fl.l_type = F_RDLCK;
    fl.l_whence = SEEK_SET;
    fl.l_start = currentBlock * BLOCK_SIZE;
    fl.l_len = BLOCK_SIZE;
    fcntl(fd, F_SETLKW, &fl);
}

void FileManager::lockExclusive() {
    flock fl = {};
    fl.l_type = F_WRLCK;
    fl.l_whence = SEEK_SET;
    fl.l_start = currentBlock * BLOCK_SIZE;
    fl.l_len = BLOCK_SIZE;
    fcntl(fd, F_SETLKW, &fl);
}

void FileManager::unlock() {
    flock fl = {};
    fl.l_type = F_UNLCK;
    fl.l_whence = SEEK_SET;
    fl.l_start = currentBlock * BLOCK_SIZE;
    fl.l_len = LINE_SIZE;
    fcntl(fd, F_SETLKW, &fl);
}
