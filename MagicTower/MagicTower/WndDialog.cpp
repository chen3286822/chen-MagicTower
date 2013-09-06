#include "WndDialog.h"
#include "Character.h"
#include "App.h"

WndDialog::WndDialog() :
UIWindow(TexManager::sInstance().GetUITex(eUIID_Dialog),0,0,651,155,75,445)
{
	m_pContainer = new hgeGUI;
	m_pBindChar = NULL;
	m_nID = -1;
	m_pHead = new hgeSprite(0,0,0,FLOAT_PIC_SQUARE_WIDTH,FLOAT_PIC_SQUARE_WIDTH);
	m_nNextWordIndex = 0;
	m_dwTime = 50;
	m_dwPastTime = 0;
	m_dwWordShowTime = 3000;
	m_dwWordShowPastTime = 0;
	m_lWords.clear();
	m_strCurWord.clear();
}

WndDialog::~WndDialog()
{
	gSafeDelete(m_pHead);
}

void WndDialog::Release()
{
	m_pBindChar = NULL;
	m_nID = -1;
	m_pHead->SetTexture(0);
	m_nNextWordIndex = 0;
	m_dwTime = 50;
	m_dwPastTime = 0;
	m_dwWordShowTime = 3000;
	m_dwWordShowPastTime = 0;
	m_lWords.clear();
	m_strCurWord.clear();
	m_strName.clear();
	SetShow(false);
}

void	WndDialog::SetBindChar(Character* bindChar)
{
	m_pBindChar = bindChar;
	if (m_pBindChar)
	{
		m_strName = m_pBindChar->GetName();
		m_nID = m_pBindChar->GetNum();
	}
}

bool WndDialog::SetHead(int ID)
{
	HTEXTURE tex = TexManager::sInstance().GetHeadTex(ID);
	if (tex)
	{
		m_nID = ID;
		m_pHead->SetTexture(tex);
		m_pHead->SetTextureRect(0,0,64,80);
		return true;
	}
	return false;
}

void WndDialog::SetName(std::string name)
{
	m_strName = name;
}

void WndDialog::Update(float dt)
{
	if (m_strCurWord.empty())
	{
		//开始显示对话
		if (!m_lWords.empty())
		{
			std::string wholeWord = m_lWords.front();
			m_strCurWord.assign(wholeWord,0,1);
			m_nNextWordIndex = 1;
			m_dwPastTime = 0;
		}
	}
	if (!m_lWords.empty())
	{
		if (m_nNextWordIndex < m_lWords.front().size())
		{
			//如果点击鼠标，则直接显示完所有的内容
			if (g_getLButtonState(App::sInstance().GetHGE()) == eLButtonState_Up && IsOnControl())
			{
				m_dwPastTime = 0;
				m_strCurWord = m_lWords.front();
				m_nNextWordIndex = m_lWords.front().size();
			}

			DWORD pastTime = (int)(dt*1000);
			if (m_dwPastTime + pastTime < m_dwTime)
			{
				m_dwPastTime += pastTime;
			}
			else
			{
				m_dwPastTime = 0;
				m_strCurWord.assign(m_lWords.front(),0,m_nNextWordIndex+1);
				m_nNextWordIndex++;
			}
		}
		else
		{
			//一句话显示完了，看是否加载下一句话
			//有两种方式，一种是计时制，一种是鼠标点击
			bool bGoNo = false;

			//判断是否按下鼠标
			if (g_getLButtonState(App::sInstance().GetHGE()) == eLButtonState_Up && IsOnControl())
			{
				bGoNo = true;
			}
			//没有按下鼠标，继续看是否时间到了
			if (!bGoNo)
			{
				DWORD pastTime = (int)(dt*1000);
				if (pastTime + m_dwWordShowPastTime < m_dwWordShowTime)
				{
					m_dwWordShowPastTime += pastTime;
				}
				else
				{
					m_dwWordShowPastTime = 0;
					bGoNo = true;
				}
			}
			if (bGoNo)
			{
				m_lWords.pop_front();
				if (m_lWords.empty())
				{
					//没有话显示了，隐藏窗口	
					SetShow(false);
				}
				m_strCurWord.clear();
				m_nNextWordIndex = 0;
			}
		}
	}
}

void WndDialog::Render()
{
	UIWindow::Render();

	m_pHead->Render(m_fPosX+20,m_fPosY+40);

	GfxFont* font = FontManager::sInstance().GetFont(FontAttr(eFontType_SongTi,eFontSize_FontSuper));
	font->SetColor(0xFF000000);
	font->Print(m_fPosX+100,m_fPosY+15,"%s",m_strName.c_str());

	font = FontManager::sInstance().GetFont(FontAttr(eFontType_SongTi,eFontSize_FontTitle));
	font->SetColor(0xFF000000);
	std::string strLine;
	int lines = m_strCurWord.size()/LINECHAR + 1;
	for (int i=0;i<lines;i++)
	{
		strLine.assign(m_strCurWord,i*LINECHAR,LINECHAR);
		font->Print(m_fPosX+100,m_fPosY+35 + i*(eFontSize_FontTitle+2) ,"%s",strLine.c_str());
	}
}

void WndDialog::PushWords(int ID,const char* name,const char* word)
{
	if (m_lWords.empty())
	{
		if(SetHead(ID))
		{
			m_lWords.push_back(word);
			m_strName = name;
		}
	}
	else
	{
		if(ID == m_nID)
		{
			m_lWords.push_back(word);
			m_strName = name;
		}
	}
}

bool WndDialog::IsFinishWords()
{
	if (!m_strCurWord.empty() && !m_lWords.empty())
	{
		return false;
	}
	return true;
}