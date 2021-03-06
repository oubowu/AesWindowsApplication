
// Example17Dlg.cpp: 实现文件
//

#include "stdafx.h"
#include "Example17.h"
#include "Example17Dlg.h"
#include "afxdialogex.h"

#include <stdlib.h> 
#include <stdio.h>
#include <openssl/aes.h>
#include <iostream>
#include <fstream>

using namespace std;

#define RELESE_ARRAY(P) if (P)  \
{                               \
    delete[] P;                 \
    P = NULL;                   \
}

//int encodeFile(const char *sourceFilePath, const char *destFilePath);
//
//int decodeFile(const char *sourceFilePath, const char *destFilePath);

// aes加密文件
int encodeFile(const char *sourceFilePath, const char *destFilePath) {

	int returnValue = NULL;

	//sourceFilePath = "C:\\Users\\Administrator\\Desktop\\aestest\\tbs_sdk_thirdapp.jar";
	//destFilePath = "C:\\Users\\Administrator\\Desktop\\aestest\\he.jar";

	ifstream fin(sourceFilePath, ios_base::binary);
	ofstream fout(destFilePath, ios_base::binary);

	if (!fin) {
		cout << "Can not open fin file." << endl;
		returnValue = -1;
		TRACE("Can not open fin file.\n");
		return returnValue;
	}
	if (!fout) {
		cout << "Can not open fout file." << endl;
		returnValue = -2;
		TRACE("Can not open fout file.\n");
		return returnValue;
	}

	//用指定密钥对一段内存进行加密，结果放在outbuffer中
	unsigned char aes_keybuf[32];
	memset(aes_keybuf, 0, sizeof(aes_keybuf));
	strcpy((char *)aes_keybuf, "woshioubowu");
	AES_KEY aeskey;
	AES_set_encrypt_key(aes_keybuf, 256, &aeskey);

	int encrypt_chunk_size = 16;

	char *in_data = new char[encrypt_chunk_size + 1];
	char *out_data = new char[encrypt_chunk_size + 1];

	while (!fin.eof()) {
		fin.read(in_data, encrypt_chunk_size);
		if (fin.gcount() < encrypt_chunk_size) {
			fout.write(in_data, fin.gcount());
		}
		else {
			AES_encrypt((const unsigned char *)in_data, (unsigned char *)out_data, &aeskey);
			fout.write(out_data, fin.gcount());
		}
	};

	fout.close();
	fin.close();

	RELESE_ARRAY(in_data);
	RELESE_ARRAY(out_data);
	if (returnValue == NULL) {
		returnValue = 1;
		TRACE("加密成功\n");
	}
	return returnValue;
}

// aes解密文件
int decodeFile(const char *sourceFilePath, const char *destFilePath) {
	int returnValue = NULL;

	ifstream fin(sourceFilePath, ios_base::binary);
	ofstream fout(destFilePath, ios_base::binary);

	if (!fin) {
		cout << "Can not open fin file." << endl;
		returnValue = -1;
		TRACE("Can not open fin file.\n");
		return returnValue;
	}
	if (!fout) {
		cout << "Can not open fout file." << endl;
		returnValue = -2;
		TRACE("Can not open fout file.\n");
		return returnValue;
	}

	//用指定密钥对一段内存进行解密，结果放在outbuffer中
	unsigned char aes_keybuf[32];
	memset(aes_keybuf, 0, sizeof(aes_keybuf));
	strcpy((char *)aes_keybuf, "woshioubowu");
	AES_KEY aeskey;
	int keyResult = AES_set_decrypt_key(aes_keybuf, 256, &aeskey);

	TRACE("keyResult = %d \n", keyResult);

	int encrypt_chunk_size = 16;

	char *in_data = new char[encrypt_chunk_size + 1];
	char *out_data = new char[encrypt_chunk_size + 1];

	while (!fin.eof()) {
		fin.read(in_data, encrypt_chunk_size);
		if (fin.gcount() < encrypt_chunk_size) {
			fout.write(in_data, fin.gcount());
		}
		else {
			AES_decrypt((unsigned char *)in_data, (unsigned char *)out_data, &aeskey);
			fout.write(out_data, fin.gcount());
		}
	};

	fout.close();
	fin.close();

	RELESE_ARRAY(in_data);
	RELESE_ARRAY(out_data);

	if (returnValue == NULL) {
		returnValue = 1;
		TRACE("解密成功\n");
	}
	return returnValue;
}


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CExample17Dlg 对话框



CExample17Dlg::CExample17Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_EXAMPLE17_DIALOG, pParent)
	, m_open_file_path(_T(""))
	, m_save_file_path(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CExample17Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_OPEN_EDIT, m_open_file_path);
	DDX_Text(pDX, IDC_SAVE_EDIT, m_save_file_path);
}

BEGIN_MESSAGE_MAP(CExample17Dlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_OPEN_BUTTON, &CExample17Dlg::OnBnClickedOpenButton)
	ON_BN_CLICKED(IDC_SAVE_BUTTON, &CExample17Dlg::OnBnClickedSaveButton)
	ON_BN_CLICKED(ID_ENCODE, &CExample17Dlg::OnBnClickedEncode)
	ON_BN_CLICKED(ID_DECODE, &CExample17Dlg::OnBnClickedDecode)
