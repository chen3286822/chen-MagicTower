#include "UI.h"

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