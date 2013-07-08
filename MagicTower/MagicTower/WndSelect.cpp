#include "WndSelect.h"
#include "TexManager.h"

WndSelect::WndSelect():
UIWindow(TexManager::sInstance().GetUITex()[eUIID_WndCharInfo],0,0,259,151,0,0)
{
	m_pContainer = new hgeGUI;
	m_pListBox = new UIListBox(eControlID_ListBox,m_fPosX,m_fPosY,259,151,FontManager::sInstance().GetFont(FontAttr(eFontType_SongTi,eFontSize_FontBig)),0xFFFF0000,0xFF00FF00,0xFF0000FF);
	
	m_pContainer->AddCtrl(m_pListBox);

	m_pListBox->AddItem(TexManager::sInstance().GetUITex()[eUIID_SkillIconRock],"中文测试1");
	m_pListBox->AddItem(TexManager::sInstance().GetUITex()[eUIID_SkillIconWind],"中文测试2");
	m_pListBox->AddItem(TexManager::sInstance().GetUITex()[eUIID_SkillIconFire],"中文测试3");
	m_pListBox->AddItem(TexManager::sInstance().GetUITex()[eUIID_SkillIconRock],"中文测试5");
	m_pListBox->AddItem(TexManager::sInstance().GetUITex()[eUIID_SkillIconWind],"中文测试6");
	m_pListBox->AddItem(TexManager::sInstance().GetUITex()[eUIID_SkillIconFire],"中文测试7");
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
			m_pContainer->Leave();
		}
	}
}