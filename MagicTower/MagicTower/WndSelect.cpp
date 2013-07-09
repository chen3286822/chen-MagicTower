#include "WndSelect.h"
#include "TexManager.h"

WndSelect::WndSelect():
UIWindow(TexManager::sInstance().GetUITex()[eUIID_WndCharInfo],0,0,259,151,0,0)
{
	m_pContainer = new hgeGUI;
	m_pListBox = new UIListBox(eControlID_ListBox,m_fPosX,m_fPosY,259,151,FontManager::sInstance().GetFont(FontAttr(eFontType_SongTi,eFontSize_FontBig)),0xFFFF0000,0xFF00FF00,0xFF0000FF,0xFFAAAAAA);
	
	m_pContainer->AddCtrl(m_pListBox);

	m_pListBox->GetPageMaxRows() = 3;
	m_pListBox->AddItem(TexManager::sInstance().GetUITex()[eUIID_SkillIconRock],"���Ĳ���1");
	m_pListBox->AddItem(TexManager::sInstance().GetUITex()[eUIID_SkillIconWind],"���Ĳ���2");
	m_pListBox->AddItem(TexManager::sInstance().GetUITex()[eUIID_SkillIconFire],"���Ĳ���3");
	m_pListBox->AddItem(TexManager::sInstance().GetUITex()[eUIID_SkillIconRock],"���Ĳ���4");
	m_pListBox->AddItem(TexManager::sInstance().GetUITex()[eUIID_SkillIconWind],"���Ĳ���5");
	m_pListBox->AddItem(TexManager::sInstance().GetUITex()[eUIID_SkillIconFire],"���Ĳ���6");
	m_pListBox->AddItem(TexManager::sInstance().GetUITex()[eUIID_SkillIconFire],"���Ĳ���7");
	m_pListBox->AddItem(TexManager::sInstance().GetUITex()[eUIID_SkillIconRock],"���Ĳ���8");
	m_pListBox->AddItem(TexManager::sInstance().GetUITex()[eUIID_SkillIconWind],"���Ĳ���9");
	m_pListBox->AddItem(TexManager::sInstance().GetUITex()[eUIID_SkillIconFire],"���Ĳ���10");

	m_pListBox->SetItemDisabled(3,true);
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
			OutputDebugString(m_pListBox->GetItemText(selectItem));
			m_pContainer->Leave();
		}
	}
}