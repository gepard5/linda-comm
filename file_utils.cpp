#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdexcept>
#include "file_utils.h"
#include "file_exceptions.h"

namespace file_utils {

    std::string readIn(int fd, int offset, size_t len) {
        lseek(fd, offset, SEEK_SET);
        char buffer[len + 1];
        if (read(fd, buffer, len) == -1) {
            throw ReadingException();
        }
        return std::string(buffer);
    }

    void writeIn(int fd, int offset, const char* data, size_t len) {
        lseek(fd, offset, SEEK_SET);
        if (write(fd, data, len) == -1) {
            throw WritingException();
        }
        lseek(fd, 0, (int) len);
    }

    void setLock(int fd, short lockType, int block, int blockSize) {
        flock fl = {};
        fl.l_type = lockType;
        fl.l_whence = SEEK_SET;
        fl.l_start = block * blockSize;
        fl.l_len = blockSize;
        int result = fcntl(fd, F_SETLKW, &fl);
        if (result == -1) {
            if (errno == EACCES || errno == EAGAIN) {
                throw BlockAlreadyLockedException();
            }
            throw UnexpectedException();
        }
    }

    bool exists(std::string filePath) {
        return access(filePath.c_str(), F_OK) != -1;
    }
}