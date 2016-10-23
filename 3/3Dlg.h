
// 3Dlg.h : ͷ�ļ�
//

#pragma once
#include "stdafx.h"
#include "afxcmn.h"
#include <fstream>
#include "resource.h"
#include "afxdialogex.h"

using namespace std;


// CMy3Dlg �Ի���
class CMy3Dlg : public CDialogEx
{
// ����
public:
	CMy3Dlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MY3_DIALOG};

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
	CString GetPath();


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	CWinThread *pThreadCompress;
	CWinThread *pThreadDecompress;
public:
	afx_msg void OnBnClickedButton1();
	CString OpenPathName;
	CString SavedPathName;
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	CProgressCtrl MyProgress;
	afx_msg void OnBnClickedButton4();
};

UINT ThreadCompress(LPVOID IpParam);
UINT ThreadDecompress(LPVOID IpParam);

struct ThreadInfo
{
	ifstream* IN_FILE;
	ofstream* OUT_FILE;
	CString NAME;
};
