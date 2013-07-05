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
	eUIID_WndCharInfo,
};

enum eWindowID
{
	eWindowID_Command = 0,
	eWindowID_CharInfo,
};

class UIButton : public hgeGUIButton
{
public:
	UIButton(int id, float x, float y, float w, float h, HTEXTURE tex, float tx, float ty) :
	  hgeGUIButton(id,x,y,w,h,tex,tx,ty)
	  {
		  m_pFont = NULL;
		  m_wstrText = (wchar_t*)"";
		  m_nID = id;
	  }
	  UIButton(int id, float x, float y, float w, float h, HTEXTURE tex, HTEXTURE tex2, float tx, float ty) :
	  hgeGUIButton(id,x,y,w,h,tex,tex2,tx,ty)
	  {
		  m_pFont = NULL;
		  m_wstrText = (wchar_t*)"";
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
	  void	SetText(const char* str,DWORD color=0xFFFFFFFF)
	  {
		  wchar_t out[256];
		  g_CTW(str,out);
		  m_wstrText = out;
		  m_dwColor = color;
	  }

	  virtual void	Render()
	  {
		  hgeGUIButton::Render();
		  if (m_pFont)
		  {
			  m_pFont->SetColor(m_dwColor);
			  if (GetState())
				  m_pFont->Render(GetX()+1,GetY()+1,m_wstrText.c_str());
			  else
				  m_pFont->Render(GetX(),GetY(),m_wstrText.c_str());
		  }

	  }

	  int GetID(){return m_nID;}

private:
	GfxFont* m_pFont;
	std::wstring m_wstrText;
	DWORD m_dwColor;
	int m_nID;
};

class Character;
class UIWindow
{
public:
	UIWindow();
	UIWindow(HTEXTURE,float,float,float,float,float,float);
	~UIWindow();

	virtual void					Init(){m_pContainer->Enter();}
	virtual void					Release(){}
	virtual void					Render();
	virtual void					Update(float dt){}

	hgeGUI*						GetContainer(){return m_pContainer;}

	float								GetX(){return m_fPosX;}
	float								GetY(){return m_fPosY;}
	virtual void					SetRenderPositon(float _x,float _y){m_fPosX = _x; m_fPosY = _y;}
	bool								IsShow(){return m_bShow;}
	void								SetShow(bool _show);
	virtual bool					IsOnControl();
	virtual void					SetBindChar(Character*)=0;
	virtual Character*	GetBindChar()=0;
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
	UISystem(){m_mWindows.clear();}
	~UISystem(){}

	void Init();
	void Release();
	void Render();
	void Update(float dt);

	UIWindow* GetWindow(eWindowID windowID);
private:
	std::map<eWindowID,UIWindow*> m_mWindows;
};
#endif