#include "WndCharInfo.h"
#include "App.h"
#include "Character.h"
#include "TexManager.h"
#include "FontManager.h"

WndCharInfo::WndCharInfo(HTEXTURE tex,float x,float y,float w,float h,float posX,float posY) :
UIWindow(tex,x,y,w,h,posX,posY)
{
	m_pContainer = new hgeGUI;
	m_pBindChar = NULL;
	m_pHead = new hgeSprite(tex,0,0,FLOAT_PIC_SQUARE_WIDTH,FLOAT_PIC_SQUARE_WIDTH);
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

	GfxFont* font = FontManager::sInstance().GetFont(FontAttr(eFontType_SongTi,eFontSize_FontMiddle));
	font->SetColor(0xFFFF0000);
	font->Print(m_fPosX+80,m_fPosY+20,"%s",m_pBindChar->GetName().c_str());
	font->Print(m_fPosX+80,m_fPosY+50,"%s",m_pBindChar->GetKind().c_str());

	//»æÖÆHP
	font->Print(m_fPosX+120,m_fPosY+90,"%d/%d",m_pBindChar->GetHP(),m_pBindChar->GetHPMax());
	App::sInstance().DrawBox(m_fPosX+100,m_fPosY+100,0xFF00FF00,1,100,10);
	float length = 100*(m_pBindChar->GetHP()/m_pBindChar->GetHPMax());
	App::sInstance().DrawRect(m_fPosX+101,m_fPosY+101,m_fPosX+99+length,m_fPosY+99+10,0xFFFF0000);
	//»æÖÆMP
	font->Print(m_fPosX+120,m_fPosY+120,"%d/%d",m_pBindChar->GetMP(),m_pBindChar->GetMPMax());
	App::sInstance().DrawBox(m_fPosX+100,m_fPosY+130,0xFF00FF00,1,100,10);
	length = 100*(m_pBindChar->GetMP()/m_pBindChar->GetMPMax());
	App::sInstance().DrawRect(m_fPosX+101,m_fPosY+131,m_fPosX+99+length,m_fPosY+129+10,0xFF0000FF);
}