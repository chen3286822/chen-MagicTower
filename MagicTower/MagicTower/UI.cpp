#include "UI.h"
#include "WndCommand.h"
#include "WndCharInfo.h"
#include "WndSelect.h"
#include "WndMainTitle.h"

UIWindow::UIWindow()
{
	m_pContainer = NULL;
	m_pBackGround = NULL;
	m_fPosX = m_fPosY = 0;
	m_bShow = false;
}

UIWindow::UIWindow(HTEXTURE tex, float x, float y, float w ,float h, float posX, float posY)
{
	m_pContainer = NULL;
	m_pBackGround = new hgeSprite(tex,x,y,w,h);
	m_fPosX = posX,
	m_fPosY = posY;
	m_bShow = false;
}

UIWindow::~UIWindow()
{
	gSafeDelete(m_pBackGround);
	gSafeDelete(m_pContainer);
}

void UIWindow::Render()
{
	if(m_bShow)
	{
		m_pBackGround->Render(m_fPosX,m_fPosY);
		m_pContainer->Render();
	}
}

void UIWindow::SetShow(bool _show)
{
	m_bShow = _show;
	if (m_bShow)
	{
		m_pContainer->Reset();
		m_pContainer->Enter();		
	}
	else
	{
		m_pContainer->Leave();
	}
}

bool UIWindow::IsOnControl()
{
	if (m_bShow && m_pBackGround)
	{
		float x,y;
		HGE* hge = hgeCreate(HGE_VERSION);
		hge->Input_GetMousePos(&x,&y);
		if (x>=m_fPosX && x<=(m_fPosX+m_pBackGround->GetWidth()) && y>=m_fPosY && y<=(m_fPosY+m_pBackGround->GetHeight()))
			return true;

		hge->Release();
	}
	return false;
}

void UISystem::Init()
{
//	WndCommand* pWndCommand = new WndCommand();
//	WndCommand* pWndCommand = NULL;
//	m_mWindows[eWindowID_Command] = pWndCommand;
//	WndCharInfo* pWndCharInfo = new WndCharInfo();
//	WndCharInfo* pWndCharInfo = NULL;
//	m_mWindows[eWindowID_CharInfo] = pWndCharInfo;
//	WndSelect* pWndSelect = new WndSelect();
//	WndSelect* pWndSelect = NULL;
//	m_mWindows[eWindowID_Select] = pWndSelect;
//	WndMainTitle* pWndMainTitle = new WndMainTitle();
//	WndMainTitle* pWndMainTitle = NULL;
//	m_mWindows[eWindowID_MainTitle] = pWndMainTitle;

	ADDWINDOW(eWindowID_Command,WndCommand)
	ADDWINDOW(eWindowID_CharInfo,WndCharInfo)
	ADDWINDOW(eWindowID_Select,WndSelect)
	ADDWINDOW(eWindowID_MainTitle,WndMainTitle)

// 	for (std::map<eWindowID,UIWindow*>::iterator mit=m_mWindows.begin();mit!=m_mWindows.end();mit++)
// 		mit->second->Init();
}

void UISystem::Release()
{
	for (std::map<eWindowID,UIWindow*>::iterator mit=m_mWindows.begin();mit!=m_mWindows.end();mit++)
	{
		if(mit->second != NULL)
		{
			mit->second->Release();
			gSafeDelete(mit->second);
		}
	}
}

void UISystem::Render()
{
	for (std::map<eWindowID,UIWindow*>::iterator mit=m_mWindows.begin();mit!=m_mWindows.end();mit++)
	{
		if(mit->second)
		{
			if (mit->second->IsShow())
			{
				mit->second->Render();
			}
		}
	}
}

void UISystem::Update(float dt)
{
	for (std::map<eWindowID,UIWindow*>::iterator mit=m_mWindows.begin();mit!=m_mWindows.end();)
	{
		if(mit->second)
		{
			if (mit->second->IsShow())
			{
				mit->second->Update(dt);
			}
			if (mit->second == NULL)
			{
				m_mWindows.erase(mit);
			}
			else
				mit++;
		}
		else
		{
			m_mWindows.erase(mit);
		}
	}
}

UIWindow* UISystem::GetWindow(eWindowID windowID)
{
	for (std::map<eWindowID,UIWindow*>::iterator mit=m_mWindows.begin();mit!=m_mWindows.end();mit++)
	{
		if(mit->first == windowID)
			return mit->second;
	}
	return NULL;
}

bool UISystem::IsInAnyControl()
{
	for (std::map<eWindowID,UIWindow*>::iterator mit=m_mWindows.begin();mit!=m_mWindows.end();mit++)
	{
		if(mit->second)
		{
			if(mit->second->IsOnControl())
			{
				return true;
			}
		}
	}
	return false;
}

UIWindow* UISystem::PopUpWindow(eWindowID windowID)
{
	std::map<eWindowID,UIWindow*>::iterator it = m_mWindows.find(windowID);
	if (it != m_mWindows.end())
	{
		//已经生成该窗口
		if(it->second != NULL)
		{
			it->second->SetShow(true);
			return it->second;
		}
	}
	else
	{
		UIWindow* window = NULL;
		std::map<eWindowID,LPCreateWindow>::iterator mit = m_mWindowCreateFunc.find(windowID);
		if(mit!=m_mWindowCreateFunc.end())
		{
			window = mit->second();
			window->Init();
			window->SetShow(true);
			m_mWindows[windowID] = window;
		}
		return window;
	}
	return NULL;
}

void UISystem::CloseWindow(eWindowID windowID)
{
	std::map<eWindowID,UIWindow*>::iterator it = m_mWindows.find(windowID);
	if (it != m_mWindows.end())
	{
		if(it->second != NULL)
		{
			it->second->Release();
			//等到下一帧移除，因为此时移除，在update时，就会导致it报错
			//m_mWindows.erase(it);
			gSafeDelete(it->second);
		}
	}
}