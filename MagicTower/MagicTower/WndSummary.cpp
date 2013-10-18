#include "WndSummary.h"
#include "Character.h"
#include "FontManager.h"
#include "TexManager.h"
#include "App.h"
#include "TipWnd.h"

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

	m_pAnim = NULL;

	m_pIcons[0] = new hgeSprite(TexManager::sInstance().GetUITex(eUIID_AttackIcon),0,0,26,25);	//物理攻击
	m_pIcons[1] = new hgeSprite(TexManager::sInstance().GetUITex(eUIID_SkillDamageIcon),0,0,26,21);	//法术攻击
	m_pIcons[2] = new hgeSprite(TexManager::sInstance().GetUITex(eUIID_DefendIcon),0,0,28,24);	//物理防御
	m_pIcons[3] = new hgeSprite(TexManager::sInstance().GetUITex(eUIID_MagicDefendIcon),0,0,28,26);	//法术防御
	m_pIcons[4] = new hgeSprite(TexManager::sInstance().GetUITex(eUIID_DodgeIcon),0,0,28,27);	//闪避
	m_pIcons[5] = new hgeSprite(TexManager::sInstance().GetUITex(eUIID_CritIcon),0,0,27,24);	//暴击

	m_nShowType = 1;
}

WndSummary::~WndSummary()
{
	m_pContainer->DelCtrl(eControlID_CharButton);
	m_pContainer->DelCtrl(eControlID_ArmsButton);
	m_pContainer->DelCtrl(eControlID_EquipButton);
	m_pContainer->DelCtrl(eControlID_SkillButton);
	gSafeDelete(m_pHead);
	gSafeDelete(m_pAnim);
	for(int i=0;i<6;i++)
		gSafeDelete(m_pIcons[i]);
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
			if(m_pAnim == NULL)
			{
				m_pAnim = new hgeAnimation(TexManager::sInstance().GetTex(m_pBindChar->GetID())[eActionTex_Walk],2,4,0,4*FLOAT_PIC_SQUARE_WIDTH,FLOAT_PIC_SQUARE_WIDTH,FLOAT_PIC_SQUARE_WIDTH);
				m_pAnim->SetMode(HGEANIM_LOOP|HGEANIM_FWD);
				m_pAnim->Play();
			}
			else			
				m_pAnim->SetTexture(TexManager::sInstance().GetTex(m_pBindChar->GetID())[eActionTex_Walk]);
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
		if(m_pAnim)
			m_pAnim->Update(App::sInstance().GetHGE()->Timer_GetDelta());
	}
}

void WndSummary::Render()
{
	UIWindow::Render();

	//人物介绍
	if (m_nShowType == 1)
	{
		//绘制头像
		m_pHead->Render(m_fPosX+80,m_fPosY+40);

		GfxFont* font = FontManager::sInstance().GetFont(FontAttr(eFontType_MSYaHei,eFontSize_FontBig));
		font->SetColor(0xFF000000);
		//名字
		font->Print(m_fPosX+93,m_fPosY+126,"%s",m_pBindChar->GetName().c_str());

		//绘制属性
		font = FontManager::sInstance().GetFont(FontAttr(eFontType_MSYaHei,eFontSize_FontMiddle));
		font->SetColor(0xFF000000);
		font->Print(m_fPosX+160,m_fPosY+48,"武力");
		App::sInstance().DrawBox(m_fPosX+190,m_fPosY+55,0xFF000000,1,100,5);
		float length = 100*((float)m_pBindChar->GetForce()/100.0f);
		if(length >= 100)
			length = 100;
		App::sInstance().DrawRect(m_fPosX+191,m_fPosY+56,m_fPosX+189+length,m_fPosY+54+5,0xFF0000FF);

		font->Print(m_fPosX+160,m_fPosY+68,"智力");
		App::sInstance().DrawBox(m_fPosX+190,m_fPosY+75,0xFF000000,1,100,5);
		length = 100*((float)m_pBindChar->GetIntelligence()/100.0f);
		if(length >= 100)
			length = 100;
		App::sInstance().DrawRect(m_fPosX+191,m_fPosY+76,m_fPosX+189+length,m_fPosY+74+5,0xFF00FF00);

		font->Print(m_fPosX+160,m_fPosY+88,"魅力");
		App::sInstance().DrawBox(m_fPosX+190,m_fPosY+95,0xFF000000,1,100,5);
		length = 100*((float)m_pBindChar->GetIntelligence()/100.0f);
		if(length >= 100)
			length = 100;
		App::sInstance().DrawRect(m_fPosX+191,m_fPosY+96,m_fPosX+189+length,m_fPosY+94+5,0xFFFFFFFF);

		font->SetColor(0xFF0000FF);
		font->Print(m_fPosX+295,m_fPosY+48,"%d",m_pBindChar->GetForce());
		font->SetColor(0xFF00FF00);
		font->Print(m_fPosX+295,m_fPosY+68,"%d",m_pBindChar->GetIntelligence());
		font->SetColor(0xFFFFFFFF);
		font->Print(m_fPosX+295,m_fPosY+88,"%d",m_pBindChar->GetCharm());

		//绘制介绍
		font->SetTextWidth(120);
		font->Print(m_fPosX+350,m_fPosY+45,"%s",m_pBindChar->GetIntro());
	}
	//兵种介绍
	else if (m_nShowType == 2)
	{
		if(m_pAnim)
			m_pAnim->Render(m_fPosX+90,m_fPosY+50);

		GfxFont* font = FontManager::sInstance().GetFont(FontAttr(eFontType_MSYaHei,eFontSize_FontBig));
		font->SetColor(0xFF000000);
		//名字
		font->Print(m_fPosX+103,m_fPosY+121,"%s",m_pBindChar->GetKind().c_str());

		//6项基本属性
		for (int i=0;i<6;i++)
		{
			m_pIcons[i]->Render(m_fPosX+160,m_fPosY+50+i*30);
		}
	}
}

void WndSummary::OnMouseOver(float x,float y)
{
	char temp[256] = {0};
	if (x >= 90 && x < 90+FLOAT_PIC_SQUARE_WIDTH &&
		y >= 50 && y < 50+FLOAT_PIC_SQUARE_WIDTH && m_nShowType==2)
	{
		TipWnd::sInstance().Clear();
		//测试，暂时没有加入兵种信息
		sprintf(temp,"    近卫兵，重步兵的进阶兵种。拥有更加坚固的盔甲，以及更高的生命。");
		TipWnd::sInstance().AddText(temp,0xFFFFFFFF,-1,-1,eFontType_MSYaHei,eFontSize_FontMiddle,true,60);
		TipWnd::sInstance().SetShow(true);
		TipWnd::sInstance().SetOffset(x+m_fPosX+15,y+m_fPosY+15);
	}
	else
	{
		TipWnd::sInstance().Clear();
		TipWnd::sInstance().SetShow(false);
	}
}

bool WndSummary::IsOnControl()
{
	return (m_bShow && (UIWindow::IsOnControl() || m_pCharButton->IsMouseOn() || m_pArmsButton->IsMouseOn() || m_pEquipButton->IsMouseOn() || m_pSkillButton->IsMouseOn()));
}