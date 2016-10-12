#include "stdafx.h"
#include "SYOpenGLResource.h"
#include <math.h>


SYOpenGLResource* SYOpenGLResource::m_instance = NULL;

SYOpenGLResource* SYOpenGLResource::Instance()
{
	if (m_instance == NULL){
		m_instance = new SYOpenGLResource(); 
	}
	return m_instance;
}

SYOpenGLResource::SYOpenGLResource()
{
	
}

SYOpenGLResource* SYOpenGLResource::Instance(WCHAR *pwszPath)
{
	if (m_instance == NULL){
		m_instance = new SYOpenGLResource(pwszPath); 
	}
	return m_instance;
}

SYOpenGLResource::SYOpenGLResource(WCHAR *pwszPath)
{
	strRootPath = pwszPath;
	OutputDebugStringW(strRootPath.c_str());
	
	m_pvecTexture = new std::vector<SYTexture2D *>;
}


SYOpenGLResource::~SYOpenGLResource()
{

	while(!m_pvecTexture->empty())
	{
		SYTexture2D *texture = m_pvecTexture->back();
		glDeleteTextures(1, &texture->texture);
		delete texture;
		m_pvecTexture->pop_back();
	}
	delete m_pvecTexture;
}

bool SYOpenGLResource::IsPowerOfTwo(int x) 
{
	return (x != 0) && ((x & (x - 1)) == 0);
}

int SYOpenGLResource::NextPowerOfTwo(int minimum) 
{
	if (IsPowerOfTwo(minimum)) {
		return minimum;
	}
	int i = 0;
	while (true) {
		i++;
		if (pow(2.0f, i) >= minimum) {
			return (int) pow(2.0f, i);
		}
	}
}


inline int next_p2 (int a )
{
	int rval=1;
	// rval<<=1 Is A Prettier Way Of Writing rval*=2;
	while(rval<a) rval<<=1;
	return rval;
}

SYTexture2D *SYOpenGLResource::IsExistTextureWithFilePath(const WCHAR *pwszFilePath)
{
	std::vector<SYTexture2D *>::iterator it;
	for (it=m_pvecTexture->begin(); it<m_pvecTexture->end(); it++)
	{
		if ( _wcsicmp( (*it)->filePath.c_str(), pwszFilePath) == 0 )
		{
			return (*it);
		}
	}//end for
	return NULL;
}

SYTexture2D *SYOpenGLResource::LoadTexture(const WCHAR *pwszFilePath)
{
	
	SYTexture2D *result = IsExistTextureWithFilePath(pwszFilePath);
	if ( result != NULL)
	{		
		return result;
	}

	// GDI+ requires unicode strings.
	// This trick only works for strings containing English ASCII text.
	/*
	std::string asciiFilename(pszFilename);
	std::wstring unicodeFilename(asciiFilename.length(), L' ');
	std::copy(asciiFilename.begin(), asciiFilename.end(), unicodeFilename.begin());
	*/
	//std::tr1::shared_ptr<Gdiplus::Bitmap> pBitmap(Gdiplus::Bitmap::FromFile(unicodeFilename.c_str(), FALSE));

	//GLuint *pTexture, GLint *pTextureWidth, GLint *pTextureHeight, GLint magFilter, GLint minFilter, GLint wrapS, GLint wrapT
	Gdiplus::Bitmap *pBitmap =  Gdiplus::Bitmap::FromFile((strRootPath +pwszFilePath).c_str(), FALSE);


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

	SYTexture2D *newTexture = new SYTexture2D();
	this->m_pvecTexture->push_back(newTexture);

	newTexture->filePath = pwszFilePath;


	glEnable(GL_ALPHA_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);


	glGenTextures(1, &newTexture->texture);
	glBindTexture(GL_TEXTURE_2D, newTexture->texture);


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	int glWidth = next_p2(pBitmap->GetWidth());
	int glHeight = next_p2(pBitmap->GetHeight());
	newTexture->texWidth = glWidth;
	newTexture->texHeight = glHeight;


	GLubyte* pBuf = new GLubyte[glWidth * glHeight * 4];
	
	for (int i=0; i<glWidth * glHeight; i++)
	{
		//B
		pBuf[4*i] = 0;  
		//G
		pBuf[4*i+1] = 0;
		//R
		pBuf[4*i+2] = 0;
		//A
		pBuf[4*i+3] = 0;

		int x = i % glWidth;
		int y = ceil((float)i / (float)glWidth);

		if (x<width && y<height)
		{
			//B
			pBuf[4*i] = pixels[4*(x+y*width)];  
			//G
			pBuf[4*i+1] = pixels[4*(x+y*width)+1];
			//R
			pBuf[4*i+2] = pixels[4*(x+y*width)+2];
			//A
			pBuf[4*i+3] = pixels[4*(x+y*width)+3];
		}		
	}

	gluBuild2DMipmaps(GL_TEXTURE_2D, 4, glWidth, glHeight, GL_BGRA_EXT, GL_UNSIGNED_BYTE, pBuf);

	delete pBuf;

	newTexture->width = width;
	newTexture->height = height;

	return newTexture;
}