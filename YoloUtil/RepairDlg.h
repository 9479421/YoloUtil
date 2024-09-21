#pragma once
#include "afxdialogex.h"


// RepairDlg 对话框

class RepairDlg : public CDialogEx
{
	DECLARE_DYNAMIC(RepairDlg)

public:
	RepairDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~RepairDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_REPAIR };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual void OnOK();
	virtual void OnCancel();

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_txtDir;
	afx_msg void OnEnChangeEditPicdir();
	CEdit m_old;
	CEdit m_new;
	afx_msg void OnBnClickedButtonDetect();
};
