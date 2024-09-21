#pragma once
#include "afxdialogex.h"


// TrainDlg 对话框

class TrainDlg : public CDialogEx
{
	DECLARE_DYNAMIC(TrainDlg)

public:
	TrainDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~TrainDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_TRAIN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持


public:
	virtual void OnOK();
	virtual void OnCancel();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedButtonSettraindir();
	CEdit m_traindir;
	CEdit m_classes;
	CListCtrl m_pics;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonSelectall();
	afx_msg void OnBnClickedButtonSelectopp();
	CEdit m_trainoutdir;
	afx_msg void OnBnClickedButtonCreatetraindir();
};
