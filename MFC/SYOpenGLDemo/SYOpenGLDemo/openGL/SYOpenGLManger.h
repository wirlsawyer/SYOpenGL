#ifndef _SYOPENGLMANGER_H_
#define _SYOPENGLMANGER_H_


//////////////////////////////////////////////////////////////////////////
#include <GL\glut.h>//使用DevC++的話要改為標入 #include <GL\openglut.h> 
#pragma comment (lib,"opengl32.lib")      //這行不在原本DevC++的範本裡，是VC++才需要的 
//////////////////////////////////////////////////////////////////////////
#include <gdiplus.h>
#pragma comment (lib, "gdiplus.lib")
using namespace Gdiplus;
//////////////////////////////////////////////////////////////////////////
#include "xFreeTypeLib.h"
//////////////////////////////////////////////////////////////////////////
#include "SYOpenGLConfig.h"
#include "SYGLUIView.h"
#include <vector>
//////////////////////////////////////////////////////////////////////////

struct perspectiveData{
	float fieldOfView;
	float aspect;
	float nearPlane;
	float farPlane;
};


class SYOpenGLManger
{

public:	
	//creat
	static SYOpenGLManger* Instance(void);
	static SYOpenGLManger* Instance(HWND hwnd, int w, int h);
	~SYOpenGLManger(void);
	
	//method
	void Render();
	void OnMouseDown(int x, int y);
	void OnMouseUp(int x, int y);
	void OnMouseMove(int x, int y);
	void OnSize(float width, float height);

	/*
	HWND GetHWND(void){ return _hWnd; }
	HDC  GetHDC(void){ return _hDC; }
	*/
	//int  GetWidth(void){ return SYOpenGLConfig::Instance()->winWidth; }
	//int  GetHeight(void){ return SYOpenGLConfig::Instance()->winHeight; }
	
	void AddSubview(SYGLUIView *view);
	void setBackgroundColor(int r, int g, int b);

private:
	//method
	SYOpenGLManger(void);	
	SYOpenGLManger(HWND hwnd, int w, int h);

	void InitGDIPlus(void);
	void ReleaseGDIPlus(void);

	void InitOpenGLWithMemoryDC(HWND hWnd, HDC *hDC, HGLRC *hRC);
	void DisableOpenGL(void) ;


	

public:
	//SYOpenGLResource *Res;
private:
	//var
	//creat
	static SYOpenGLManger*	m_instance;

	HWND	_hWnd;
	HDC		_hDC;   //Handle to a device context
	HGLRC	_hRC;   //Handle to an OpenGL rendering context 
	//int     _Width;
	//int     _Height;
	ULONG_PTR           _gdiplusToken;

	perspectiveData		_pD;


	//////////////////////////////////////////////////////////////////////////
	
	std::vector<SYGLUIView *> *m_pvecSubview;
	float m_color[4];

};

#endif