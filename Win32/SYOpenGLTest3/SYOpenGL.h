#ifndef _SYOPENGL_H_
#define _SYOPENGL_H_

#include <objbase.h>

#include <gl/gl.h> 
#include "GL\glut.h"//使用DevC++的話要改為標入 #include <GL\openglut.h> 
#pragma comment (lib,"opengl32.lib")      //這行不在原本DevC++的範本裡，是VC++才需要的 


#include <gdiplus.h>
#pragma comment (lib, "gdiplus.lib")
using namespace Gdiplus;


#include <string>
#include <vector>
using namespace std;
//namespace std { namespace tr1 { using namespace std; } }

struct perspectiveData{
	float fieldOfView;
	float aspect;
	float nearPlane;
	float farPlane;
};

class SYOpenGL{
public:
	SYOpenGL(HWND hwnd, int w, int h, const WCHAR *pBgPath, const WCHAR *pImgPath)
	{		
		_hWnd = hwnd;
		_Width = w;
		_Height = h;

		_theta = 0;

		BITMAPINFO m_bmi;
		int nWidth = w;
		int nHeight = h;
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
		/* enable OpenGL for the window */ 
		InitOpenGLWithMemoryDC (hwnd, &_hDC, &_hRC); 

		InitGDIPlus();

		//load png file
		LoadTexture(pImgPath, &_textureImage, &_textureImage_Width, &_textureImage_Height);

		LoadTexture(pBgPath, &_textureBG, &_textureBG_Width, &_textureBG_Height);
	}

	~SYOpenGL()
	{
		DisableOpenGL();
		ReleaseGDIPlus();
	}


	void InitGDIPlus()
	{
		GdiplusStartupInput gdiplusStartupInput;		
		// Initialize GDI+.
		GdiplusStartup(&_gdiplusToken, &gdiplusStartupInput, NULL);
	}

	void ReleaseGDIPlus()
	{
		GdiplusShutdown(_gdiplusToken);
	}


