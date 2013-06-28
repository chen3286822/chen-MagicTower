#include "WndCommand.h"

WndCommand::WndCommand(HTEXTURE tex,float x,float y,float w,float h,float posX,float posY) :
UIWindow(tex,x,y,w,h,posX,posY)
{
	m_pContainer = new hgeGUI;
	m_pAttackButton = new UIButton(eControlID_AttackButton,m_fPosX+x,m_fPosY+y,83,21,eUIID_ButtonNormal,eUIID_ButtonPress,0,0);
	m_pFinishButton = new UIButton(eControlID_FinishButton,m_fPosX+x,m_fPosY+y,83,21,eUIID_ButtonNormal,eUIID_ButtonPress,0,0);
	m_pContainer->AddCtrl(m_pAttackButton);
	m_pContainer->AddCtrl(m_pFinishButton);

	m_pAttackButton->SetFont(eFontType_MSYaHei,eFontSize_FontMiddle);
	m_pAttackButton->SetText("¹¥»÷");

	m_pAttackButton->SetFont(eFontType_MSYaHei,eFontSize_FontMiddle);
	m_pAttackButton->SetText("´ıÃü");
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