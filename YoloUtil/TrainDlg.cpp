// TrainDlg.cpp: 实现文件
//

#include "pch.h"
#include "YoloUtil.h"
#include "afxdialogex.h"
#include "TrainDlg.h"
#include "Utils.h"
#include <string>
// TrainDlg 对话框

IMPLEMENT_DYNAMIC(TrainDlg, CDialogEx)

TrainDlg::TrainDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_TRAIN, pParent)
{

}

TrainDlg::~TrainDlg()
{
}

void TrainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_TRAINDIR, m_traindir);
	DDX_Control(pDX, IDC_EDIT_CLASSES, m_classes);
	DDX_Control(pDX, IDC_LIST_PICS, m_pics);
	DDX_Control(pDX, IDC_EDIT_TRAINOUTDIR, m_trainoutdir);
}



void TrainDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

	//CDialogEx::OnOK();
}


void TrainDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类

	//CDialogEx::OnCancel();
}
BEGIN_MESSAGE_MAP(TrainDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_SETTRAINDIR, &TrainDlg::OnBnClickedButtonSettraindir)
	ON_BN_CLICKED(IDC_BUTTON_SELECTALL, &TrainDlg::OnBnClickedButtonSelectall)
	ON_BN_CLICKED(IDC_BUTTON_SELECTOPP, &TrainDlg::OnBnClickedButtonSelectopp)
	ON_BN_CLICKED(IDC_BUTTON_CREATETRAINDIR, &TrainDlg::OnBnClickedButtonCreatetraindir)
END_MESSAGE_MAP()




void TrainDlg::OnBnClickedButtonSettraindir()
{
	// TODO: 在此添加控件通知处理程序代码
	CString trainDir = Utils::selectFolder();
	m_traindir.SetWindowTextW(trainDir);


	CString classesStr = Utils::readFile(trainDir + _T("\\classes.txt"));
	classesStr.Replace(_T("\n"), _T("\r\n"));
	m_classes.SetWindowTextW(classesStr);


	//枚举txt和 jpg png bmp
	std::list<File> fileList = Utils::enumFiles(trainDir);
	for (std::list<File>::iterator it = fileList.begin(); it!=fileList.end(); it++)
	{	
		CString fileName = (*it).fileName;
		CString name = fileName.Mid(0, fileName.Find(L".", 0));
		CString fmt = fileName.Mid(fileName.Find(L".", 0)+1);

		if (fmt.Compare(L"png")==0 || fmt.Compare(L"bmp") == 0 || fmt.Compare(L"jpg") == 0 || fmt.Compare(L"jpeg") == 0)
		{

			int row = m_pics.GetItemCount();
			m_pics.InsertItem(row, _T(""));
			m_pics.SetItemText(row, 1, fileName);
			m_pics.SetItemText(row, 2, (*it).filePath +"\\" + fileName);
			m_pics.SetItemText(row, 3, _T("1"));
		}
		else if (fmt.Compare(L"txt") == 0) {
			for (size_t i = 0; i < m_pics.GetItemCount(); i++)
			{
				
				if (m_pics.GetItemText(i, 1).Mid(0, fileName.Find(L".", 0)).Compare(name) == 0) {
					m_pics.SetItemText(i, 3, _T("有标注"));
					break;
				}
			}
		}

	}
	



}


BOOL TrainDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_pics.SetExtendedStyle(m_pics.GetExtendedStyle() | LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);


	m_pics.InsertColumn(0, L"", LVCFMT_CENTER, 40, 0);
	m_pics.InsertColumn(1, L"文件名", LVCFMT_LEFT, 140, 0);
	m_pics.InsertColumn(2, L"标记数", LVCFMT_LEFT, 140, 0);
	m_pics.InsertColumn(3, L"校验", LVCFMT_LEFT, 50, 0);

	


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void TrainDlg::OnBnClickedButtonSelectall()
{
	// TODO: 在此添加控件通知处理程序代码

	for (size_t i = 0; i < m_pics.GetItemCount(); i++)
	{
		m_pics.SetCheck(i, TRUE);
	}
}


