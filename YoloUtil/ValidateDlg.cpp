// ValidateDlg.cpp: 实现文件
//

#include "pch.h"
#include "YoloUtil.h"
#include "afxdialogex.h"
#include "ValidateDlg.h"

// ValidateDlg 对话框

IMPLEMENT_DYNAMIC(ValidateDlg, CDialogEx)

ValidateDlg::ValidateDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_VALIDATE, pParent)
{

}

ValidateDlg::~ValidateDlg()
{
}

void ValidateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_ONNX, m_onnx);
	DDX_Control(pDX, IDC_EDIT_PICDIR, m_picdir);



	m_onnx.SetWindowTextW(L"C:\\Users\\Administrator\\Desktop\\best.onnx");
	m_picdir.SetWindowTextW(L"C:\\Users\\Administrator\\Desktop\\chessdata\\whiteblack");
}


BEGIN_MESSAGE_MAP(ValidateDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &ValidateDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &ValidateDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON_DETECT, &ValidateDlg::OnBnClickedButtonDetect)
END_MESSAGE_MAP()


// ValidateDlg 消息处理程序
// DetectDlg 消息处理程序
#include<vector>
#include "engine.hpp"
#include "Utils.h"

#include<algorithm>

void ValidateDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	m_onnx.SetWindowTextW(CA2W(Utils::selectFile().c_str()));

}


void ValidateDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	CString picdir = Utils::selectFolder();
	m_picdir.SetWindowTextW(picdir);
}


