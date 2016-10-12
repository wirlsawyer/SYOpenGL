
#ifndef _XFREETYPELIB_H_
#define _XFREETYPELIB_H_



#include <map>
#include <string>
#include <sstream>
#include <GL\glut.h>//使用DevC++的話要改為標入 #include <GL\openglut.h> 

//http://www.freetype.org/freetype2/docs/reference/ft2-base_interface.html
#include <ft2build.h>
#include FT_FREETYPE_H  
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <freetype/ftoutln.h>
#include <freetype/fttrigon.h>
#pragma comment (lib, "freetype250MT_d.lib")


class xCharTexture
{
public:
	GLuint  m_texID;
	wchar_t m_chaID;
	int     m_Width;
	int     m_Height;
	int     m_adv_x;
	int     m_adv_y;
	int     m_delta_x;
	int     m_delta_y;
public:
	xCharTexture()
	{
		m_texID  = 0;
		m_chaID  = 0;
		m_Width  = 0;
		m_Height = 0;
	}
};

class xFreeTypeLib
{
private:
	static xFreeTypeLib*	m_instance;
	xFreeTypeLib();

private:
	FT_Library m_FT2Lib;
	FT_Face    m_FT_Face;
	int   m_w;
	int   m_h;
	std::map<std::wstring, xCharTexture*> m_MapTexData; 
	
public:
	static xFreeTypeLib* Instance(void);
	~xFreeTypeLib();

	void setFontWithSize(const char* font_file , int _w = 24, int _h = 24)
	{
		
		//FT_Library library;
		if (FT_Init_FreeType( &m_FT2Lib) ) 
			exit(0);

		m_FT_Face = NULL;
		
		//載入 一個 字體,取預設 的Face,一般為
		if (FT_New_Face( m_FT2Lib, font_file, 0, &m_FT_Face )) 
			exit(0);

		
		FT_Select_Charmap(m_FT_Face, FT_ENCODING_UNICODE);
		m_w = _w ; m_h = _h;
		//m_FT_Face->num_fixed_sizes;
		//大小要乘.這m_h);
		//FT_Set_Char_Size( m_FT_Face , 0 , m_w << 6, 96, 96);
		FT_Set_Pixel_Sizes(m_FT_Face, m_w, m_h);
	}

	inline int next_p2 (int a)
	{
		int rval=1;
		// rval<<=1 Is A Prettier Way Of Writing rval*=2;
		while(rval<a) rval<<=1;
		return rval;
	}

	
	std::wstring int2str(int &i) 
	{
		std::wstring s;
		std::wstringstream  ss(s);		  
		ss << i;	  
		return ss.str();	
	}

	GLuint loadChar(wchar_t ch, int fontSize = 24)
	{

		wchar_t buf[2];
		buf[0] = ch;
		buf[1] = '\0';
		std::wstring key = buf;
		key.append(L",");
		key.append(int2str(fontSize));
		memset(buf, 0, sizeof(buf));

		
		std::map<std::wstring, xCharTexture*>::iterator iter = m_MapTexData.find(key);

		if (iter != m_MapTexData.end())
		{
			return (*iter).second->m_texID;
		}

		
		FT_Set_Pixel_Sizes(m_FT_Face, fontSize, fontSize);

		if(FT_Load_Char(m_FT_Face, ch, FT_LOAD_RENDER | FT_LOAD_FORCE_AUTOHINT |
			(TRUE ? FT_LOAD_TARGET_NORMAL : FT_LOAD_MONOCHROME | FT_LOAD_TARGET_MONO) )   )
		{
			return 0;
		}

		// Load The Glyph For Our Character.
		//if(FT_Load_Glyph( m_FT_Face, FT_Get_Char_Index( m_FT_Face, ch ), FT_LOAD_FORCE_AUTOHINT ))
		{
			//throw std::runtime_error("FT_Load_Glyph failed");
		}


		xCharTexture *newChar = new xCharTexture();
		m_MapTexData.insert(std::map<std::wstring, xCharTexture*>::value_type(key, newChar)); 

		//xCharTexture& charTex = m_TexID[ch];

		//得到字模
		FT_Glyph glyph;
		if(FT_Get_Glyph( m_FT_Face->glyph, &glyph ))
			return 0;
		//轉化成點陣圖 		FT_Render_Glyph( m_FT_Face->glyph,   FT_RENDER_MODE_LCD );//FT_RENDER_MODE_NORMAL  ); 
		FT_Glyph_To_Bitmap( &glyph, ft_render_mode_normal, 0, 1 );
		FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)glyph;
		//取道點陣圖 ?據
		FT_Bitmap &bitmap = bitmap_glyph->bitmap;
		//把點陣圖 ?據拷貝 自己定義 的數 據區 裡.這樣舊可以畫 到需要的東 西上面了。
		int width  =  next_p2(bitmap.width);
		int height =  next_p2(bitmap.rows);
		m_FT_Face->size->metrics.y_ppem;
		m_FT_Face->glyph->metrics.horiAdvance;