END_MESSAGE_MAP()


// CExample17Dlg 消息处理程序

BOOL CExample17Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CExample17Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CExample17Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CExample17Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CExample17Dlg::OnBnClickedOpenButton()
{
	// TODO: 在此添加控件通知处理程序代码

	// 设置过滤器
	TCHAR szFilter[] = _T("jar文件(*.jar)|*.jar|所有文件(*.*)|*.*|| ");
	// 构造打开文件对话框
	CFileDialog fileDlg(TRUE, _T("jar"), NULL, 0, szFilter, this);
	CString strFilePath;

	// 显示打开文件对话框
	if (IDOK == fileDlg.DoModal())
	{
		// 如果点击了文件对话框上的“打开”按钮， 则将选择的文件路径显示到编辑框里
		strFilePath = fileDlg.GetPathName();
		SetDlgItemText(IDC_OPEN_EDIT, strFilePath);
	}

}


void CExample17Dlg::OnBnClickedSaveButton()
{
	// TODO: 在此添加控件通知处理程序代码

	 // 设置过滤器
	TCHAR szFilter[] = _T("jar文件(*.jar)|*.jar|所有文件(*.*)|*.*|| ");
	// 构造保存文件对话框
	CFileDialog fileDlg(FALSE, _T("jar"), _T("myJar"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, this);
	CString strFilePath;

	// 显示保存文件对话框
	if (IDOK == fileDlg.DoModal())
	{
		// 如果点击了文件对话框上的“保存”按钮， 则将选择的文件路径显示到编辑框里
		strFilePath = fileDlg.GetPathName();
		SetDlgItemText(IDC_SAVE_EDIT, strFilePath);
	}

}

void CExample17Dlg::OnBnClickedEncode()
{
	// TODO: 在此添加控件通知处理程序代码

	UpdateData(TRUE);


	//使用UNICODE字符集编程时，总是需要使用那些不支持UNICODE的库，例如sqlite3，Lua等必须使用char*类型的。这个时候用CStringA是最好的。

	//	另外CStringA与CString可以灵活地随意转换赋值，注意不能这样用：
	//	CString str1;
	//CStringA str2 = str1;
	//而要这样用：
	//	CStringA str2;
	//str2 = str1;

	//这样就可以把UNICODE版本字符串转换为Ansi版本了，非常之强大，非常之方便！
	//	在使用UNICODE而非多字节字符集时，可以通过如下方法把CString 转化为const char *
	//	CString str1;
	//CStringA  str2;
	//str2 = str1;
	//const char* pszStr = str2.GetBuffer();
	//这样就把CString 转化为const char *


	CStringA  openFilePathA;
	openFilePathA = m_open_file_path;
	const char* openFilePath = openFilePathA.GetBuffer();

	CStringA  saveFilePathA;
	saveFilePathA = m_save_file_path;
	const char* saveFilePath = saveFilePathA.GetBuffer();

	int res = encodeFile(openFilePath, saveFilePath);

	// 如果你的程序是UNICODE，那么%s表示对应的参数是一个WCHAR*，而%S对应的参数是一个char*
	// 如果你的程序是ANSI，那么%s表示对应的参数是一个char*，而%S对应的参数是一个WCHAR*

	TRACE("打开需要加密的文件路径为 %s \n", openFilePath);
	TRACE("保存加密后的文件路径为 %s \n", saveFilePathA);

	TRACE("加密结果为 %d \n", res);

	INT_PTR nRes;

	// 显示消息对话框
	if (res == 1) {
		nRes = MessageBox(_T("加密成功"), _T("加密结果"), MB_OK | MB_ICONINFORMATION);
	}
	else {
		nRes = MessageBox(_T("加密失败"), _T("加密结果"), MB_OK | MB_ICONINFORMATION);
	}

}


void CExample17Dlg::OnBnClickedDecode()
{
	// TODO: 在此添加控件通知处理程序代码

	UpdateData(TRUE);

	CStringA  openFilePathA;
	openFilePathA = m_open_file_path;
	const char* openFilePath = openFilePathA.GetBuffer();

	CStringA  saveFilePathA;
	saveFilePathA = m_save_file_path;
	const char* saveFilePath = saveFilePathA.GetBuffer();

	int res = decodeFile(openFilePath, saveFilePath);

	TRACE("打开需要解密的文件路径为 %s \n", openFilePath);
	TRACE("保存解密后的文件路径为 %s \n", saveFilePathA);

	TRACE("解密结果为 %d \n", res);

	INT_PTR nRes;

	// 显示消息对话框
	if (res == 1) {
		nRes = MessageBox(_T("解密成功"), _T("解密结果"), MB_OK | MB_ICONINFORMATION);
	}
	else {
		nRes = MessageBox(_T("解密失败"), _T("解密结果"), MB_OK | MB_ICONINFORMATION);
	}

}
