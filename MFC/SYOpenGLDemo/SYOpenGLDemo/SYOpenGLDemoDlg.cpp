// SYOpenGLDemoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SYOpenGLDemo.h"
#include "SYOpenGLDemoDlg.h"




#define IDT_TIMER1 1003 

SYOpenGLManger *g_glMgr = NULL;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CSYOpenGLDemoDlg dialog




CSYOpenGLDemoDlg::CSYOpenGLDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSYOpenGLDemoDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSYOpenGLDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSYOpenGLDemoDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_DESTROY()
	ON_WM_MBUTTONDOWN()
	ON_WM_SIZE()
	ON_WM_SIZING()
	ON_BN_CLICKED(IDOK, &CSYOpenGLDemoDlg::OnBnClickedOk)
//	ON_WM_CLOSE()
ON_WM_MBUTTONUP()
ON_WM_MOUSEMOVE()
ON_WM_LBUTTONDOWN()
ON_WM_LBUTTONUP()
ON_WM_TIMER()
END_MESSAGE_MAP()


// CSYOpenGLDemoDlg message handlers

BOOL CSYOpenGLDemoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	RECT r;	
	GetWindowRect(&r); 
	g_glMgr = SYOpenGLManger::Instance(this->GetSafeHwnd(), r.right, r.bottom);
	g_glMgr->setBackgroundColor(255, 255, 255);

	m_btn1 = new SYGLUIButton(10, 10, L"O2_n.png", L"O2_d.png", L"O2_o.png", L"O2_un.png");
	m_btn1->name = L"v1";	

	m_btn2 = new SYGLUIButton(50, -50, L"O2_n.png", L"O2_d.png", L"O2_o.png", L"O2_un.png");
	m_btn2->name = L"v2";
	m_btn2->setCaptionWithStatus(SYGLUIBUTTON_NORMAL, L"Caption");

	m_btn1->addSubview(m_btn2);
	g_glMgr->AddSubview(m_btn1);

	
	//中文Bad Apple!!影絵で砂絵
	m_lbl1 = new SYGLUILabel(100, 200, 100, 15, L"中文Bad Apple!!影絵で砂絵", 24);
	m_lbl1->name = L"lb1";
	m_lbl1->setWrap(true);

	//g_glMgr->AddSubview(m_lbl1);

	__hook(&SYGLUIButton::OnEventClicked, m_btn1, &CSYOpenGLDemoDlg::onDidClicked);
	__hook(&SYGLUIButton::OnEventClicked, m_btn2, &CSYOpenGLDemoDlg::onDidClicked);
	

	
	::SetTimer(GetSafeHwnd(), IDT_TIMER1, 100, NULL);
 
	return TRUE;  // return TRUE  unless you set the focus to a control
}


void CSYOpenGLDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSYOpenGLDemoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}

	
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSYOpenGLDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CSYOpenGLDemoDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: Add your message handler code here
	::KillTimer(GetSafeHwnd(), IDT_TIMER1);
	
	__unhook(&SYGLUIButton::OnEventClicked, m_btn1, &CSYOpenGLDemoDlg::onDidClicked);
	__unhook(&SYGLUIButton::OnEventClicked, m_btn2, &CSYOpenGLDemoDlg::onDidClicked);

	delete g_glMgr;
	
}


void CSYOpenGLDemoDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	OutputDebugStringW(L"OnSize\n");
	//g_glMgr->OnSize(cx, cy);
}

void CSYOpenGLDemoDlg::OnSizing(UINT fwSide, LPRECT pRect)
{
	CDialog::OnSizing(fwSide, pRect);

	// TODO: Add your message handler code here
	OutputDebugStringW(L"OnSizing\n");
	//g_glMgr->OnSize(pRect->right, pRect->bottom);
}

void CSYOpenGLDemoDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}

void CSYOpenGLDemoDlg::OnMButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	OutputDebugStringW(L"OnMButtonDown\n");
	CDialog::OnMButtonDown(nFlags, point);
}

void CSYOpenGLDemoDlg::OnMButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	OutputDebugStringW(L"OnMButtonUp\n");
	CDialog::OnMButtonUp(nFlags, point);
}

void CSYOpenGLDemoDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	//OutputDebugStringW(L"OnMouseMove\n");
	g_glMgr->OnMouseMove(point.x, point.y);
	CDialog::OnMouseMove(nFlags, point);
}

void CSYOpenGLDemoDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	//OutputDebugStringW(L"OnLButtonDown\n");
	g_glMgr->OnMouseDown(point.x, point.y);
	CDialog::OnLButtonDown(nFlags, point);
}

void CSYOpenGLDemoDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	g_glMgr->OnMouseUp(point.x, point.y);
	CDialog::OnLButtonUp(nFlags, point);
}

void CSYOpenGLDemoDlg::onDidClicked(SYGLUIButton *sender, int x, int y)
{
	/*
	OutputDebugStringW(L"onDidClicked\n");
	OutputDebugStringW( (sender->name + L"\n").c_str() );
	*/
	if (sender == m_btn1)
	{
		m_btn2->setEnable(true);
	}

	if (sender == m_btn2)
	{
		m_btn2->setEnable(false);
	}
}

void CSYOpenGLDemoDlg::onDidMouseDown(SYGLUIView *sender, int x, int y)
{
	OutputDebugStringW(L"onDidMouseDown\n");
	OutputDebugStringW( (sender->name + L"\n").c_str() );
	
}

void CSYOpenGLDemoDlg::onDidMouseUp(SYGLUIView *sender, int x, int y)
{
	OutputDebugStringW(L"onDidMouseUp\n");
	OutputDebugStringW( (sender->name + L"\n").c_str() );
	
}

void CSYOpenGLDemoDlg::onDidMouseMove(SYGLUIView *sender, int x, int y)
{
	
	
}



void CSYOpenGLDemoDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	//OutputDebugStringW(L"Ontimer\n");
	g_glMgr->Render();

	

	CDialog::OnTimer(nIDEvent);
}
