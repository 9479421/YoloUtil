// RepairDlg.cpp: 实现文件
//

#include "pch.h"
#include "YoloUtil.h"
#include "afxdialogex.h"
#include "RepairDlg.h"


// RepairDlg 对话框

IMPLEMENT_DYNAMIC(RepairDlg, CDialogEx)

RepairDlg::RepairDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_REPAIR, pParent)
{


}

RepairDlg::~RepairDlg()
{
}

void RepairDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_TXTDIR, m_txtDir);
	DDX_Control(pDX, IDC_EDIT_OLD, m_old);
	DDX_Control(pDX, IDC_EDIT_NEW, m_new);



	//m_txtDir.SetWindowTextW(L"C:\\Users\\Administrator\\Desktop\\tt831");
	m_txtDir.SetWindowTextW(L"C:\\Users\\Administrator\\Desktop\\test\\labels");
	m_old.SetWindowTextW(L"0\r\n1\r\n2\r\n3\r\n4\r\n5\r\n6\r\n7\r\n8\r\n9\r\n10\r\n11\r\n12\r\n13\r\n14");
	m_new.SetWindowTextW(L"13\r\n7\r\n11\r\n8\r\n12\r\n10\r\n9\r\n14\r\n6\r\n0\r\n4\r\n1\r\n5\r\n3\r\n2");


}


BEGIN_MESSAGE_MAP(RepairDlg, CDialogEx)

	ON_BN_CLICKED(IDC_BUTTON_DETECT, &RepairDlg::OnBnClickedButtonDetect)
END_MESSAGE_MAP()


// RepairDlg 消息处理程序


void RepairDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

	//CDialogEx::OnOK();
}


void RepairDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类

	//CDialogEx::OnCancel();
}

#include<list>
#include"QModule.h"

void RepairDlg::OnBnClickedButtonDetect()
{
	// TODO: 在此添加控件通知处理程序代码


	// TODO: 在此添加控件通知处理程序代码
	CString txtDir;
	m_txtDir.GetWindowTextW(txtDir);
	std::string txtDirStr = CW2A(txtDir.GetString());

	CString old_;
	m_old.GetWindowTextW(old_);
	std::string oldStr = CW2A(old_.GetString());

	CString new_;
	m_new.GetWindowTextW(new_);
	std::string newStr = CW2A(new_.GetString());


	//id修复
	std::vector<std::string> oldArr = QModule::splitStr(oldStr, "\r\n");
	std::vector<std::string> newArr = QModule::splitStr(newStr, "\r\n");


	if (oldArr.size() != newArr.size())
	{
		MessageBoxA(NULL, "id数量不一样！", "提示", 0);
		return;
	}

	printf("oldArr.size()=%d\n", oldArr.size());
	printf("newArr.size()=%d\n", newArr.size());

	std::vector<QFile> fileList = QModule::enumFile(txtDirStr);
	for (size_t i = 0; i < fileList.size(); i++)
	{

		if (!fileList[i].getFileFormat().compare("txt"))
		{
			printf("%s\n", fileList[i].getFullName().c_str());

			std::string txtStr = QModule::readFile(fileList[i].getFullName());
			std::vector<std::string> rowArr = QModule::splitStr(txtStr, "\n");

			std::string newTxtStr = "";
			for (size_t j = 0; j < rowArr.size(); j++)
			{
				std::vector<std::string> colArr = QModule::splitStr(rowArr[j], " ");
				if (colArr.size() > 1) {
					int id = atoi(colArr[0].c_str());
					if (id >= oldArr.size())
					{
						MessageBoxA(NULL, "id超出范围！", "提示", 0);
						return;
					}
					colArr[0] = newArr[id];
					std::string rowStr = "";
					for (size_t k = 0; k < colArr.size(); k++)
					{
						rowStr += colArr[k];
						if (k != colArr.size() - 1)
						{
							rowStr += " ";
						}
					}
					newTxtStr += rowStr;
					if (j != rowArr.size() - 1)
					{
						newTxtStr += "\n";
					}
				}
			}
			QModule::writeFile(fileList[i].getFullName(), newTxtStr);
		}
	}

}
