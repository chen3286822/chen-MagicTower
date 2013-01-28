#include "App.h"
#include "MapManager.h"
#include "TexManager.h"
#include "Character.h"

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

	MapManager::sDestroy();
	TexManager::sDestroy();
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
	player->render();

	hge->Gfx_EndScene();
	return false;
}

bool App::appUpdate()
{
	if (hge->Input_GetKeyState(HGEK_ESCAPE))
		return true;
	float dt = hge->Timer_GetDelta();
	player->update();

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