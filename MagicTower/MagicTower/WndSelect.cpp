#include "WndSelect.h"
#include "ConfigManager.h"
#include "TexManager.h"
#include "Character.h"
#include "CreatureManager.h"
#include "TipWnd.h"

WndSelect::WndSelect():
UIWindow(TexManager::sInstance().GetUITex(eUIID_WndCharInfo),0,0,257,183,0,0)
{
	m_pContainer = new hgeGUI;
	m_pListBox = new UIListBox(eControlID_ListBox,m_fPosX+3,m_fPosY+8,257-3,183-8,FontManager::sInstance().GetFont(FontAttr(eFontType_SongTi,eFontSize_FontBig)),0xFFFF0000,0xFF00FF00,0xFFAAAAAA,TexManager::sInstance().GetUITex(eUIID_SelectRect));
	m_pListBox->AddPageButton(30,130,150,130,105,27,TexManager::sInstance().GetUITex(eUIID_SmallButtonUp),TexManager::sInstance().GetUITex(eUIID_SmallButtonDown),TexManager::sInstance().GetUITex(eUIID_SmallButtonOn),TexManager::sInstance().GetUITex(eUIID_SmallButtonDisable),0,0);
	m_pListBox->OffsetX = 3;
	m_pListBox->OffsetY = 8;
	m_pContainer->AddCtrl(m_pListBox);

	m_pListBox->GetPageMaxRows() = 5;
	m_mListItemToSkillId.clear();
	m_mListItemToItemId.clear();

	m_bNeedResetPos = true;
}

WndSelect::~WndSelect()
{
	m_pListBox->Clear();
	m_pContainer->DelCtrl(eControlID_ListBox);
}

void WndSelect::SetRenderPositon(float _x,float _y)
{
	if(_x == -1 && _y == -1)
	{
		m_fPosX = m_pBindChar->GetShowX() + 50; 
		m_fPosY = m_pBindChar->GetShowY();
	}
	else
	{
		m_fPosX = _x; 
		m_fPosY = _y;
	}


	m_pListBox->ResetPosition(m_fPosX + m_pListBox->OffsetX,m_fPosY + m_pListBox->OffsetY);
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
					m_pBindChar->SetActionStage(eActionStage_SkillTargetStage);
					m_pBindChar->GetCastSkill() = it->second;
					//SetShow(false);				
					UISystem::sInstance().CloseWindow(eWindowID_Select);
					return;
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
					m_pBindChar->SetActionStage(eActionStage_ItemTargetStage);
					m_pBindChar->GetUseItem() = it->second;
					//SetShow(false);				
					UISystem::sInstance().CloseWindow(eWindowID_Select);
					return;
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
		m_fPosX = bindChar->GetShowX() + 50; 
		m_fPosY = bindChar->GetShowY();

		m_pListBox->ResetPosition(m_fPosX + m_pListBox->OffsetX,m_fPosY + m_pListBox->OffsetY);
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
				sprintf(mp,"%dMP",info.m_nCost);
				skillName.append(mp);
				int num = m_pListBox->AddItem(TexManager::sInstance().GetUITex(info.m_nIcon),const_cast<char*>(skillName.c_str()));
				m_mListItemToSkillId[num] = info.m_nID;

				if(m_pBindChar->GetMP() < info.m_nCost)
					m_pListBox->SetItemDisabled(num,true);
			}
		}
		else if (m_pBindChar->GetActionStage() == eActionStage_ItemStage)
		{
			std::map<int,Item> mItem = ConfigManager::sInstance().GetItemInfo();
			LItems itemsList = CreatureManager::sInstance().GetItems();
			for (LItems::iterator it=itemsList.begin();it!=itemsList.end();it++)
			{
				if(it->m_iItem.m_nType == 1)
				{
					Item item = mItem.find(it->m_iItem.m_nID)->second;
					std::string name = item.m_strName;
					int length = strlen(item.m_strName.c_str());
					name.insert(name.end(),30-length,' ');
					char num[10];
					sprintf(num,"%d",it->m_nNum);
					name.append(num);
					int nums = m_pListBox->AddItem(0,const_cast<char*>(name.c_str()));
					m_mListItemToItemId[nums] = item.m_nID;
				}
			}
		}
	}
}

void WndSelect::OnMouseOver(float x,float y)
{
	int mouseOnItem = m_pListBox->GetMouseOnItem();
	if(mouseOnItem == -1)
		return;

	if(m_pBindChar->GetActionStage() == eActionStage_SkillStage)
	{
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
	else if(m_pBindChar->GetActionStage() == eActionStage_ItemStage)
	{
		std::map<int,int>::iterator it = m_mListItemToItemId.find(mouseOnItem);
		if(it!=m_mListItemToItemId.end())
		{
			std::map<int,Item>::iterator mit = ConfigManager::sInstance().GetItemInfo().find(it->second);
			if(mit!=ConfigManager::sInstance().GetItemInfo().end())
			{
				TipWnd::sInstance().ParseItem(mit->second);
				TipWnd::sInstance().SetOffset(x+m_fPosX+15,y+m_fPosY+15);
			}
		}
	}
}