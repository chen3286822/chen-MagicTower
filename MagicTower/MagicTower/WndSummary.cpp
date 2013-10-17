#include "WndSummary.h"
#include "Character.h"
#include "FontManager.h"
#include "App.h"

WndSummary::WndSummary() :
UIWindow(TexManager::sInstance().GetUITex(eUIID_WndSummary),0,0,500,184,0,0)
{
	m_pContainer = new hgeGUI;
	m_pCharButton = new UIButton(eControlID_CharButton,m_fPosX+50,m_fPosY+150,105,27,TexManager::sInstance().GetUITex(eUIID_SmallButtonUp),TexManager::sInstance().GetUITex(eUIID_SmallButtonDown),TexManager::sInstance().GetUITex(eUIID_SmallButtonOn),TexManager::sInstance().GetUITex(eUIID_SmallButtonDisable),0,0);
	m_pArmsButton = new UIButton(eControlID_ArmsButton,m_fPosX+165,m_fPosY+150,105,27,TexManager::sInstance().GetUITex(eUIID_SmallButtonUp),TexManager::sInstance().GetUITex(eUIID_SmallButtonDown),TexManager::sInstance().GetUITex(eUIID_SmallButtonOn),TexManager::sInstance().GetUITex(eUIID_SmallButtonDisable),0,0);
	m_pEquipButton = new UIButton(eControlID_EquipButton,m_fPosX+280,m_fPosY+150,105,27,TexManager::sInstance().GetUITex(eUIID_SmallButtonUp),TexManager::sInstance().GetUITex(eUIID_SmallButtonDown),TexManager::sInstance().GetUITex(eUIID_SmallButtonOn),TexManager::sInstance().GetUITex(eUIID_SmallButtonDisable),0,0);
	m_pSkillButton = new UIButton(eControlID_SkillButton,m_fPosX+395,m_fPosY+150,105,27,TexManager::sInstance().GetUITex(eUIID_SmallButtonUp),TexManager::sInstance().GetUITex(eUIID_SmallButtonDown),TexManager::sInstance().GetUITex(eUIID_SmallButtonOn),TexManager::sInstance().GetUITex(eUIID_SmallButtonDisable),0,0);


	m_pCharButton->OffsetX = 50;
	m_pCharButton->OffsetY = 150;
	m_pArmsButton->OffsetX = 165;
	m_pArmsButton->OffsetY = 150;
	m_pEquipButton->OffsetX = 280;
	m_pEquipButton->OffsetY = 150;
	m_pSkillButton->OffsetX = 395;
	m_pSkillButton->OffsetY = 150;
	m_pContainer->AddCtrl(m_pCharButton);
	m_pContainer->AddCtrl(m_pArmsButton);
	m_pContainer->AddCtrl(m_pEquipButton);
	m_pContainer->AddCtrl(m_pSkillButton);

	m_pCharButton->SetFont(eFontType_MSYaHei,eFontSize_FontMiddle);
	m_pCharButton->SetText("人物介绍",0xFF9D988E);

	m_pArmsButton->SetFont(eFontType_MSYaHei,eFontSize_FontMiddle);
	m_pArmsButton->SetText("兵种介绍",0xFF9D988E);

	m_pEquipButton->SetFont(eFontType_MSYaHei,eFontSize_FontMiddle);
	m_pEquipButton->SetText("查看装备",0xFF9D988E);

	m_pSkillButton->SetFont(eFontType_MSYaHei,eFontSize_FontMiddle);
	m_pSkillButton->SetText("查看技能",0xFF9D988E);

	m_pBindChar = NULL;
	m_pHead = new hgeSprite(0,0,0,64,80);

	m_nShowType = 1;
}

WndSummary::~WndSummary()
{
	m_pContainer->DelCtrl(eControlID_CharButton);
	m_pContainer->DelCtrl(eControlID_ArmsButton);
	m_pContainer->DelCtrl(eControlID_EquipButton);
	m_pContainer->DelCtrl(eControlID_SkillButton);
	gSafeDelete(m_pHead);
}

