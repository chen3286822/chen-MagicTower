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
#include "SkillManager.h"
#include "Scene.h"
#include "LuaGlobalFunc.h"
#include "WndDialog.h"
#include "MyLua.h"


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

void App::SetLayer(eLayer layer)
{
	m_eCurLayer = layer;
}

bool App::LoadResource()
{
	//lua初始化
	g_MyLua.Init();

	//lua简单测试
// 	char pBuf[MAX_PATH];
// 	char pathConfig[MAX_PATH];
// 	GetCurrentDirectory(MAX_PATH,pBuf);
// 	sprintf(pathConfig,"%s\\res\\script\\1_0.lua",pBuf);
//  	luaL_dofile(g_pLua,pathConfig);
//  	lua_getglobal(g_pLua,"var");
// 	const char* errMsg = lua_tostring(g_pLua,-1);
//  	int var = lua_tonumber(g_pLua,-1);
//  	char str[50];
//  	sprintf(str,"var is %d",var);
//  	OutputDebugString(str);

	MapManager::sCreate();
	TexManager::sCreate();
	FontManager::sCreate();
	CreatureManager::sCreate();
	ActionProcess::sCreate();
	TipWnd::sCreate();
	UISystem::sCreate();
	ConfigManager::sCreate();
	SkillManager::sCreate();
	//载入UI
	UISystem::sInstance().Init();
	Scene::sCreate();
	Scene::sInstance().Init();



	CreatureManager::sInstance().Init();
	FontManager::sInstance().InitFont();

	ConfigManager::sInstance().LoadConfig();

	if(!TexManager::sInstance().LoadTex() || !MapManager::sInstance().LoadMaps())
	{
		MessageBox(NULL, "纹理或关卡载入失败", "Error", MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);
		return false;
	}

// 	player = new Character;
// 	player->Init(MapManager::sInstance().GetCurrentMap()->GetLevel(),34,100,1,Block(5,5));
// 	player->SetCamp(eCamp_Friend);
// 	player->SetMoveAbility(4,MapManager::sInstance().GetCurrentMap());
// 	player->GetName() = "路人甲";
// 	CreatureManager::sInstance().AddFriend(player);
// 
// 	Character* player2 = new Character;
// 	player2->Init(MapManager::sInstance().GetCurrentMap()->GetLevel(),25,101,1,Block(5,6));
// 	player2->SetCamp(eCamp_Enemy);
// 	player2->SetMoveAbility(3,MapManager::sInstance().GetCurrentMap());
// 	player2->GetName() = "路人乙";
// 	CreatureManager::sInstance().AddEnemy(player2);

	CreatureManager::sInstance().AddItem(98,30);
	CreatureManager::sInstance().AddItem(49,1);


	//lua test
// 	char pBuf[MAX_PATH];
// 	char pathConfig[MAX_PATH];
// 	GetCurrentDirectory(MAX_PATH,pBuf);
// 	sprintf(pathConfig,"%s\\res\\script\\1_1.lua",pBuf);
// 	luaL_dofile(g_MyLua.GetLuaState(),pathConfig);
// 	sprintf(pathConfig,"%s\\res\\script\\1_2.lua",pBuf);
// 	luaL_dofile(g_MyLua.GetLuaState(),pathConfig);
// 	//luaL_loadfile(g_pLua,pathConfig);
// 	lua_getglobal(g_MyLua.GetLuaState(), "PreScene1_1");
// 	lua_pcall(g_MyLua.GetLuaState(), 0, LUA_MULTRET, 0);

	//初始化，载入第一关脚本
	char pBuf[MAX_PATH];
	char pathConfig[MAX_PATH];
	GetCurrentDirectory(MAX_PATH,pBuf);
	sprintf(pathConfig,"%s\\res\\script\\top.lua",pBuf);
	g_MyLua.LoadFile(pathConfig);
	g_MyLua.RunFunc("init","");
	g_MyLua.LoadScript(MapManager::sInstance().GetCurrentMap()->GetLevel());
	//运行第一关剧情脚本
//	sprintf(pBuf,"PreScene%d",MapManager::sInstance().GetCurrentMap()->GetLevel());
//	g_MyLua.RunFunc(pBuf,"");

	m_eCurLayer = eLayer_Scene;
	m_bCheckNextScene = true;
	m_bCheckPreFight = false;
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

	Scene::sDestroy();
	SkillManager::sDestroy();
	ConfigManager::sDestroy();
	ActionProcess::sDestroy();
	UISystem::sDestroy();
	TipWnd::sDestroy();
	CreatureManager::sDestroy();
	FontManager::sDestroy();
	TexManager::sDestroy();
	MapManager::sDestroy();

	//lua 释放
	g_MyLua.Release();
}

