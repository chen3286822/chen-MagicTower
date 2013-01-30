#include "Application.h"
#include "PictureController.h"


bool update();
bool render();


Application::Application(void)
{
	m_goNext = false;
	m_goLast = false;

	m_MapVertex[0].x = MAP_OFF_X;
	m_MapVertex[0].y = MAP_OFF_Y;
	m_MapVertex[0].tx = 0;
	m_MapVertex[0].ty = 0;
	m_MapVertex[1].x = MAP_OFF_X + MAP_WIDTH;
	m_MapVertex[1].y = MAP_OFF_Y;
	m_MapVertex[1].tx = 1.0f;
	m_MapVertex[1].ty = 0;
	m_MapVertex[2].x = MAP_OFF_X + MAP_WIDTH;
	m_MapVertex[2].y = MAP_OFF_Y + MAP_LENGTH;
	m_MapVertex[2].tx = 1.0f;
	m_MapVertex[2].ty = 1.0f;
	m_MapVertex[3].x = MAP_OFF_X;
	m_MapVertex[3].y = MAP_OFF_Y + MAP_LENGTH;
	m_MapVertex[3].tx = 0;
	m_MapVertex[3].ty = 1.0f;
	for (int i=0;i<4;i++)
	{
		m_MapVertex[i].col = 0xFFFFFFFF;
		m_MapVertex[i].z = 0.5;
	}

	m_mouseSpr = NULL;
	m_tag = 0;
	m_action = 0;
}