	//[OpenGL]
	void InitOpenGLWithMemoryDC(HWND hWnd, HDC *hDC, HGLRC *hRC)
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


		
		//glEnable(GL_DEPTH_TEST);
		glEnable( GL_ALPHA_TEST );
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);	

		
		glViewport(0, 0, _Width, _Height);


		// Perspective projection parameters
		_pD.fieldOfView = 45.0;
		_pD.aspect      = (float)_Width/_Height;
		_pD.nearPlane   = 0.1;
		_pD.farPlane    = 100.0;

		// setup context
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective( _pD.fieldOfView, _pD.aspect, _pD.nearPlane, _pD.farPlane);

		// set basic matrix mode
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}

	void DisableOpenGL () 
	{ 
		wglMakeCurrent (NULL, NULL); 
		wglDeleteContext (_hRC); 
		ReleaseDC (_hWnd, _hDC); 
	} 


	
	void Draw_Plane(float n)
	{

		glColor3f(1.0f, 1.0f, 1.0f);// 指定使用的顏色
		glBegin(GL_POLYGON);
		glTexCoord2f(0,0); glVertex3f(-n, n, 0);
		glTexCoord2f(1,0); glVertex3f( n, n, 0);
		glTexCoord2f(1,1); glVertex3f( n,-n, 0);
		glTexCoord2f(0,1); glVertex3f(-n,-n, 0);
		glEnd();

		glColor3f(0.0f, 0.0f, 0.0f);// 指定使用的顏色
		glBegin(GL_LINES);

		for (float i=-n; i<=n; i+=1.0f){
			glVertex3f( i, n, 0.1f);
			glVertex3f( i,-n, 0.1f);
			glVertex3f( n, i, 0.1f);
			glVertex3f(-n, i, 0.1f);
		}
		glEnd();
	}

	
	void Render()
	{
		glClearColor (0.0f, 0.0f, 0.0f, 1.0f); 
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	
		//////////////////////////////////////////////////////////////////////////
	
		glPushMatrix (); 

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, _Width, 0, _Height, 0.0, 1000.0);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();	
		gluLookAt(0, 0, 500.0f,	0, 0, -10.0f, 	0.0f, 1.0f, 0.0f);
		


		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, _textureBG);
		glColor4f(1.0,1.0,1.0,1.0);
		glBegin(GL_QUADS);

		
		glTexCoord2f(0.0f, 0.0f);glVertex3f( 0.0, _Height-0.0, -1.0);
		glTexCoord2f(1.0f, 0.0f);glVertex3f( _textureBG_Width, _Height-0.0, -1.0);
		glTexCoord2f(1.0f, 1.0f);glVertex3f( _textureBG_Width, _Height-_textureBG_Height, -1.0);
		glTexCoord2f(0.0f, 1.0f);glVertex3f( 0.0, _Height-_textureBG_Height, -1.0);
		
		
		glEnd(); 
		glDisable(GL_TEXTURE_2D);


		glPopMatrix ();
		
		
		//////////////////////////////////////////////////////////////////////////
		glPushMatrix ();
		

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective( _pD.fieldOfView, _pD.aspect, _pD.nearPlane, _pD.farPlane);


		glMatrixMode(GL_MODELVIEW); 
		glLoadIdentity();
		
		//center + zoom
		glTranslatef(-0, 0, -10);
		glRotatef(-50+_theta, 1.0f, 0.0f, 0.0f);
		glRotatef(-0+_theta, 0.0f, 0.0f, 1.0f);

		//Draw_Plane(5);

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, _textureImage);
		glColor4f(1.0,1.0,1.0,1.0);
		glBegin(GL_QUADS);


		GLint base;
		if (_textureImage_Width > _textureImage_Height){
			base = _textureImage_Height;
		}else{
			base = _textureImage_Width;
		}

		glTexCoord2f(0.0f, 0.0f);glVertex3f( -_textureImage_Width/base,  _textureImage_Height/base, 1.0);
		glTexCoord2f(1.0f, 0.0f);glVertex3f(  _textureImage_Width/base,  _textureImage_Height/base, 1.0);
		glTexCoord2f(1.0f, 1.0f);glVertex3f(  _textureImage_Width/base, -_textureImage_Height/base, 1.0);
		glTexCoord2f(0.0f, 1.0f);glVertex3f( -_textureImage_Width/base, -_textureImage_Height/base, 1.0);


		glEnd(); 
		glDisable(GL_TEXTURE_2D);
		
		glPopMatrix ();
		
		//////////////////////////////////////////////////////////////////////////


		SwapBuffers (_hDC); 

		_theta += 1.0f; 

		glReadBuffer(GL_FRONT);

		BitBlt(GetDC(_hWnd), 0, 0, _Width, _Height, _hDC, 0, 0, SRCCOPY);
	}

	//image
	bool LoadTexture(const WCHAR *pszFilename, GLuint *pTexture, GLint *pTextureWidth, GLint *pTextureHeight)
	{
		return LoadTexture(pszFilename, pTexture, pTextureWidth, pTextureHeight, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, GL_REPEAT, GL_REPEAT);
	}

	bool LoadTexture(const WCHAR *pszFilename, GLuint *pTexture, GLint *pTextureWidth, GLint *pTextureHeight, GLint magFilter, GLint minFilter, GLint wrapS, GLint wrapT)
	{
		// GDI+ requires unicode strings.
		// This trick only works for strings containing English ASCII text.
		/*
		std::string asciiFilename(pszFilename);
		std::wstring unicodeFilename(asciiFilename.length(), L' ');
		std::copy(asciiFilename.begin(), asciiFilename.end(), unicodeFilename.begin());
*/
		//std::tr1::shared_ptr<Gdiplus::Bitmap> pBitmap(Gdiplus::Bitmap::FromFile(unicodeFilename.c_str(), FALSE));

		Gdiplus::Bitmap *pBitmap =  Gdiplus::Bitmap::FromFile(pszFilename, FALSE);

		
		//if (pBitmap.get() == 0)
		//	return false;

		// GDI+ orients bitmap images top-bottom.
		// OpenGL expects bitmap images to be oriented bottom-top by default.
		//pBitmap->RotateFlip(Gdiplus::RotateNoneFlipY);

		// GDI+ pads each scanline of the loaded bitmap image to 4-byte memory
		// boundaries. Fortunately OpenGL also aligns bitmap images to 4-byte
		// memory boundaries by default.
		int width = pBitmap->GetWidth();
		int height = pBitmap->GetHeight();
		int pitch = ((width * 32 + 31) & ~31) >> 3;

		std::vector<unsigned char> pixels(pitch * height);
		Gdiplus::BitmapData data;
		Gdiplus::Rect rect(0, 0, width, height);

		// Convert to 32-bit BGRA pixel format and fetch the pixel data.

		if (pBitmap->LockBits(&rect, Gdiplus::ImageLockModeRead, PixelFormat32bppARGB, &data) != Gdiplus::Ok)
			return false;

		if (data.Stride == pitch)
		{
			memcpy(&pixels[0], data.Scan0, pitch * height);
		}
		else
		{
			unsigned char *pSrcPixels = static_cast<unsigned char *>(data.Scan0);

			for (int i = 0; i < height; ++i)
				memcpy(&pixels[i * pitch], &pSrcPixels[i * data.Stride], pitch);
		}

		pBitmap->UnlockBits(&data);

		// Create an OpenGL texture object to store the loaded bitmap image.

		glGenTextures(1, pTexture);
		glBindTexture(GL_TEXTURE_2D, *pTexture);

	
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);

		gluBuild2DMipmaps(GL_TEXTURE_2D, 4, width, height, GL_BGRA_EXT, GL_UNSIGNED_BYTE, &pixels[0]);

		(*pTextureWidth) = width;
		(*pTextureHeight) = height;

		return true;
	}

private:
	HWND	_hWnd;
	HDC		_hDC;   //Handle to a device context
	HGLRC	_hRC;   //Handle to an OpenGL rendering context 
	int     _Width;
	int     _Height;
	ULONG_PTR           _gdiplusToken;

	perspectiveData		_pD;

	float		_theta;

	GLuint      _textureBG;
	GLint       _textureBG_Width;
	GLint       _textureBG_Height;

	GLuint		_textureImage;
	GLint		_textureImage_Width;
	GLint		_textureImage_Height;
};

#endif