#include "WndMainTitle.h"

WndMainTitle::WndMainTitle():
UIWindow(TexManager::sInstance().GetSceneTex(0),0,0,640,400,0,0)
{

}

WndMainTitle::~WndMainTitle()
{

}

void WndMainTitle::Update(float dt)
{
}

void	WndMainTitle::SetRenderPositon(float _x,float _y)
{

}

bool	WndMainTitle::IsOnControl()
{
	return (m_bShow && UIWindow::IsOnControl());
}