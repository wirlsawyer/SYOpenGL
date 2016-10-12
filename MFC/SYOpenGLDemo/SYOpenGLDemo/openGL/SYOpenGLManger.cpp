#include "stdafx.h"
#include "SYOpenGLManger.h"
#include "SYOpenGLResource.h"
static GdiplusStartupInput gdiplusStartupInput;
static GdiplusStartupOutput gdiplusStartupOutput;
static ULONG_PTR gdiplusBGThreadToken;

SYOpenGLManger* SYOpenGLManger::m_instance = NULL;

SYOpenGLManger* SYOpenGLManger::Instance()
{
	if (m_instance == NULL){
		m_instance = new SYOpenGLManger(); 
	}
	return m_instance;
}

SYOpenGLManger::SYOpenGLManger()
{
	
}

SYOpenGLManger* SYOpenGLManger::Instance(HWND hwnd, int w, int h)
{
	if (m_instance == NULL){
		m_instance = new SYOpenGLManger(hwnd, w, h); 
	}
	return m_instance;
}

SYOpenGLManger::SYOpenGLManger(HWND hwnd, int w, int h)
{
	this->_hWnd = hwnd;
	SYOpenGLConfig::Instance()->dynamicWidth = SYOpenGLConfig::Instance()->screenWidth = GetSystemMetrics(SM_CXSCREEN); 
	SYOpenGLConfig::Instance()->dynamicHeight = SYOpenGLConfig::Instance()->screenHeight = GetSystemMetrics(SM_CYSCREEN); 
	SYOpenGLConfig::Instance()->winWidth = w;
	SYOpenGLConfig::Instance()->winHeight = h;

	
	//Get Exe Path
	WCHAR exePath[MAX_PATH];
	ZeroMemory(exePath, MAX_PATH);
	GetModuleFileNameW((HMODULE)GetWindowLong(hwnd, 0), exePath, MAX_PATH);
	for (size_t i = wcslen(exePath); exePath[i] !='\\'; i--) exePath[i] = NULL;
	SYOpenGLResource::Instance(exePath);


	this->setBackgroundColor(0, 0, 0);
	this->m_pvecSubview = new std::vector<SYGLUIView *>;
	

	BITMAPINFO m_bmi;
	int nWidth = SYOpenGLConfig::Instance()->screenWidth;
	int nHeight = SYOpenGLConfig::Instance()->screenHeight;
	memset(&m_bmi, 0, sizeof(BITMAPINFO));
	m_bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	m_bmi.bmiHeader.biWidth = nWidth;
	m_bmi.bmiHeader.biHeight = nHeight;
	m_bmi.bmiHeader.biPlanes = 1;
	m_bmi.bmiHeader.biBitCount = 32;
	m_bmi.bmiHeader.biCompression = BI_RGB;
	m_bmi.bmiHeader.biSizeImage = nWidth * nHeight * 4; 
	void* m_pBits;
	HBITMAP m_hBitmap = ::CreateDIBSection(GetDC (hwnd), &m_bmi, DIB_RGB_COLORS, &m_pBits, NULL, (DWORD)0);
	_hDC = CreateCompatibleDC(GetDC (hwnd));
	::SelectObject(_hDC, m_hBitmap);
	//g_hDC = GetDC (hwnd); 
	// enable OpenGL for the window 
	InitOpenGLWithMemoryDC (hwnd, &_hDC, &_hRC); 

	InitGDIPlus();

	xFreeTypeLib::Instance();

}


SYOpenGLManger::~SYOpenGLManger()
{
	DisableOpenGL();
	ReleaseGDIPlus();

	delete xFreeTypeLib::Instance();
	delete SYOpenGLConfig::Instance();
	delete SYOpenGLResource::Instance();
	
	while(!m_pvecSubview->empty())
	{
		delete (m_pvecSubview->back());
		m_pvecSubview->pop_back();
	}
	delete m_pvecSubview;
	
	
}


void SYOpenGLManger::InitGDIPlus()
{
	/*
	GdiplusStartupInput gdiplusStartupInput;		
	// Initialize GDI+.
	GdiplusStartup(&_gdiplusToken, &gdiplusStartupInput, NULL);
*/
	//http://mikevdm.com/BlogEntry/Key/GdiplusShutdown-Hangs-Mysteriously

	// Initialization
	gdiplusStartupInput.SuppressBackgroundThread = TRUE;
	GdiplusStartup(&_gdiplusToken, &gdiplusStartupInput, 
		&gdiplusStartupOutput);
	Status stat = gdiplusStartupOutput.NotificationHook(
		&gdiplusBGThreadToken);
	//ASSERT(stat == Ok);

}

void SYOpenGLManger::ReleaseGDIPlus()
{
	/*
	GdiplusShutdown(_gdiplusToken);
	*/

	// Termination
	gdiplusStartupOutput.NotificationUnhook(gdiplusBGThreadToken);
	GdiplusShutdown(_gdiplusToken);
}


