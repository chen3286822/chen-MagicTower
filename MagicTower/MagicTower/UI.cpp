#include "UI.h"
#include "WndCommand.h"
#include "TexManager.h"

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


void UISystem::Init()
{
	WndCommand* pWndCommand = new WndCommand(TexManager::sInstance().GetUITex()[eUIID_WndCommand],0,0,89,122,0,0);
	m_mWindows[eWindowID_Command] = pWndCommand;

	pWndCommand->SetShow(true);
	pWndCommand->Init();
}

void UISystem::Release()
{
	for (std::map<int,UIWindow*>::iterator mit=m_mWindows.begin();mit!=m_mWindows.end();mit++)
	{
		mit->second->Release();
		gSafeDelete(mit->second);
	}
}

void UISystem::Render()
{
	for (std::map<int,UIWindow*>::iterator mit=m_mWindows.begin();mit!=m_mWindows.end();mit++)
	{
		if (mit->second->IsShow())
		{
			mit->second->Render();
		}
	}
}

void UISystem::Update(float dt)
{
	for (std::map<int,UIWindow*>::iterator mit=m_mWindows.begin();mit!=m_mWindows.end();mit++)
	{
		if (mit->second->IsShow())
		{
			mit->second->Update(dt);
		}
	}
}