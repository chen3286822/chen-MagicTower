#ifndef UI_H
#define UI_H

#include "commonTools.h"
#include "FontManager.h"
#include "GfxFont.h"
#include "TexManager.h"

#define ADDWINDOW(windowID,windowName) m_mWindowCreateFunc[windowID]=&windowName::CreateWnd;
#define CREATEWINDOW(windowName) \
	public:\
	static UIWindow* CreateWnd() \
	{ \
		return new windowName(); \
	} \

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
	eUIID_NewButtonUp = 8,
	eUIID_NewButtonDown = 10,
	eUIID_NewButtonOn = 9,
	eUIID_NewButtonDisable = 11,
	eUIID_SmallButtonUp = 12,
	eUIID_SmallButtonDown = 14,
	eUIID_SmallButtonOn = 13,
	eUIID_SmallButtonDisable = 15,
	eUIID_SelectRect = 16,
	eUIID_VerticalButtonUp = 17,
	eUIID_VerticalButtonOn = 18,
	eUIID_VerticalButtonDown = 19,
	eUIID_VerticalButtonDisable = 20,
	eUIID_Dialog = 21,
	eUIID_WndSummary = 22,
	eUIID_DefendIcon = 23,
	eUIID_AttackIcon = 24,
	eUIID_DodgeIcon = 25,
	eUIID_SkillDamageIcon = 26,
	eUIID_CritIcon = 27,
	eUIID_MagicDefendIcon = 28,
};

enum eWindowID
{
	eWindowID_Command = 0,
	eWindowID_CharInfo,
	eWindowID_Select,
	eWindowID_MainTitle,
	eWindowID_Dialog,
	eWindowID_Summary,
};

enum eMessage
{
	eMessage_CloseWindow,
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
		  m_nTextSize.cx = 0;
		  m_nTextSize.cy = 0;
	  }
	  UIButton(int id, float x, float y, float w, float h, HTEXTURE tex, HTEXTURE tex2, float tx, float ty) :
	  hgeGUIButton(id,x,y,w,h,tex,tex2,tx,ty)
	  {
		  m_pFont = NULL;
		  m_wstrText = (wchar_t*)"";
		  m_nID = id;
		  m_nTextSize.cx = 0;
		  m_nTextSize.cy = 0;
	  }
	  UIButton(int id, float x, float y, float w, float h, HTEXTURE tex, HTEXTURE tex2, HTEXTURE texOn, HTEXTURE texDisable, float tx, float ty) :
	  hgeGUIButton(id,x,y,w,h,tex,tex2,texOn,texDisable,tx,ty)
	  {
		  m_pFont = NULL;
		  m_wstrText = (wchar_t*)"";
		  m_nID = id;
		  m_nTextSize.cx = 0;
		  m_nTextSize.cy = 0;
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
		  m_nTextSize = m_pFont->GetTextSize(m_wstrText.c_str());
	  }

	  virtual void	Render()
	  {
		  hgeGUIButton::Render();
		  if (m_pFont)
		  {
			  m_pFont->SetColor(m_dwColor);
			  if (GetState())
				  m_pFont->Render(GetX()+(int)((rect.x2-rect.x1)/2-m_nTextSize.cx/2)+1,GetY()+(int)((rect.y2-rect.y1)/2-m_nTextSize.cy/2)+1,m_wstrText.c_str());
			  else
				  m_pFont->Render(GetX()+(int)((rect.x2-rect.x1)/2-m_nTextSize.cx/2),GetY()+(int)((rect.y2-rect.y1)/2-m_nTextSize.cy/2),m_wstrText.c_str());
		  }

	  }

	  int GetID(){return m_nID;}

private:
	GfxFont* m_pFont;
	std::wstring m_wstrText;
	DWORD m_dwColor;
	int m_nID;
	SIZE m_nTextSize;
};

