// SYOpenGLDemoDlg.h : header file
//
#include "openGL/SYOpenGLManger.h"
#include "openGL/SYGLUIButton.h"
#include "openGL/SYGLUILabel.h"

#pragma once


// CSYOpenGLDemoDlg dialog
class CSYOpenGLDemoDlg : public CDialog
{
// Construction
public:
	CSYOpenGLDemoDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_SYOPENGLDEMO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;
	SYGLUIButton *m_btn1;
	SYGLUIButton *m_btn2;

	SYGLUILabel *m_lbl1;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	void onDidClicked(SYGLUIButton *sender, int x, int y);
	void onDidMouseDown(SYGLUIView *sender, int x, int y);
	void onDidMouseUp(SYGLUIView *sender, int x, int y);
	void onDidMouseMove(SYGLUIView *sender, int x, int y);
public:
	afx_msg void OnDestroy();
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
public:
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
public:
	afx_msg void OnBnClickedOk();
public:
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
public:
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
public:
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
