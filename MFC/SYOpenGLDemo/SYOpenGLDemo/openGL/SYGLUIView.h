
#ifndef _SYOPENG_UIVIEW_H_
#define _SYOPENG_UIVIEW_H_

//#include "SYOpenGLConfig.h"
#include "SYOpenGLResource.h"
#include <vector>

/*
__hook(&SYGLUIView::OnEventMouseDown, m_view, &CSYOpenGLDemoDlg::onDidMouseDown);
__hook(&SYGLUIView::OnEventMouseUp, m_view, &CSYOpenGLDemoDlg::onDidMouseUp);
__hook(&SYGLUIView::OnEventMouseMove, m_view, &CSYOpenGLDemoDlg::onDidMouseMove);

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

__unhook(&SYGLUIView::OnEventMouseDown, m_view, &CSYOpenGLDemoDlg::onDidMouseDown);
__unhook(&SYGLUIView::OnEventMouseUp, m_view, &CSYOpenGLDemoDlg::onDidMouseUp);
__unhook(&SYGLUIView::OnEventMouseMove, m_view, &CSYOpenGLDemoDlg::onDidMouseMove);
*/


class SYGLUIView
{
protected:
	float m_color[4];
	Rect m_touchRect;
	std::vector<SYGLUIView *> *m_pvecSubview;
	SYTexture2D *m_texture; //no release
	bool flag_IsTouch;

	//__declspec(property(get = GetPosition, put = SetPosition)) int Position;
public:
	// var
	float X;
	float Y;
	float W;
	float H;
	std::vector<SYGLUIView *> *Superview; //no release 

	boolean Visible ;
	std::wstring name;

	__event void OnEventMouseDown(SYGLUIView *sender, int x, int y);
	__event void OnEventMouseUp(SYGLUIView *sender, int x, int y);
	__event void OnEventMouseMove(SYGLUIView *sender, int x, int y);
	//event
	//typedef void (__thiscall *EventMouseOver)(SYGLUIView *sender, int x, int y);
	//void (__cdecl *OnEventMouseOver)(SYGLUIView *sender, int x, int y);

/*
	

	template < class T >
	static void setE(void (T::*TMethod)(SYGLUIView *sender, int x, int y))
	{
		
		TMethod(NULL, 0, 0);

		//__hook(&SYGLUIView::OnEventMouseOver, this, (T::*p));

		//T* p = static_cast< T* >(object_ptr);
		
		//OnEventMouseOverPtr = &(p->onDidMouseOver);
	}

	*/
	
/*
	template < class T, void (T::*TMethod)(void) >
	static void setE(void* object_ptr)
	{
		T* p = static_cast< T* >(object_ptr);
		OnEventMouseOverPtr = (p->*TMethod);
	}
	*/
	/*
	

	 void setevent( template < class T, void (T::*TMethod)(OnEventMouseOver *even) > )
	 {
		__hook(&SYGLUIView::EventMouseOver, &this, even);
	 }
*/

	//


	virtual ~SYGLUIView()
	{
		
		//OutputDebugStringW((this->name + L"delete\n").c_str());
		while(!m_pvecSubview->empty())
		{
			delete (m_pvecSubview->back());
			m_pvecSubview->pop_back();
		}
		delete m_pvecSubview;

		//Superview = NULL;

		
	}

	//method
	SYGLUIView(float x, float y, float w, float h) 
	{
		// TODO Auto-generated method 
		
		this->setBackgroundColor(255, 255, 255, 255);
		this->Visible = true;

		this->X = x;
		this->Y = y;
		this->W = w;
		this->H = h;
		this->Superview = NULL;
		//m_resImageID = -1;

		this->m_pvecSubview = new std::vector<SYGLUIView *>;
		this->m_texture = NULL;
		this->flag_IsTouch = false;
	}

	void setAlpha(int alpha)
	{
		// 0~255
		if (alpha>255) alpha = 255;
		m_color[3] = (float) alpha / 255.0f;
	}

	void setBackgroundColor(int r, int g, int b) 
	{
		// 0~255
		if (r>255) r = 255;
		if (g>255) g = 255;
		if (b>255) b = 255;
	
		m_color[0] = (float) r / 255.0f;
		m_color[1] = (float) g / 255.0f;
		m_color[2] = (float) b / 255.0f;
	}

	void setBackgroundColor(int r, int g, int b, int a) 
	{
		this->setBackgroundColor(r, g, b);
		this->setAlpha(a);
	}

	void setImageWithFilePath(const WCHAR *pwszFilePath)
	{
		m_texture = SYOpenGLResource::Instance()->LoadTexture(pwszFilePath);
		this->W = m_texture->width;
		this->H = m_texture->height;
	}

	void setImageWithTexture(SYTexture2D *pTexture)
	{
		m_texture = pTexture;
		this->W = m_texture->width;
		this->H = m_texture->height;
	}


