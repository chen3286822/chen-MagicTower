#include "WndSummary.h"
#include "Character.h"
#include "FontManager.h"
#include "TexManager.h"
#include "App.h"
#include "TipWnd.h"

std::string WndSummary::m_sStrAttr[6] = {"物理攻击，普通攻击期望造成的伤害","技能攻击，技能攻击造成的伤害加成",
	"物理防御，用于防御物理攻击","技能防御，用于防御技能伤害",
	"闪避，躲闪敌方物理攻击的几率","暴击，对敌方造成两倍伤害的几率"};

WndSummary::WndSummary() :
UIWindow(TexManager::sInstance().GetUITex(eUIID_WndSummary),0,0,500,184,0,0)
{
	m_pContainer = new hgeGUI;
	m_pCharButton = new UIButton(eControlID_CharButton,m_fPosX+50,m_fPosY+150,105,27,TexManager::sInstance().GetUITex(eUIID_SmallButtonUp),TexManager::sInstance().GetUITex(eUIID_SmallButtonDown),TexManager::sInstance().GetUITex(eUIID_SmallButtonOn),TexManager::sInstance().GetUITex(eUIID_SmallButtonDisable),0,0);
	m_pArmsButton = new UIButton(eControlID_ArmsButton,m_fPosX+165,m_fPosY+150,105,27,TexManager::sInstance().GetUITex(eUIID_SmallButtonUp),TexManager::sInstance().GetUITex(eUIID_SmallButtonDown),TexManager::sInstance().GetUITex(eUIID_SmallButtonOn),TexManager::sInstance().GetUITex(eUIID_SmallButtonDisable),0,0);
	m_pSkillButton = new UIButton(eControlID_SkillButton,m_fPosX+280,m_fPosY+150,105,27,TexManager::sInstance().GetUITex(eUIID_SmallButtonUp),TexManager::sInstance().GetUITex(eUIID_SmallButtonDown),TexManager::sInstance().GetUITex(eUIID_SmallButtonOn),TexManager::sInstance().GetUITex(eUIID_SmallButtonDisable),0,0);


	m_pCharButton->OffsetX = 80;
	m_pCharButton->OffsetY = 150;
	m_pArmsButton->OffsetX = 205;
	m_pArmsButton->OffsetY = 150;
	m_pSkillButton->OffsetX = 330;
	m_pSkillButton->OffsetY = 150;
	m_pContainer->AddCtrl(m_pCharButton);
	m_pContainer->AddCtrl(m_pArmsButton);
	m_pContainer->AddCtrl(m_pSkillButton);

	m_pCharButton->SetFont(eFontType_MSYaHei,eFontSize_FontMiddle);
	m_pCharButton->SetText("人物介绍",0xFF9D988E);

	m_pArmsButton->SetFont(eFontType_MSYaHei,eFontSize_FontMiddle);
	m_pArmsButton->SetText("兵种介绍",0xFF9D988E);

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
	
	m_pWeaponGrid = new hgeSprite(TexManager::sInstance().GetUITex(eUIID_WeaponGrid),0,0,52,52);	//武器栏
	m_pClothGrid = new hgeSprite(TexManager::sInstance().GetUITex(eUIID_ClothGrid),0,0,52,52);	//盔甲栏
	m_pEquipGrid = new hgeSprite(TexManager::sInstance().GetUITex(eUIID_EquipGrid),0,0,52,52);	//装备栏

	for (int i=0;i<eEquipGrid_TotalEquip;i++)
	{
		m_pEquip[i] = NULL;
	}

	m_pListBox = new UIListBox(eControlID_ListBox,m_fPosX+20,m_fPosY+50,257-20,95-10,FontManager::sInstance().GetFont(FontAttr(eFontType_SongTi,eFontSize_FontBig)),0xFFFF0000,0xFF00FF00,0xFFAAAAAA,TexManager::sInstance().GetUITex(eUIID_SelectRect));
	m_pListBox->AddPageButton(30,95,150,95,53,14,TexManager::sInstance().GetUITex(eUIID_SmallerButtonUp),TexManager::sInstance().GetUITex(eUIID_SmallerButtonDown),TexManager::sInstance().GetUITex(eUIID_SmallerButtonOn),TexManager::sInstance().GetUITex(eUIID_SmallerButtonDisable),0,0);
	m_pListBox->OffsetX = 50;
	m_pListBox->OffsetY = 30;
	m_pContainer->AddCtrl(m_pListBox);
	m_pListBox->GetPageMaxRows() = 5;
	m_pListBox->bVisible = false;
	m_pListBox->bEnabled = false;
	m_mListItemToSkillId.clear();

	m_pSkillRange = new hgeSprite(TexManager::sInstance().GetUITex(eUIID_RangeNone),0,0,64,64);

	m_nShowType = 1;
}

