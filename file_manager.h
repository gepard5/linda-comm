#ifndef __FILE_MANAGER__
#define __FILE_MANAGER__

#include <string>
#include <vector>

struct ReadLine {
	bool success;
	std::string line;
};

class FileManager {
public:
	FileManager() : EMPTY_LINE( LINE_SIZE, char('\0') ) {}
    ReadLine getNextLine(int timeout = -1, bool loop = false);
    std::vector<std::string> getAllLines();
    bool deleteLine(const std::string &line, int timeout = -1);
    void writeLine(const std::string &line);
    void setFile(const std::string &filepath);
    void clear();
	bool goToNextLine(int timeout, bool loop);
    std::string getCurrentLine() const;
	bool isEmpty( const std::string& s ) const;

private:
    const static int BLOCKS_IN_FILE = 10;
    const static int LINES_IN_BLOCK = 5;
    const static int LINE_SIZE = 512;
    const static int BLOCK_SIZE = LINES_IN_BLOCK * LINE_SIZE;
    const std::string EMPTY_LINE;
    int fd;
    int currentLineInBlock;
    int currentBlock;

    std::vector<size_t> nextBlocks = {};
    std::vector<std::string> lineCache = {};
    bool lockShared(int timeout);
    bool lockExclusive(int timeout);
    bool unlock(int timeout);
    bool loadLinesToCache(bool loop, int timeout);
    void deleteCurrentLine();
    std::string loadCurrentLine();
    void findEmptyLine();
    void fillFileWithEmptyLines();
    void fillNextBlocksArray();
    std::string readIn(int offset);
    void writeIn(int offset, const char* data);
    bool setLock(short lockType, int timeout);
    bool exists(std::string filePath);
};

#endif // file_manager
