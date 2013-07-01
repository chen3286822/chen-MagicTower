#include "WndCommand.h"
#include "TexManager.h"
#include "Character.h"

WndCommand::WndCommand(HTEXTURE tex,float x,float y,float w,float h,float posX,float posY) :
UIWindow(tex,x,y,w,h,posX,posY)
{
	m_pContainer = new hgeGUI;
	m_pAttackButton = new UIButton(eControlID_AttackButton,m_fPosX,m_fPosY,83,21,TexManager::sInstance().GetUITex()[eUIID_ButtonNormal],TexManager::sInstance().GetUITex()[eUIID_ButtonPress],0,0);
	m_pFinishButton = new UIButton(eControlID_FinishButton,m_fPosX,m_fPosY+21,83,21,TexManager::sInstance().GetUITex()[eUIID_ButtonNormal],TexManager::sInstance().GetUITex()[eUIID_ButtonPress],0,0);
	m_pContainer->AddCtrl(m_pAttackButton);
	m_pContainer->AddCtrl(m_pFinishButton);

	m_pAttackButton->SetFont(eFontType_MSYaHei,eFontSize_FontMiddle);
	m_pAttackButton->SetText("¹¥»÷",0xFF000000);

	m_pFinishButton->SetFont(eFontType_MSYaHei,eFontSize_FontMiddle);
	m_pFinishButton->SetText("´ýÃü",0xFF000000);

	m_pChar = NULL;
}

WndCommand::~WndCommand()
{
	m_pContainer->DelCtrl(eControlID_AttackButton);
	m_pContainer->DelCtrl(eControlID_FinishButton);
}

void	WndCommand::SetBindChar(Character* bindChar)
{
	m_pChar = bindChar;
	if (bindChar)
	{
		m_fPosX = bindChar->GetRealX() + 50; 
		m_fPosY = bindChar->GetRealY();

		m_pAttackButton->ResetPosition(m_fPosX,m_fPosY);
		m_pFinishButton->ResetPosition(m_fPosX,m_fPosY+21);
	}
}

Character*	WndCommand::GetBindChar()
{
	return m_pChar;
}

void WndCommand::SetRenderPositon(float _x,float _y)
{
	m_fPosX = _x; 
	m_fPosY = _y;

	m_pAttackButton->ResetPosition(m_fPosX,m_fPosY);
	m_pFinishButton->ResetPosition(m_fPosX,m_fPosY+21);
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
			if(m_pChar)
			{
				m_pChar->SetActionStage(eActionStage_AttackStage);
				SetShow(false);
			}
			m_pContainer->Leave();
		}
		else if (id == eControlID_FinishButton)
		{
			if(m_pChar)
			{
				m_pChar->SetFinish(true);
				SetShow(false);
			}
			m_pContainer->Leave();
		}
	}
}

bool WndCommand::IsOnControl()
{
	return (m_bShow && (UIWindow::IsOnControl() || m_pAttackButton->IsMouseOn() || m_pFinishButton->IsMouseOn()));
}