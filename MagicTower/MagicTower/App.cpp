#include "App.h"
#include "resource.h"
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
#include "WndMainTitle.h"


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
	m_pHge->System_SetState(HGE_ICON, MAKEINTRESOURCE(IDI_ICON));
	m_pHge->System_SetState(HGE_TITLE, "云长传");
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

void App::SetLayer(eLayer layer,bool postScene)
{
	m_eCurLayer = layer;
	if (postScene)
	{
		m_bPreOrPostScene = false;
	}
}

bool App::LoadResource()
{
	//取得游戏版本
	g_getGameVersion();


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

	if(!TexManager::sInstance().LoadTex() /*|| !MapManager::sInstance().LoadMaps()*/)
	{
		MessageBox(NULL, "纹理载入失败", "Error", MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);
		return false;
	}

	m_bQuit = false;

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


	m_eCurLayer = eLayer_MainWnd;
	UISystem::sInstance().PopUpWindow(eWindowID_MainTitle);
	m_bCheckNextScene = false;
	m_bPreOrPostScene = true;
	m_nCurSceneNum = 1;
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
			UISystem::sInstance().Render();
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

	if(m_bQuit)
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
		g_MyLua.RunFunc(true,"PreScene1","");
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
			UISystem::sInstance().Update(dt);
		}
		break;
	case eLayer_Scene:
		{
			if (m_bCheckNextScene)
			{
				//先清楚上一个场景数据
				Scene::sInstance().Release();
				//重新初始化
				Scene::sInstance().Init();
				
				//检查当前关卡PreScene 是否有脚本
				if (m_bPreOrPostScene)
				{
					bool bBeginFight = false;
					if(g_MyLua.LoadSpecifyScriptOrNot(MapManager::sInstance().GetCurrentMap()->GetLevel(),eScript_PreScene))
					{
						//存在脚本并且已经加载
						//运行此脚本
						char pBuf[512];
						sprintf(pBuf,"PreScene%d_%d",MapManager::sInstance().GetCurrentMap()->GetLevel(),m_nCurSceneNum);
						if(g_MyLua.RunFunc(false,pBuf,""))
						{
							m_nCurSceneNum++;
							m_bCheckNextScene = false;
						}
						else
						{
							//没有剧情函数，则直接跳至战斗阶段 
							bBeginFight = true;
						}
					}
					else
					{
						//不存在PreScene脚本，直接跳至战斗阶段
						bBeginFight = true;
					}
					if (bBeginFight)
					{
						m_nCurSceneNum = 1;
						m_eCurLayer = eLayer_Fight;
						m_bCheckNextScene = false;
						m_bCheckPreFight = true;
					}
				}
				//检查当前关卡PostScene 是否有脚本
				else
				{
					bool bNextLevel = false;
					if(g_MyLua.LoadSpecifyScriptOrNot(MapManager::sInstance().GetCurrentMap()->GetLevel(),eScript_PostScene))
					{
						//存在脚本并且已经加载
						//运行此脚本
						char pBuf[512];
						sprintf(pBuf,"PostScene%d_%d",MapManager::sInstance().GetCurrentMap()->GetLevel(),m_nCurSceneNum);
						if(g_MyLua.RunFunc(false,pBuf,""))
						{
							m_nCurSceneNum++;
							m_bCheckNextScene = false;
						}
						else
						{
							//没有剧情函数，则直接跳至下一关
							bNextLevel = true;
						}
					}
					else
					{
						//不存在PreScene脚本，直接跳至下一关
						bNextLevel = true;
					}
					if (bNextLevel)
					{
						m_bPreOrPostScene = true;
						m_nCurSceneNum = 1;
						m_bCheckNextScene = true;

						//释放上一关相关数据
						int level = MapManager::sInstance().GetCurrentMap()->GetLevel();
						MapManager::sInstance().GetCurrentMap()->Release();
						CreatureManager::sInstance().Release();

						//设置下一关
						SetCurrentMap(level+1);
						CreatureManager::sInstance().Init();
					}
				}
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
				//载入战前脚本
				if(g_MyLua.LoadSpecifyScriptOrNot(MapManager::sInstance().GetCurrentMap()->GetLevel(),eScript_PreFight))
				{
					//存在脚本并且已经加载
					//运行此脚本
					char pBuf[512];
					sprintf(pBuf,"PreFight%d",MapManager::sInstance().GetCurrentMap()->GetLevel());
					g_MyLua.RunFunc(false,pBuf,"");
				}
				//战前脚本只能有一个
				//不管是否存在战前脚本，都要继续进行战斗
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