void TrainDlg::OnBnClickedButtonSelectopp()
{
	// TODO: 在此添加控件通知处理程序代码

	for (size_t i = 0; i < m_pics.GetItemCount(); i++)
	{
		m_pics.SetCheck(i, !m_pics.GetCheck(i));
	}
}


void TrainDlg::OnBnClickedButtonCreatetraindir()
{
	// TODO: 在此添加控件通知处理程序代码
	CString yaml_train;
	CString yaml_val;
	CString yaml_nc;
	CString yaml_names;

	CString classesStr;
	m_classes.GetWindowTextW(classesStr);
	std::list<CString> classesList = Utils::splitStr(classesStr, L"\r\n");
	
	yaml_names = "names: [";
	for (std::list<CString>::iterator it = classesList.begin(); it!= classesList.end() ; it++)
	{
		yaml_names += L"'"+(*it)+ L"',";
	}
	yaml_names = yaml_names.Left(yaml_names.GetLength() - 1);
	yaml_names += L"]";

	yaml_nc.Format(L"nc: %d", classesList.size());
	printf("%ls\n", yaml_names);
	printf("%ls\n", yaml_nc);


	//创建当前时间戳文件夹
	CString trainoutdir;
	CString dirName = Utils::getTimeStamp();
	yaml_train = L"train: ./" + dirName + L"/train.txt";
	yaml_val = L"val: ./" + dirName + L"/val.txt";
	
	m_trainoutdir.GetWindowTextW(trainoutdir);
	trainoutdir += L"\\" + dirName;
	CreateDirectoryW(trainoutdir, NULL);
	CreateDirectoryW(trainoutdir+L"\\images", NULL);
	CreateDirectoryW(trainoutdir+L"\\images\\train", NULL);
	CreateDirectoryW(trainoutdir+L"\\images\\val", NULL);
	CreateDirectoryW(trainoutdir+L"\\labels", NULL);
	CreateDirectoryW(trainoutdir+L"\\labels\\train", NULL);
	CreateDirectoryW(trainoutdir+L"\\labels\\val", NULL);
	
	//先写入yaml
	Utils::writeFile(trainoutdir + L"\\data.yaml", yaml_train + "\r\n"+ yaml_val+ "\r\n" + yaml_nc + "\r\n" + yaml_names);

	//开始写入images和labels    train和val
	CString trainImgs, valImgs;
	for (size_t i = 0; i < m_pics.GetItemCount(); i++)
	{
		if (m_pics.GetCheck(i) && m_pics.GetItemText(i,3).Compare(L"有标注") == 0 )
		{
			// 复制png jpg png
			CString filePath = m_pics.GetItemText(i, 2);
			CString fileName = m_pics.GetItemText(i, 1);
			Utils::copyFile(filePath, trainoutdir + L"\\images\\train\\");
			Utils::copyFile(filePath, trainoutdir + L"\\images\\val\\");
			// 复制txt
			Utils::copyFile(filePath.Mid(0, filePath.Find(L".", 0)) + L".txt", trainoutdir + L"\\labels\\train\\");
			Utils::copyFile(filePath.Mid(0, filePath.Find(L".", 0)) + L".txt", trainoutdir + L"\\labels\\val\\");
			//printf("copy:%ls to %ls", filePath, trainoutdir + L"\\images\\train\\");
			trainImgs += dirName +  L"/images/train/" + fileName + L"\r\n";
			valImgs += dirName + L"/images/val/" + fileName + L"\r\n";
		}
	}

	//写入train.txt和val.txt

	Utils::writeFile(trainoutdir + L"\\train.txt", trainImgs);
	Utils::writeFile(trainoutdir + L"\\val.txt", valImgs);
}

