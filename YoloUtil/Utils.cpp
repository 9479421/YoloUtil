﻿#include "pch.h"
#include "Utils.h"


//
// Function  : XSleepThread()
// Purpose   : The thread which will sleep for the given duration
// Returns   : DWORD WINAPI
// Parameters:       
//  1. pWaitTime -
//
DWORD WINAPI XSleepThread(LPVOID pWaitTime)
{
	XSleep_Structure* sleep = (XSleep_Structure*)pWaitTime;

	Sleep(sleep->duration);
	SetEvent(sleep->eventHandle);

	return 0;
}

void Utils::XSleep(int nWaitInMsecs)
{

	XSleep_Structure sleep;
	sleep.duration = nWaitInMsecs;
	sleep.eventHandle = CreateEvent(NULL, TRUE, FALSE, NULL);

	/*DWORD dwThreadId;
	CreateThread(NULL, 0, &XSleepThread, &sleep, 0, &dwThreadId);*/

	HANDLE getHandle;
	getHandle = (HANDLE)_beginthreadex(NULL, 0,
		(unsigned(__stdcall*)(void*))XSleepThread, &sleep, 0, NULL);

	MSG msg;
	while (::WaitForSingleObject(sleep.eventHandle, 0) == WAIT_TIMEOUT)
	{
		// get and dispatch message
		if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
	}
	CloseHandle(sleep.eventHandle);
	CloseHandle(getHandle);

}



HBITMAP Utils::WindowCapture(HWND hwnd)
{
	if (hwnd == NULL) //截图屏幕
	{
		hwnd = ::GetDesktopWindow();
	}

	RECT rect;
	GetWindowRect(hwnd, &rect);

	HDC hdc = GetWindowDC(hwnd);
	HDC memDC;
	memDC = ::CreateCompatibleDC(hdc);
	HBITMAP memBitmap;
	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;
	memBitmap = ::CreateCompatibleBitmap(hdc, width, height);
	SelectObject(memDC, memBitmap);
	BitBlt(memDC, 0, 0, width, height, hdc, 0, 0, SRCCOPY);

	//删除创建的兼容内存DC
	DeleteDC(memDC);
	//释放窗口DC
	ReleaseDC(hwnd, hdc);

	return memBitmap;
}

