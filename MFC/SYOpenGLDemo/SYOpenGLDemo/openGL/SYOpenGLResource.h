#ifndef _SYOPENGRESOURCE_H_
#define _SYOPENGRESOURCE_H_

#ifdef BCB
typedef unsigned char  	BYTE;
typedef wchar_t 		WCHAR;
#else
#endif

#include <objbase.h>

#include <GL\glut.h>//使用DevC++的話要改為標入 #include <GL\openglut.h> 
#pragma comment (lib,"opengl32.lib")      //這行不在原本DevC++的範本裡，是VC++才需要的 

#include <gdiplus.h>
#pragma comment (lib, "gdiplus.lib")
using namespace Gdiplus;

#include <string>
#include <vector>


class SYTexture2D
{
public:
	GLuint      texture;
	GLint       width;
	GLint       height;
	GLint		texWidth;
	GLint		texHeight;
	std::wstring filePath;
};

class SYOpenGLResource
{

public:	
	//creat
	static SYOpenGLResource* Instance(void);
	static SYOpenGLResource* Instance(WCHAR *pwszPath);
	~SYOpenGLResource(void);
	
	//method
	SYTexture2D *IsExistTextureWithFilePath(const WCHAR *pwszFilePath);
	SYTexture2D *LoadTexture(const WCHAR *pwszFilePath);

private:
	//method
	SYOpenGLResource(void);	
	SYOpenGLResource(WCHAR *pwszPath);	

	bool IsPowerOfTwo(int x);
	int NextPowerOfTwo(int minimum);
private:
	//var
	//creat
	static SYOpenGLResource*	m_instance;
	//////////////////////////////////////////////////////////////////////////
	std::wstring strRootPath;
	std::vector<SYTexture2D *> *m_pvecTexture;
};

#endif