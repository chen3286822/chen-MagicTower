#include "App.h"
#include "MapManager.h"
#include "TexManager.h"
#include "Character.h"
#include "CreatureManager.h"
#include "TipWnd.h"
#include "FontManager.h"
#include "UI.h"
#include "ActionProcess.h"
#include "ConfigManager.h"

bool update();
bool render();

Character* player;

App::App(void)
{
}


App::~App(void)
{
}

bool App::SystemInit()
{
	m_pHge = hgeCreate(HGE_VERSION);

	// Set up log file, frame function, render function and window title
	m_pHge->System_SetState(HGE_FRAMEFUNC, update);
	m_pHge->System_SetState(HGE_RENDERFUNC,render);
	m_pHge->System_SetState(HGE_TITLE, "Magic Tower");
	m_pHge->System_SetState(HGE_HIDEMOUSE,false);

	// Set up video mode
	m_pHge->System_SetState(HGE_WINDOWED, true);
	m_pHge->System_SetState(HGE_SCREENWIDTH, APP_WIDTH);
	m_pHge->System_SetState(HGE_SCREENHEIGHT, APP_HEIGHT);
	m_pHge->System_SetState(HGE_SCREENBPP, 32);
	m_pHge->System_SetState(HGE_ZBUFFER,true);

	if(m_pHge->System_Initiate())
		return true;
	else
	{
		MessageBox(NULL, m_pHge->System_GetErrorMessage(), "Error", MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);
		return false;
	}
}

bool App::LoadResource()
{
	MapManager::sCreate();
	TexManager::sCreate();
	FontManager::sCreate();
	CreatureManager::sCreate();
	ActionProcess::sCreate();
	TipWnd::sCreate();
	UISystem::sCreate();
	ConfigManager::sCreate();

	CreatureManager::sInstance().Init();
	FontManager::sInstance().InitFont();

	ConfigManager::sInstance().LoadCreatureConfig();

	char pBuf[MAX_PATH];
	char pathTex[MAX_PATH],pathMap[MAX_PATH],pathMaps[MAX_PATH],pathUI[MAX_PATH];
	GetCurrentDirectory(MAX_PATH,pBuf);
	sprintf(pathTex,"%s\\res\\tex",pBuf);
	sprintf(pathMap,"%s\\res\\map",pBuf);
	sprintf(pathMaps,"%s\\res\\Maps",pBuf);
	sprintf(pathUI,"%s\\res\\UI",pBuf);
	if(!TexManager::sInstance().LoadTex(pathTex) ||
		!TexManager::sInstance().LoadMap(pathMap) ||
		!MapManager::sInstance().LoadMaps(pathMaps) ||
		!TexManager::sInstance().LoadUI(pathUI))
	{
		MessageBox(NULL, "纹理或者关卡载入失败", "Error", MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);
		return false;
	}


	//载入UI
	UISystem::sInstance().Init();

	player = new Character;
	player->Init(MapManager::sInstance().GetCurrentMap()->GetLevel(),34,100,1,Block(5,5));
	player->SetCamp(eCamp_Friend);
	player->SetMoveAbility(4,MapManager::sInstance().GetCurrentMap());
	CreatureManager::sInstance().AddFriend(player);

	Character* player2 = new Character;
	player2->Init(MapManager::sInstance().GetCurrentMap()->GetLevel(),25,101,1,Block(5,6));
	player2->SetCamp(eCamp_Enemy);
	player2->SetMoveAbility(3,MapManager::sInstance().GetCurrentMap());
	CreatureManager::sInstance().AddEnemy(player2);



//  	TipWnd::sInstance().AddText("卧室",0xFFFF0000,20.0f,10.0f,MSYaHei,FontBig,false);
//  	TipWnd::sInstance().AddText("aBCDEFTG",0xFFFF0000,30.0f,30.0f,Calibri,FontBig,false);
//  	TipWnd::sInstance().AddText("a我B是C中D国E人F啊T卧G艹",0xFFFF0000,45.0f,50.0f,SongTi,FontBig,false,100);
//   	TipWnd::sInstance().AddText("AAA",0xFF00FF00,-1.0f,-1.0f,Calibri,FontBig,false);
//   	TipWnd::sInstance().AddText("我是中国人",0xFF00FF00,-1.0f,-1.0f,SongTi,FontBig,false);
//   	TipWnd::sInstance().AddText("我是中国人",0xFF000000,-1.0f,-1.0f,MSYaHei,FontBig);
// 	TipWnd::sInstance().SetShow(true);

	return true;
}

void App::Run()
{
	m_pHge->System_Start();
	m_pHge->Random_Seed();
}

void App::FreeResource()
{
	//gSafeDelete(player);
	CreatureManager::sInstance().Release();
	UISystem::sInstance().Release();

	ConfigManager::sDestroy();
	ActionProcess::sDestroy();
	UISystem::sDestroy();
	TipWnd::sDestroy();
	CreatureManager::sDestroy();
	FontManager::sDestroy();
	TexManager::sDestroy();
	MapManager::sDestroy();
}

