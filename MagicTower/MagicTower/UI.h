#ifndef UI_H
#define UI_H

#include "commonTools.h"
#include <hgeguictrls.h>
#include "GfxFont.h"

class UIButton : hgeGUIButton
{
public:
	UIButton(int id, float x, float y, float w, float h, HTEXTURE tex, float tx, float ty) :
	  hgeGUIButton(id,x,y,w,h,tex,tx,ty)
	  {
		  m_pFont = NULL;
		  m_wstrText = (wchar_t*)"";
	  }
	UIButton(int id, float x, float y, float w, float h, HTEXTURE tex, HTEXTURE tex2, float tx, float ty) :
	  hgeGUIButton(id,x,y,w,h,tex,tex2,tx,ty)
	  {
		  m_pFont = NULL;
		  m_wstrText = (wchar_t*)"";
	  }
	virtual			~UIButton();

	void	SetFont(eFontType _type,eFontSize _size)
	{
		FontAttr attr(_type,_size);
		GfxFont* font = FontManager::sInstance().GetFont(attr);
		if(font)
			m_pFont = font;
	}
	void	SetText(const char* str)
	{
		wchar_t out[256];
		g_CTW(str,out);
		m_wstrText = out;
	}

	virtual void	Render()
	{
		if(bPressed)
		{
				sprDown->Render(rect.x1, rect.y1);
		}
		else
		{
				sprUp->Render(rect.x1, rect.y1);
		}
	}
	virtual bool	MouseLButton(bool bDown);

private:
	GfxFont* m_pFont;
	wstring m_wstrText;
};


#endif