void ValidateDlg::OnBnClickedButtonDetect()
{
	// TODO: 在此添加控件通知处理程序代码
	


	// TODO: 在此添加控件通知处理程序代码
	CString onnx;
	CString picdir;
	m_onnx.GetWindowTextW(onnx);
	m_picdir.GetWindowTextW(picdir);
	std::string onnxstr = CW2A(onnx.GetString());
	std::string picdirstr = CW2A(picdir.GetString());
	

	std::wstring onnxwstr = CA2W(onnxstr.c_str());
	YoloInferencer yolo(onnxwstr,"yolo_inference","CPU");

	std::string className[] = { "red_che", "red_ma", "red_xiang", "red_shi", "red_shuai", "red_pao", "red_bing", "black_che", "black_ma", "black_xiang",
		"black_shi", "black_jiang", "black_pao", "black_zu", "board" };



	std::list<File> fileList = Utils::enumFiles(picdir);

	for (std::list<File>::iterator it = fileList.begin(); it != fileList.end(); it++)
	{
		std::string fileName = CW2A((*it).fileName.GetString());
		std::string filePath = CW2A((*it).filePath.GetString());

		std::string fullName = picdirstr + "\\" + fileName;


		if (fileName.find(".png") == -1 && fileName.find(".bmp") == -1 && fileName.find(".jpg") == -1 && fileName.find(".jpeg") == -1)
		{
			continue;
		}

		std::string txtFullName = picdirstr + "\\" + fileName.substr(0, fileName.find_last_of(".")) + ".txt";
		std::string strResult = CW2A(Utils::readFile(CString(txtFullName.c_str())).GetString());
		std::vector<std::string> strList = Utils::splitStr(strResult, "\n");


		//读取fullName图片的width和height

		cv::Mat image = cv::imread(fullName);
		int width = image.size().width;
		int height = image.size().height;
		std::vector<OutResult> results = yolo.infer(image, 0.8, 0.5);



		for (int i = 0; i < results.size(); i++)
		{
			float x = ((results[i].x1 + results[i].x2) / 2.0f) / width;
			float y = ((results[i].y1 + results[i].y2) / 2.0f) / height;
			float w = (results[i].x2 - results[i].x1) * 1.0 / width;
			float h = (results[i].y2 - results[i].y1) * 1.0 / height;


			boolean flag = false;
			for (size_t j = 0; j < strList.size();j++)
			{
				std::string temp = strList[j];
				std::vector<std::string> tempArr = Utils::splitStr(temp, " ");

				if (tempArr.size()!=5)
				{
					flag = true;
					break;
				}

				int id_ = atoi(tempArr[0].c_str());
				float x_ = std::stof(tempArr[1]);
				float y_ = std::stof(tempArr[2]);
				float w_ = std::stof(tempArr[3]);
				float h_ = std::stof(tempArr[4]);

				//printf("%d %f %f %f %f\n", id_, x_, y_, w_, h_);

				if (id_ == results[i].classId)
				{
					if (abs(x_ - x) * width <= 20)
					{
						if (abs(y_ - y) * height <= 20)
						{
							if (abs(w_ - w) * width <= 20)
							{
								if (abs(h_ - h) * height <= 20)
								{
									flag = true;
								}
							}
						}
					}
				}

			}


			if (!flag)
			{
				printf("%s %s\n", fileName.c_str(), className[results[i].classId].c_str());
				//printf("%f %f %f %f\n", x, y, w, h);
				//printf("%d %d %d %d\n", results[i].x1, results[i].y1, results[i].x2, results[i].y2);
			}


		}


		////判断strList里有没有xywh相近的
		//for (size_t i = 0; i < strList.size(); i++)
		//{
		//	std::string temp = strList[i];
		//	std::vector<std::string> tempArr = Utils::splitStr(temp, " ");
		//	int id_ = atoi(tempArr[0].c_str());
		//	float x_ = std::stof(tempArr[1]);
		//	float y_ = std::stof(tempArr[2]);
		//	float w_ = std::stof(tempArr[3]);
		//	float h_ = std::stof(tempArr[4]);

		//	boolean flag = false;
		//	for (size_t j = 0; j < strList.size(); j++)
		//	{
		//		if (j == i)
		//		{
		//			continue;
		//		}
		//		std::string temp1 = strList[j];
		//		std::vector<std::string> tempArr1 = Utils::splitStr(temp1, " ");
		//		int id_1 = atoi(tempArr1[0].c_str());
		//		float x_1 = std::stof(tempArr1[1]);
		//		float y_1 = std::stof(tempArr1[2]);
		//		float w_1 = std::stof(tempArr1[3]);
		//		float h_1 = std::stof(tempArr1[4]);
		//		if (abs(x_ - x_1) * width <= 20)
		//		{
		//			if (abs(y_ - y_1) * height <= 20)
		//			{
		//				if (abs(w_ - w_1) * width <= 20)
		//				{
		//					if (abs(h_ - h_1)* height <= 20)
		//					{
		//						flag = true;  //说明有相近的框框
		//					}
		//				}
		//			}
		//		}
		//	}
		//	if (flag)
		//	{
		//		printf("相近检测： %s %s\n", fileName.c_str(), className[id_].c_str());
		//	}
		//}



		//for (size_t j = 0; j < strList.size(); j++)
		//{
		//	std::string temp = strList[j];
		//	std::vector<std::string> tempArr = Utils::splitStr(temp, " ");
		//	int id_ = atoi(tempArr[0].c_str());
		//	float x_ = std::stof(tempArr[1]);
		//	float y_ = std::stof(tempArr[2]);
		//	float w_ = std::stof(tempArr[3]);
		//	float h_ = std::stof(tempArr[4]);

		//	boolean flag = false;
		//	for (size_t i = 0; i < results.size(); i++)
		//	{
		//		float x = ((results[i].x1 + results[i].x2) / 2.0f) / width;
		//		float y = ((results[i].y1 + results[i].y2) / 2.0f) / height;
		//		float w = (results[i].x2 - results[i].x1) * 1.0 / width;
		//		float h = (results[i].y2 - results[i].y1) * 1.0 / height;

		//		if (id_ == results[i].classId)
		//		{
		//			if (abs(x_ - x) <= 0.2)
		//			{
		//				if (abs(y_ - y) <= 0.2)
		//				{
		//					if (abs(w_ - w) <= 0.2)
		//					{
		//						if (abs(h_ - h) <= 0.2)
		//						{
		//							flag = true;
		//						}
		//					}
		//				}
		//			}
		//		}

		//	}

		//	if (!flag)
		//	{
		//		printf("%s %s\n", fileName.c_str(), className[id_].c_str());
		//	}

		//}

	}



}