		newChar->m_Width = width;
		newChar->m_Height = height;
		newChar->m_adv_x = m_FT_Face->glyph->advance.x / 64.0f;
		newChar->m_adv_y = m_FT_Face->size->metrics.y_ppem; //m_FT_Face->glyph->metrics.horiBearingY / 64.0f;
		newChar->m_delta_x = (float)bitmap_glyph->left;
		newChar->m_delta_y = (float)bitmap_glyph->top - height;

		glEnable(GL_ALPHA_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

		glGenTextures(1, &newChar->m_texID);
		glBindTexture(GL_TEXTURE_2D, newChar->m_texID);
		GLubyte* pBuf = new GLubyte[width * height * 4];
		for(int j=0; j  < height ; j++)
		{
			for(int i=0; i < width; i++)
			{
				GLubyte _vl =  (i>=bitmap.width || j>=bitmap.rows) ? 0 : bitmap.buffer[i + bitmap.width*j];
				pBuf[(4*i + (height - j - 1) * width * 4)  ] = 0xFF;
				pBuf[(4*i + (height - j - 1) * width * 4)+1] = 0xFF;
				pBuf[(4*i + (height - j - 1) * width * 4)+2] = 0xFF;
				pBuf[(4*i + (height - j - 1) * width * 4)+3] = _vl;
			}
		}
	
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		gluBuild2DMipmaps(GL_TEXTURE_2D, 4, width, height, GL_BGRA_EXT, GL_UNSIGNED_BYTE, pBuf);
		
		delete[] pBuf;
	    FT_Done_Glyph( glyph ); 
		//_CrtSetBreakAlloc(2731);
		
		return newChar->m_chaID;

	}
	xCharTexture* getTextChar(wchar_t ch, int fontSize = 24)
	{
		//this->loadChar(ch, fontSize);
		wchar_t buf[2];
		buf[0] = ch;
		buf[1] = '\0';
		std::wstring key = buf;
		key.append(L",");
		key.append(int2str(fontSize));
		memset(buf, 0, sizeof(buf));

		std::map<std::wstring, xCharTexture*>::iterator iter = m_MapTexData.find(key);
	
		if (iter != m_MapTexData.end())
		{
			return (*iter).second;
		}
		
		return NULL;
		//return &m_TexID[ch];
	}


	void createStrintTexture(const wchar_t *strText, int fontSize)
	{
		FT_Set_Pixel_Sizes(m_FT_Face, fontSize, fontSize);
		
		size_t nLen = wcslen(strText);
		for (unsigned int i = 0 ; i <nLen ; i ++)
		{		
			
			this->loadChar(strText[i], fontSize);
		}
	}

	int drawText(const wchar_t* _strText,int x , int y, int maxW , int h, int fontSize = 24, bool wrap = true)
	{
		

		int sx = x;
		int sy = y;
		int maxH = h;
		int showWidth = 0;
		size_t nLen = wcslen(_strText);


		glPushMatrix();

		for (unsigned int i = 0 ; i <nLen ; i ++)
		{
			
			
			glColor4f(1.0, 0.0, 0.0, 1.0);
			//glScalef(1.0f/2, 1.0f/2, 1.0f);

			xCharTexture* pCharTex = this->getTextChar(_strText[i], fontSize);

			if (wrap)
			{
				if(_strText[i] =='\n')
				{
					sx = x ; sy += pCharTex->m_Height;//maxH + 12;
					continue;
				}
			}
			

	
			if (pCharTex == NULL) return 0;

			glDisable(GL_DEPTH_TEST);			
			glEnable(GL_TEXTURE_2D);			
			glBindTexture(GL_TEXTURE_2D, pCharTex->m_texID);	
			
			
			int w = pCharTex->m_Width;
			int h = pCharTex->m_Height;
			int ch_x = sx + pCharTex->m_delta_x;
			int ch_y = sy - h - pCharTex->m_delta_y;
			if(maxH < h) maxH = h;
			glBegin ( GL_QUADS );
			{
				glTexCoord2f(0.0f, 1.0f); glVertex3f(ch_x     , ch_y    ,  0.0f);
				glTexCoord2f(1.0f, 1.0f); glVertex3f(ch_x +  w, ch_y    ,  0.0f);
				glTexCoord2f(1.0f, 0.0f); glVertex3f(ch_x +  w, ch_y + h,  0.0f);
				glTexCoord2f(0.0f, 0.0f); glVertex3f(ch_x     , ch_y + h,  0.0f);
			}

			glEnd();


			sx += pCharTex->m_adv_x;
			if ( (sx+pCharTex->m_Width) > showWidth)
			{
				showWidth = (sx+pCharTex->m_Width);
			}

			if(sx > x + maxW)
			{
				if (wrap){
					sx = x ; sy += pCharTex->m_Height;
				}else{
					glEnable(GL_DEPTH_TEST);
					glPopMatrix();
					return showWidth;
				}				
			}
		}
		glEnable(GL_DEPTH_TEST);
		glPopMatrix();

		return showWidth;
	}
	

};

#endif