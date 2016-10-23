
// 3Dlg.cpp : ʵ���ļ�
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
	ZeroMemory(&bInfo, sizeof(bInfo));	//��ʼ��0

	bInfo.hwndOwner = m_hWnd;
	bInfo.lpszTitle = _T("��ѡ��·��: ");
	bInfo.ulFlags = BIF_RETURNONLYFSDIRS;

	LPITEMIDLIST lpDlist; //�������淵����Ϣ��IDList
	lpDlist = SHBrowseForFolder(&bInfo); //��ʾѡ��Ի���
	if (lpDlist != NULL)  //�û�����ȷ����ť
	{
		TCHAR chPath[255]; //�����洢·�����ַ���
		SHGetPathFromIDList(lpDlist, chPath);//����Ŀ��ʶ�б�ת�����ַ���
		strPath = chPath; //��TCHAR���͵��ַ���ת��ΪCString���͵��ַ���
	}
	return strPath;
}

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CMy3Dlg �Ի���



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


// CMy3Dlg ��Ϣ�������

BOOL CMy3Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMy3Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CMy3Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


UINT ThreadCompress(LPVOID IpParam)		//ѹ���ļ��߳�
{
	ThreadInfo* pInfo = (ThreadInfo*)IpParam;
	MyCompression.FileCompress(*pInfo->IN_FILE, *pInfo->OUT_FILE);
	return 0;
}

UINT ThreadDecompress(LPVOID IpParam)	//��ѹ�ļ��߳�
{
	ThreadInfo* pInfo = (ThreadInfo*)IpParam;
	MyCompression.FileDecompress(*pInfo->IN_FILE, *pInfo->OUT_FILE);
	return 0;
}

void CMy3Dlg::OnBnClickedButton1()	//��
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
	CFileDialog OpenDlg(true);
	OpenDlg.DoModal();
	OpenPathName = OpenDlg.GetPathName();
	FileName = OpenDlg.GetFileName();
	UpdateData(false);
}


void CMy3Dlg::OnBnClickedButton2()	//����
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
	SavedPathName = GetPath();
	UpdateData(false);
}


void CMy3Dlg::OnBnClickedButton3()	//ѹ��
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);

	if (OpenPathName == "" )
	{
		MessageBox(_T("��ѡ���ѹ�����ļ���"), _T("����"), MB_OK);
		return;
	}
	else if (SavedPathName == "")
	{
		MessageBox(_T("��ѡ�񱣴�·����"), _T("����"), MB_OK);
		return;
	}
	MyProgress.SetRange(0, 100);
	MyProgress.SetPos(0);

	CString CompressFileName = FileName + ".zym";	//ѹ������ļ���

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
			MessageBox(_T("ѹ����ɣ�"), _T("��ʾ"), MB_OK);
			break;
		}
	}
	UpdateData(false);
}



void CMy3Dlg::OnBnClickedButton4()	//��ѹ
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);

	if (OpenPathName == "")
	{
		MessageBox(_T("��ѡ���ѹ�����ļ���"), _T("����"), MB_OK);
		return;
	}
	else if (SavedPathName == "")
	{
		MessageBox(_T("��ѡ�񱣴�·����"), _T("����"), MB_OK);
		return;
	}
	MyProgress.SetRange(0, 100);
	MyProgress.SetPos(0);

	FileName.Delete(FileName.GetLength() - 4, 4);	//��ѹ����ļ���
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
			MessageBox(_T("��ѹ��ɣ�"), _T("��ʾ"), MB_OK);
			break;
		}
	}
	UpdateData(false);
}
