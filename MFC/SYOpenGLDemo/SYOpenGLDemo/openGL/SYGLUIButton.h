
#ifndef _SYOPENG_UIBUTTON_H_
#define _SYOPENG_UIBUTTON_H_

#include "SYGLUIView.h"
#include "SYGLUILabel.h"

enum SYGLUIBUTTON {
	SYGLUIBUTTON_NORMAL = 0,
	SYGLUIBUTTON_DOWN,
	SYGLUIBUTTON_OVER,
	SYGLUIBUTTON_DISABLE
};


class SYGLUIButton : public SYGLUIView
{
private:
	typedef SYGLUIView super_t;

	SYTexture2D *m_texture_Norm;	//no release
	SYTexture2D *m_texture_Down;	//no release
	SYTexture2D *m_texture_Over;	//no release
	SYTexture2D *m_texture_Disb;	//no release

	bool flag_Down;
	bool flag_Enable;

	SYGLUIBUTTON cur_status;
protected:
	SYGLUILabel *m_lblCaption;


public:
	__event void OnEventClicked(SYGLUIButton *sender, int x, int y);

public:
	SYGLUIButton(float x, float y, const WCHAR *pwszNormal, const WCHAR *pwszDown, const WCHAR *pwszOver, const WCHAR *pwszDisable)
		:super_t(x, y , 1 , 1)
	{	
	
		this->flag_Down = false;
		this->flag_Enable = true;

		m_texture_Norm = SYOpenGLResource::Instance()->LoadTexture(pwszNormal);
		this->setImageWithTexture(m_texture_Norm);

		if (pwszDown == NULL)
		{
			m_texture_Down = m_texture_Norm;
		}else{
			m_texture_Down = SYOpenGLResource::Instance()->LoadTexture(pwszDown);		
		}
	
		if (pwszOver == NULL)
		{
			m_texture_Over = m_texture_Norm;
		}else{
			m_texture_Over = SYOpenGLResource::Instance()->LoadTexture(pwszOver);		
		}

		if (pwszDisable == NULL)
		{
			m_texture_Disb = m_texture_Norm;
		}else{
			m_texture_Disb = SYOpenGLResource::Instance()->LoadTexture(pwszDisable);		
		}

		
		this->m_lblCaption = new SYGLUILabel(0, 0, m_texture_Norm->width-2, 1, L"");
		this->addSubview(m_lblCaption);
	}

	virtual ~SYGLUIButton()
	{
		
	}

	virtual void onClicked(){};
	//////////////////////////////////////////////////////////////////////////
	
	
	void touchDown(int x, int y)
	{
		if (!flag_Enable) return;
		flag_Down = true;
		this->setImageWithTexture(m_texture_Down);
	}

	void touchUp(int x, int y)
	{
		if (!flag_Enable) return;
		if (flag_Down == false)	return;
		flag_Down = false;
		__raise OnEventClicked(this, x, y);
		this->onClicked();
		if (flag_Enable) this->setImageWithTexture(m_texture_Norm);
	}

	void touchMove(int x, int y)
	{
		if (!flag_Enable) return;
		if (flag_Down == true)	return;
		this->setImageWithTexture(m_texture_Over);
	}

	virtual void touchOutside()
	{
		if (!flag_Enable) return;
		if (flag_Down) flag_Down = false;
		this->setImageWithTexture(m_texture_Norm);
	}

	//////////////////////////////////////////////////////////////////////////
	void setEnable(bool enable)
	{
		flag_Enable = enable;

		if (flag_Enable)
		{
			this->setImageWithTexture(m_texture_Norm);
		}else{
			this->setImageWithTexture(m_texture_Disb);
		}
	}

	void setImageWithStatus(SYGLUIBUTTON status, const WCHAR *pwszFilePath)
	{
		switch (status)
		{
		case SYGLUIBUTTON_NORMAL:
			m_texture_Norm = SYOpenGLResource::Instance()->LoadTexture(pwszFilePath);
			break;

		case SYGLUIBUTTON_DOWN:
			m_texture_Down = SYOpenGLResource::Instance()->LoadTexture(pwszFilePath);
			break;

		case SYGLUIBUTTON_OVER:
			m_texture_Over = SYOpenGLResource::Instance()->LoadTexture(pwszFilePath);
			break;

		case SYGLUIBUTTON_DISABLE:
			m_texture_Disb = SYOpenGLResource::Instance()->LoadTexture(pwszFilePath);
			break;
		}
	}
	
	void setCaptionWithStatus(SYGLUIBUTTON status, const WCHAR *pwszCaption)
	{
		this->m_lblCaption->setText(pwszCaption);
		this->m_lblCaption->Y = (m_texture_Norm->height-this->m_lblCaption->H)/2;		
	}

	virtual void layoutSubviews()
	{
		
	}

};

#endif