WndSummary::~WndSummary()
{
	m_pListBox->Clear();
	m_pContainer->DelCtrl(eControlID_CharButton);
	m_pContainer->DelCtrl(eControlID_ArmsButton);
	m_pContainer->DelCtrl(eControlID_SkillButton);
	m_pContainer->DelCtrl(eControlID_ListBox);
	gSafeDelete(m_pHead);
	gSafeDelete(m_pAnim);
	for(int i=0;i<6;i++)
		gSafeDelete(m_pIcons[i]);
	gSafeDelete(m_pWeaponGrid);
	gSafeDelete(m_pClothGrid);
	gSafeDelete(m_pEquipGrid);
	for (int i=0;i<eEquipGrid_TotalEquip;i++)
		gSafeDelete(m_pEquip[i]);
	gSafeDelete(m_pSkillRange);
}

void	WndSummary::SetBindChar(Character* bindChar)
{
	m_pBindChar = bindChar;
	m_pHead->SetTexture(TexManager::sInstance().GetHeadTex(bindChar->GetHead()));
	if (bindChar)
	{
		if(bindChar->GetShowX() + 550 > APP_WIDTH)
			m_fPosX = 250;
		else
			m_fPosX = bindChar->GetShowX() + 50; 
 		if(bindChar->GetShowY() + 184 + FLOAT_PIC_SQUARE_HEIGHT > APP_HEIGHT)
 			m_fPosY = bindChar->GetShowY() - 184;
		else
			m_fPosY = bindChar->GetShowY() + FLOAT_PIC_SQUARE_HEIGHT;

		m_pListBox->Clear();
		m_mListItemToSkillId.clear();
		std::map<int,SkillInfo> mInfo = ConfigManager::sInstance().GetSkillInfo(); 
		std::vector<int> skillList = bindChar->GetSkillList();
		for (std::vector<int>::iterator it=skillList.begin();it!=skillList.end();it++)
		{
			SkillInfo info = mInfo.find(*it)->second;
			std::string skillName = info.m_strName;
			int length = strlen(info.m_strName.c_str());
			skillName.insert(skillName.end(),27-length,' ');
			char mp[10];
			sprintf(mp,"%dMP",info.m_nCost);
			skillName.append(mp);
			int num = m_pListBox->AddItem(TexManager::sInstance().GetUITex(info.m_nIcon),const_cast<char*>(skillName.c_str()));
			m_mListItemToSkillId[num] = info.m_nID;
		}

		m_pCharButton->ResetPosition(m_fPosX+m_pCharButton->OffsetX,m_fPosY+m_pCharButton->OffsetY);
		m_pArmsButton->ResetPosition(m_fPosX+m_pArmsButton->OffsetX,m_fPosY+m_pArmsButton->OffsetY);
		m_pSkillButton->ResetPosition(m_fPosX+m_pSkillButton->OffsetX,m_fPosY+m_pSkillButton->OffsetY);
		m_pListBox->ResetPosition(m_fPosX + m_pListBox->OffsetX,m_fPosY + m_pListBox->OffsetY);
	}
}

