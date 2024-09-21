// RenameDlg.cpp: 实现文件
//

#include "pch.h"
#include "YoloUtil.h"
#include "afxdialogex.h"
#include "RenameDlg.h"
#include "Utils.h"

// RenameDlg 对话框

IMPLEMENT_DYNAMIC(RenameDlg, CDialogEx)

RenameDlg::RenameDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_RENAME, pParent)
{

}

RenameDlg::~RenameDlg()
{
}

void RenameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_RENAMEDIR, m_renamedir);
	DDX_Control(pDX, IDC_EDIT_PREFIX, m_prefix);
}


BEGIN_MESSAGE_MAP(RenameDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_MULTIRENAME, &RenameDlg::OnBnClickedButtonMultirename)
	ON_BN_CLICKED(IDC_BUTTON_SETRENAMEDIR, &RenameDlg::OnBnClickedButtonSetrenamedir)
END_MESSAGE_MAP()


// RenameDlg 消息处理程序


void RenameDlg::OnBnClickedButtonMultirename()
{
	// TODO: 在此添加控件通知处理程序代码
	int i = 0;


	CString renamedirStr;
	m_renamedir.GetWindowTextW(renamedirStr);
	if (MessageBoxA(NULL,CW2A(L"该操作不可恢复，请确认好目录再执行\n" + renamedirStr), "警告", MB_OKCANCEL) == IDOK)
	{

		std::list<File> fileList = Utils::enumFiles(renamedirStr);
		for (std::list<File>::iterator it = fileList.begin(); it != fileList.end(); it++)
		{
			i++;
			CString fileName = (*it).fileName;
			CString filePath = (*it).filePath;
			CString fmt = fileName.Mid(fileName.Find(L".", 0) + 1);


			CString prefixStr;
			m_prefix.GetWindowTextW(prefixStr);
			CString newNameStr;
			newNameStr.Format(L"%ls%d.%ls", prefixStr, i , fmt);
			
			//char newName[100]{ 0 };
			//strcat(newName, CW2A(prefixStr));
			//itoa(i, newName, 10);
			//strcat(newName, CW2A(L"." + fmt));

			rename(CW2A(filePath + "\\" + fileName), CW2A(filePath + "\\" + newNameStr));
			printf("oldName:%ls   newName:%ls\n", fileName, newNameStr);
			//break;
		}
	}


}


void RenameDlg::OnBnClickedButtonSetrenamedir()
{
	// TODO: 在此添加控件通知处理程序代码
	CString renamedirStr = Utils::selectFolder();
	m_renamedir.SetWindowTextW(renamedirStr);
}
