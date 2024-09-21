// ScreenShotDlg.cpp: 实现文件
//

#include "pch.h"
#include "YoloUtil.h"
#include "afxdialogex.h"
#include "ScreenShotDlg.h"
#include "Utils.h"

// ScreenShotDlg 对话框

IMPLEMENT_DYNAMIC(ScreenShotDlg, CDialogEx)

ScreenShotDlg::ScreenShotDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_SCREENSHOT, pParent)
{
	
}

ScreenShotDlg::~ScreenShotDlg()
{
}

void ScreenShotDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_GETHANDLE, m_gethandle);
	DDX_Control(pDX, IDC_EDIT_HANDLE, m_handle);
	DDX_Control(pDX, IDC_EDIT_TIMES, m_times);
	DDX_Control(pDX, IDC_RADIO_WINSHOT, m_winshot);
	DDX_Control(pDX, IDC_RADIO_RECTSHOT, m_rectshot);
	DDX_Control(pDX, IDC_PICSHOW, m_picshow);
	DDX_Control(pDX, IDC_EDIT_SAVEDIR, m_savedir);
}

BOOL ScreenShotDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_times.SetWindowTextW(L"1");
	m_winshot.SetCheck(TRUE);

	m_savedir.SetWindowTextW(_T("C:\\Users\\Administrator\\Desktop"));
	return TRUE;
}

BEGIN_MESSAGE_MAP(ScreenShotDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_GETHANDLE, &ScreenShotDlg::OnBnClickedButtonGethandle)
	ON_BN_CLICKED(IDC_BUTTON_SHOT, &ScreenShotDlg::OnBnClickedButtonShot)
	ON_BN_CLICKED(IDC_BUTTON_SETSAVEDIR, &ScreenShotDlg::OnBnClickedButtonSetsavedir)
END_MESSAGE_MAP()


// ScreenShotDlg 消息处理程序



void ScreenShotDlg::OnBnClickedButtonGethandle()
{
	
	// TODO: 在此添加控件通知处理程序代码
	m_gethandle.EnableWindow(FALSE);
	Utils::XSleep(2000);

	POINT point;
	GetCursorPos(&point);
	CWnd *cwnd = WindowFromPoint(point);
	if (cwnd->m_hWnd == NULL || cwnd->m_hWnd == INVALID_HANDLE_VALUE) {
		printf("WindowFromPoint point=(%ld, %ld) -> hwnd=%p -> fail(%ld)\n", point.x, point.y, cwnd->m_hWnd, GetLastError());
	}
	else {
		printf("WindowFromPoint -> hwnd=%p\n", cwnd->m_hWnd);

		CString cstr;
		cstr.Format(_T("%d"), cwnd->m_hWnd);
		m_handle.SetWindowTextW(cstr);
	}
	m_gethandle.EnableWindow(TRUE);
	
}


void ScreenShotDlg::OnBnClickedButtonShot()
{
	// TODO: 在此添加控件通知处理程序代码

	//获取路径
	CString savedirStr;
	m_savedir.GetWindowTextW(savedirStr);
	CString fileNameStr;
	m_times.GetWindowTextW(fileNameStr);
	CString pathStr = savedirStr + "\\" + fileNameStr + L".png";

	//获取句柄
	HBITMAP bitmap;
	CString hwndEdit;
	m_handle.GetWindowTextW(hwndEdit);
	HWND hwnd = (HWND)(atoi((CW2A)hwndEdit));
	if (m_winshot.GetCheck()) {
		if (hwndEdit.IsEmpty())
		{
			bitmap = Utils::WindowCapture();
		}
		else {
			bitmap = Utils::WindowCapture(hwnd);
		}
		//将位图显示到Picture Control
		m_picshow.SetBitmap(Utils::stretchBitMap(bitmap,230,230));
		Utils::saveBitMap(pathStr, bitmap);
	}

	if (m_rectshot.GetCheck()) {
		HBITMAP bitmap;
		bitmap = Utils::WindowCapture_Front(hwnd);
		//将位图显示到Picture Control
		m_picshow.SetBitmap(Utils::stretchBitMap(bitmap, 230, 230));
		Utils::saveBitMap(pathStr, bitmap);
	}

	CString newTimes;
	newTimes.Format(_T("%d"), atoi(CW2A(fileNameStr)) + 1);
	m_times.SetWindowTextW(newTimes);
}


void ScreenShotDlg::OnBnClickedButtonSetsavedir()
{
	// TODO: 在此添加控件通知处理程序代码
	// 创建文件对话框对象并设置属性
	CFileDialog fileDialog(TRUE, NULL, NULL, OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST, _T("所有文件 (*.*)"), NULL);

	CString str(Utils::selectFolder());
	printf("选择的目录为:%ls", str);
	m_savedir.SetWindowTextW(str);
}