//支持中文的listbox
class UIListBox : public hgeGUIListbox
{
public:
	enum eControlID
	{
		eControlID_NextButton = 1,
		eControlID_PreviousButton,
	};
	UIListBox(int id, float x, float y, float w, float h, GfxFont* font, DWORD tColor, DWORD thColor, DWORD hColor,DWORD disColor,DWORD mouseOnColor) :
	  hgeGUIListbox(id, x, y, w, h, NULL, tColor, thColor, hColor,disColor)
	{
		m_pFont = font;
		sprHighlight=new hgeSprite(0, 0, 0, w, m_pFont->GetFontSize());
		sprHighlight->SetColor(hColor);

		//由于图标是16大小，所以每项最低高度为16
		if(m_pFont->GetFontSize() > 16)
			m_fFontHeight = m_pFont->GetFontSize();
		else
			m_fFontHeight = 16;

		m_pContainer = new hgeGUI;
		m_pNext = NULL;
		m_pPrevious = NULL;

		m_nPageMaxRows = int((rect.y2-rect.y1)/m_fFontHeight);
		m_nCurrentPage = 0;

		m_pMouseOnSpr = new hgeSprite(0, 0, 0, w, m_pFont->GetFontSize());
		m_pMouseOnSpr->SetColor(mouseOnColor);
		m_nMouseOnItem = -1;

		nSelectedItem = -1;
	  }

	  UIListBox(int id, float x, float y, float w, float h, GfxFont* font, DWORD tColor, DWORD thColor,DWORD disColor,HTEXTURE mouseTex) :
	  hgeGUIListbox(id, x, y, w, h, NULL, tColor, thColor, 0x00FFFFFF,disColor)
	  {
		  m_pFont = font;
		  sprHighlight=NULL;

		  int texHeight = hge->Texture_GetHeight(mouseTex);
		  //由于图标是16大小，所以每项最低高度为16
		  if(texHeight > 16)
			  m_fFontHeight = texHeight;
		  else
			  m_fFontHeight = 16;

		  m_pContainer = new hgeGUI;
		  m_pNext = NULL;
		  m_pPrevious = NULL;

		  m_nPageMaxRows = int((rect.y2-rect.y1)/m_fFontHeight);
		  m_nCurrentPage = 0;

		  m_pMouseOnSpr = new hgeSprite(mouseTex, 0, 0, w, texHeight);
		  m_nMouseOnItem = -1;

		  nSelectedItem = -1;
	  }

	  virtual ~UIListBox()
	  {
		  if(m_pNext)
			 m_pContainer->DelCtrl(eControlID_NextButton);
		  if(m_pPrevious)
			m_pContainer->DelCtrl(eControlID_PreviousButton);
		  gSafeDelete(m_pContainer);
		  gSafeDelete(m_pMouseOnSpr);
	  }

	  void AddPageButton(int x1,int y1,int x2,int y2,int w,int h,HTEXTURE texUp,HTEXTURE texDown,HTEXTURE texOn,HTEXTURE texDisable,float fx,float fy)
	  {
		  if (!m_pNext || !m_pPrevious)
		  {
			  m_pNext = new UIButton(eControlID_NextButton,rect.x1+x2,rect.y1+y2,w,h,texUp,texDown,texOn,texDisable,fx,fy);
			  m_pPrevious = new UIButton(eControlID_PreviousButton,rect.x1+x1,rect.y1+y1,w,h,texUp,texDown,texOn,texDisable,fx,fy);
			  m_pNext->OffsetX = x1;
			  m_pNext->OffsetY = y1;
			  m_pPrevious->OffsetX = x2;
			  m_pPrevious->OffsetY = y2;
			  m_pContainer->AddCtrl(m_pNext);
			  m_pContainer->AddCtrl(m_pPrevious);
			  m_pNext->SetFont(eFontType_MSYaHei,eFontSize_FontMiddle);
			  m_pNext->SetText("下一页",0xFF000000);
			  m_pPrevious->SetFont(eFontType_MSYaHei,eFontSize_FontMiddle);
			  m_pPrevious->SetText("上一页",0xFF000000);
		  }
	  }
	  