void WndSummary::SetRenderPositon(float _x,float _y)
{
	m_fPosX = _x; 
	m_fPosY = _y;

	m_pCharButton->ResetPosition(m_fPosX+m_pCharButton->OffsetX,m_fPosY+m_pCharButton->OffsetY);
	m_pArmsButton->ResetPosition(m_fPosX+m_pArmsButton->OffsetX,m_fPosY+m_pArmsButton->OffsetY);
	m_pSkillButton->ResetPosition(m_fPosX+m_pSkillButton->OffsetX,m_fPosY+m_pSkillButton->OffsetY);
	m_pListBox->ResetPosition(m_fPosX + m_pListBox->OffsetX,m_fPosY + m_pListBox->OffsetY);
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
			m_pListBox->bVisible = false;
			m_pListBox->bEnabled = false;
			m_nShowType = 1;
			m_pContainer->Leave();
		}
		else if (id == eControlID_ArmsButton)
		{
			m_pListBox->bVisible = false;
			m_pListBox->bEnabled = false;
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
		else if (id == eControlID_SkillButton)
		{
			m_nShowType = 3;
			m_pListBox->bVisible = true;
			m_pListBox->bEnabled = true;
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
		App::sInstance().DrawRect(m_fPosX+190,m_fPosY+55,m_fPosX+190+length,m_fPosY+55+5,0xFF0000FF);

		font->Print(m_fPosX+160,m_fPosY+68,"智力");
		App::sInstance().DrawBox(m_fPosX+190,m_fPosY+75,0xFF000000,1,100,5);
		length = 100*((float)m_pBindChar->GetIntelligence()/100.0f);
		if(length >= 100)
			length = 100;
		App::sInstance().DrawRect(m_fPosX+190,m_fPosY+75,m_fPosX+190+length,m_fPosY+75+5,0xFF00FF00);

		font->Print(m_fPosX+160,m_fPosY+88,"魅力");
		App::sInstance().DrawBox(m_fPosX+190,m_fPosY+95,0xFF000000,1,100,5);
		length = 100*((float)m_pBindChar->GetCharm()/100.0f);
		if(length >= 100)
			length = 100;
		App::sInstance().DrawRect(m_fPosX+190,m_fPosY+95,m_fPosX+190+length,m_fPosY+95+5,0xFFFFFFFF);

		font->Print(m_fPosX+160,m_fPosY+108,"经验");
		App::sInstance().DrawBox(m_fPosX+190,m_fPosY+115,0xFF000000,1,100,5);
		length = 100*((float)m_pBindChar->GetExp()/100.0f);
		if(length >= 100)
			length = 100;
		App::sInstance().DrawRect(m_fPosX+190,m_fPosY+115,m_fPosX+190+length,m_fPosY+115+5,0xFFAC1FB4);

		font->SetColor(0xFF0000FF);
		font->Print(m_fPosX+295,m_fPosY+48,"%d",m_pBindChar->GetForce());
		font->SetColor(0xFF00FF00);
		font->Print(m_fPosX+295,m_fPosY+68,"%d",m_pBindChar->GetIntelligence());
		font->SetColor(0xFFFFFFFF);
		font->Print(m_fPosX+295,m_fPosY+88,"%d",m_pBindChar->GetCharm());
		font->SetColor(0xFFAC1FB4);
		font->Print(m_fPosX+295,m_fPosY+108,"Lv.%d",m_pBindChar->GetLevel());

		//绘制介绍
		font->SetColor(0xFFFFFFFF);
		font->SetTextWidth(120);
		font->Print(m_fPosX+350,m_fPosY+45,"%s",m_pBindChar->GetIntro());
		font->SetTextWidth(-1);
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
		font = FontManager::sInstance().GetFont(FontAttr(eFontType_MSYaHei,eFontSize_FontMiddle));
		font->SetColor(0xFF000000);
		float attr[6] = {m_pBindChar->GetAttack(),m_pBindChar->GetSkillDamage(),m_pBindChar->GetDefend(),
			m_pBindChar->GetSkillDefend(),m_pBindChar->GetDodge(),m_pBindChar->GetCrit()};
		for (int i=0;i<3;i++)
		{
			for(int j=0;j<2;j++)
			{
				m_pIcons[j+i*2]->Render(m_fPosX+160+j*60,m_fPosY+50+i*30);
				if(j + i*2 >= 4)
					font->Print(m_fPosX+160+j*60+28,m_fPosY+52+i*30,"%d%%",(int)(100*attr[j+i*2]));
				else
					font->Print(m_fPosX+160+j*60+28,m_fPosY+52+i*30,"%.0f",attr[j+i*2]);
			}
		}

		//3个物品栏
		m_pWeaponGrid->Render(m_fPosX+290,m_fPosY+52);
		m_pClothGrid->Render(m_fPosX+360,m_fPosY+52);
		m_pEquipGrid->Render(m_fPosX+430,m_fPosY+52);
		font = FontManager::sInstance().GetFont(FontAttr(eFontType_MSYaHei,eFontSize_FontBig));
		font->SetColor(0xFFFFFFFF);
		for (int i=0;i<eEquipGrid_TotalEquip;i++)
		{
			Item itemEquip = m_pBindChar->GetEquip(i);
			if(itemEquip.m_nID != -1)
			{
				if(m_pEquip[i] == NULL)
					m_pEquip[i] = new hgeSprite(TexManager::sInstance().GetItemTex(itemEquip.m_nID),0,0,32,32);
				else
					m_pEquip[i]->SetTexture(TexManager::sInstance().GetItemTex(itemEquip.m_nID));
				m_pEquip[i]->Render(m_fPosX+300+i*70,m_fPosY+62);

				char temp[256];
				sprintf(temp,itemEquip.m_strName.c_str());
				g_getAlignString(temp,52,eAlign_Center,eFontType_MSYaHei,eFontSize_FontBig);
				font->Print(m_fPosX+293+70*i,m_fPosY+105,temp);
			}
		}
	}
	//查看技能
	else if (m_nShowType == 3)
	{
		int selectItem = m_pListBox->GetSelectedItem();
		if (selectItem != -1)
		{
			std::map<int,int>::iterator it = m_mListItemToSkillId.find(selectItem);
			if(it!=m_mListItemToSkillId.end())
			{
				std::map<int,SkillInfo>::iterator mInfo = ConfigManager::sInstance().GetSkillInfo().find(it->second);
				if (mInfo != ConfigManager::sInstance().GetSkillInfo().end())
				{
					m_pSkillRange->SetTexture(TexManager::sInstance().GetUITex(eUIID_RangeNone+mInfo->second.m_nAttackRange));
				}
			}
		}
		m_pSkillRange->Render(m_fPosX+300,m_fPosY+40);
	}
}

