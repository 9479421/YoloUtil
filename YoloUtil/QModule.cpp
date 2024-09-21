#include "QModule.h"


std::vector<std::string> QModule::splitStr(std::string str, std::string splitStr)
{
	std::vector<std::string> strList;
	int idx = 0;
	int position = 0;
	do
	{
		position = str.find(splitStr, idx);
		if (position == -1)
		{
			std::string tmp = str.substr(idx);
			if (tmp.length() > 0) //结尾的\r\n不予计入空文本
			{
				strList.push_back(tmp);
			}
			break;
		}
		else {
			std::string tmp = str.substr(idx, position - idx);
			strList.push_back(tmp);
			idx = position + splitStr.length();
		}
	} while (true);
	return strList;
}

std::vector<QFile> QModule::enumFile(std::string dir)
{
	std::vector<QFile> fileList;

	//去除尾部斜杠
	if (dir.find_last_of("\\") == dir.length())
	{
		dir = dir.substr(0, dir.length() - 1);
	}


	WIN32_FIND_DATAA ListFile;
	HANDLE hListFile = FindFirstFileA((dir + "\\*").c_str(), &ListFile);
	if (hListFile != INVALID_HANDLE_VALUE) {
		do
		{
			if (lstrcmpA(ListFile.cFileName, "..") == 0 || lstrcmpA(ListFile.cFileName, ".") == 0)
				continue;

			if (!(ListFile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				//文件
				std::string fileName = ListFile.cFileName;
				std::string fileFormat = fileName.substr(fileName.find_last_of(".") + 1);
				
				fileList.push_back(QFile(fileName, dir.c_str(), fileFormat,(dir  + "\\" + fileName).c_str()));
			}
			else
			{
				//目录
			}
		} while (FindNextFileA(hListFile, &ListFile));
	}
	return fileList;

}

std::string QModule::readFile(std::string fullPath)
{
	FILE* file = fopen(fullPath.c_str(), "r");
	if (file)
	{
		fseek(file, 0, SEEK_END);
		long size = ftell(file);
		fseek(file, 0, SEEK_SET);
		char* buffer = new char[size + 1];
		memset(buffer, 0, size + 1);
		fread(buffer, 1, size, file);
		fclose(file);
		std::string str(buffer);
		delete[] buffer;
		return str;
	}
	return "";
}

bool QModule::writeFile(std::string fullPath, std::string content)
{
	FILE* file = fopen(fullPath.c_str(), "w");
	if (file)
	{
		fwrite(content.c_str(), 1, content.length(), file);
		fclose(file);
		return true;
	}
	return true;
}
