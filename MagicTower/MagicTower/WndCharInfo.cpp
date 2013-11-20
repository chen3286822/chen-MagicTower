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
	m_bNeedResetPos = true;
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
		if(m_pBindChar->GetShowX() + 307 > APP_WIDTH)
			m_fPosX = m_pBindChar->GetShowX() - 257;
		else
			m_fPosX = m_pBindChar->GetShowX() + 50; 
		if(m_pBindChar->GetShowY() + 183 > APP_HEIGHT)
			m_fPosY = m_pBindChar->GetShowY() - 183;
		else
			m_fPosY = m_pBindChar->GetShowY();

		m_pHead->SetTexture(TexManager::sInstance().GetTex(bindChar->GetID())[eActionTex_Walk]);
		m_pHead->SetTextureRect(0,6*FLOAT_PIC_SQUARE_WIDTH,FLOAT_PIC_SQUARE_WIDTH,FLOAT_PIC_SQUARE_WIDTH);
	}
}

void	WndCharInfo::SetRenderPositon(float _x,float _y)
{
	if (_x==-1 && _y == -1)
	{
		if(m_pBindChar->GetShowX() + 307 > APP_WIDTH)
			m_fPosX = m_pBindChar->GetShowX() - 257;
		else
			m_fPosX = m_pBindChar->GetShowX() + 50; 
		if(m_pBindChar->GetShowY() + 183 > APP_HEIGHT)
			m_fPosY = m_pBindChar->GetShowY() - 183;
		else
			m_fPosY = m_pBindChar->GetShowY();
	}
	else
	{
		m_fPosX = _x;
		m_fPosY = _y;
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
	App::sInstance().DrawBox(m_fPosX+100,m_fPosY+95,0xFFFF0000,1,100,8);
	float length = 100*((float)m_pBindChar->GetHP()/(float)m_pBindChar->GetHPMax());
	App::sInstance().DrawRect(m_fPosX+100,m_fPosY+95,m_fPosX+100+length,m_fPosY+95+8,0xFFFF0000);
	//»æÖÆMP
	font->Print(m_fPosX+120,m_fPosY+110,"%d/%d",m_pBindChar->GetMP(),m_pBindChar->GetMPMax());
	App::sInstance().DrawBox(m_fPosX+100,m_fPosY+125,0xFF0000FF,1,100,8);
	length = 100*((float)m_pBindChar->GetMP()/(float)m_pBindChar->GetMPMax());
	App::sInstance().DrawRect(m_fPosX+100,m_fPosY+125,m_fPosX+100+length,m_fPosY+125+8,0xFF0000FF);
}