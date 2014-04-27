
// OGL2Dlg.h : header file
//

#pragma once

#include "RgbImage.h"

// COGL2Dlg dialog
class COGL2Dlg : public CDialogEx
{
// Construction
public:
	COGL2Dlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_OGL2_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnEnChangeEdit1();
	CString m_czolg1_kat;
	CString m_czolg2_v0;
	afx_msg void OnBnClickedButton2();
	afx_msg void OnEnChangeEdit2();
	CString czolg1;
	int gracz;
	void loadTextureFromFile(char *filename);
};