void App::CleanUp()
{
	m_pHge->System_Shutdown();
	m_pHge->Release();
}

bool App::AppRender()
{
	m_pHge->Gfx_Clear(0X00000000);
	m_pHge->Gfx_BeginScene();

	MapManager::sInstance().Render();
	DrawMouseRect();
	CreatureManager::sInstance().Render();
	TipWnd::sInstance().Render();
	UISystem::sInstance().Render();

	m_pHge->Gfx_EndScene();
	return false;
}

void App::DrawMouseRect()
{
	if(m_iBlock.xpos!=-1 && m_iBlock.ypos!=-1)
		DrawSmallRect(m_iBlock,0x4F48A4D5);
}

void App::DrawRect(float left,float top,float right,float bottom,DWORD color)
{
	hgeQuad quad;
	quad.v[0].x = left;
	quad.v[0].y = top;
	quad.v[0].tx = 0;		quad.v[0].ty = 0;
	quad.v[1].x = right;
	quad.v[1].y = top;
	quad.v[1].tx = 1;		quad.v[1].ty = 0;
	quad.v[2].x = right;
	quad.v[2].y = bottom;
	quad.v[2].tx = 1;		quad.v[2].ty = 1;
	quad.v[3].x = left;
	quad.v[3].y = bottom;
	quad.v[3].tx = 0;		quad.v[3].ty = 1;

	for (int i=0;i<4;i++)
	{
		quad.v[i].col = color;
		quad.v[i].z = 0.5;
	}
	quad.blend = BLEND_DEFAULT_Z;
	quad.tex = 0;
	m_pHge->Gfx_RenderQuad(&quad);
}

void App::DrawSmallRect(Block block,DWORD color)
{
	int xNum = block.xpos,yNum = block.ypos;

	DrawRect(MAP_OFF_X + xNum*MAP_RECT,MAP_OFF_Y + yNum*MAP_RECT,
		MAP_OFF_X + xNum*MAP_RECT + MAP_RECT,MAP_OFF_Y + yNum*MAP_RECT + MAP_RECT,color);
}

void App::DrawBox(int startX,int startY,DWORD color,int width,int boxWidth,int boxLength)
{
	DrawRect(startX,startY,	startX + boxWidth,startY + width,color);
	DrawRect(startX,startY + width,startX + width,startY + boxLength - width,color);
	DrawRect(startX + boxWidth - width,startY + width,startX+ boxWidth,startY + boxLength - width,color);
	DrawRect(startX,startY + boxLength - width,startX + boxWidth,startY + boxLength,color);
}

bool App::AppUpdate()
{
	if (m_pHge->Input_GetKeyState(HGEK_ESCAPE))
		return true;

	float xpos,ypos;
	m_pHge->Input_GetMousePos(&xpos,&ypos);
	if(xpos >= MAP_OFF_X && xpos < MAP_OFF_X+MAP_WIDTH && ypos >= MAP_OFF_Y && ypos < MAP_OFF_Y+MAP_LENGTH)
	{
		float xMap,yMap;
		xMap = xpos - MAP_OFF_X;
		yMap = ypos - MAP_OFF_Y;
		m_iBlock.xpos = (int)(xMap/MAP_RECT);
		m_iBlock.ypos = (int)(yMap/MAP_RECT);

// 		if(g_getLButtonState(m_pHge) == eLButtonState_Up)
// 		{
// 			player->Move(m_iBlock.xpos,m_iBlock.ypos);
// 		}
	}
	else
	{
		m_iBlock.xpos = -1;
		m_iBlock.ypos = -1;
	}

// 	if (g_getKeyState(m_pHge,HGEK_W)==eKeyState_Down)
// 		player->Move(eDirection_Up);
// 	else if (g_getKeyState(m_pHge,HGEK_S)==eKeyState_Down)
// 		player->Move(eDirection_Down);
// 	else if (g_getKeyState(m_pHge,HGEK_A)==eKeyState_Down)
// 		player->Move(eDirection_Left);
// 	else if (g_getKeyState(m_pHge,HGEK_D)==eKeyState_Down)
// 		player->Move(eDirection_Right);
// 	else if (g_getKeyState(m_pHge,HGEK_J)==eKeyState_Down)
// 		player->GeginHit();

	float dt = m_pHge->Timer_GetDelta();

	CreatureManager::sInstance().Strategy();

	MapManager::sInstance().Update();
	ActionProcess::sInstance().Update();
	CreatureManager::sInstance().Update(dt);
	TipWnd::sInstance().Update(dt);
	UISystem::sInstance().Update(dt);
	return false;
}

bool update()
{
	return App::sInstance().AppUpdate();
}

bool render()
{
	return App::sInstance().AppRender();
}