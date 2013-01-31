#include "App.h"
#include "MapManager.h"
#include "TexManager.h"
#include "Character.h"
#include "CreatureManager.h"

bool update();
bool render();

Character* player;

App::App(void)
{
}


App::~App(void)
{
}

bool App::systemInit()
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

void App::loadResource()
{
	MapManager::sCreate();
	TexManager::sCreate();
	CreatureManager::sCreate();
	char pBuf[MAX_PATH];
	char pathTex[MAX_PATH],pathMap[MAX_PATH],pathMaps[MAX_PATH];
	GetCurrentDirectory(MAX_PATH,pBuf);
	sprintf(pathTex,"%s\\Ä§ËþµØÍ¼±à¼­Æ÷\\Debug\\res\\tex",pBuf);
	sprintf(pathMap,"%s\\Ä§ËþµØÍ¼±à¼­Æ÷\\Debug\\res\\map",pBuf);
	sprintf(pathMaps,"%s\\res\\Maps",pBuf);
	if(!TexManager::sInstance().LoadTex(pathTex) ||
		!TexManager::sInstance().LoadMap(pathMap) ||
		!MapManager::sInstance().LoadMaps(pathMaps))
	{
		MessageBox(NULL, "ÎÆÀí»òÕß¹Ø¿¨ÔØÈëÊ§°Ü", "Error", MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);
		return;
	}

	player = new Character;
	player->init(TexManager::sInstance().getTex(1),1,1,Block(5,5));
}

void App::run()
{
	hge->System_Start();
}

void App::freeResource()
{
	gSafeDelete(player);

	CreatureManager::sDestroy();
	TexManager::sDestroy();
	MapManager::sDestroy();
}

void App::cleanUp()
{
	hge->System_Shutdown();
	hge->Release();
}

bool App::appRender()
{
	hge->Gfx_Clear(0X00000000);
	hge->Gfx_BeginScene();

	MapManager::sInstance().render();
	float xpos,ypos;
	hge->Input_GetMousePos(&xpos,&ypos);
	if(xpos >= MAP_OFF_X && xpos < MAP_OFF_X+MAP_WIDTH && ypos >= MAP_OFF_Y && ypos < MAP_OFF_Y+MAP_LENGTH)
	{
		drawSmallRect(xpos,ypos);
	}
	player->render();

	hge->Gfx_EndScene();
	return false;
}

void App::drawSmallRect(float x,float y)
{
	float xMap,yMap;
	xMap = x - MAP_OFF_X;
	yMap = y - MAP_OFF_Y;
	int xNum = 0,yNum = 0;
	xNum = (int)(xMap/MAP_RECT);
	yNum = (int)(yMap/MAP_RECT);

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

bool App::appUpdate()
{
	if (hge->Input_GetKeyState(HGEK_ESCAPE))
		return true;
	if (g_getKeyState(hge,HGEK_W)==KEY_DOWN)
		player->move(UP);
	else if (g_getKeyState(hge,HGEK_S)==KEY_DOWN)
		player->move(DOWN);
	else if (g_getKeyState(hge,HGEK_A)==KEY_DOWN)
		player->move(LEFT);
	else if (g_getKeyState(hge,HGEK_D)==KEY_DOWN)
		player->move(RIGHT);

	float dt = hge->Timer_GetDelta();
	player->update(dt);

	MapManager::sInstance().update();
	return false;
}

bool update()
{
	return App::sInstance().appUpdate();
}

bool render()
{
	return App::sInstance().appRender();
}