void WndSummary::OnMouseOver(float x,float y)
{
	char temp[256] = {0};
	if (m_nShowType == 1)
	{
		if(x >= 190 && x < 290 && y >= 115 && y< 119)
		{
			TipWnd::sInstance().Clear();
			sprintf(temp,"%d/%d",m_pBindChar->GetExp(),m_pBindChar->GetExpTotal());
			TipWnd::sInstance().AddText(temp,0xFFFFFFFF,-1,-1,eFontType_MSYaHei,eFontSize_FontMiddle,false);
			TipWnd::sInstance().SetShow(true);
			TipWnd::sInstance().SetOffset(x+m_fPosX+15,y+m_fPosY+15);
			return;
		}
	}
	else if (m_nShowType == 2)
	{
		if(x >= 90 && x < 90+FLOAT_PIC_SQUARE_WIDTH &&
		y >= 50 && y < 50+FLOAT_PIC_SQUARE_WIDTH)
		{
			TipWnd::sInstance().Clear();
			//测试，暂时没有加入兵种信息
			sprintf(temp,"    近卫兵，重步兵的进阶兵种。拥有更加坚固的盔甲，以及更高的生命。");
			TipWnd::sInstance().AddText(temp,0xFFFFFFFF,-1,-1,eFontType_MSYaHei,eFontSize_FontMiddle,true,60);
			TipWnd::sInstance().SetShow(true);
			TipWnd::sInstance().SetOffset(x+m_fPosX+15,y+m_fPosY+15);
			return;
		}
		else
		{
			for (int n=0;n<eEquipGrid_TotalEquip;n++)
			{
				if(x>= 300+n*70 && x<300+n*70+52 && y>=62 && y<62+52)
				{
					Item itemEquip = m_pBindChar->GetEquip(n);
					if(itemEquip.m_nID != -1)
					{
						TipWnd::sInstance().ParseItem(itemEquip);
						TipWnd::sInstance().SetOffset(x+m_fPosX+15,y+m_fPosY+15);
					}
					return;
				}
			}

			for (int i=0;i<3;i++)
			{
				for(int j=0;j<2;j++)
				{
					if(x >= 160+j*60 && x < 160+j*60+26 && y >=50+i*30 && y < 50+i*30+25)
					{
						TipWnd::sInstance().Clear();
						sprintf(temp,"%s",m_sStrAttr[j+i*2].c_str());
						TipWnd::sInstance().AddText(temp,0xFFFFFFFF,-1,-1,eFontType_MSYaHei,eFontSize_FontMiddle,false);
						TipWnd::sInstance().SetShow(true);
						TipWnd::sInstance().SetOffset(x+m_fPosX+15,y+m_fPosY+15);
						return;
					}
				}
			}
		}
	}
	else if (m_nShowType == 3)
	{
		int mouseOnItem = m_pListBox->GetMouseOnItem();
		std::map<int,int>::iterator it = m_mListItemToSkillId.find(mouseOnItem);
		if(it!=m_mListItemToSkillId.end())
		{
			std::map<int,SkillInfo>::iterator mInfo = ConfigManager::sInstance().GetSkillInfo().find(it->second);
			if (mInfo != ConfigManager::sInstance().GetSkillInfo().end())
			{
				TipWnd::sInstance().ParseSkill(mInfo->second);
				TipWnd::sInstance().SetOffset(x+m_fPosX+15,y+m_fPosY+15);
			}
		}
	}
	else
	{
		TipWnd::sInstance().Clear();
		TipWnd::sInstance().SetShow(false);
		return;
	}
}

bool WndSummary::IsOnControl()
{
	return (m_bShow && (UIWindow::IsOnControl() || m_pCharButton->IsMouseOn() || m_pArmsButton->IsMouseOn() || m_pSkillButton->IsMouseOn()));
}