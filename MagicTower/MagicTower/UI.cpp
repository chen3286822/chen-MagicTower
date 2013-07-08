#include "UI.h"
#include "WndCommand.h"
#include "WndCharInfo.h"
#include "WndSelect.h"

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
	WndCommand* pWndCommand = new WndCommand();
	m_mWindows[eWindowID_Command] = pWndCommand;
	WndCharInfo* pWndCharInfo = new WndCharInfo();
	m_mWindows[eWindowID_CharInfo] = pWndCharInfo;
	WndSelect* pWndSelect = new WndSelect();
	m_mWindows[eWindowID_Select] = pWndSelect;

	for (std::map<eWindowID,UIWindow*>::iterator mit=m_mWindows.begin();mit!=m_mWindows.end();mit++)
		mit->second->Init();
}

void UISystem::Release()
{
	for (std::map<eWindowID,UIWindow*>::iterator mit=m_mWindows.begin();mit!=m_mWindows.end();mit++)
	{
		mit->second->Release();
		gSafeDelete(mit->second);
	}
}

void UISystem::Render()
{
	for (std::map<eWindowID,UIWindow*>::iterator mit=m_mWindows.begin();mit!=m_mWindows.end();mit++)
	{
		if (mit->second->IsShow())
		{
			mit->second->Render();
		}
	}
}

void UISystem::Update(float dt)
{
	for (std::map<eWindowID,UIWindow*>::iterator mit=m_mWindows.begin();mit!=m_mWindows.end();mit++)
	{
		if (mit->second->IsShow())
		{
			mit->second->Update(dt);
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