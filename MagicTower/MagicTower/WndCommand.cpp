#include "WndCommand.h"
#include "TexManager.h"
#include "FontManager.h"
#include "Character.h"
#include "WndSelect.h"
#include "UI.h"

WndCommand::WndCommand() :
UIWindow(TexManager::sInstance().GetUITex()[eUIID_WndCommand],0,0,89,122,0,0)
{
	m_pContainer = new hgeGUI;
	m_pAttackButton = new UIButton(eControlID_AttackButton,m_fPosX,m_fPosY,83,21,TexManager::sInstance().GetUITex()[eUIID_ButtonNormal],TexManager::sInstance().GetUITex()[eUIID_ButtonPress],0,0);
	m_pFinishButton = new UIButton(eControlID_FinishButton,m_fPosX,m_fPosY+63,83,21,TexManager::sInstance().GetUITex()[eUIID_ButtonNormal],TexManager::sInstance().GetUITex()[eUIID_ButtonPress],0,0);
	m_pSkillButton = new UIButton(eControlID_SkillButton,m_fPosX,m_fPosY+21,83,21,TexManager::sInstance().GetUITex()[eUIID_ButtonNormal],TexManager::sInstance().GetUITex()[eUIID_ButtonPress],0,0);
	m_pItemButton = new UIButton(eControlID_ItemButton,m_fPosX,m_fPosY+42,83,21,TexManager::sInstance().GetUITex()[eUIID_ButtonNormal],TexManager::sInstance().GetUITex()[eUIID_ButtonPress],0,0);

	m_pContainer->AddCtrl(m_pAttackButton);
	m_pContainer->AddCtrl(m_pFinishButton);
	m_pContainer->AddCtrl(m_pSkillButton);
	m_pContainer->AddCtrl(m_pItemButton);

	m_pAttackButton->SetFont(eFontType_MSYaHei,eFontSize_FontMiddle);
	m_pAttackButton->SetText("攻击",0xFF000000);

	m_pFinishButton->SetFont(eFontType_MSYaHei,eFontSize_FontMiddle);
	m_pFinishButton->SetText("待命",0xFF000000);

	m_pSkillButton->SetFont(eFontType_MSYaHei,eFontSize_FontMiddle);
	m_pSkillButton->SetText("技能",0xFF000000);

	m_pItemButton->SetFont(eFontType_MSYaHei,eFontSize_FontMiddle);
	m_pItemButton->SetText("物品",0xFF000000);

	m_pChar = NULL;
}

WndCommand::~WndCommand()
{
	m_pContainer->DelCtrl(eControlID_AttackButton);
	m_pContainer->DelCtrl(eControlID_FinishButton);
	m_pContainer->DelCtrl(eControlID_SkillButton);
	m_pContainer->DelCtrl(eControlID_ItemButton);
}

void	WndCommand::SetBindChar(Character* bindChar)
{
	m_pChar = bindChar;
	if (bindChar)
	{
		m_fPosX = bindChar->GetRealX() + 50; 
		m_fPosY = bindChar->GetRealY();

		m_pAttackButton->ResetPosition(m_fPosX,m_fPosY);
		m_pSkillButton->ResetPosition(m_fPosX,m_fPosY+21);
		m_pItemButton->ResetPosition(m_fPosX,m_fPosY+42);
		m_pFinishButton->ResetPosition(m_fPosX,m_fPosY+63);	
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
	m_pSkillButton->ResetPosition(m_fPosX,m_fPosY+21);
	m_pItemButton->ResetPosition(m_fPosX,m_fPosY+42);
	m_pFinishButton->ResetPosition(m_fPosX,m_fPosY+63);
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
		else if (id == eControlID_SkillButton || id == eControlID_ItemButton)
		{
			if(m_pChar)
			{
				if(id == eControlID_SkillButton)
					m_pChar->SetActionStage(eActionStage_SkillStage);
				else
					m_pChar->SetActionStage(eActionStage_ItemStage);
				SetShow(false);

				//打开技能面板
				UIWindow* wndSelect = UISystem::sInstance().GetWindow(eWindowID_Select);
				if(wndSelect)
				{
					wndSelect->SetShow(true);
					wndSelect->SetBindChar(m_pChar);
				}
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
	return (m_bShow && (UIWindow::IsOnControl() || m_pAttackButton->IsMouseOn() || m_pFinishButton->IsMouseOn() || m_pSkillButton->IsMouseOn() || m_pItemButton->IsMouseOn()));
}