	std::vector<SYGLUIView *> *getSubviews() { return m_pvecSubview; }


	virtual void onDrawFrame(float DisplayX, float DisplayY, float DisplayW, float DisplayH, 
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
			glDisable(GL_DEPTH_TEST);

			if (m_texture != NULL)
			{				
				float UX = offsetUVx / (float)m_texture->texWidth;
				float UY = offsetUVy / (float)m_texture->texHeight;
				float UW = UX + ((w - x)/m_texture->width)*(m_texture->width / (float)m_texture->texWidth);
				float UV = UY + ((h - y)/m_texture->height)*(m_texture->height / (float)m_texture->texHeight);
				
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, m_texture->texture);
				glBegin(GL_QUADS);				
				glTexCoord2f(UX, UY);glVertex3f( x, (y), 0.0);	//左上
				glTexCoord2f(UW, UY);glVertex3f( w, (y), 0.0);	//右上
				glTexCoord2f(UW, UV);glVertex3f( w, (h), 0.0);	//右下
				glTexCoord2f(UX, UV);glVertex3f( x, (h), 0.0);	//左下
				glEnd(); 
				glDisable(GL_TEXTURE_2D);
				
			}else{
				
				glBindTexture(GL_TEXTURE_2D, 0);
				glBegin(GL_QUADS);
				glVertex3f( x, y ,0.0);
				glVertex3f( w, y, 0.0);
				glVertex3f( w, h, 0.0);
				glVertex3f( x, h, 0.0);
				glEnd();				
				
			}

			

			
			glEnable(GL_DEPTH_TEST);
			glColor4f(1.0, 1.0, 1.0, 1.0);
			
			//////////////////////////////////////////////////////////////////////////
			glPopMatrix();

			
		}//end if (bIsIntersect)

		this->layoutSubviews();

		for (unsigned int i = 0; i < m_pvecSubview->size(); i++) {
			SYGLUIView *v = m_pvecSubview->at(i);
			v->onDrawFrame(x, y, (w - x), (h - y), this->X,	this->Y);
		}// end for(m_subview.size)
	}//end sub onDrawFrame


	bool IsIntersect(int x, int y)
	{
		return (m_touchRect.Contains(x, y));		
	}

	SYGLUIView *OnMouseDown(int x, int y)
	{
		this->flag_IsTouch = false;

		std::vector<SYGLUIView *>::iterator it;
		for (it=m_pvecSubview->begin(); it<m_pvecSubview->end(); it++)
		{
			SYGLUIView *view = (*it);
			if (view->OnMouseDown(x, y)!=NULL)
			{
				return view;
			}
		}//end for

		if (m_touchRect.Contains(x, y))
		{
			this->flag_IsTouch = true;
			__raise this->OnEventMouseDown(this, x-m_touchRect.X, y-m_touchRect.Y);
			this->touchDown(x-m_touchRect.X, y-m_touchRect.Y);
			return this;
		}
		return NULL;	
	}

	SYGLUIView *OnMouseUp(int x, int y)
	{
		if (this->flag_IsTouch == false)
		{
			std::vector<SYGLUIView *>::iterator it;
			for (it=m_pvecSubview->begin(); it<m_pvecSubview->end(); it++)
			{
				SYGLUIView *view = (*it);
				if (view->OnMouseUp(x, y) != NULL)
				{
					return view;
				}
			}//end for
		}else{
			if (m_touchRect.Contains(x, y))
			{
				__raise this->OnEventMouseUp(this, x-m_touchRect.X, y-m_touchRect.Y);
				this->touchUp(x-m_touchRect.X, y-m_touchRect.Y);
				return this;
			}

		}
		
		this->flag_IsTouch = false;
		
		return NULL;	
	}


	SYGLUIView *OnMouseMove(int x, int y)
	{
		
		std::vector<SYGLUIView *>::iterator it;
		for (it=m_pvecSubview->begin(); it<m_pvecSubview->end(); it++)
		{
			SYGLUIView *view = (*it);
			if (view->OnMouseMove(x, y) != NULL)
			{
				this->touchOutside();
				return view;
			}
		}//end for

		if (m_touchRect.Contains(x, y))
		{
			__raise this->OnEventMouseMove(this, x-m_touchRect.X, y-m_touchRect.Y);
			this->touchMove(x-m_touchRect.X, y-m_touchRect.Y);
			return this;
		}
		this->touchOutside();
		return NULL;	
	}

	

	

	void addSubview(SYGLUIView *view) 
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
	}//end sub addSubview






	virtual void layoutSubviews(){};

	virtual void touchDown(int x, int y){};
	virtual void touchUp(int x, int y){};
	virtual void touchMove(int x, int y){};
	virtual void touchOutside(){};
private:
	
};

#endif