	  virtual bool	MouseMove(float x, float y)
	  { 
		  mx=x; 
		  my=y;
		  if(int(my)/int(m_fFontHeight) >= m_nPageMaxRows)
			  return false;
		  int item =nTopItem+int(my)/int(m_fFontHeight);
		  if(item<nItems)
		  {
			  if(GetItemDisabled(item))
			  {
				  return false;
			  }

			  m_nMouseOnItem=item;
			  return false;
		  }
		  m_nMouseOnItem = -1;
		  return false;
	  }

	  int& GetPageMaxRows(){return m_nPageMaxRows;}

	  virtual void ResetPosition(float x, float y)
	  {
		  hgeGUIObject::ResetPosition(x,y);
		  if(m_pNext)
			m_pNext->ResetPosition(x+m_pNext->OffsetX,y+m_pNext->OffsetY);
		  if(m_pNext)
			m_pPrevious->ResetPosition(x+m_pPrevious->OffsetX,y+m_pPrevious->OffsetY);
	  }

	  virtual void Update(float dt)
	  {
		  int id = m_pContainer->Update(dt);
		  if (id == -1)
		  {
			  m_pContainer->Enter();
		  }
		  else if (id == eControlID_NextButton)
		  {
			  m_pContainer->Leave();
			  if(nTopItem + m_nPageMaxRows < nItems)
			  {
				  nTopItem += m_nPageMaxRows;
				  m_nCurrentPage++;
			  }
			  
		  }
		  else if (id == eControlID_PreviousButton)
		  {
			  m_pContainer->Leave();
			  if (nTopItem - m_nPageMaxRows >= 0)
			  {
				  nTopItem -= m_nPageMaxRows;
				  m_nCurrentPage--;
			  }	  
		  }
	  }

	  virtual int	AddItem(HTEXTURE tex,char *item)
	  {
		  hgeGUIListboxItem *pItem=pItems, *pPrev=0, *pNew;

		  pNew = new hgeGUIListboxItem;
		  memcpy(pNew->text, item, min(sizeof(pNew->text), strlen(item)+1));
		  pNew->text[sizeof(pNew->text)-1]='\0';
		  pNew->icon = NULL;
		  if(tex != 0)
			 pNew->icon = new hgeSprite(tex,0,0,16,16);	//图标必须为16*16
		  pNew->next=0;
		  pNew->disabled = false;

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
		 return m_nPageMaxRows; 
	 }

	 virtual void Render()
	  {
		  m_pContainer->Render();

		  int i;
		  hgeGUIListboxItem *pItem=pItems;

		  for(i=0;i<nTopItem;i++) pItem=pItem->next;
		  for(i=0;i<GetNumRows();i++)
		  {
			  if(i>=nItems) return;

			  if(pItem)
			  {
				  wchar_t itemText[256];
				  g_CTW(pItem->text,itemText);
				  if(nTopItem+i == nSelectedItem)
				  {
					  if(sprHighlight)
						sprHighlight->Render(rect.x1,rect.y1+i*m_fFontHeight);
					  else
						   m_pMouseOnSpr->Render(rect.x1,rect.y1+i*m_fFontHeight);
					  m_pFont->SetColor(texthilColor);
				  }
				  else
				  {
					  if(pItem->disabled)
						  m_pFont->SetColor(disabledColor);
					  else
						m_pFont->SetColor(textColor);

					  if(m_nMouseOnItem == nTopItem+i)
					  {
						  m_pMouseOnSpr->Render(rect.x1,rect.y1+i*m_fFontHeight);
					  }
				  }

				  if(pItem->icon)
					  pItem->icon->Render(rect.x1+3,rect.y1+i*m_fFontHeight);
				  m_pFont->Render(rect.x1+19, rect.y1+i*m_fFontHeight, itemText);
				  pItem=pItem->next;
			  }
		  }
	  }

