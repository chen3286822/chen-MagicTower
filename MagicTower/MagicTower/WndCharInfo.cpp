#include "WndCharInfo.h"
#include "App.h"
#include "Character.h"
#include "TexManager.h"
#include "FontManager.h"

WndCharInfo::WndCharInfo() :
	UIWindow(TexManager::sInstance().GetUITex(eUIID_WndCharInfo),0,0,259,151,0,0)
{
	m_pContainer = new hgeGUI;
	m_pBindChar = NULL;
	m_pHead = new hgeSprite(0,0,0,FLOAT_PIC_SQUARE_WIDTH,FLOAT_PIC_SQUARE_WIDTH);
}

WndCharInfo::~WndCharInfo()
{
	gSafeDelete(m_pHead);
}

void	WndCharInfo::SetBindChar(Character* bindChar)
{
	m_pBindChar = bindChar;
	if (bindChar)
	{
		m_fPosX = bindChar->GetRealX() + 50; 
		m_fPosY = bindChar->GetRealY();

		m_pHead->SetTexture(TexManager::sInstance().GetTex(bindChar->GetID())[eActionTex_Walk]);
		m_pHead->SetTextureRect(0,6*FLOAT_PIC_SQUARE_WIDTH,FLOAT_PIC_SQUARE_WIDTH,FLOAT_PIC_SQUARE_WIDTH);
	}
}

void WndCharInfo::Render()
{
	UIWindow::Render();

	m_pHead->Render(m_fPosX+20,m_fPosY+20);

	GfxFont* font = FontManager::sInstance().GetFont(FontAttr(eFontType_SongTi,eFontSize_FontBig));
	font->SetColor(0xFF000000);
	font->Print(m_fPosX+100,m_fPosY+25,"%s",m_pBindChar->GetName().c_str());
	font->Print(m_fPosX+100,m_fPosY+45,"%s",m_pBindChar->GetKind().c_str());

	//»æÖÆHP
	font->Print(m_fPosX+120,m_fPosY+80,"%d/%d",m_pBindChar->GetHP(),m_pBindChar->GetHPMax());
	App::sInstance().DrawBox(m_fPosX+100,m_fPosY+95,0xFFFF0000,1,100,10);
	float length = 100*((float)m_pBindChar->GetHP()/(float)m_pBindChar->GetHPMax());
	App::sInstance().DrawRect(m_fPosX+101,m_fPosY+96,m_fPosX+99+length,m_fPosY+94+10,0xFFFF0000);
	//»æÖÆMP
	font->Print(m_fPosX+120,m_fPosY+110,"%d/%d",m_pBindChar->GetMP(),m_pBindChar->GetMPMax());
	App::sInstance().DrawBox(m_fPosX+100,m_fPosY+125,0xFF0000FF,1,100,10);
	length = 100*((float)m_pBindChar->GetMP()/(float)m_pBindChar->GetMPMax());
	App::sInstance().DrawRect(m_fPosX+101,m_fPosY+126,m_fPosX+99+length,m_fPosY+124+10,0xFF0000FF);
}