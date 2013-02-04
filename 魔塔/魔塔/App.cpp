#include "App.h"
#include "MapManager.h"
#include "TexManager.h"
#include "Character.h"
#include "CreatureManager.h"
#include "TipWnd.h"
#include "FontManager.h"

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
	hge = hgeCreate(HGE_VERSION);

	// Set up log file, frame function, render function and window title
	hge->System_SetState(HGE_LOGFILE, "MagicTowerlog");
	hge->System_SetState(HGE_FRAMEFUNC, update);
	hge->System_SetState(HGE_RENDERFUNC,render);
	hge->System_SetState(HGE_TITLE, "Magic Tower");
	hge->System_SetState(HGE_HIDEMOUSE,false);

	// Set up video mode
	hge->System_SetState(HGE_WINDOWED, true);
	hge->System_SetState(HGE_SCREENWIDTH, APP_WIDTH);
	hge->System_SetState(HGE_SCREENHEIGHT, APP_HEIGHT);
	hge->System_SetState(HGE_SCREENBPP, 32);
	hge->System_SetState(HGE_ZBUFFER,true);

	if(hge->System_Initiate())
		return true;
	else
	{
		MessageBox(NULL, hge->System_GetErrorMessage(), "Error", MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);
		return false;
	}
}

bool App::LoadResource()
{
	MapManager::sCreate();
	TexManager::sCreate();
	FontManager::sCreate();
	CreatureManager::sCreate();
	TipWnd::sCreate();

	FontManager::sInstance().InitFont();

	char pBuf[MAX_PATH];
	char pathTex[MAX_PATH],pathMap[MAX_PATH],pathMaps[MAX_PATH];
	GetCurrentDirectory(MAX_PATH,pBuf);
	sprintf(pathTex,"%s\\魔塔地图编辑器\\Debug\\res\\tex",pBuf);
	sprintf(pathMap,"%s\\魔塔地图编辑器\\Debug\\res\\map",pBuf);
	sprintf(pathMaps,"%s\\res\\Maps",pBuf);
	if(!TexManager::sInstance().LoadTex(pathTex) ||
		!TexManager::sInstance().LoadMap(pathMap) ||
		!MapManager::sInstance().LoadMaps(pathMaps))
	{
		MessageBox(NULL, "纹理或者关卡载入失败", "Error", MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);
		return false;
	}

	player = new Character;
	player->Init(TexManager::sInstance().GetTex(1),MapManager::sInstance().GetCurrentMap()->GetLevel(),1,1,1,Block(5,5));


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
	hge->System_Start();
	hge->Random_Seed();
}

void App::FreeResource()
{
	gSafeDelete(player);

	TipWnd::sDestroy();
	CreatureManager::sDestroy();
	FontManager::sDestroy();
	TexManager::sDestroy();
	MapManager::sDestroy();
}

void App::CleanUp()
{
	hge->System_Shutdown();
	hge->Release();
}

bool App::AppRender()
{
	hge->Gfx_Clear(0X00000000);
	hge->Gfx_BeginScene();

	MapManager::sInstance().Render();
	DrawMouseRect();
	CreatureManager::sInstance().Render();
	player->Render();
	TipWnd::sInstance().Render();

	hge->Gfx_EndScene();
	return false;
}

void App::DrawMouseRect()
{
	if(m_Block.xpos!=-1 && m_Block.ypos!=-1)
		DrawSmallRect(m_Block);
}

void App::DrawSmallRect(Block block)
{
	int xNum = block.xpos,yNum = block.ypos;

	hgeQuad quad;
	quad.v[0].x = MAP_OFF_X + xNum*MAP_RECT;
	quad.v[0].y = MAP_OFF_Y + yNum*MAP_RECT;
	quad.v[0].tx = 0;		quad.v[0].ty = 0;
	quad.v[1].x = MAP_OFF_X + xNum*MAP_RECT + MAP_RECT;
	quad.v[1].y = MAP_OFF_Y + yNum*MAP_RECT;
	quad.v[1].tx = 1;		quad.v[1].ty = 0;
	quad.v[2].x = MAP_OFF_X + xNum*MAP_RECT + MAP_RECT;
	quad.v[2].y = MAP_OFF_Y + yNum*MAP_RECT + MAP_RECT;
	quad.v[2].tx = 1;		quad.v[2].ty = 1;
	quad.v[3].x = MAP_OFF_X + xNum*MAP_RECT;
	quad.v[3].y = MAP_OFF_Y + yNum*MAP_RECT + MAP_RECT;
	quad.v[3].tx = 0;		quad.v[3].ty = 1;

	for (int i=0;i<4;i++)
	{
		quad.v[i].col = 0x4F48A4D5;
		quad.v[i].z = 0.5;
	}
	quad.blend = BLEND_DEFAULT_Z;
	quad.tex = 0;
	hge->Gfx_RenderQuad(&quad);
}

bool App::AppUpdate()
{
	if (hge->Input_GetKeyState(HGEK_ESCAPE))
		return true;

	float xpos,ypos;
	hge->Input_GetMousePos(&xpos,&ypos);
	if(xpos >= MAP_OFF_X && xpos < MAP_OFF_X+MAP_WIDTH && ypos >= MAP_OFF_Y && ypos < MAP_OFF_Y+MAP_LENGTH)
	{
		float xMap,yMap;
		xMap = xpos - MAP_OFF_X;
		yMap = ypos - MAP_OFF_Y;
		m_Block.xpos = (int)(xMap/MAP_RECT);
		m_Block.ypos = (int)(yMap/MAP_RECT);
	}
	else
	{
		m_Block.xpos = -1;
		m_Block.ypos = -1;
	}

	if (g_getKeyState(hge,HGEK_W)==KEY_DOWN)
		player->Move(UP);
	else if (g_getKeyState(hge,HGEK_S)==KEY_DOWN)
		player->Move(DOWN);
	else if (g_getKeyState(hge,HGEK_A)==KEY_DOWN)
		player->Move(LEFT);
	else if (g_getKeyState(hge,HGEK_D)==KEY_DOWN)
		player->Move(RIGHT);

	float dt = hge->Timer_GetDelta();
	player->Update(dt);

	CreatureManager::sInstance().Strategy();

	MapManager::sInstance().Update();
	CreatureManager::sInstance().Update(dt);
	TipWnd::sInstance().Update(dt);
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