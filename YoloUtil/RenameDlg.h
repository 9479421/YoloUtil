#pragma once
#include "afxdialogex.h"


// RenameDlg 对话框

class RenameDlg : public CDialogEx
{
	DECLARE_DYNAMIC(RenameDlg)

public:
	RenameDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~RenameDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_RENAME };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonMultirename();
	afx_msg void OnBnClickedButtonSetrenamedir();
	CEdit m_renamedir;
	CEdit m_prefix;
};
