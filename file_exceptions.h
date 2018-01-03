#ifndef FILE_EXCEPTIONS
#define FILE_EXCEPTIONS

#include <stdexcept>

class BlockAlreadyLockedException : public std::logic_error {
public:
    BlockAlreadyLockedException() :
            std::logic_error("This block is already locked by another process.") {}

};

class ReadingException : public std::logic_error {
public:
    ReadingException() :
            std::logic_error("Cannot read from file.") {}

};

class WritingException : public std::logic_error {
public:
    WritingException() :
            std::logic_error("Cannot write to file.") {}

};

class UnexpectedException : public std::logic_error {
public:
    UnexpectedException() :
            std::logic_error("Unexpected exception occurred.") {}

};

class EndOfFileException : public std::logic_error {
public:
    EndOfFileException() :
            std::logic_error("Reached end of file.") {}

};

#endif
