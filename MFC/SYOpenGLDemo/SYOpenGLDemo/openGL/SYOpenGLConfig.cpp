#include "stdafx.h"
#include "SYOpenGLConfig.h"

SYOpenGLConfig* SYOpenGLConfig::m_instance = NULL;

SYOpenGLConfig* SYOpenGLConfig::Instance()
{
	if (m_instance == NULL){
		m_instance = new SYOpenGLConfig(); 
	}
	return m_instance;
}

SYOpenGLConfig::SYOpenGLConfig()
{

}

SYOpenGLConfig::~SYOpenGLConfig()
{

}
