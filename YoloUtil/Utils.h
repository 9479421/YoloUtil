#pragma once
#include<list>
#include<time.h>
#include<fstream>

#include<vector>


struct XSleep_Structure
{
	int duration;
	HANDLE eventHandle;
};

struct File
{
	CString fileName;
	CString filePath;
	
	File(CString fileName, CString filePath) {
		this->fileName = fileName;
		this->filePath = filePath;
	}
};
class Utils
{
public:
	static void XSleep(int nWaitInMsecs);
	static HBITMAP WindowCapture(HWND hwnd = NULL);
	static HBITMAP WindowCapture_Front(HWND hwnd);
	static void saveBitMap(CString path, HBITMAP bitmap);
	static HBITMAP stretchBitMap(HBITMAP bitmap,int width,int height);
	static CString selectFolder();


	static std::string selectFile();

	static CString readFile(CString path);
	static void writeFile(CString path,CString content);
	static std::list<File> enumFiles(CString path);
	static std::list<CString> splitStr(CString str,CString splitStr);
	static std::vector<std::string> splitStr(std::string str, std::string splitStr);
	static void copyFile(CString filePath,CString distPath);
	static CString getTimeStamp(bool isMilliSecond = FALSE);

	static std::string ReplaceAllText(std::string str, std::string a, std::string b);
	static int getTextTimes(std::string str, std::string flag);
	static std::string trim(const std::string& str);

};





