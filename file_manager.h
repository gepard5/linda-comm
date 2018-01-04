#ifndef __FILE_MANAGER__
#define __FILE_MANAGER__

#include <string>
#include <vector>

class FileManager {
public:
	std::string getNextLine(int timeout = -1, bool loop = false);
    std::vector<std::string> getAllLines();
	bool deleteLine(const std::string& line, int timeout = -1);
	void writeLine(const std::string& line);
	void setFile(const std::string& filepath);
private:
    const static int BLOCKS_IN_FILE = 512;
    const static int LINES_IN_BLOCK = 20;
    const static int LINE_SIZE = 512;
    const static int BLOCK_SIZE = LINES_IN_BLOCK * LINE_SIZE;
    const char EMPTY_LINE[LINE_SIZE] = { '\0' };
    int fd;
    int currentLineInBlock;
    int currentBlock;

	std::vector<size_t> nextBlocks = {};
    std::vector<std::string> lineCache = {};
    void lockShared();
    void lockExclusive();
    void unlock();
    void loadLinesToCache(bool b);
    std::string loadCurrentLine();
    void deleteCurrentLine();
    void findEmptyLine();
    void fillFileWithEmptyLines();

	void fillNextBlocksArray();

    void runTimer(int timeout);
};

#endif // file_manager
