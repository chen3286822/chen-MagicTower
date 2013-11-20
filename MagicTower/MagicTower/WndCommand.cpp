#include "WndCommand.h"
#include "TexManager.h"
#include "FontManager.h"
#include "Character.h"
#include "WndSelect.h"
#include "UI.h"

WndCommand::WndCommand() :
UIWindow(TexManager::sInstance().GetUITex(eUIID_WndCommand),0,0,166,170,0,0)
{
	m_pContainer = new hgeGUI;
	m_pAttackButton = new UIButton(eControlID_AttackButton,m_fPosX+4,m_fPosY+15,157,32,TexManager::sInstance().GetUITex(eUIID_NewButtonUp),TexManager::sInstance().GetUITex(eUIID_NewButtonDown),TexManager::sInstance().GetUITex(eUIID_NewButtonOn),TexManager::sInstance().GetUITex(eUIID_NewButtonDisable),0,0);
	m_pSkillButton = new UIButton(eControlID_SkillButton,m_fPosX+4,m_fPosY+54,157,32,TexManager::sInstance().GetUITex(eUIID_NewButtonUp),TexManager::sInstance().GetUITex(eUIID_NewButtonDown),TexManager::sInstance().GetUITex(eUIID_NewButtonOn),TexManager::sInstance().GetUITex(eUIID_NewButtonDisable),0,0);
	m_pItemButton = new UIButton(eControlID_ItemButton,m_fPosX+4,m_fPosY+91,157,32,TexManager::sInstance().GetUITex(eUIID_NewButtonUp),TexManager::sInstance().GetUITex(eUIID_NewButtonDown),TexManager::sInstance().GetUITex(eUIID_NewButtonOn),TexManager::sInstance().GetUITex(eUIID_NewButtonDisable),0,0);
	m_pFinishButton = new UIButton(eControlID_FinishButton,m_fPosX+4,m_fPosY+129,157,32,TexManager::sInstance().GetUITex(eUIID_NewButtonUp),TexManager::sInstance().GetUITex(eUIID_NewButtonDown),TexManager::sInstance().GetUITex(eUIID_NewButtonOn),TexManager::sInstance().GetUITex(eUIID_NewButtonDisable),0,0);

	m_pAttackButton->OffsetX = 4;
	m_pAttackButton->OffsetY = 15;
	m_pSkillButton->OffsetX = 4;
	m_pSkillButton->OffsetY = 54;
	m_pItemButton->OffsetX = 4;
	m_pItemButton->OffsetY = 91;
	m_pFinishButton->OffsetX = 4;
	m_pFinishButton->OffsetY = 129;

	m_pContainer->AddCtrl(m_pAttackButton);
	m_pContainer->AddCtrl(m_pFinishButton);
	m_pContainer->AddCtrl(m_pSkillButton);
	m_pContainer->AddCtrl(m_pItemButton);

	m_pAttackButton->SetFont(eFontType_MSYaHei,eFontSize_FontMiddle);
	m_pAttackButton->SetText("攻击",0xFF9D988E);

	m_pFinishButton->SetFont(eFontType_MSYaHei,eFontSize_FontMiddle);
	m_pFinishButton->SetText("待命",0xFF9D988E);

	m_pSkillButton->SetFont(eFontType_MSYaHei,eFontSize_FontMiddle);
	m_pSkillButton->SetText("技能",0xFF9D988E);

	m_pItemButton->SetFont(eFontType_MSYaHei,eFontSize_FontMiddle);
	m_pItemButton->SetText("物品",0xFF9D988E);

	m_bNeedResetPos = true;

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
		if(m_pChar->GetShowX() + 216 > APP_WIDTH)
			m_fPosX = m_pChar->GetShowX() - 170;
		else
			m_fPosX = m_pChar->GetShowX() + 50; 
		if(m_pChar->GetShowY() + 170 > APP_HEIGHT)
			m_fPosY = m_pChar->GetShowY() - 170;
		else
			m_fPosY = m_pChar->GetShowY();

		m_pAttackButton->ResetPosition(m_fPosX+m_pAttackButton->OffsetX,m_fPosY+m_pAttackButton->OffsetY);
		m_pSkillButton->ResetPosition(m_fPosX+m_pSkillButton->OffsetX,m_fPosY+m_pSkillButton->OffsetY);
		m_pItemButton->ResetPosition(m_fPosX+m_pItemButton->OffsetX,m_fPosY+m_pItemButton->OffsetY);
		m_pFinishButton->ResetPosition(m_fPosX+m_pFinishButton->OffsetX,m_fPosY+m_pFinishButton->OffsetY);	
	}
}

Character*	WndCommand::GetBindChar()
{
	return m_pChar;
}

void WndCommand::SetRenderPositon(float _x,float _y)
{
	if(_x == -1 && _y == -1)
	{
		if(m_pChar->GetShowX() + 216 > APP_WIDTH)
			m_fPosX = m_pChar->GetShowX() - 170;
		else
			m_fPosX = m_pChar->GetShowX() + 50; 
		if(m_pChar->GetShowY() + 170 > APP_HEIGHT)
			m_fPosY = m_pChar->GetShowY() - 170;
		else
			m_fPosY = m_pChar->GetShowY();
	}
	else
	{
		m_fPosX = _x; 
		m_fPosY = _y;
	}

	m_pAttackButton->ResetPosition(m_fPosX+m_pAttackButton->OffsetX,m_fPosY+m_pAttackButton->OffsetY);
	m_pSkillButton->ResetPosition(m_fPosX+m_pSkillButton->OffsetX,m_fPosY+m_pSkillButton->OffsetY);
	m_pItemButton->ResetPosition(m_fPosX+m_pItemButton->OffsetX,m_fPosY+m_pItemButton->OffsetY);
	m_pFinishButton->ResetPosition(m_fPosX+m_pFinishButton->OffsetX,m_fPosY+m_pFinishButton->OffsetY);	
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
				//SetShow(false);
				UISystem::sInstance().CloseWindow(eWindowID_Command);
				return;
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
				//SetShow(false);

				UIWindow* wndSelect = UISystem::sInstance().PopUpWindow(eWindowID_Select);
				if(wndSelect)
					wndSelect->SetBindChar(m_pChar);
				UISystem::sInstance().CloseWindow(eWindowID_Command);
				return;
				//打开技能面板
// 				UIWindow* wndSelect = UISystem::sInstance().GetWindow(eWindowID_Select);
// 				if(wndSelect)
// 				{
// 					wndSelect->SetShow(true);
// 					wndSelect->SetBindChar(m_pChar);
// 				}

			}
			m_pContainer->Leave();
		}
		else if (id == eControlID_FinishButton)
		{
			if(m_pChar)
			{
				m_pChar->SetFinish(true);
				//SetShow(false);
				UISystem::sInstance().CloseWindow(eWindowID_Command);
				return;
			}
			m_pContainer->Leave();
		}
	}
}

bool WndCommand::IsOnControl()
{
	return (m_bShow && (UIWindow::IsOnControl() || m_pAttackButton->IsMouseOn() || m_pFinishButton->IsMouseOn() || m_pSkillButton->IsMouseOn() || m_pItemButton->IsMouseOn()));
}