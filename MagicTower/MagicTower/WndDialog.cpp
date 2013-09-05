#include "WndDialog.h"
#include "Character.h"

WndDialog::WndDialog() :
UIWindow(TexManager::sInstance().GetUITex(eUIID_Dialog),0,0,651,155,75,445)
{
	m_pContainer = new hgeGUI;
	m_pBindChar = NULL;
	m_pHead = new hgeSprite(0,0,0,FLOAT_PIC_SQUARE_WIDTH,FLOAT_PIC_SQUARE_WIDTH);
}

WndDialog::~WndDialog()
{
	gSafeDelete(m_pHead);
}

void	WndDialog::SetBindChar(Character* bindChar)
{
	m_pBindChar = bindChar;
	if (m_pBindChar)
	{
		m_strName = m_pBindChar->GetName();
	}
}

void WndDialog::SetHead(int ID)
{
	m_pHead->SetTexture(TexManager::sInstance().GetHeadTex(ID));
	m_pHead->SetTextureRect(0,0,64,80);
}

void WndDialog::SetName(std::string name)
{
	m_strName = name;
}

void WndDialog::Render()
{
	UIWindow::Render();

	m_pHead->Render(m_fPosX+20,m_fPosY+40);

	GfxFont* font = FontManager::sInstance().GetFont(FontAttr(eFontType_SongTi,eFontSize_FontSuper));
	font->SetColor(0xFF000000);
	font->Print(m_fPosX+100,m_fPosY+15,"%s",m_strName.c_str());

}