//[OpenGL]
void SYOpenGLManger::InitOpenGLWithMemoryDC(HWND hWnd, HDC *hDC, HGLRC *hRC)
{
	//*hDC = GetDC (hWnd); 
	PIXELFORMATDESCRIPTOR pfd=
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_DRAW_TO_BITMAP |
		PFD_SUPPORT_OPENGL| PFD_SUPPORT_GDI,
		/*PFD_DRAW_TO_WINDOW |
		PFD_SUPPORT_OPENGL|PFD_DOUBLEBUFFER,*/
		PFD_TYPE_RGBA,
		24,
		0,0,0,0,0,0,
		0,0,0,0,0,0,0,
		32,
		0,0,
		PFD_MAIN_PLANE,
		0,
		0,0,0
	};

	int pixelFormat=ChoosePixelFormat(*hDC, &pfd);
	SetPixelFormat (*hDC, pixelFormat, &pfd); 
	/* create and enable the render context (RC) */ 
	*hRC = wglCreateContext( *hDC );         //wgl開頭的都是WINDOWS用來連接OpenGL用的 
	wglMakeCurrent( *hDC, *hRC );   



	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_ALPHA_TEST );
	glEnable(GL_BLEND);	
	//glAlphaFunc(GL_NOTEQUAL, 0);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	

	//////////////////////////////////////////////////////////////////////////
	float width = SYOpenGLConfig::Instance()->screenWidth;
	float height = SYOpenGLConfig::Instance()->screenHeight;
		
	glViewport(0, 0, width, height);


	// Perspective projection parameters
	_pD.fieldOfView = 45.0;
	_pD.aspect      = width/height;
	_pD.nearPlane   = 0.1;
	_pD.farPlane    = 100.0;

	// setup context
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective( _pD.fieldOfView, _pD.aspect, _pD.nearPlane, _pD.farPlane);

	// set basic matrix mode
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width, height, 0, 0.0, 1000.0); // 原本是glOrtho(0, width, 0.0, height, 0.0, 1000.0); 會導致Y軸顛倒

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();	
	gluLookAt(0, 0, 500.0f,	0, 0, -10.0f, 	0.0f, 1.0f, 0.0f);



}

void SYOpenGLManger::DisableOpenGL() 
{ 
	wglMakeCurrent (NULL, NULL); 
	wglDeleteContext (_hRC); 
	ReleaseDC (_hWnd, _hDC); 
} 

void SYOpenGLManger::OnMouseDown(int x, int y)
{
	//////////////////////////////////////////////////////////////////////////	
	std::vector<SYGLUIView *>::iterator it;
	for (it=m_pvecSubview->begin(); it<m_pvecSubview->end(); it++)
	{
		SYGLUIView *view = (*it);
		if ( view->OnMouseDown(x, y) != NULL)
		{
			return;
		}
	}//end for
	

	OutputDebugStringW(L"SYOpenGLManger::OnMouseDown\n");
}

void SYOpenGLManger::OnMouseUp(int x, int y)
{
	
	//////////////////////////////////////////////////////////////////////////	
	std::vector<SYGLUIView *>::iterator it;
	for (it=m_pvecSubview->begin(); it<m_pvecSubview->end(); it++)
	{
		SYGLUIView *view = (*it);
		if (view->OnMouseUp(x, y) != NULL)
		{
			return;
		}
	}//end for

	OutputDebugStringW(L"SYOpenGLManger::OnMouseUp\n");
}

void SYOpenGLManger::OnMouseMove(int x, int y)
{
	
	//////////////////////////////////////////////////////////////////////////	
	std::vector<SYGLUIView *>::iterator it;
	for (it=m_pvecSubview->begin(); it<m_pvecSubview->end(); it++)
	{
		SYGLUIView *view = (*it);
		if (view->OnMouseMove(x, y) != NULL)
		{
			return;
		}
	}//end for

	//OutputDebugStringW(L"SYOpenGLManger::OnMouseMove\n");

}

void SYOpenGLManger::OnSize(float width, float height)
{
	SYOpenGLConfig::Instance()->dynamicWidth = width;
	SYOpenGLConfig::Instance()->dynamicHeight = height;
}

void SYOpenGLManger::Render()
{
	//glClearColor (1.0f, 1.0f, 1.0f, 1.0f); 
	glClearColor(m_color[0], m_color[1], m_color[2], m_color[3]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//////////////////////////////////////////////////////////////////////////	
	std::vector<SYGLUIView *>::iterator it;
	for (it=m_pvecSubview->begin(); it<m_pvecSubview->end(); it++)
	{
		SYGLUIView *view = (*it);
		view->onDrawFrame(0, 0, SYOpenGLConfig::Instance()->screenWidth, SYOpenGLConfig::Instance()->screenHeight, 0, 0);
	}//end for

	
	//xFreeTypeLib::Instance()->drawText(L"Abcde中文", 100, 100, 100, 50, 24);

	//xFreeTypeLib::Instance()->drawText(L"Abcde中文", 100, 200, 100, 50, 12);


	//////////////////////////////////////////////////////////////////////////
	SwapBuffers (_hDC); 
	glReadBuffer(GL_FRONT);
	BitBlt(GetDC(_hWnd), 0, 0, SYOpenGLConfig::Instance()->screenWidth, SYOpenGLConfig::Instance()->screenHeight, _hDC, 0, 0, SRCCOPY);
}
//////////////////////////////////////////////////////////////////////////
void SYOpenGLManger::AddSubview(SYGLUIView *view) 
{
	std::vector<SYGLUIView *>::iterator it;
	for (it=m_pvecSubview->begin(); it<m_pvecSubview->end(); it++)
	{
		if ((*it) == view )
		{
			m_pvecSubview->erase(it);
		}
	}//end for

	m_pvecSubview->push_back(view);
	view->Superview = m_pvecSubview;
}

void SYOpenGLManger::setBackgroundColor(int r, int g, int b) 
{
	// 0~255
	if (r>255) r = 255;
	if (g>255) g = 255;
	if (b>255) b = 255;

	m_color[0] = (float) r / 255.0f;
	m_color[1] = (float) g / 255.0f;
	m_color[2] = (float) b / 255.0f;
	m_color[3] = 1.0f;
}