void	WndSummary::SetBindChar(Character* bindChar)
{
	m_pBindChar = bindChar;
	m_pHead->SetTexture(TexManager::sInstance().GetHeadTex(bindChar->GetHead()));
	if (bindChar)
	{
		if(bindChar->GetRealX() + 550 > APP_WIDTH)
			m_fPosX = 250;
		else
			m_fPosX = bindChar->GetRealX() + 50; 
 		if(bindChar->GetRealY() + 184 + FLOAT_PIC_SQUARE_HEIGHT > APP_HEIGHT)
 			m_fPosY = bindChar->GetRealY() - 184;
		else
			m_fPosY = bindChar->GetRealY() + FLOAT_PIC_SQUARE_HEIGHT;

		m_pCharButton->ResetPosition(m_fPosX+m_pCharButton->OffsetX,m_fPosY+m_pCharButton->OffsetY);
		m_pArmsButton->ResetPosition(m_fPosX+m_pArmsButton->OffsetX,m_fPosY+m_pArmsButton->OffsetY);
		m_pEquipButton->ResetPosition(m_fPosX+m_pEquipButton->OffsetX,m_fPosY+m_pEquipButton->OffsetY);
		m_pSkillButton->ResetPosition(m_fPosX+m_pSkillButton->OffsetX,m_fPosY+m_pSkillButton->OffsetY);
	}
}

void WndSummary::SetRenderPositon(float _x,float _y)
{
	m_fPosX = _x; 
	m_fPosY = _y;

	m_pCharButton->ResetPosition(m_fPosX+m_pCharButton->OffsetX,m_fPosY+m_pCharButton->OffsetY);
	m_pArmsButton->ResetPosition(m_fPosX+m_pArmsButton->OffsetX,m_fPosY+m_pArmsButton->OffsetY);
	m_pEquipButton->ResetPosition(m_fPosX+m_pEquipButton->OffsetX,m_fPosY+m_pEquipButton->OffsetY);
	m_pSkillButton->ResetPosition(m_fPosX+m_pSkillButton->OffsetX,m_fPosY+m_pSkillButton->OffsetY);
}

void WndSummary::Update(float dt)
{
	if (m_bShow)
	{
		int id = m_pContainer->Update(dt);
		if (id == -1)
		{
			m_pContainer->Enter();
		}
		else if (id == eControlID_CharButton)
		{
			m_nShowType = 1;
			m_pContainer->Leave();
		}
		else if (id == eControlID_ArmsButton)
		{
			m_nShowType = 2;
			m_pContainer->Leave();
		}
		else if (id == eControlID_EquipButton)
		{
			m_nShowType = 3;
			m_pContainer->Leave();
		}
		else if (id == eControlID_SkillButton)
		{
			m_nShowType = 4;
			m_pContainer->Leave();
		}
	}
}

void WndSummary::Render()
{
	UIWindow::Render();

	if (m_nShowType == 1)
	{
		//绘制头像
		m_pHead->Render(m_fPosX+100,m_fPosY+40);

		GfxFont* font = FontManager::sInstance().GetFont(FontAttr(eFontType_SongTi,eFontSize_FontBig));
		font->SetColor(0xFF000000);

		//绘制属性
		font->Print(m_fPosX+200,m_fPosY+40,"%d/%d",m_pBindChar->GetHP(),m_pBindChar->GetHPMax());
		App::sInstance().DrawBox(m_fPosX+180,m_fPosY+55,0xFFFF0000,1,100,5);
		float length = 100*((float)m_pBindChar->GetHP()/(float)m_pBindChar->GetHPMax());
		App::sInstance().DrawRect(m_fPosX+181,m_fPosY+56,m_fPosX+179+length,m_fPosY+54+5,0xFFFF0000);
	}
}

bool WndSummary::IsOnControl()
{
	return (m_bShow && (UIWindow::IsOnControl() || m_pCharButton->IsMouseOn() || m_pArmsButton->IsMouseOn() || m_pEquipButton->IsMouseOn() || m_pSkillButton->IsMouseOn()));
}