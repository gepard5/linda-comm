#ifndef __FILE_MANAGER__
#define __FILE_MANAGER__

class FileManager {
public:
	std::string getNextLine(int timeout = -1, bool loop = false);
	bool deleteLine(const std::string& line, int timeout = -1);
	void writeLine(const std::string& line);
	void setFile(const std::string& filepath);

};

#endif // file_manager
