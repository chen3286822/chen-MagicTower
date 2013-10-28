#include "WndMainTitle.h"
#include "FontManager.h"
#include "App.h"
#include "MyLua.h"

WndMainTitle::WndMainTitle():
UIWindow(TexManager::sInstance().GetSceneTex(4),0,0,960,614,(APP_WIDTH-960)/2,(APP_HEIGHT-614)/2)
{
	m_pContainer = new hgeGUI;

	m_pNewGame = new UIButton(eControlID_NewGameButton,m_fPosX+250,m_fPosY+240,409,54,TexManager::sInstance().GetUITex(eUIID_LongButtonUp),TexManager::sInstance().GetUITex(eUIID_LongButtonDown),0,0);
	m_pLoadGame = new UIButton(eControlID_LoadGameButton,m_fPosX+276,m_fPosY+300,409,54,TexManager::sInstance().GetUITex(eUIID_LongButtonUp),TexManager::sInstance().GetUITex(eUIID_LongButtonDown),0,0);
	m_pSet = new UIButton(eControlID_SetButton,m_fPosX+302,m_fPosY+360,409,54,TexManager::sInstance().GetUITex(eUIID_LongButtonUp),TexManager::sInstance().GetUITex(eUIID_LongButtonDown),0,0);
	m_pQuit = new UIButton(eControlID_QuitButton,m_fPosX+328,m_fPosY+420,409,54,TexManager::sInstance().GetUITex(eUIID_LongButtonUp),TexManager::sInstance().GetUITex(eUIID_LongButtonDown),0,0);

	m_pNewGame->OffsetX = 250;
	m_pNewGame->OffsetY = 240;
	m_pLoadGame->OffsetX = 276;
	m_pLoadGame->OffsetY = 300;
	m_pSet->OffsetX = 302;
	m_pSet->OffsetY = 360;
	m_pQuit->OffsetX = 328;
	m_pQuit->OffsetY = 420;
	m_pContainer->AddCtrl(m_pNewGame);
	m_pContainer->AddCtrl(m_pLoadGame);
	m_pContainer->AddCtrl(m_pSet);
	m_pContainer->AddCtrl(m_pQuit);

	m_pNewGame->SetFont(eFontType_KaiTi,eFontSize_FontSuperEx);
	m_pNewGame->SetText("新  的  游  戏",0xFF000000);

	m_pLoadGame->SetFont(eFontType_KaiTi,eFontSize_FontSuperEx);
	m_pLoadGame->SetText("载  入  游  戏",0xFF000000);

	m_pSet->SetFont(eFontType_KaiTi,eFontSize_FontSuperEx);
	m_pSet->SetText("设          置",0xFF000000);

	m_pQuit->SetFont(eFontType_KaiTi,eFontSize_FontSuperEx);
	m_pQuit->SetText("退  出  游  戏",0xFF000000);

	m_pLogo = new hgeSprite(TexManager::sInstance().GetUITex(eUIID_Logo),0,0,407,135);
}

WndMainTitle::~WndMainTitle()
{
	m_pContainer->DelCtrl(eControlID_NewGameButton);
	m_pContainer->DelCtrl(eControlID_LoadGameButton);
	m_pContainer->DelCtrl(eControlID_SetButton);
	m_pContainer->DelCtrl(eControlID_QuitButton);
	gSafeDelete(m_pLogo);
}

void WndMainTitle::Update(float dt)
{
	if (m_bShow)
	{
		int id = m_pContainer->Update(dt);
		if (id == -1)
		{
			m_pContainer->Enter();
		}
		else if (id == eControlID_NewGameButton)
		{
			UISystem::sInstance().CloseWindow(eWindowID_MainTitle);

			//执行场景脚本
			char pBuf[MAX_PATH];
			char pathConfig[MAX_PATH];
			GetCurrentDirectory(MAX_PATH,pBuf);
			sprintf(pathConfig,"%s\\res\\script\\top.lua",pBuf);
			g_MyLua.LoadFile(pathConfig);
			//必须执行inti 函数，否则终止游戏
			g_MyLua.RunFunc(true,"init","");

			//开始执行剧情脚本
			App::sInstance().StartNextScene();
			App::sInstance().SetLayer(eLayer_Scene);

			m_pContainer->Leave();
		}
		else if (id == eControlID_LoadGameButton)
		{
			m_pContainer->Leave();
		}
		else if (id == eControlID_SetButton)
		{
			m_pContainer->Leave();
		}
		else if (id == eControlID_QuitButton)
		{
			App::sInstance().QuitGame();
			m_pContainer->Leave();
		}
	}
}

void WndMainTitle::Render()
{
	UIWindow::Render();

	m_pLogo->Render(m_fPosX+(APP_WIDTH-407)/2,m_fPosY+40);

	GfxFont* font = FontManager::sInstance().GetFont(FontAttr(eFontType_SongTi,eFontSize_FontMiddle));
	font->SetColor(0xFFFFFFFF);
	font->Print(10,900,"%s",g_strVersion);
	font->Print(10,920,"Author. Chen");
}

void	WndMainTitle::SetRenderPositon(float _x,float _y)
{
	m_fPosX = _x; 
	m_fPosY = _y;

	m_pNewGame->ResetPosition(m_fPosX+m_pNewGame->OffsetX,m_fPosY+m_pNewGame->OffsetY);
	m_pLoadGame->ResetPosition(m_fPosX+m_pLoadGame->OffsetX,m_fPosY+m_pLoadGame->OffsetY);
	m_pSet->ResetPosition(m_fPosX+m_pSet->OffsetX,m_fPosY+m_pSet->OffsetY);
	m_pQuit->ResetPosition(m_fPosX + m_pQuit->OffsetX,m_fPosY + m_pQuit->OffsetY);
}

bool	WndMainTitle::IsOnControl()
{
	return (m_bShow && UIWindow::IsOnControl());
}