Application::~Application(void)
{
}

 bool Application::systemInit(HWND hwnd)
{
	hge = hgeCreate(HGE_VERSION);

	// Set up log file, frame function, render function and window title
	hge->System_SetState(HGE_LOGFILE, "MagicTowerEditor.log");
	hge->System_SetState(HGE_FRAMEFUNC, update);
	hge->System_SetState(HGE_RENDERFUNC,render);
	hge->System_SetState(HGE_TITLE, "Magic Tower Editor");
	hge->System_SetState(HGE_HIDEMOUSE,false);
	hge->System_SetState(HGE_HWNDPARENT, hwnd);

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

 void Application::loadResource()
 {
	 m_markUp = NULL;
//	 m_markUp = new CMarkup;
// 	 if(!m_markUp->Load("map.xml"))
// 	 {
// 		 m_markUp->AddElem("map");
// 		 m_markUp->AddChildElem("Static item");                      
// 		 m_markUp->IntoElem();
// 		 m_markUp->AddChildElem("Wall","first");
// 		 m_markUp->AddChildElem("Tree","two");
// 		 m_markUp->AddChildElem("Pool","third");
// 		 m_markUp->Save("map.xml");
// 	 }
//	 const char* doc = (m_markUp->GetDoc()).c_str();
	 m_vMapObject.clear();
	 m_vMapObject.resize(MAP_WIDTH_NUM*MAP_LENGTH_NUM);
	 m_Blocks.clear();
	 m_Blocks.resize(MAP_WIDTH_NUM*MAP_LENGTH_NUM);
//	 m_vMapObject.clear();

	 tex = hge->Texture_Load("res/tex/1.png");
	 nextButton = new hgeGUIButton(ID_CTRL_BTN_1,600,48,48,64,tex,0,0);
	 lastButton = new hgeGUIButton(ID_CTRL_BTN_2,664,48,48,64,tex,0,0);
	 pic = new PictureController(hge,600,100,800,550,18);
	 pic->init();
	 gui = new hgeGUI();
	 gui->AddCtrl(nextButton);
	 gui->AddCtrl(lastButton);
	 gui->AddCtrl(pic);
	 gui->Enter();
 }

 void Application::run()
 {
	 hge->System_Start();
 }

 void Application::freeResource()
 {
	 gui->DelCtrl(ID_CTRL_PIC_1);
	 gui->DelCtrl(ID_CTRL_BTN_1);
	 gui->DelCtrl(ID_CTRL_BTN_2);
	gSafeDelete(gui);

	hge->Texture_Free(tex);
	gSafeDelete(m_mouseSpr);

	gSafeDelete(m_markUp);

	for (int i=0;i<MAP_WIDTH_NUM*MAP_LENGTH_NUM;i++)
	{
		if(m_vMapObject[i]!=NULL)
		{
			gSafeDelete(m_vMapObject[i]->spr);
			gSafeDelete(m_vMapObject[i]);
		}
	}
 }

 void Application::cleanUp()
 {
	 hge->System_Shutdown();
	 hge->Release();
 }

 bool Application::appRender()
 {
	 hge->Gfx_Clear(0X00000000);
	 hge->Gfx_BeginScene();
	 gui->Render(); 
	 drawMap();
	 drawMouseTex();
	 hge->Gfx_EndScene();
	 return false;
 }

 bool Application::appUpdate()
 {
 	 if (hge->Input_GetKeyState(HGEK_ESCAPE))
 	 {
		 SendMessage(hge->System_GetState(HGE_HWNDPARENT),WM_DESTROY,NULL,NULL);
 		 return true;
 	 }
	 float dt = hge->Timer_GetDelta();
	 int id = gui->Update(dt);
	 if (id == -1)
	 {
		 gui->Enter();
	 }
	 else if (id == ID_CTRL_BTN_1)
	 {
		 setLast(true);
		 gui->Leave();
	 }
	 else if (id == ID_CTRL_BTN_2)
	 {
		 setNext(true);
		 gui->Leave();
	 }

	 updateMap();
	 return false;
 }

 void Application::updateMap()
 {
	 if(getLButtonState(hge) == LBUTTON_DOWN &&
		 m_mouseSpr!=NULL)
	 {
		 float xpos,ypos;
		 hge->Input_GetMousePos(&xpos,&ypos);
		 if(xpos >= MAP_OFF_X && xpos < MAP_OFF_X+MAP_WIDTH && ypos >= MAP_OFF_Y && ypos < MAP_OFF_Y+MAP_LENGTH)
		 {
			 float xMap,yMap;
			 xMap = xpos - MAP_OFF_X;
			 yMap = ypos - MAP_OFF_Y;
			 int xNum = 0,yNum = 0;
			 xNum = (int)(xMap/MAP_RECT);
			 yNum = (int)(yMap/MAP_RECT);

			 //先检查原来是否有物体了，有的话删除掉
			 if (m_vMapObject[xNum+yNum*MAP_WIDTH_NUM]!=NULL)
			 {
				 gSafeDelete(m_vMapObject[xNum+yNum*MAP_WIDTH_NUM]->spr);
				 gSafeDelete(m_vMapObject[xNum+yNum*MAP_WIDTH_NUM]);
			 }

			 MapObject* mo = new MapObject;
			 mo->spr				=	new hgeSprite(*m_mouseSpr);
			 mo->ID				=	m_tag;
			 mo->action			=	m_action;
			 mo->xpos			=	xNum;
			 mo->ypos			=	yNum;
			 m_vMapObject[xNum+yNum*MAP_WIDTH_NUM] = mo;
		 }

	 }



 }

 void Application::drawMap()
 {
	 hgeQuad mapQuad;
	 mapQuad.v[0] = m_MapVertex[0];
	 mapQuad.v[1] = m_MapVertex[1];
	 mapQuad.v[2] = m_MapVertex[2];
	 mapQuad.v[3] = m_MapVertex[3];
	 mapQuad.tex = 0;
	 mapQuad.blend = BLEND_DEFAULT_Z;
	 hge->Gfx_RenderQuad(&mapQuad);

	 drawMapLine();

	 for (int i=0;i<MAP_WIDTH_NUM*MAP_LENGTH_NUM;i++)
	 {
		 if(m_vMapObject[i]!=NULL)
			 m_vMapObject[i]->spr->Render((MAP_RECT-FLOAT_PIC_SQUARE_WIDTH)/2+MAP_OFF_X +MAP_RECT*m_vMapObject[i]->xpos,MAP_OFF_Y+MAP_RECT*m_vMapObject[i]->ypos);
	 }
 }

 void Application::drawMapLine()
 {
	 for (int i=1;i<MAP_WIDTH_NUM;i++)
	 {
		hge->Gfx_RenderLine(MAP_OFF_X+i*MAP_RECT,MAP_OFF_Y,MAP_OFF_X+i*MAP_RECT,MAP_OFF_Y+MAP_LENGTH,0xFF7F7F7F); 
	 }
	for (int j=1;j<MAP_LENGTH_NUM;j++)
	{
		hge->Gfx_RenderLine(MAP_OFF_X,j*MAP_RECT+MAP_OFF_Y,MAP_OFF_X+MAP_WIDTH,j*MAP_RECT+MAP_OFF_Y,0xFF7F7F7F);
	}
 }

 void Application::drawSmallRect(float x,float y)
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
		 quad.v[i].col = 0x4F7F7F7F;
		 quad.v[i].z = 0.5;
	 }
	 quad.blend = BLEND_DEFAULT_Z;
	 quad.tex = 0;
	 hge->Gfx_RenderQuad(&quad);
 }

 void Application::drawMouseTex()
 {
	if (m_mouseSpr != NULL)
	{
		float xpos,ypos;
		hge->Input_GetMousePos(&xpos,&ypos);
		if(xpos >= MAP_OFF_X && xpos < MAP_OFF_X+MAP_WIDTH && ypos >= MAP_OFF_Y && ypos < MAP_OFF_Y+MAP_LENGTH)
		{
			m_mouseSpr->Render(xpos-FLOAT_PIC_SQUARE_WIDTH/2,ypos-FLOAT_PIC_SQUARE_HEIGHT/2);
			drawSmallRect(xpos,ypos);
		}
	}
 }

 void Application::setMouseTex(hgeSprite* spr)
 {
	//需要先删除以前可能被赋值过的sprite，防止赋值新的后内存泄露
	 if(m_mouseSpr != NULL)
		 delete m_mouseSpr;
	 m_mouseSpr = spr;
 }

 void	 Application::resetMap()
 {
	 m_markUp->SetDoc("");
	 m_markUp->AddElem("map");
	 m_markUp->AddAttrib("level",m_config.level);
	 m_markUp->AddAttrib("width",MAP_WIDTH_NUM);
	 m_markUp->AddAttrib("length",MAP_LENGTH_NUM);
	 m_markUp->IntoElem();
 }

 bool Application::saveMapToXml()
 {
	 if (m_markUp==NULL)
	 {
		 MessageBox(NULL,TEXT("xml 未初始化，请先new file"),"Error", MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);
		 return false;
	 }
	 resetMap();
	 //为地图添加默认地形
	 m_markUp->AddElem("Block");
	 m_markUp->IntoElem();
	 for (int i=0;i<MAP_WIDTH_NUM*MAP_LENGTH_NUM;i++)
	 {
		 m_markUp->AddElem("Terrain");
		 //设置地图块属性
		 int attr = Road;
		 setStandOn(attr,1);
		 m_markUp->AddAttrib("type",attr);
		 m_markUp->AddAttrib("xpos",i%MAP_WIDTH_NUM);
		 m_markUp->AddAttrib("ypos",i/MAP_WIDTH_NUM);
	 }
	 m_markUp->OutOfElem();
	 m_markUp->AddElem("Object");
	 m_markUp->IntoElem();
	 //添加单位
	 for (int i=0;i<MAP_WIDTH_NUM*MAP_LENGTH_NUM;i++)
	 {
		 if(m_vMapObject[i]!=NULL)
		 {
			 m_markUp->AddElem("Man");
			 m_markUp->AddAttrib("ID",m_vMapObject[i]->ID);
			 m_markUp->AddAttrib("Action",m_vMapObject[i]->action);
			 m_markUp->AddAttrib("xpos",m_vMapObject[i]->xpos);
			 m_markUp->AddAttrib("ypos",m_vMapObject[i]->ypos);
		 }
	 }
	 m_markUp->OutOfElem();
	 return true;
 }

 bool Application::loadMapFromXml()
 {
	 m_config.clear();
	 for(int i=0;i<MAP_WIDTH_NUM*MAP_LENGTH_NUM;i++)
		 if(m_vMapObject[i]!=NULL)
		 {
			 gSafeDelete(m_vMapObject[i]->spr);
			 gSafeDelete(m_vMapObject[i]);
		 }

	 if(m_markUp==NULL)
		 return false;
	 int _ID = 0;
	 int _width=0,_length=0;
	 int _level=-1;
	 int _Action = 0;
	 int _xpos = 0;
	 int _ypos = 0;
	 m_markUp->ResetMainPos();
	 if(m_markUp->FindElem("map"))
	 {
		 _level = atoi(m_markUp->GetAttrib("level").c_str());
		 _width = atoi(m_markUp->GetAttrib("width").c_str());
		 _length = atoi(m_markUp->GetAttrib("length").c_str());
		 if( _width==0 || _length==0 || _level==-1)
			 return false;
		 m_config.level = _level;
	 }
	 m_markUp->IntoElem();
	 if(m_markUp->FindElem("Block"))
	 {
		 m_markUp->IntoElem();
		 int _type = 0;
		 while(m_markUp->FindElem("Terrain"))
		 {
			 _type = atoi(m_markUp->GetAttrib("type").c_str());
			 _xpos = atoi(m_markUp->GetAttrib("xpos").c_str());
			 _ypos = atoi(m_markUp->GetAttrib("ypos").c_str());
			 Block _block(_xpos,_ypos);
			 setTerrain(_block.attri,_type);
			 m_Blocks[_ypos*_length+_xpos] = _block;
		 }
		 m_markUp->OutOfElem();
	 }

	if(m_markUp->FindElem("Object"))
	{
		m_markUp->IntoElem();
		while (m_markUp->FindElem("Man"))
		{
			_ID = atoi(m_markUp->GetAttrib("ID").c_str());
			_Action = atoi(m_markUp->GetAttrib("Action").c_str());
			_xpos = atoi(m_markUp->GetAttrib("xpos").c_str());
			_ypos = atoi(m_markUp->GetAttrib("ypos").c_str());

			MapObject* mo = new MapObject;
			HTEXTURE tempTex;
			std::map<HTEXTURE,int>& texMap = pic->getTexMap();
			for (std::map<HTEXTURE,int>::iterator mit=texMap.begin();mit!=texMap.end();mit++)
			{
				if(mit->second == _ID)
				{
					tempTex = mit->first;
					break;
				}
			}
			float xNum=0,yNum=0;
			xNum = _Action%(FLOAT_PIC_WIDTH/FLOAT_PIC_SQUARE_WIDTH);
			yNum = _Action/(FLOAT_PIC_WIDTH/FLOAT_PIC_SQUARE_WIDTH);
			mo->spr			=	new hgeSprite(tempTex,xNum*FLOAT_PIC_SQUARE_WIDTH,yNum*FLOAT_PIC_SQUARE_HEIGHT,FLOAT_PIC_SQUARE_WIDTH,FLOAT_PIC_SQUARE_HEIGHT);
			mo->ID				=	_ID;
			mo->action		=	_Action;
			mo->xpos			=	_xpos;
			mo->ypos			=	_ypos;
			m_vMapObject[_xpos+_ypos*MAP_WIDTH_NUM] = mo;
		}
		m_markUp->OutOfElem();
	}
	return true;
 }

 bool update()
 {
	return Application::sInstance().appUpdate();
 }

 bool render()
 {
	return Application::sInstance().appRender();
 }