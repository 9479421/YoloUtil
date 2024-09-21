#pragma once
#include "afxdialogex.h"


// DetectDlg 对话框

class DetectDlg : public CDialogEx
{
	DECLARE_DYNAMIC(DetectDlg)

public:
	DetectDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~DetectDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_DETECT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_onnx;
	CEdit m_picdir;
	afx_msg void OnBnClickedButtonDetect();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
};
