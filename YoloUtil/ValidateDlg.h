#pragma once
#include "afxdialogex.h"


// ValidateDlg 对话框

class ValidateDlg : public CDialogEx
{
	DECLARE_DYNAMIC(ValidateDlg)

public:
	ValidateDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~ValidateDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_VALIDATE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButtonDetect();
	CEdit m_onnx;
	CEdit m_picdir;
};
