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
	eUIID_SkillIconRock,
	eUIID_SkillIconWind,
	eUIID_SkillIconFire,
	eUIID_SkillIconWater,
};

enum eWindowID
{
	eWindowID_Command = 0,
	eWindowID_CharInfo,
	eWindowID_Select,
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

//支持中文的listbox
class UIListBox : public hgeGUIListbox
{
public:
	UIListBox(int id, float x, float y, float w, float h, GfxFont* font, DWORD tColor, DWORD thColor, DWORD hColor) :
	  hgeGUIListbox(id, x, y, w, h, NULL, tColor, thColor, hColor)
	{
		m_pFont = font;
		sprHighlight=new hgeSprite(0, 0, 0, w, m_pFont->GetFontSize());
		sprHighlight->SetColor(hColor);

		//由于图标是16大小，所以每项最低高度为16
		if(m_pFont->GetFontSize() > 16)
			m_fFontHeight = m_pFont->GetFontSize();
		else
			m_fFontHeight = 16;
	}

	  virtual int	AddItem(HTEXTURE tex,char *item)
	  {
		  hgeGUIListboxItem *pItem=pItems, *pPrev=0, *pNew;

		  pNew = new hgeGUIListboxItem;
		  memcpy(pNew->text, item, min(sizeof(pNew->text), strlen(item)+1));
		  pNew->text[sizeof(pNew->text)-1]='\0';
		  if(tex != 0)
			 pNew->icon = new hgeSprite(tex,0,0,16,16);	//图标必须为16*16
		  pNew->next=0;

		  while(pItem) { pPrev=pItem;	pItem=pItem->next; }

		  if(pPrev) pPrev->next=pNew;
		  else pItems=pNew;
		  nItems++;

		  return nItems-1;
	  }

	  virtual void	DeleteItem(int n)
	  {
		  int i;
		  hgeGUIListboxItem *pItem=pItems, *pPrev=0;

		  if(n<0 || n>=GetNumItems()) return;

		  for(i=0;i<n;i++) { pPrev=pItem;	pItem=pItem->next; }

		  if(pPrev) pPrev->next=pItem->next;
		  else pItems=pItem->next;

		  if(pItem->icon)
			  gSafeDelete(pItem->icon);
		  gSafeDelete(pItem);
		  nItems--;
	  }

	 virtual int	GetNumRows()
	 { 
		 return int((rect.y2-rect.y1)/m_fFontHeight); 
	 }

	 virtual void Render()
	  {
		  int i;
		  hgeGUIListboxItem *pItem=pItems;

		  for(i=0;i<nTopItem;i++) pItem=pItem->next;
		  for(i=0;i<GetNumRows();i++)
		  {
			  if(i>=nItems) return;

			  wchar_t itemText[256];
			  g_CTW(pItem->text,itemText);
			  if(nTopItem+i == nSelectedItem)
			  {
				  sprHighlight->Render(rect.x1,rect.y1+i*m_fFontHeight);
				  m_pFont->SetColor(texthilColor);
			  }
			  else
				  m_pFont->SetColor(textColor);

			  if(pItem->icon)
				pItem->icon->Render(rect.x1+3,rect.y1+i*m_fFontHeight);
			  m_pFont->Render(rect.x1+19, rect.y1+i*m_fFontHeight, itemText);
			  pItem=pItem->next;
		  }
	  }

	 virtual bool MouseLButton(bool bDown)
	 {
		 int nItem;

		 if(bDown)
		 {
			 nItem=nTopItem+int(my)/int(m_fFontHeight);
			 if(nItem<nItems)
			 {
				 nSelectedItem=nItem;
				 return true;
			 }
		 }
		 return false;
	 }

private:
	GfxFont* m_pFont;
	float m_fFontHeight;
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
	virtual void	SetBindChar(Character* bindChar){}
	virtual Character*	GetBindChar(){return NULL;}
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