
// 3Dlg.h : 头文件
//

#pragma once
#include "stdafx.h"
#include "afxcmn.h"
#include <fstream>
#include "resource.h"
#include "afxdialogex.h"

using namespace std;


// CMy3Dlg 对话框
class CMy3Dlg : public CDialogEx
{
// 构造
public:
	CMy3Dlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MY3_DIALOG};

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	CString GetPath();


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