void App::CleanUp()
{
	m_pHge->System_Shutdown();
	m_pHge->Release();
}

void App::DrawMouseRect()
{
	if(m_iBlock.xpos!=-1 && m_iBlock.ypos!=-1 && !(UISystem::sInstance().IsInAnyControl()))
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
	DrawRect(startX-width,startY-width,	startX + boxWidth+width,startY,color);
	DrawRect(startX-width,startY,startX,startY + boxLength,color);
	DrawRect(startX + boxWidth,startY,startX+ boxWidth+width,startY + boxLength,color);
	DrawRect(startX-width,startY + boxLength,startX + boxWidth+width,startY + boxLength+width,color);
}

bool App::AppRender()
{
	m_pHge->Gfx_Clear(0X00000000);
	m_pHge->Gfx_BeginScene();

	switch(m_eCurLayer)
	{
	case eLayer_MainWnd:
		{

		}
		break;
	case eLayer_Scene:
		{
			Scene::sInstance().Render();
			UISystem::sInstance().Render();
		}		
		break;
	case eLayer_Fight:
		{
			MapManager::sInstance().Render();
			DrawMouseRect();
			CreatureManager::sInstance().Render();
			UISystem::sInstance().Render();
			TipWnd::sInstance().Render();
			SkillManager::sInstance().Render();
			MapManager::sInstance().GetCurrentMap()->RenderTitle();
		}
		break;
	}
	m_pHge->Gfx_EndScene();
	return false;
}

bool App::AppUpdate()
{
	if (m_pHge->Input_GetKeyState(HGEK_ESCAPE))
		return true;

	if (g_getKeyState(m_pHge,HGEK_F1)==eKeyState_Down)
	{
		g_MyLua.PopErrorMsg();
	}

	if (g_getKeyState(m_pHge,HGEK_F2)==eKeyState_Down)
	{
		Scene::sInstance().Release();
		WndDialog* dialog = (WndDialog*)UISystem::sInstance().GetWindow(eWindowID_Dialog);
		if(dialog)
			dialog->Release();
		g_MyLua.RunFunc("PreScene1","");
	}


	float xpos,ypos;
	m_pHge->Input_GetMousePos(&xpos,&ypos);
	if(xpos >= MAP_OFF_X && xpos < MAP_OFF_X+MAP_WIDTH && ypos >= MAP_OFF_Y && ypos < MAP_OFF_Y+MAP_LENGTH)
	{
		float xMap,yMap;
		xMap = xpos - MAP_OFF_X;
		yMap = ypos - MAP_OFF_Y;
		m_iBlock.xpos = (int)(xMap/MAP_RECT);
		m_iBlock.ypos = (int)(yMap/MAP_RECT);
	}
	else
	{
		m_iBlock.xpos = -1;
		m_iBlock.ypos = -1;
	}

	float dt = m_pHge->Timer_GetDelta();

	switch(m_eCurLayer)
	{
	case eLayer_MainWnd:
		{

		}
		break;
	case eLayer_Scene:
		{
			if (m_bCheckNextScene)
			{
				//先清楚上一个场景数据
				Scene::sInstance().Release();
				//检查有没有下一个场景需要加载

				//没有的话，进入战斗
				m_eCurLayer = eLayer_Fight;
				m_bCheckNextScene = false;
				m_bCheckPreFight = true;
				return false;
			}
			Scene::sInstance().Update(dt);
			UISystem::sInstance().Update(dt);
		}		
		break;
	case eLayer_Fight:
		{
			if (m_bCheckPreFight)
			{
				//载入战斗前的脚本
				char pBuf[MAX_PATH];
				sprintf(pBuf,"PreFight%d",MapManager::sInstance().GetCurrentMap()->GetLevel());
				g_MyLua.RunFunc(pBuf,"");
				m_bCheckPreFight = false;
			}
			MapManager::sInstance().Update(dt);
			if(!MapManager::sInstance().GetCurrentMap()->IsShowTitle())
			{
				CreatureManager::sInstance().Strategy();
				ActionProcess::sInstance().Update();
				CreatureManager::sInstance().Update(dt);
				UISystem::sInstance().Update(dt);
				TipWnd::sInstance().Update(dt);				
				SkillManager::sInstance().Update(dt);
			}
		}
		break;
	}
	return false;
}

void App::StartNextScene()
{
	m_bCheckNextScene = true;
}

bool update()
{
	return App::sInstance().AppUpdate();
}

bool render()
{
	return App::sInstance().AppRender();
}