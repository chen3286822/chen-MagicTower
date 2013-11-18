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
	m_pHge->System_SetState(HGE_TITLE, "�Ƴ���");
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
	//ȡ����Ϸ�汾
	g_getGameVersion();


	//lua��ʼ��
	g_MyLua.Init();

	//lua�򵥲���
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
	//����UI
	UISystem::sInstance().Init();
	Scene::sCreate();
	Scene::sInstance().Init();



	FontManager::sInstance().InitFont();

	ConfigManager::sInstance().LoadConfig();

	if(!TexManager::sInstance().LoadTex() /*|| !MapManager::sInstance().LoadMaps()*/)
	{
		MessageBox(NULL, "��������ʧ��", "Error", MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);
		return false;
	}

	m_bQuit = false;

// 	player = new Character;
// 	player->Init(MapManager::sInstance().GetCurrentMap()->GetLevel(),34,100,1,Block(5,5));
// 	player->SetCamp(eCamp_Friend);
// 	player->SetMoveAbility(4,MapManager::sInstance().GetCurrentMap());
// 	player->GetName() = "·�˼�";
// 	CreatureManager::sInstance().AddFriend(player);
// 
// 	Character* player2 = new Character;
// 	player2->Init(MapManager::sInstance().GetCurrentMap()->GetLevel(),25,101,1,Block(5,6));
// 	player2->SetCamp(eCamp_Enemy);
// 	player2->SetMoveAbility(3,MapManager::sInstance().GetCurrentMap());
// 	player2->GetName() = "·����";
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


	StartMainWnd();
	return true;
}

void App::StartMainWnd()
{
	m_eCurLayer = eLayer_MainWnd;
	UISystem::sInstance().PopUpWindow(eWindowID_MainTitle);
	m_bCheckNextScene = false;
	m_bPreOrPostScene = true;
	m_nCurSceneNum = 1;
	m_bCheckPreFight = false;
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
	MapManager::sInstance().Release();

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

	//lua �ͷ�
	g_MyLua.Release();
	
	//�ͷ�С��ͼ
	m_iSmallMap.Destroy();
}

void App::CleanUp()
{
	m_pHge->System_Shutdown();
	m_pHge->Release();
}