	 virtual bool MouseLButton(bool bDown)
	 {
		 //拦截点击到前页后页按钮的消息
// 		 float fx=0,fy=0;
// 		 hge->Input_GetMousePos(&fx,&fy);
// 		 if(m_pNext->rect.TestPoint(fx,fy) || m_pPrevious->rect.TestPoint(fx,fy))
// 			 return false;

		 int nItem;

		 if(bDown)
		 {
			 if(int(my)/int(m_fFontHeight) >= m_nPageMaxRows)
				 return false;
			 nItem=nTopItem+int(my)/int(m_fFontHeight);
			 if(nItem<nItems)
			 {
				 if(GetItemDisabled(nItem))
					 return false;

				 nSelectedItem=nItem;
				 return true;
			 }
		 }
		 return false;
	 }

	 virtual bool	MouseWheel(int nNotches)
	 {
		 return false;
	 }

	 virtual bool	KeyClick(int key, int chr)
	 {
		 return false;
	 }

private:
	GfxFont* m_pFont;
	float m_fFontHeight;
	int m_nPageMaxRows;	//每页最多项数
	int m_nCurrentPage;		//当前页
	hgeGUI* m_pContainer;	//gui容器
	UIButton* m_pNext;		//下一页
	UIButton* m_pPrevious;	//上一页
	hgeSprite* m_pMouseOnSpr;
	int m_nMouseOnItem;
};

class Character;
class UIWindow
{
public:
	UIWindow();
	UIWindow(HTEXTURE,float,float,float,float,float,float);
	virtual ~UIWindow();

	virtual void					Init(){m_pContainer->Enter();}
	virtual void					Release(){}
	virtual void					Render();
	virtual void					Update(float dt){}
	virtual void					OnMouseOver(float x,float y){}

	hgeGUI*						GetContainer(){return m_pContainer;}

	float								GetX(){return m_fPosX;}
	float								GetY(){return m_fPosY;}
	virtual void					SetRenderPositon(float _x,float _y){m_fPosX = _x; m_fPosY = _y;}
	bool								IsShow(){return m_bShow;}
	void								SetShow(bool _show);
	virtual bool					IsOnControl();
	virtual void					SetBindChar(Character* bindChar){}
	virtual Character*		GetBindChar(){return NULL;}

	eWindowID&				GetID(){return m_eID;}
protected:
	hgeGUI* m_pContainer;
	hgeSprite* m_pBackGround;
	float m_fPosX;
	float m_fPosY;

	bool m_bShow;
	eWindowID m_eID;
};

struct WindowNode
{
	eWindowID m_eWindowID;
	UIWindow*	m_pWindow;
	WindowNode* m_pNext;
};

struct MsgNode
{
	eMessage m_eMsgID;
	eWindowID m_eWndID;
	DWORD m_dwData;
};

typedef UIWindow* (*LPCreateWindow)();
class UISystem : public Singleton<UISystem>
{
public:
	UISystem();
	virtual ~UISystem(){}

	void Init();
	void Release();
	void Render();
	void Update(float dt);
	bool IsInAnyControl();
	UIWindow* PopUpWindow(eWindowID windowID);
	void CloseWindow(eWindowID windowID);

	void MsgWnd(eMessage eMsgID,eWindowID eWndID,DWORD dwData=0);

	UIWindow* GetWindow(eWindowID windowID);
private:
	WindowNode* m_pHeadWindow;
	std::map<eWindowID,LPCreateWindow> m_mWindowCreateFunc;
	//延迟消息队列，此队列中的消息将会延迟处理
	std::list<MsgNode> m_lMsgList;

	void ProcessMsg();
	void RemoveWindow(eWindowID windowID);
};
#endif