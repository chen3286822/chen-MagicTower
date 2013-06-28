#include "WndCommand.h"
#include "TexManager.h"

WndCommand::WndCommand(HTEXTURE tex,float x,float y,float w,float h,float posX,float posY) :
UIWindow(tex,x,y,w,h,posX,posY)
{
	m_pContainer = new hgeGUI;
	m_pAttackButton = new UIButton(eControlID_AttackButton,m_fPosX+x,m_fPosY+y,83,21,TexManager::sInstance().GetUITex()[eUIID_ButtonNormal],TexManager::sInstance().GetUITex()[eUIID_ButtonPress],0,0);
	m_pFinishButton = new UIButton(eControlID_FinishButton,m_fPosX+x,m_fPosY+y+21,83,21,TexManager::sInstance().GetUITex()[eUIID_ButtonNormal],TexManager::sInstance().GetUITex()[eUIID_ButtonPress],0,0);
	m_pContainer->AddCtrl(m_pAttackButton);
	m_pContainer->AddCtrl(m_pFinishButton);

	m_pAttackButton->SetFont(eFontType_MSYaHei,eFontSize_FontMiddle);
	m_pAttackButton->SetText("¹¥»÷",0xFF000000);

	m_pFinishButton->SetFont(eFontType_MSYaHei,eFontSize_FontMiddle);
	m_pFinishButton->SetText("´ıÃü",0xFF000000);
}

WndCommand::~WndCommand()
{
	m_pContainer->DelCtrl(eControlID_AttackButton);
	m_pContainer->DelCtrl(eControlID_FinishButton);
}

void WndCommand::Update(float dt)
{
	if (m_bShow)
	{
		int id = m_pContainer->Update(dt);
		if (id == -1)
		{
			m_pContainer->Enter();
		}
		else if (id == eControlID_AttackButton)
		{

			m_pContainer->Leave();
		}
		else if (id == eControlID_FinishButton)
		{

			m_pContainer->Leave();
		}
	}
}