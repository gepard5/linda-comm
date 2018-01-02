#ifndef FILE_MANAGER_EXCEPTIONS
#define FILE_MANAGER_EXCEPTIONS

#include <exception>
#include <stdexcept>

class EndOfFileException : public std::logic_error {
    public:
    EndOfFileException() :
            std::logic_error( "Reached end of file.") {}

};

#endif
