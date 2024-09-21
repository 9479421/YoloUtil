// DetectDlg.cpp: 实现文件
//

#include "pch.h"
#include "YoloUtil.h"
#include "afxdialogex.h"
#include "DetectDlg.h"

#include <string>
#include "Utils.h"


ULONG_PTR g_gdiplusToken = 0;
// DetectDlg 对话框

IMPLEMENT_DYNAMIC(DetectDlg, CDialogEx)

DetectDlg::DetectDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_DETECT, pParent)
{


}

DetectDlg::~DetectDlg()
{
}

void DetectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_ONNX, m_onnx);
	DDX_Control(pDX, IDC_EDIT_PICDIR, m_picdir);

	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	Gdiplus::GdiplusStartup(&g_gdiplusToken, &gdiplusStartupInput, NULL);

	m_onnx.SetWindowTextW(L"F:\\XRChess\\ChessAI\\ChessAI\\best1.onnx");
	m_picdir.SetWindowTextW(L"C:\\Users\\Administrator\\Desktop\\apple");
}


BEGIN_MESSAGE_MAP(DetectDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_DETECT, &DetectDlg::OnBnClickedButtonDetect)
	ON_BN_CLICKED(IDC_BUTTON1, &DetectDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &DetectDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// DetectDlg 消息处理程序
#include<vector>
#include "engine.hpp"


#include<algorithm>

void DetectDlg::OnBnClickedButtonDetect()
{




	// TODO: 在此添加控件通知处理程序代码
	CString onnx;
	CString picdir;
	m_onnx.GetWindowTextW(onnx);
	m_picdir.GetWindowTextW(picdir);
	std::string onnxstr = CW2A(onnx.GetString());
	std::string picdirstr = CW2A(picdir.GetString());


	std::wstring onnxwstr = CA2W(onnxstr.c_str());
	YoloInferencer yolo(onnxwstr, "yolo_inference", "CPU");




	std::list<File> fileList = Utils::enumFiles(picdir);

	for (std::list<File>::iterator it = fileList.begin(); it != fileList.end(); it++)
	{
		std::string fileName = CW2A((*it).fileName.GetString());
		std::string filePath = CW2A((*it).filePath.GetString());

		std::string fullName = picdirstr + "\\" + fileName;
		std::string txtFullName = picdirstr + "\\" + fileName.substr(0,fileName.find(".")) + ".txt";
		printf("file:%s\n", txtFullName.c_str());

		//读取fullName图片的width和height
		Gdiplus::Bitmap* bitmap = Gdiplus::Bitmap::FromFile(CA2W(fullName.c_str()));
		int width = bitmap->GetWidth();
		int height = bitmap->GetHeight();
		delete bitmap;

		

		cv::Mat image = cv::imread(fullName);
		std::vector<OutResult> results = yolo.infer(image, 0.8, 0.5);
		//让results的成员按照坐标进行排序，先从y1排序，再从x1排序
		std::sort(results.begin(), results.end(), [](OutResult a, OutResult b) {
			if (abs(a.y1 - b.y1) > 20) {
				return a.y1 < b.y1;
			}
			return a.x1 < b.x1;
			});





		std::string content;
		for (int i = 0 ; i < results.size() ; i++)
		{
			//printf("%d %f %d %d %d %d \n",results[i].classId, results[i].confidence , results[i].x1, results[i].y1, results[i].x2, results[i].y2);
			

			std::string x = std::to_string(((results[i].x1 + results[i].x2) / 2.0f) / width);
			std::string y = std::to_string(((results[i].y1 + results[i].y2) / 2.0f) / height);

			std::string w = std::to_string((results[i].x2 - results[i].x1)*1.0 / width);
			std::string h = std::to_string((results[i].y2 - results[i].y1)*1.0 / height);

			std::string line = std::to_string(results[i].classId) + " " + x + " " + y + " " + w + " " + h + "\n";
			content += line;
		}

		Utils::writeFile(CString(txtFullName.c_str()),CString(content.c_str()));
	}


}


void DetectDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码

	
	m_onnx.SetWindowTextW(CA2W(Utils::selectFile().c_str()));

}


void DetectDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码

	CString picdir = Utils::selectFolder();
	m_picdir.SetWindowTextW(picdir);

}
