#include "WndSelect.h"
#include "ConfigManager.h"
#include "TexManager.h"
#include "Character.h"
#include "CreatureManager.h"

WndSelect::WndSelect():
UIWindow(TexManager::sInstance().GetUITex()[eUIID_WndCharInfo],0,0,259,151,0,0)
{
	m_pContainer = new hgeGUI;
	m_pListBox = new UIListBox(eControlID_ListBox,m_fPosX,m_fPosY,259,151,FontManager::sInstance().GetFont(FontAttr(eFontType_SongTi,eFontSize_FontBig)),0xFFFF0000,0xFF00FF00,0xFF0000FF,0xFFAAAAAA);
	
	m_pContainer->AddCtrl(m_pListBox);

	m_pListBox->GetPageMaxRows() = 5;
// 	m_pListBox->AddItem(TexManager::sInstance().GetUITex()[eUIID_SkillIconRock],"中文测试1");
// 	m_pListBox->AddItem(TexManager::sInstance().GetUITex()[eUIID_SkillIconWind],"中文测试2");
// 	m_pListBox->AddItem(TexManager::sInstance().GetUITex()[eUIID_SkillIconFire],"中文测试3");
// 	m_pListBox->AddItem(TexManager::sInstance().GetUITex()[eUIID_SkillIconRock],"中文测试4");
// 	m_pListBox->AddItem(TexManager::sInstance().GetUITex()[eUIID_SkillIconWind],"中文测试5");
// 	m_pListBox->AddItem(TexManager::sInstance().GetUITex()[eUIID_SkillIconFire],"中文测试6");
// 	m_pListBox->AddItem(TexManager::sInstance().GetUITex()[eUIID_SkillIconFire],"中文测试7");
// 	m_pListBox->AddItem(TexManager::sInstance().GetUITex()[eUIID_SkillIconRock],"中文测试8");
// 	m_pListBox->AddItem(TexManager::sInstance().GetUITex()[eUIID_SkillIconWind],"中文测试9");
// 	m_pListBox->AddItem(TexManager::sInstance().GetUITex()[eUIID_SkillIconFire],"中文测试10");

//	m_pListBox->SetItemDisabled(3,true);
}

WndSelect::~WndSelect()
{
	m_pListBox->Clear();
	m_pContainer->DelCtrl(eControlID_ListBox);
}

void WndSelect::SetRenderPositon(float _x,float _y)
{
	m_fPosX = _x; 
	m_fPosY = _y;

	m_pListBox->ResetPosition(m_fPosX,m_fPosY);
}

void WndSelect::Update(float dt)
{
	if (m_bShow)
	{
		int id = m_pContainer->Update(dt);
		if (id == -1)
		{
			m_pContainer->Enter();
		}
		else if (id == eControlID_ListBox)
		{
			int selectItem = m_pListBox->GetSelectedItem();
			CreatureManager::sInstance().GetCastSkill() = selectItem;
			m_pBindChar->SetActionStage(eActionStage_SkillStage);
			SetShow(false);
			m_pContainer->Leave();
		}
	}
}

void WndSelect::SetBindChar(Character* bindChar)
{
	m_pBindChar= bindChar;
	if (bindChar)
	{
		m_fPosX = bindChar->GetRealX() + 50; 
		m_fPosY = bindChar->GetRealY();

		m_pListBox->ResetPosition(m_fPosX,m_fPosY);
		m_pListBox->Clear();

		std::map<int,SkillInfo> mInfo = ConfigManager::sInstance().GetSkillInfo(); 
		std::vector<int> skillList = bindChar->GetSkillList();
		for (std::vector<int>::iterator it=skillList.begin();it!=skillList.end();it++)
		{
			SkillInfo info = mInfo.find(*it)->second;
			m_pListBox->AddItem(TexManager::sInstance().GetUITex()[info.m_nIcon],const_cast<char*>(info.m_strName.c_str()));
		}
	}
}