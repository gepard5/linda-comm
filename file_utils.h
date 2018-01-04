#ifndef FILE_UTILS
#define FILE_UTILS

#include <string>

namespace file_utils {
    std::string readIn(int fd, int offset, size_t len);
    void writeIn(int fd, int offset, const char* data, size_t len);
    void setLock(int fd, short lockType, int block, int blockSize);
    bool exists(std::string filePath);
}

#endif
