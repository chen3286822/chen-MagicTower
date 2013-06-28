#ifndef UI_H
#define UI_H

#include "commonTools.h"
#include "FontManager.h"
#include "GfxFont.h"

enum eUIID
{
	eUIID_WndCommand = 0,
	eUIID_ButtonNormal,
	eUIID_ButtonPress,
};

class UIButton : public hgeGUIButton
{
public:
	UIButton(int id, float x, float y, float w, float h, HTEXTURE tex, float tx, float ty) :
	  hgeGUIButton(id,x,y,w,h,tex,tx,ty)
	  {
		  m_pFont = NULL;
		  m_wstrText = (wchar_t*)"";
		  m_fX = x;
		  m_fY = y;
		  m_nID = id;
	  }
	UIButton(int id, float x, float y, float w, float h, HTEXTURE tex, HTEXTURE tex2, float tx, float ty) :
	  hgeGUIButton(id,x,y,w,h,tex,tex2,tx,ty)
	  {
		  m_pFont = NULL;
		  m_wstrText = (wchar_t*)"";
		  m_fX = x;
		  m_fY = y;
		  m_nID = id;
	  }
	virtual			~UIButton(){}

	void	SetFont(eFontType _type = eFontType_MSYaHei,eFontSize _size = eFontSize_FontMiddle)
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
		hgeGUIButton::Render();
		if (m_pFont)
		{
			if (GetState())
				m_pFont->Render(m_fX+1,m_fY+1,m_wstrText.c_str());
			else
				m_pFont->Render(m_fX,m_fY,m_wstrText.c_str());
		}

	}

	int GetID(){return m_nID;}

private:
	GfxFont* m_pFont;
	std::wstring m_wstrText;
	float m_fX;
	float m_fY;
	int m_nID;
};

class UIWindow
{
public:
	UIWindow();
	UIWindow(HTEXTURE,float,float,float,float,float,float);
	~UIWindow();

	virtual void Init(){m_pContainer->Enter();}
	virtual void Release(){}
	virtual void Render();
	virtual void Update(float dt){}

	hgeGUI*		GetContainer(){return m_pContainer;}

	float				GetX(){return m_fPosX;}
	float				GetY(){return m_fPosY;}
	void				SetRenderPositon(float _x,float _y){m_fPosX = _x; m_fPosY = _y;}
	bool				GetShow(){return m_bShow;}
	void				SetShow(bool _show){m_bShow = _show;}
protected:
	hgeGUI* m_pContainer;
	hgeSprite* m_pBackGround;
	float m_fPosX;
	float m_fPosY;

	bool m_bShow;
};

class UISystem : public Singleton<UISystem>
{
public:
	UISystem();
	~UISystem();

	void Init();
	void Release();
	void Render();
	void Update();
private:
	std::map<int,UIWindow*> m_mWindows;
};
#endif