
// 3Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "3.h"
#include "3Dlg.h"
#include "afxdialogex.h"
#include "Function.h"
#include <Windows.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

Compression MyCompression;

char* CStrToCh(CString& j)
{
	int l = j.GetLength();
	char* result = new char[l + 1];
	for (int i = 0; i < l; i++)
	{
		result[i] = j[i];
	}
	result[l] = '\0';
	return result;
}

CString CMy3Dlg::GetPath()
{
	CString strPath = _T("");
	BROWSEINFO bInfo;
	ZeroMemory(&bInfo, sizeof(bInfo));	//初始化0

	bInfo.hwndOwner = m_hWnd;
	bInfo.lpszTitle = _T("请选择路径: ");
	bInfo.ulFlags = BIF_RETURNONLYFSDIRS;

	LPITEMIDLIST lpDlist; //用来保存返回信息的IDList
	lpDlist = SHBrowseForFolder(&bInfo); //显示选择对话框
	if (lpDlist != NULL)  //用户按了确定按钮
	{
		TCHAR chPath[255]; //用来存储路径的字符串
		SHGetPathFromIDList(lpDlist, chPath);//把项目标识列表转化成字符串
		strPath = chPath; //将TCHAR类型的字符串转换为CString类型的字符串
	}
	return strPath;
}

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMy3Dlg 对话框



CMy3Dlg::CMy3Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMy3Dlg::IDD, pParent)
	, OpenPathName(_T(""))
	, SavedPathName(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMy3Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, OpenPathName);
	DDX_Text(pDX, IDC_EDIT2, SavedPathName);
	DDX_Control(pDX, IDC_PROGRESS1, MyProgress);
}

BEGIN_MESSAGE_MAP(CMy3Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CMy3Dlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CMy3Dlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CMy3Dlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CMy3Dlg::OnBnClickedButton4)
END_MESSAGE_MAP()


// CMy3Dlg 消息处理程序

BOOL CMy3Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

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

	// TODO:  在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMy3Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMy3Dlg::OnPaint()
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
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMy3Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


UINT ThreadCompress(LPVOID IpParam)		//压缩文件线程
{
	ThreadInfo* pInfo = (ThreadInfo*)IpParam;
	MyCompression.FileCompress(*pInfo->IN_FILE, *pInfo->OUT_FILE);
	return 0;
}

UINT ThreadDecompress(LPVOID IpParam)	//解压文件线程
{
	ThreadInfo* pInfo = (ThreadInfo*)IpParam;
	MyCompression.FileDecompress(*pInfo->IN_FILE, *pInfo->OUT_FILE);
	return 0;
}

void CMy3Dlg::OnBnClickedButton1()	//打开
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(true);
	CFileDialog OpenDlg(true);
	OpenDlg.DoModal();
	OpenPathName = OpenDlg.GetPathName();
	FileName = OpenDlg.GetFileName();
	UpdateData(false);
}


void CMy3Dlg::OnBnClickedButton2()	//保存
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(true);
	SavedPathName = GetPath();
	UpdateData(false);
}


void CMy3Dlg::OnBnClickedButton3()	//压缩
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(true);

	if (OpenPathName == "" )
	{
		MessageBox(_T("请选择待压缩的文件！"), _T("错误"), MB_OK);
		return;
	}
	else if (SavedPathName == "")
	{
		MessageBox(_T("请选择保存路径！"), _T("错误"), MB_OK);
		return;
	}
	MyProgress.SetRange(0, 100);
	MyProgress.SetPos(0);

	CString CompressFileName = FileName + ".zym";	//压缩后的文件名

	CString FinName = OpenPathName;
	CString FoutName = SavedPathName + "\\" + CompressFileName;

	ifstream FIN(FinName.GetBuffer(0), ios::binary);
	ofstream FOUT(FoutName.GetBuffer(0), ios::binary);

	Info.IN_FILE = &FIN;
	Info.OUT_FILE = &FOUT;
	Info.NAME = FileName;
	
	CUR = 0;

	pThreadCompress = AfxBeginThread(ThreadCompress,&Info);

	while (1)
	{
		if (TAG)
		{
			MyProgress.SetPos(CUR);
		}
		if (CUR == 100.0)
		{
			MyProgress.SetPos(CUR);
			MessageBox(_T("压缩完成！"), _T("提示"), MB_OK);
			break;
		}
	}
	UpdateData(false);
}



void CMy3Dlg::OnBnClickedButton4()	//解压
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(true);

	if (OpenPathName == "")
	{
		MessageBox(_T("请选择待压缩的文件！"), _T("错误"), MB_OK);
		return;
	}
	else if (SavedPathName == "")
	{
		MessageBox(_T("请选择保存路径！"), _T("错误"), MB_OK);
		return;
	}
	MyProgress.SetRange(0, 100);
	MyProgress.SetPos(0);

	FileName.Delete(FileName.GetLength() - 4, 4);	//解压后的文件名
	CString CompressFileName = FileName;

	CString FinName = OpenPathName;
	CString FoutName = SavedPathName + "\\" + CompressFileName;

	ifstream FIN(FinName.GetBuffer(0), ios::binary);
	ofstream FOUT(FoutName.GetBuffer(0), ios::binary);

	Info.IN_FILE = &FIN;
	Info.OUT_FILE = &FOUT;
	Info.NAME = FileName;

	CUR = 0;

	pThreadDecompress = AfxBeginThread(ThreadDecompress, &Info);

	while (1)
	{
		if (TAG)
		{
			MyProgress.SetPos(CUR);
		}
		if (CUR == 100.0)
		{
			MyProgress.SetPos(CUR);
			MessageBox(_T("解压完成！"), _T("提示"), MB_OK);
			break;
		}
	}
	UpdateData(false);
}