void App::DrawMouseRect()
{
	if(m_iBlock.xpos!=-1 && m_iBlock.ypos!=-1 && !(UISystem::sInstance().IsInAnyControl()))
	{
		Block drawBlock = m_iBlock;
		Map* theMap = MapManager::sInstance().GetCurrentMap();
		int offx = 0,offy = 0;
		if (theMap)
		{
			offx = theMap->GetOffX();
			offy = theMap->GetOffY();
		}
		drawBlock.xpos -= offx;
		drawBlock.ypos -= offy;
		DrawSmallRect(drawBlock,0x4F48A4D5);
	}
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
	if(xpos >= MAP_OFF_X && xpos < MAP_OFF_X+g_nMapWidth && ypos >= MAP_OFF_Y && ypos < MAP_OFF_Y+g_nMapHeight)
	{
		float xMap,yMap;
		xMap = xpos - MAP_OFF_X;
		yMap = ypos - MAP_OFF_Y;
		Map* theMap = MapManager::sInstance().GetCurrentMap();
		int offx = 0,offy = 0;
		if (theMap)
		{
			offx = theMap->GetOffX();
			offy = theMap->GetOffY();
		}
		m_iBlock.xpos = (int)(xMap/MAP_RECT) + offx;
		m_iBlock.ypos = (int)(yMap/MAP_RECT) + offy;
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
				//�������һ����������
				Scene::sInstance().Release();
				//���³�ʼ��
				Scene::sInstance().Init();
				
				//��鵱ǰ�ؿ�PreScene �Ƿ��нű�
				if (m_bPreOrPostScene)
				{
					bool bBeginFight = false;
					if(g_MyLua.LoadSpecifyScriptOrNot(MapManager::sInstance().GetCurrentMap()->GetLevel(),eScript_PreScene))
					{
						//���ڽű������Ѿ�����
						//���д˽ű�
						char pBuf[512];
						sprintf(pBuf,"PreScene%d_%d",MapManager::sInstance().GetCurrentMap()->GetLevel(),m_nCurSceneNum);
						if(g_MyLua.RunFunc(false,pBuf,""))
						{
							m_nCurSceneNum++;
							m_bCheckNextScene = false;
						}
						else
						{
							//û�о��麯������ֱ������ս���׶� 
							bBeginFight = true;
						}
					}
					else
					{
						//������PreScene�ű���ֱ������ս���׶�
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
				//��鵱ǰ�ؿ�PostScene �Ƿ��нű�
				else
				{
					bool bNextLevel = false;
					if(g_MyLua.LoadSpecifyScriptOrNot(MapManager::sInstance().GetCurrentMap()->GetLevel(),eScript_PostScene))
					{
						//���ڽű������Ѿ�����
						//���д˽ű�
						char pBuf[512];
						sprintf(pBuf,"PostScene%d_%d",MapManager::sInstance().GetCurrentMap()->GetLevel(),m_nCurSceneNum);
						if(g_MyLua.RunFunc(false,pBuf,""))
						{
							m_nCurSceneNum++;
							m_bCheckNextScene = false;
						}
						else
						{
							//û�о��麯������ֱ��������һ��
							bNextLevel = true;
						}
					}
					else
					{
						//������PreScene�ű���ֱ��������һ��
						bNextLevel = true;
					}
					if (bNextLevel)
					{
						m_bPreOrPostScene = true;
						m_nCurSceneNum = 1;
						m_bCheckNextScene = true;

						//�ͷ���һ���������
						int level = MapManager::sInstance().GetCurrentMap()->GetLevel();
						MapManager::sInstance().GetCurrentMap()->Release();
						CreatureManager::sInstance().Release();

						//������һ��
						if(!SetCurrentMap(level+1))
						{
							//������һ��ʧ�ܣ�����������
							m_eCurLayer = eLayer_MainWnd;
							UISystem::sInstance().PopUpWindow(eWindowID_MainTitle);
							m_bCheckNextScene = false;
							m_bCheckPreFight = false;
							return false;
						}
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
				//����սǰ�ű�
				if(g_MyLua.LoadSpecifyScriptOrNot(MapManager::sInstance().GetCurrentMap()->GetLevel(),eScript_PreFight))
				{
					//���ڽű������Ѿ�����
					//���д˽ű�
					char pBuf[512];
					sprintf(pBuf,"PreFight%d",MapManager::sInstance().GetCurrentMap()->GetLevel());
					g_MyLua.RunFunc(false,pBuf,"");
				}
				//սǰ�ű�ֻ����һ��
				//�����Ƿ����սǰ�ű�����Ҫ��������ս��
				m_bCheckPreFight = false;

				//����С��ͼ�Ӵ���
				CreateSmallMapWnd();
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

LRESULT CALLBACK SmallMapProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
//	int wmId, wmEvent;  
	PAINTSTRUCT ps;  
	HDC hdc; 
	switch(msg)
	{	
	case WM_CREATE:
		{
			char pBuf[MAX_PATH];
			char pathConfig[MAX_PATH];
			GetCurrentDirectory(MAX_PATH,pBuf);
			sprintf(pathConfig,"%s\\res\\tex\\SmallMap\\%d.png",pBuf,MapManager::sInstance().GetCurrentMap()->GetLevel());
			if(!App::sInstance().GetSmallMap().IsNull())
				App::sInstance().GetSmallMap().Destroy();
			if(App::sInstance().GetSmallMap().Load(pathConfig) != S_OK)
				g_debugString(__FILE__,__FUNCDNAME__,__LINE__,"ȱ��С��ͼ");
			
			//����С��ͼ���ڴ�С
			int mapWidth = App::sInstance().GetSmallMap().GetWidth();
			int mapHeight = App::sInstance().GetSmallMap().GetHeight();
			RECT wndRect;
			GetWindowRect(hwnd,&wndRect);
			RECT clientRect;
			GetClientRect(hwnd,&clientRect);
			int clientWidth = mapWidth + (wndRect.right-wndRect.left-clientRect.right);
			int clientHeight = mapHeight + (wndRect.bottom-wndRect.top-clientRect.bottom);
			MoveWindow(hwnd,wndRect.left,wndRect.top,clientWidth,clientHeight,true);
			
// 			//��ʼ��С��ͼλ�ÿ�
 			RECT& smallRect = App::sInstance().GetSmallMapRect();
 			int offx = MapManager::sInstance().GetCurrentMap()->GetOffX();
 			int offy = MapManager::sInstance().GetCurrentMap()->GetOffY();
 			smallRect.left = offx*mapWidth/g_nMapWidthNum;
 			smallRect.top = offy*mapHeight/g_nMapHeightNum;
 			smallRect.right = smallRect.left + (APP_WIDTH/MAP_RECT)*mapWidth/g_nMapWidthNum;
 			smallRect.bottom = smallRect.top + (APP_HEIGHT/MAP_RECT)*mapHeight/g_nMapHeightNum;
		}
		return FALSE;
	case WM_PAINT:
		{
			hdc = BeginPaint(hwnd, &ps); 

			//����С��ͼ
			App::sInstance().GetSmallMap().Draw(hdc,0,0);
		
			//��С��ͼ��
			HPEN hPen;
			HPEN hPenOld;
			hPen = CreatePen( PS_SOLID, 1, RGB( 255, 255, 255 ));
			hPenOld = ( HPEN )SelectObject ( hdc, hPen );

			RECT smallRect = App::sInstance().GetSmallMapRect();
			MoveToEx(hdc, smallRect.left,smallRect.top, NULL );
			LineTo(hdc, smallRect.right,smallRect.top);
			MoveToEx(hdc, smallRect.left,smallRect.top, NULL );
			LineTo(hdc, smallRect.left,smallRect.bottom);
			MoveToEx(hdc, smallRect.right-1,smallRect.top, NULL );
			LineTo(hdc, smallRect.right-1,smallRect.bottom);
			MoveToEx(hdc, smallRect.left,smallRect.bottom-1, NULL );
			LineTo(hdc, smallRect.right,smallRect.bottom-1);

			SelectObject (hdc , hPenOld);
			DeleteObject ( hPen );

			//���Ƶ�λ�����
			//�����ú�ɫ�ĵ��ʾ
			HPEN hPenEnemy = CreatePen( PS_SOLID, 4, RGB( 255, 0, 0 ));
			//�ѷ�����ɫ�ĵ��ʾ
			HPEN hPenFriend = CreatePen( PS_SOLID, 4, RGB( 0, 255, 0 ));

			int smallMapWidth = App::sInstance().GetSmallMap().GetWidth();
			int smallMapHeight = App::sInstance().GetSmallMap().GetHeight();
			
			SelectObject ( hdc, hPenEnemy );
			VCharacter mylist = CreatureManager::sInstance().GetEnemy();
			VCharacter::iterator it=CreatureManager::sInstance().GetEnemy().begin();
			VCharacter::iterator it2=CreatureManager::sInstance().GetEnemy().end();
			for (;it!=CreatureManager::sInstance().GetEnemy().end();it++)
			{
				Block chaBlock = (*it)->GetBlock();
				int startX = chaBlock.xpos*smallMapWidth*MAP_RECT/APP_WIDTH;
				int startY = chaBlock.ypos*smallMapHeight*MAP_RECT/APP_HEIGHT;
				MoveToEx(hdc, startX+1,startY+1, NULL );
				LineTo(hdc, startX+1,startY+1);
			}
			SelectObject ( hdc, hPenFriend );
			for (VCharacter::iterator it=CreatureManager::sInstance().GetFriend().begin();it!=CreatureManager::sInstance().GetFriend().end();it++)
			{
				Block chaBlock = (*it)->GetBlock();
				int startX = chaBlock.xpos*smallMapWidth/g_nMapWidthNum;
				int startY = chaBlock.ypos*smallMapHeight/g_nMapHeightNum;
				MoveToEx(hdc, startX+1,startY+1, NULL );
				LineTo(hdc, startX+1,startY+1);
			}

			SelectObject (hdc , hPenOld);
			DeleteObject ( hPenEnemy );
			DeleteObject ( hPenFriend );

			EndPaint(hwnd, &ps);  
		}
		return FALSE;
	case WM_MOUSEMOVE:
	case WM_LBUTTONDOWN:
		{
			//�����ǰ�ס����϶������ƶ���ͼ��
			if (msg==WM_MOUSEMOVE && g_getLButtonState(App::sInstance().GetHGE()) != eLButtonState_Hold)
				return FALSE;
			
			//��괦��С��ͼ����һ����Χ�����ƶ���ͼ��
			int xPos = GET_X_LPARAM(lparam); 
			int yPos = GET_Y_LPARAM(lparam);
			RECT& smallRect = App::sInstance().GetSmallMapRect();
			//�����ϵĿ򣬱��ڶԱ��ǲ���Ҫ�ػ�
			RECT oldRect = App::sInstance().GetSmallMapRect();
			RECT clientRect;
			GetClientRect(hwnd,&clientRect);
			int width = smallRect.right - smallRect.left;
			int heigth = smallRect.bottom - smallRect.top;
			//��ͼ�����ĵ�ļ��޷�Χ
			RECT limitRect;
			limitRect.left = clientRect.left + width/2;
			limitRect.right = clientRect.right - width/2;
			limitRect.top = clientRect.top + heigth/2;
			limitRect.bottom = clientRect.bottom - heigth/2;
			//�����µĵ�ͼ�����ĵ�
			POINT newCenter;
			if (xPos < limitRect.left)
				newCenter.x = limitRect.left;
			else if(xPos > limitRect.right)
				newCenter.x = limitRect.right;
			else
				newCenter.x = xPos;
			if(yPos < limitRect.top)
				newCenter.y = limitRect.top;
			else if(yPos > limitRect.bottom)
				newCenter.y = limitRect.bottom;
			else
				newCenter.y = yPos;
			//�������ĵ㣬ʹ�÷��ϴ��ͼ����ȷƫ��
			int mapWidth = App::sInstance().GetSmallMap().GetWidth();
			int mapHeight = App::sInstance().GetSmallMap().GetHeight();
			int offsetX = newCenter.x*g_nMapWidthNum/mapWidth;
			int offsetY = newCenter.y*g_nMapHeightNum/mapHeight;
			newCenter.x = offsetX*mapWidth/g_nMapWidthNum;
			newCenter.y = offsetY*mapHeight/g_nMapHeightNum;
			//���µ�ͼ��
			smallRect.left = newCenter.x - width/2;
			smallRect.right = newCenter.x + width/2;
			smallRect.top = newCenter.y - heigth/2;
			smallRect.bottom = newCenter.y + heigth/2;
			//�ж��Ƿ�Ҫ�ػ��Ѿ�֪ͨ����
			if (oldRect.left == smallRect.left && oldRect.right == smallRect.right &&
				oldRect.top == smallRect.top && oldRect.bottom == smallRect.bottom)
			{
				return FALSE;
			}
			//֪ͨ���ͼ
			MapManager::sInstance().GetCurrentMap()->SetOffXY(smallRect.left*g_nMapWidthNum/mapWidth,smallRect.top*g_nMapHeightNum/mapHeight);
			RECT bigMapRect;
			HWND father = App::sInstance().GetHGE()->System_GetState(HGE_HWND);
			GetClientRect(father,&bigMapRect);
			InvalidateRect(father,&bigMapRect,FALSE);
			//�ػ�С��ͼ
			InvalidateRect(hwnd,&clientRect,FALSE);
		}
		return FALSE;
	}

	return DefWindowProc(hwnd, msg, wparam, lparam);
}

void App::CreateSmallMapWnd()
{
	HINSTANCE hInstance = GetModuleHandle(0);
	WNDCLASS		winclass;
	winclass.style = CS_DBLCLKS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW | CS_NOCLOSE;
	winclass.lpfnWndProc	= SmallMapProc;
	winclass.cbClsExtra		= 0;
	winclass.cbWndExtra		= 0;
	winclass.hInstance		= hInstance;
	winclass.hCursor		= LoadCursor(NULL, IDC_ARROW);
	winclass.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);
	winclass.lpszMenuName	= NULL; 
	winclass.lpszClassName	= "SmallMapClass";
	winclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClass(&winclass)) {
		g_debugString(__FILE__,__FUNCDNAME__,__LINE__,"ע��С��ͼ������ʧ��");
		return;
	}

	m_iSmallMapHwnd = CreateWindowEx(0,"SmallMapClass","С��ͼ",WS_CHILD|WS_POPUP|WS_CAPTION|WS_MINIMIZEBOX|WS_VISIBLE,0,0,240,240,m_pHge->System_GetState(HGE_HWND),0,GetModuleHandle(0),0);
	if (!m_iSmallMapHwnd)
	{
		g_debugString(__FILE__,__FUNCDNAME__,__LINE__,"����С��ͼ����ʧ��");
		return;
	}
	ShowWindow(m_iSmallMapHwnd, SW_SHOW);
	UpdateWindow(m_iSmallMapHwnd);
}

void App::ShutDownSmallMap()
{
	DestroyWindow(m_iSmallMapHwnd);
	UnregisterClass("SmallMapClass",GetModuleHandle(0));
	m_iSmallMapHwnd = NULL;
	m_iWndRect.top = m_iWndRect.bottom = m_iWndRect.left = m_iWndRect.right = 0;
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