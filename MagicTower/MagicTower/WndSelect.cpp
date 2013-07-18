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
	m_mListItemToSkillId.clear();
	m_mListItemToItemId.clear();
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
			if(m_pBindChar->GetActionStage() == eActionStage_SkillStage)
			{
				std::map<int,int>::iterator it = m_mListItemToSkillId.find(selectItem);
				if(it!=m_mListItemToSkillId.end())
				{
					m_pBindChar->GetCastSkill() = it->second;
					SetShow(false);					
				}
				//错误分支
				else
					g_debugString(__FILE__,__FUNCTION__,__LINE__,"不存在选择的技能");
			}
			else if (m_pBindChar->GetActionStage() == eActionStage_ItemStage)
			{
				std::map<int,int>::iterator it = m_mListItemToItemId.find(selectItem);
				if(it!=m_mListItemToItemId.end())
				{
					m_pBindChar->GetUseItem() = it->second;
					SetShow(false);					
				}
				//错误分支
				else
					g_debugString(__FILE__,__FUNCTION__,__LINE__,"不存在选择的物品");
			}
			m_pContainer->Leave();
		}
	}
}

void WndSelect::Render()
{
	UIWindow::Render();

	GfxFont* font = FontManager::sInstance().GetFont(FontAttr(eFontType_SongTi,eFontSize_FontBig));
	font->SetColor(0xFF000000);
	if(m_pBindChar->GetActionStage() == eActionStage_SkillStage)
		font->Print(m_fPosX+120,m_fPosY+85,"%s : %d","当前MP",m_pBindChar->GetMP());
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
		m_mListItemToSkillId.clear();
		m_mListItemToItemId.clear();

		if(m_pBindChar->GetActionStage() == eActionStage_SkillStage)
		{
			std::map<int,SkillInfo> mInfo = ConfigManager::sInstance().GetSkillInfo(); 
			std::vector<int> skillList = bindChar->GetSkillList();
			for (std::vector<int>::iterator it=skillList.begin();it!=skillList.end();it++)
			{
				SkillInfo info = mInfo.find(*it)->second;
				std::string skillName = info.m_strName;
				int length = strlen(info.m_strName.c_str());
				skillName.insert(skillName.end(),30-length,' ');
				char mp[10];
				sprintf(mp,"%d",info.m_nCost);
				skillName.append(mp);
				int num = m_pListBox->AddItem(TexManager::sInstance().GetUITex()[info.m_nIcon],const_cast<char*>(skillName.c_str()));
				m_mListItemToSkillId[num] = info.m_nID;

				if(m_pBindChar->GetMP() < info.m_nCost)
					m_pListBox->SetItemDisabled(num,true);
			}
		}
		else if (m_pBindChar->GetActionStage() == eActionStage_ItemStage)
		{

		}
	}
}