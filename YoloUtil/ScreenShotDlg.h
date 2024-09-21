#pragma once
#include "afxdialogex.h"


// ScreenShotDlg 对话框

class ScreenShotDlg : public CDialogEx
{
	DECLARE_DYNAMIC(ScreenShotDlg)

public:
	ScreenShotDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~ScreenShotDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SCREENSHOT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonGethandle();
	CButton m_gethandle;
	CEdit m_handle;
	CEdit m_times;
	CButton m_winshot;
	CButton m_rectshot;
	afx_msg void OnBnClickedButtonShot();
	CStatic m_picshow;
	CEdit m_savedir;
	afx_msg void OnBnClickedButtonSetsavedir();
};
