#include "stdafx.h"
#include "xFreeTypeLib.h"

xFreeTypeLib* xFreeTypeLib::m_instance = NULL;

xFreeTypeLib* xFreeTypeLib::Instance()
{
	if (m_instance == NULL){
		m_instance = new xFreeTypeLib(); 
	}
	return m_instance;
}

xFreeTypeLib::xFreeTypeLib()
{
	this->setFontWithSize("c:\\windows\\fonts\\kaiu.ttf");
	//m_pMapTexData = new std::map<std::wstring, xCharTexture *>();
}


xFreeTypeLib::~xFreeTypeLib()
{
	// We Don't Need The Face Information Now That The Display
	// Lists Have Been Created, So We Free The Assosiated Resources.
	//FT_Done_Face(m_FT_Face); //no need  add this command will memory leak alert

	size_t SIZE = m_MapTexData.size();

	while (m_MapTexData.size())
	{
		std::map<std::wstring, xCharTexture*>::iterator iter = m_MapTexData.begin();
		xCharTexture *tex = (*iter).second;
		glDeleteTextures(1, &tex->m_texID);

		delete (*iter).second;
		m_MapTexData.erase(iter);
		//OutputDebugStringW(L"xFreeTypeLib::~xFreeTypeLib():free glDeleteTextures\n");
	}
	m_MapTexData.clear();


	FT_Done_Face(m_FT_Face);

	// Ditto For The Font Library.
	FT_Error err = FT_Done_FreeType(m_FT2Lib);
	if ( err == 0)
	{
		OutputDebugStringW(L"xFreeTypeLib::~xFreeTypeLib():free lib Success\n");
	}

}

