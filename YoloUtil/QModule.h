#pragma once
#include<vector>
#include<string>
#include<Windows.h>



class QFile
{
private:
	std::string fileName;
	std::string filePath;
	std::string fileFormat;

	std::string fullName;
public:
	QFile(std::string fileName, std::string filePath, std::string fileFormat, std::string fullName) {
		this->fileName = fileName;
		this->filePath = filePath;
		this->fileFormat = fileFormat;
		this->fullName = fullName;
	}

	std::string getFileName() {
		return fileName;
	}
	std::string getFilePath() {
		return filePath;
	}
	std::string getFileFormat() {
		return fileFormat;
	}
	std::string getFullName() {
		return fullName;
	}

};

class QModule
{
public:
	static std::vector<std::string> splitStr(std::string str, std::string splitStr);
	static std::vector<QFile> enumFile(std::string dir);
	static std::string readFile(std::string fullPath);
	static bool writeFile(std::string fullPath, std::string content);
};