HBITMAP Utils::WindowCapture_Front(HWND hwnd)
{
	RECT rect;
	GetWindowRect(hwnd, &rect);

	//父窗口置顶 为截图做准备
	::SetWindowPos((hwnd), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	::SetWindowPos(::GetParent(hwnd), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

	HDC hdc = GetWindowDC(::GetDesktopWindow());
	HDC memDC;
	memDC = ::CreateCompatibleDC(hdc);
	HBITMAP memBitmap;
	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;
	memBitmap = ::CreateCompatibleBitmap(hdc, width, height);
	SelectObject(memDC, memBitmap);
	BitBlt(memDC, 0, 0, width, height, hdc, rect.left, rect.top, SRCCOPY);

	////删除创建的兼容内存DC
	DeleteDC(memDC);
	////释放窗口DC
	ReleaseDC(hwnd, hdc);

	return memBitmap;
}

void Utils::saveBitMap(CString path, HBITMAP bitmap)
{
	CImage image;
	image.Attach(bitmap);
	image.Save(path);
	image.Detach();

	DeleteObject(bitmap);
}

HBITMAP Utils::stretchBitMap(HBITMAP bitmap, int width, int height)
{
	CImage image;
	image.Attach(bitmap);

	CImage imageNew;
	imageNew.Create(width, height, 32);
	HDC dcNew = imageNew.GetDC();
	SetStretchBltMode(dcNew, HALFTONE);
	image.StretchBlt(dcNew, 0, 0, width, height);//将image输出到imageNew的DC上
	image.Detach();
	imageNew.ReleaseDC();


	//获取imageNew位图
	HBITMAP hBmp = imageNew.Detach();

	//DeleteDC(dcNew);
	return hBmp;
}

CString Utils::selectFolder()
{
	LPITEMIDLIST lpitemidlist;
	SHGetFolderLocation(NULL, CSIDL_DESKTOP, NULL, NULL, &lpitemidlist);
	if (lpitemidlist == NULL)
		return CString("");

	//配置对话框
	BROWSEINFO browseinfo;
	ZeroMemory(&browseinfo, sizeof(browseinfo));
	browseinfo.pidlRoot = lpitemidlist;
	browseinfo.lpszTitle = _T("选择目录的路径");
	browseinfo.ulFlags = BIF_EDITBOX | BIF_RETURNONLYFSDIRS;

	//打开对话框
	LPITEMIDLIST obj = SHBrowseForFolder(&browseinfo);
	if (obj != NULL)
	{
		TCHAR szPath[MAX_PATH];
		SHGetPathFromIDList(obj, szPath);
		return CString(szPath);
	}
	return CString("");
}

std::string Utils::selectFile()
{
	OPENFILENAME ofn;       // common dialog box structure
	wchar_t szFile[260];       // buffer for file name

	// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;  // if null, must set lpszTitle to a non-empty string
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = L"All Files\0*.*\0Text Files\0*.txt\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;  // use NULL to use the current directory
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	// Display the Open dialog box. 
	if (GetOpenFileNameW(&ofn) == TRUE) {
		// File selected
		return CW2A(ofn.lpstrFile);
	}
	return std::string();

}

CString Utils::readFile(CString path)
{
	FILE* filePtr = fopen(CW2A(path), "rb");
	CString sb;
	char buffer[1024];
	while (!feof(filePtr))
	{
		memset(buffer, 0, sizeof(buffer));
		fread(buffer, sizeof(char), sizeof(buffer) - 1, filePtr);
		/*printf("%s", buffer);*/
		sb += CA2W(buffer);
	}
	fclose(filePtr);  // 关闭文件
	return sb;
}

void Utils::writeFile(CString path, CString content)
{
	FILE* filePtr = fopen(CW2A(path), "wb");
	if (filePtr == NULL)
	{
		MessageBoxA(NULL, "写入文件失败", "信息", 0);
	}
	fwrite(CW2A(content), sizeof(char), strlen(CW2A(content)), filePtr);
	fclose(filePtr);  // 关闭文件
}


std::list<File> Utils::enumFiles(CString path)
{
	std::list<File> fileList;

	WIN32_FIND_DATA ListFile;
	HANDLE hListFile = FindFirstFile(path + "\\*", &ListFile);
	if (hListFile != INVALID_HANDLE_VALUE) {
		do
		{
			if (lstrcmp(ListFile.cFileName, L"..") == 0 || lstrcmp(ListFile.cFileName, L".") == 0)
				continue;

			if (!(ListFile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				//文件
				//printf("%ls\n", ListFile.cFileName);
				fileList.push_back(File(ListFile.cFileName, path));
			}
			else
			{
				//目录
			}
		} while (FindNextFile(hListFile, &ListFile));
	}
	return fileList;
}

std::list<CString> Utils::splitStr(CString str, CString splitStr)
{
	std::list<CString> strList;
	int idx = 0;
	int position = 0;
	do
	{
		position = str.Find(splitStr, idx);
		if (position == -1)
		{
			CString tmp = str.Mid(idx);
			if (tmp.GetLength() > 0) //结尾的\r\n不予计入空文本
			{
				strList.push_back(tmp);
			}
			break;
		}
		else {
			CString tmp = str.Mid(idx, position - idx);
			strList.push_back(tmp);
			idx = position + splitStr.GetLength();
		}
	} while (true);
	return strList;
}

std::vector<std::string> Utils::splitStr(std::string str, std::string splitStr)
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






void Utils::copyFile(CString filePath, CString distPath)
{
	system(CW2A(L"copy " + filePath + L" " + distPath));
}

CString Utils::getTimeStamp(bool isMilliSecond)
{
	CString timestampStr;
	time_t timestamp = time(NULL);
	if (isMilliSecond)
	{
		timestampStr.Format(L"%ld000", timestamp); //13位暂时不精确，用到的时候再改进
	}
	else {
		timestampStr.Format(L"%ld", timestamp); //10位时间戳
	}
	return timestampStr;
}



std::string Utils::ReplaceAllText(std::string str, std::string a, std::string b)
{
	int oldPos = 0;
	while (str.find(a, oldPos) != -1)//在未被替换的文本中寻找目标文本
	{
		int start = str.find(a, oldPos);//找到目标文本的起始下标

		str.replace(start, a.size(), b);
		//从str[start]开始到str[a.size()]替换为b
		//str[start]到str[a.size()]也就是a所在得片段

		oldPos = start + b.size();//记录未替换文本的起始下标
	}
	return str;
}

int Utils::getTextTimes(std::string str, std::string flag)
{
	int times = 0;

	int idx = 0;
	while (true)
	{
		idx = str.find(flag, idx);
		if (idx == std::string::npos)
		{
			return times;
		}
		else {
			times++;
			idx += flag.size();
		}
	}
}

std::string Utils::trim(const std::string& str) {
	// 找到第一个不是空格的字符的位置
	auto start = str.find_first_not_of(' ');
	// 如果字符串全是空格，返回一个空字符串
	if (start == std::string::npos)
		return "";

	// 找到最后一个不是空格的字符的位置
	auto end = str.find_last_not_of(' ');

	// 返回从开始到结束位置的子字符串
	return str.substr(start, end - start + 1);
}
