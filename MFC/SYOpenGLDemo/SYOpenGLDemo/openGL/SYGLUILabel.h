
#ifndef _SYOPENG_UILABEL_H_
#define _SYOPENG_UILABEL_H_

//#include "SYOpenGLConfig.h"
#include "SYGLUIView.h"
#include <vector>
#include "xFreeTypeLib.h"


class SYGLUILabel : public SYGLUIView
{
private:
	typedef SYGLUIView super_t;
	int m_fontSize;
	std::wstring m_caption;

	bool flag_Wrap;

public:
	// var
	//method
	SYGLUILabel(float x, float y, float w, float h, const wchar_t *strText, int fontSize = 24) 
	:super_t(x, y , w , h)
	{
		// TODO Auto-generated method 
		this->m_fontSize = fontSize;
		this->setText(strText);
		this->flag_Wrap = false;
	}

	virtual ~SYGLUILabel()
	{
		//OutputDebugStringW(L"lbl release\n");
	}

	void setWrap(bool wrap)
	{
		this->flag_Wrap = wrap;
	}

	void setText(const wchar_t *strText)
	{
		xFreeTypeLib::Instance()->createStrintTexture(strText, this->m_fontSize);
		this->m_caption = strText;
	}

	void setFontColor(int r, int g, int b) 
	{
		this->setBackgroundColor(r, g, b);		
	}

	void setFontColor(int r, int g, int b, int a) 
	{
		this->setFontColor(r, g, b);
		this->setAlpha(a);
	}

	

	void onDrawFrame(float DisplayX, float DisplayY, float DisplayW, float DisplayH, 
		float ParentX, float ParentY) 
	{
		if (Visible == false) {
			return;
		}


		float x = this->X + DisplayX;
		float y = this->Y + DisplayY;
		float w = x + this->W;
		float h = y + this->H;

		if (ParentX < 0) {
			x += ParentX;
		}

		if (ParentY < 0) {
			y += ParentY;
		}


		float offsetUVx = 0.0f;
		float offsetUVy = 0.0f;

		if (x < DisplayX) {
			offsetUVx = DisplayX - x;
			x = DisplayX;
		}

		if (y < DisplayY) {
			offsetUVy = DisplayY - y;
			y = DisplayY;
		}

		if (w > DisplayX + DisplayW) {
			w = DisplayX + DisplayW;
		}

		if (h > DisplayY + DisplayH) {
			h = DisplayY + DisplayH;
		}

		m_touchRect.X = x;
		m_touchRect.Y = y;
		m_touchRect.Width =  (w - x);
		m_touchRect.Height = (h - y);

		boolean bIsIntersect = true;
		if (x > w) {
			bIsIntersect = false;
		}
		if (y > h) {
			bIsIntersect = false;
		}

		if (w < DisplayX) {
			bIsIntersect = false;
		}

		if (h < DisplayY) {
			bIsIntersect = false;
		}


		if (bIsIntersect) 
		{
			glPushMatrix();

			//////////////////////////////////////////////////////////////////////////
			glColor4f(m_color[0], m_color[1], m_color[2], m_color[3]);
		
			int maxW = xFreeTypeLib::Instance()->drawText(this->m_caption.c_str(), x, y, w, h, this->m_fontSize, this->flag_Wrap);
			
		
			glColor4f(1.0, 1.0, 1.0, 1.0);			
			//////////////////////////////////////////////////////////////////////////
			glPopMatrix();
			
		}//end if (bIsIntersect)

		this->layoutSubviews();

		for (unsigned int i = 0; i < this->getSubviews()->size(); i++) {
			SYGLUIView *v = this->getSubviews()->at(i);
			v->onDrawFrame(x, y, (w - x), (h - y), this->X,	this->Y);
		}// end for(m_subview.size)
	}//end sub onDrawFrame



	virtual void layoutSubviews(){};
	virtual void touchDown(int x, int y){};
	virtual void touchUp(int x, int y){};
	virtual void touchMove(int x, int y){};
	virtual void touchOutside(){};
private:
	
};

#endif