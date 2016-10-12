
#ifndef _SYOPENGCONFIG_H_
#define _SYOPENGCONFIG_H_


class SYOpenGLConfig
{
public:
	static SYOpenGLConfig* Instance(void);
	~SYOpenGLConfig();

public:
	int winWidth;
	int winHeight;
	int dynamicWidth;
	int dynamicHeight;

	int screenWidth;
	int screenHeight;
private:
	static SYOpenGLConfig*	m_instance;

private:
	SYOpenGLConfig();

};

#endif