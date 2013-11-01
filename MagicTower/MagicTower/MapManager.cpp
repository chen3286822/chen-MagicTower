#include "MapManager.h"
#include "TexManager.h"
#include "Character.h"
#include "CreatureManager.h"
#include "FontManager.h"
#include "GfxFont.h"
#include "App.h"
#include "MyLua.h"
#include "UI.h"

Map::Map()
{
	m_vObjList.clear();
	m_vBlocks.clear();
	m_nlevel = 0;
	m_nWidth = 0;
	m_nLength = 0;
	m_nTurns = 0;
	m_nCurTurn = 0;
	m_bShowTurns = false;
	m_bShowCampTurn = false;
	m_nShowTurnTime = 0;
	m_nShowCampTurnTime = 0;
	m_eCurCampTurn = eCampTurn_Friend;
	m_pMapSpr = new hgeSprite(0,0,0,0,0);
	m_nOffX = 0;
	m_nOffY = 0;

	m_iVictory.m_eCondition = eVictoryCondition_KillAllEnemies;
	m_iVictory.m_vData.clear();
	m_iVictory.m_nNum = -1;
	m_bVictory = false;
	m_bFailed = false;
}

Map::~Map()
{
	for (std::vector<MapObject*>::iterator it=m_vObjList.begin();it!=m_vObjList.end();it++)
	{
		gSafeDelete((*it)->spr);
		gSafeDelete(*it);
	}
	gSafeDelete(m_pMapSpr);
}

void Map::Init()
{
	m_iPathFinder.Init(m_nWidth,m_nLength);

	AddTurn();
	GoIntoTurn(eCampTurn_Friend);

	m_dwMapMoveTime[0] = 0;
	m_dwMapMoveTime[1] = 0;
	m_dwMapMoveTime[2] = 0;
	m_dwMapMoveTime[3] = 0;
}

void Map::Release()
{
	m_iPathFinder.Release();
}

void Map::AddTurn()
{
	m_nCurTurn++;
	m_bShowTurns = true;
	m_nShowTurnTime = 0;

	IsTriggerTurns(m_nCurTurn);

	//隐藏鼠标，禁止操作
//	App::sInstance().GetHGE()->System_SetState(HGE_HIDEMOUSE,true);
}

void Map::GoIntoTurn(eCampTurn turn)
{
	m_eCurCampTurn = turn;
	m_bShowCampTurn = true;
	m_nShowCampTurnTime = 0;

	//隐藏鼠标，禁止操作
//	App::sInstance().GetHGE()->System_SetState(HGE_HIDEMOUSE,true);
}

bool Map::IsShowTitle()
{
	return m_bShowCampTurn || m_bShowTurns;
}

void Map::SetMapTex(int level)
{
	if (!m_pMapSpr)
		return;
	
	HTEXTURE mapTex = TexManager::sInstance().GetMapTex(level);
	if (mapTex)
	{
		m_pMapSpr->SetTexture(mapTex);
		HGE* hge = hgeCreate(HGE_VERSION);
		int texWidth = hge->Texture_GetWidth(mapTex);
		int texHeight = hge->Texture_GetHeight(mapTex);
		m_pMapSpr->SetTextureRect(0,0,texWidth,texHeight);
		hge->Release();
	}
}

void Map::SetOffXY(int x,int y)
{
	if(x < 0 || y < 0)
		return;
	
	if(x > m_nWidth - APP_WIDTH/MAP_RECT || y > m_nLength - APP_HEIGHT/MAP_RECT)
		return;

	m_nOffX = x;
	m_nOffY = y;

	UISystem::sInstance().ResetPos();
}

void Map::Render()
{
#ifdef _LOAD_MAP_FROM_PNG
	if(m_pMapSpr)
	{
		m_pMapSpr->SetTextureRect(m_nOffX*MAP_RECT,m_nOffY*MAP_RECT,APP_WIDTH,APP_HEIGHT);
		m_pMapSpr->Render(0,0);
	}
#else
	for (std::vector<Block>::iterator it=m_vBlocks.begin();it!=m_vBlocks.end();it++)
	{
		blockInfo _blockInfo(TexManager::sInstance().GetBlock(getTerrain(it->attri)));
		m_pMapSpr->SetTexture(_blockInfo.m_iTex);
		m_pMapSpr->SetTextureRect(_blockInfo.m_fX,_blockInfo.m_fY,_blockInfo.m_fWidth,_blockInfo.m_fHeight);
		m_pMapSpr->RenderStretch(MAP_OFF_X +MAP_RECT*(*it).xpos-m_nOffX*MAP_RECT,MAP_OFF_Y+MAP_RECT*(*it).ypos-m_nOffY*MAP_RECT,MAP_OFF_X +MAP_RECT*(*it).xpos+MAP_RECT-m_nOffX*MAP_RECT,MAP_OFF_Y+MAP_RECT*(*it).ypos+MAP_RECT-m_nOffY*MAP_RECT);
	}
#endif

	for (std::vector<MapObject*>::iterator it=m_vObjList.begin();it!=m_vObjList.end();it++)
	{
		(*it)->spr->Render((MAP_RECT-FLOAT_PIC_SQUARE_WIDTH)/2+MAP_OFF_X +MAP_RECT*(*it)->xpos-m_nOffX*MAP_RECT,MAP_OFF_Y+MAP_RECT*(*it)->ypos-m_nOffY*MAP_RECT);
	}
}

void Map::RenderTitle()
{
	if (m_bShowTurns)
	{
		DWORD color = ((int)(255*m_nShowTurnTime/1500) << 24) | 0x00FFFFFF;
		GfxFont* font = FontManager::sInstance().GetFont(FontAttr(eFontType_SongTi,eFontSize_FontHuge));
		font->SetColor(color);
		font->Print(APP_WIDTH/2,APP_HEIGHT/2,"第%d回合",m_nCurTurn);
		if(m_nShowTurnTime <= 0)
		{
			m_bShowTurns = false;
			App::sInstance().GetHGE()->System_SetState(HGE_HIDEMOUSE,false);
		}
	}
	if (!m_bShowTurns && m_bShowCampTurn)
	{
		DWORD color = ((int)(255*m_nShowCampTurnTime/1500) << 24) | 0x00FFFFFF;
		GfxFont* font = FontManager::sInstance().GetFont(FontAttr(eFontType_SongTi,eFontSize_FontHuge));
		font->SetColor(color);
		string str = (m_eCurCampTurn == eCampTurn_Friend)?"我方回合":"敌方回合";
		font->Print(APP_WIDTH/2,APP_HEIGHT/2,str.c_str());
		if(m_nShowCampTurnTime <= 0)
		{
			m_bShowCampTurn = false;
			App::sInstance().GetHGE()->System_SetState(HGE_HIDEMOUSE,false);
		}
	}
}

void Map::Update(float dt)
{
	if(m_nCurTurn > m_nTurns)
	{
		//回合用完，结束游戏

	}
	if(m_bShowTurns && m_nShowTurnTime > 0)
	{
		m_nShowTurnTime -= (int)(dt*1000);
		if(m_nShowTurnTime <= 0)
			m_nShowTurnTime = 0;
	}
	if(m_bShowCampTurn && !m_bShowTurns && m_nShowCampTurnTime > 0)
	{
		m_nShowCampTurnTime -= (int)(dt*1000); 
		if(m_nShowCampTurnTime <= 0)
			m_nShowCampTurnTime = 0;
	}

	//移动地图
	//必须是自己回合
	if (CreatureManager::sInstance().GetCampTurn() == eCampTurn_Friend)
	{
		Block mouseBlock = App::sInstance().GetMouseBlock();
		//如果已经在地图边界了，那么就不需要移动地图
		if (mouseBlock.xpos == 0 || mouseBlock.xpos == m_nWidth-1 ||
			mouseBlock.ypos == 0 || mouseBlock.ypos == m_nLength-1)
		{
			return;
		}

		//单位移动、正在攻击、正在使用物品、正在释放技能阶段不可移动地图
		int selectNum = CreatureManager::sInstance().GetSelectNum();
		if (selectNum != -1)
		{
			Character* cha = CreatureManager::sInstance().GetCreature(selectNum);
			if(cha)
			{
				if(cha->GetActionStage() == eActionStage_MovingStage || cha->GetActionStage() == eActionStage_AttackCastStage||
					cha->GetActionStage() == eActionStage_SkillCastStage ||cha->GetActionStage() == eActionStage_ItemCastStage)
					return;
			}
		}

		bool isMoveMap = false;
		if(mouseBlock.xpos == m_nOffX)
		{
			if(m_nOffX > 0)
			{
				m_dwMapMoveTime[0] += dt*1000;
				if(m_dwMapMoveTime[0] >= 100)
				{
					m_nOffX--;
					isMoveMap = true;
					m_dwMapMoveTime[0] = 0;
				}
			}
		}
		else if(mouseBlock.xpos == m_nOffX + APP_WIDTH/MAP_RECT-1)
		{
			if(m_nOffX < m_nWidth-1)
			{
				m_dwMapMoveTime[1] += dt*1000;
				if(m_dwMapMoveTime[1] >= 100)
				{
					m_nOffX++;
					isMoveMap = true;
					m_dwMapMoveTime[1] = 0;
				}
			}
		}
		else
		{
			m_dwMapMoveTime[0] = 0;
			m_dwMapMoveTime[1] = 0;
		}

		if(mouseBlock.ypos == m_nOffY)
		{
			if(m_nOffY > 0)
			{
				m_dwMapMoveTime[2] += dt*1000;
				if(m_dwMapMoveTime[2] >= 100)
				{
					m_nOffY--;
					isMoveMap = true;
					m_dwMapMoveTime[2] = 0;
				}
			}
		}
		else if(mouseBlock.ypos == m_nOffY + APP_HEIGHT/MAP_RECT-1)
		{
			if(m_nOffY < m_nLength - 1)
			{
				m_dwMapMoveTime[3] += dt*1000;
				if(m_dwMapMoveTime[3] >= 100)
				{
					m_nOffY++;
					isMoveMap = true;
					m_dwMapMoveTime[3] = 0;
				}
			}
		}
		else
		{
			m_dwMapMoveTime[2] = 0;
			m_dwMapMoveTime[3] = 0;
		}

		//这里只是为了重新设定一些窗口位置
		if(isMoveMap)
			SetOffXY(m_nOffX,m_nOffY);
	}
}

MapObject* Map::GetObject(int x,int y)
{
	for (int i=0;i<m_vObjList.size();i++)
	{
		if(m_vObjList[i]->xpos == x && m_vObjList[i]->ypos == y)
			return m_vObjList[i];
	}
	return NULL;
}

Block* Map::GetBlock(int x,int y)
{
	if(x < 0 || x >= g_nMapWidthNum || y < 0 || y >= g_nMapHeightNum)
		return NULL;
	return &(m_vBlocks[x+y*g_nMapWidthNum]);
}

void Map::SetBlockOccupied(int xpos,int ypos)
{
	Block* block = GetBlock(xpos,ypos);
	if(block!=NULL)
	{
		setOccupied((block->attri),1);
	}
}

bool Map::GetBlockOccupied(int xpos,int ypos)
{
	Block* block = GetBlock(xpos,ypos);
	if(block!=NULL)
	{
		return IsOccupied(block->attri);
	}
	return false;
}

bool Map::GetCreatureStandOn(int xpos,int ypos)
{
	Block* block = GetBlock(xpos,ypos);
	if(block!=NULL)
	{
		return (IsOccupied(block->attri) && IsCanStandOn(block->attri));
	}
	return false;
}

void Map::SetSpecificRange(std::vector<Block*> _range)
{
	if (!_range.empty())
	{
		m_vSpecificRange = _range;
	}
}

std::vector<Block*> Map::FindPath(int startX,int startY,int endX,int endY)
{
	std::vector<Block*> vPath;

	//过滤不正确点
	if (startX < 0 || startX >= m_nWidth || startY < 0 || startY >= m_nLength
		|| endX < 0 || endX >= m_nWidth || endY <0 || endY >= m_nLength)
	{
		vPath.clear();
		m_vSpecificRange.clear();
		return vPath;
	}
	
	//调用A* 算法得到路径
	m_iPathFinder.UpdateMap();
	m_iPathFinder.Run(startX,startY,endX,endY);
	list<pNode> path = m_iPathFinder.GetPath();
	if (!path.empty())
	{
		for (list<pNode>::iterator it=path.begin();it!=path.end();it++)
		{
			Block* pathBlock = GetBlock((*it)->postionX,(*it)->postionY);
			if(pathBlock)
				vPath.push_back(pathBlock);
			else
			{
				//路线有问题，此时需要清空路线
				vPath.clear();
			}
		}
	}
	m_vSpecificRange.clear();
	return vPath;
}

void Map::SetVictoryCondition(int condition,int data,int num)
{
	switch(condition)
	{
	case eVictoryCondition_KillAllEnemies:
		m_iVictory.m_eCondition = eVictoryCondition_KillAllEnemies;
		m_iVictory.m_vData.clear();
		break;
	case eVictoryCondition_KillSpecificEnemy:
		{
			if(m_iVictory.m_eCondition == eVictoryCondition_KillSpecificEnemy)
			{
				m_iVictory.m_vData.push_back(data);
			}
			else
			{
				m_iVictory.m_eCondition = eVictoryCondition_KillSpecificEnemy;
				m_iVictory.m_vData.clear();
				m_iVictory.m_vData.push_back(data);
			}
		}
		break;
	case eVictoryCondition_GetToPosition:
		{
			if(m_iVictory.m_eCondition == eVictoryCondition_GetToPosition && m_iVictory.m_nNum == num)
			{
				m_iVictory.m_vData.push_back(data);
			}
			else
			{
				m_iVictory.m_eCondition = eVictoryCondition_GetToPosition;
				m_iVictory.m_vData.clear();
				m_iVictory.m_vData.push_back(data);
				m_iVictory.m_nNum = num;
			}
		}
		break;
	}
}

bool Map::CheckVictory(eVictoryCondition condition,int data)
{
	if(m_bVictory)
		return true;

	if(condition != eVictoryCondition_KillSpecificEnemy)
		return false;

	switch(m_iVictory.m_eCondition)
	{
	case eVictoryCondition_KillAllEnemies:
		if(CreatureManager::sInstance().IsEnemyAllDead())
			return true;
		break;
	case eVictoryCondition_KillSpecificEnemy:
		{
			//data 表示刚刚死的单位
			for(std::vector<int>::iterator it=m_iVictory.m_vData.begin();it!=m_iVictory.m_vData.end();)
			{
				if(*it == data)
					it = m_iVictory.m_vData.erase(it);
				else
					it++;
			}
			//特定单位死完
			if(m_iVictory.m_vData.empty())
				return true;
		}
		break;
	case eVictoryCondition_GetToPosition:
		{
			//data 表示刚刚发生移动的单位
			if (m_iVictory.m_nNum == data)
			{
				Character* cha = CreatureManager::sInstance().GetFriend(data);
				if (cha)
				{
					for(std::vector<int>::iterator it=m_iVictory.m_vData.begin();it!=m_iVictory.m_vData.end();it++)
					{
						int x = (*it) >> 8;
						int y = (*it) & 0x00FF;
						if(cha->GetBlock().xpos == x && cha->GetBlock().ypos == y)
							return true;
					}
				}
			}
		}
	}

	return false;
}

void Map::AddTrigger(int triggerID,const char* func,int turns,int num1,int num2,int x,int y)
{
	Trigger aTrigger;
	switch((eTrigger)triggerID)
	{
	case eTrigger_Turns:
		{
			aTrigger.m_eTrigger = eTrigger_Turns;
			aTrigger.m_nTurns = turns;
		}
		break;
	case eTrigger_Touch:
		{
			aTrigger.m_eTrigger = eTrigger_Touch;
			aTrigger.m_nNum1 = num1;
			aTrigger.m_nNum2 = num2;
		}
		break;
	case eTrigger_Location:
		{
			aTrigger.m_eTrigger = eTrigger_Location;
			aTrigger.m_nNum1 = num1;
			aTrigger.m_nPosX = x;
			aTrigger.m_nPosY = y;
		}
		break;
	case eTrigger_Kill:
		{
			aTrigger.m_eTrigger = eTrigger_Kill;
			aTrigger.m_nNum1 = num1;
			aTrigger.m_nNum2 = num2;
		}
		break;
	default:
		return;
	}
	aTrigger.m_strFunc = func;
	aTrigger.m_bEffective = true;
	m_vTriggers.push_back(aTrigger);
}

void Map::ClearTrigger()
{
	m_vTriggers.clear();
}

bool Map::IsTriggerTurns(int turns)
{
	for (std::vector<Trigger>::iterator it=m_vTriggers.begin();it!=m_vTriggers.end();it++)
	{
		if(it->m_eTrigger == eTrigger_Turns && it->m_bEffective == true)
		{
			if (turns >= it->m_nTurns)
			{
				g_MyLua.RunFunc(false,it->m_strFunc.c_str(),"");
				it->m_bEffective = false;
				return true;
			}
		}
	}
	return false;
}

bool Map::IsTriggerLocation(int num)
{
	for (std::vector<Trigger>::iterator it=m_vTriggers.begin();it!=m_vTriggers.end();it++)
	{
		if(it->m_eTrigger == eTrigger_Location && it->m_bEffective == true)
		{
			if (num == it->m_nNum1)
			{
				Character* cha = CreatureManager::sInstance().GetCreature(num);
				if (cha->GetBlock().xpos == it->m_nPosX && cha->GetBlock().ypos ==it->m_nPosY)
				{
					g_MyLua.RunFunc(false,it->m_strFunc.c_str(),"");
					it->m_bEffective = false;
					return true;
				}
			}
			//不指定单位
			else if (it->m_nNum1 == -1)
			{
				//检查指定地点是否有玩家单位
				Character* cha = CreatureManager::sInstance().GetFriend(it->m_nPosX,it->m_nPosY);
				if (cha != NULL)
				{
					g_MyLua.RunFunc(false,it->m_strFunc.c_str(),"");
					it->m_bEffective = false;
					return true;
				}
			}
		}
	}
	return false;
}

bool Map::IsTriggerTouch(int num)
{
	for (std::vector<Trigger>::iterator it=m_vTriggers.begin();it!=m_vTriggers.end();it++)
	{
		if(it->m_eTrigger == eTrigger_Touch && it->m_bEffective == true)
		{
			if (num == it->m_nNum1)
			{
				Character* cha = CreatureManager::sInstance().GetCreature(num);
				Character* target = CreatureManager::sInstance().GetCreature(it->m_nNum2);
				if (cha && target)
				{
					//是否接触，即是否是上下左右相邻的关系
					if(abs(cha->GetBlock().xpos-target->GetBlock().xpos)+abs(cha->GetBlock().ypos-target->GetBlock().ypos) == 1)
					{
						g_MyLua.RunFunc(false,it->m_strFunc.c_str(),"");
						it->m_bEffective = false;
						return true;
					}
				}				
			}
			//不指定单位
			else if (it->m_nNum1 == -1)
			{
				//检查指定单位周围是否有玩家单位
				Character* target = CreatureManager::sInstance().GetCreature(it->m_nNum2);
				int x[4] = {0,0,1,-1};
				int y[4] = {1,-1,0,0};
				for (int i=0;i<4;i++)
				{
					Character* cha = CreatureManager::sInstance().GetFriend(target->GetBlock().xpos+x[i],target->GetBlock().ypos+y[i]);
					if(cha != NULL)
					{
						g_MyLua.RunFunc(false,it->m_strFunc.c_str(),"");
						it->m_bEffective = false;
						return true;
					}
				}
			}
		}
	}
	return false;
}

bool Map::IsTriggerKill(int num)
{
	for (std::vector<Trigger>::iterator it=m_vTriggers.begin();it!=m_vTriggers.end();it++)
	{
		if(it->m_eTrigger == eTrigger_Kill && it->m_bEffective == true)
		{
			if (num == it->m_nNum1)
			{
				Character* target = CreatureManager::sInstance().GetCreature(it->m_nNum2);
				//被击杀
				if (target && target->GetDead()==true)
				{
					g_MyLua.RunFunc(false,it->m_strFunc.c_str(),"");
					it->m_bEffective = false;
					return true;
				}				
			}
			//不指定单位
			else if (it->m_nNum1 == -1)
			{
				//目标死亡
				Character* target = CreatureManager::sInstance().GetCreature(it->m_nNum2);
				if(target != NULL && target->GetDead() == true)
				{
					g_MyLua.RunFunc(false,it->m_strFunc.c_str(),"");
					it->m_bEffective = false;
					return true;
				}
			}
		}
	}
	return false;
}

MapManager::MapManager(void)
{
	m_vMaps.clear();
	m_pMarkUp = NULL;
	m_nCurrentLevel = 0;
}


MapManager::~MapManager(void)
{
	//释放当前地图资源
	Map* theMap = GetCurrentMap();
	if(theMap)
		theMap->Release();

	gSafeDelete(m_pMarkUp);
	for (std::vector<Map*>::iterator it=m_vMaps.begin();it!=m_vMaps.end();it++)
	{
		gSafeDelete((*it));
	}
}


//不应使用该函数，因为地图是一次性全部载入，而每关的单位都会同时载入
bool MapManager::LoadMaps()
{
	m_vMaps.clear();
	if(m_pMarkUp==NULL)
		m_pMarkUp = new CMarkup;

	char pBuf[MAX_PATH];
	char pathMap[MAX_PATH];
	GetCurrentDirectory(MAX_PATH,pBuf);
	sprintf(pathMap,"%s\\res\\Maps",pBuf);

	std::map<std::string,std::string> files;
	g_getFiles(pathMap,files,".xml",LEVEL_NUM,true);
	
	for (std::map<std::string,std::string>::iterator mit=files.begin();mit!=files.end();mit++)
	{
		m_pMarkUp->Load(mit->first);
		Map* level = new Map;
		int _level = 0;
		int _width = 0,_length = 0;
		int _ID = 0;
		int _Action = 0;
		int _xpos = 0;
		int _ypos = 0;
		eCamp _camp = eCamp_Neutral;
		int num = 0;
		m_pMarkUp->ResetMainPos();
		if(m_pMarkUp->FindElem("map"))
		{
			_level = atoi(m_pMarkUp->GetAttrib("level").c_str());
			_width = atoi(m_pMarkUp->GetAttrib("width").c_str());
			_length = atoi(m_pMarkUp->GetAttrib("length").c_str());
			if(_level!=0 && _width!=0 && _length!=0)
			{
				level->SetLevel(_level);
				level->SetWidthLength(_width,_length);
				level->GetTurns() = 20;	//默认20回合一关
			}
			else
				return false;
		}
		m_pMarkUp->IntoElem();
		if(m_pMarkUp->FindElem("Block"))
		{
			m_pMarkUp->IntoElem();
			int _type = 0;
			while(m_pMarkUp->FindElem("Terrain"))
			{
				_type = atoi(m_pMarkUp->GetAttrib("type").c_str());
				_xpos = atoi(m_pMarkUp->GetAttrib("xpos").c_str());
				_ypos = atoi(m_pMarkUp->GetAttrib("ypos").c_str());
				Block _block(_xpos,_ypos);
// 				setTerrain(_block.attri,_type);
// 				if (_type == HillTop || _type == CityWall)
// 				{
// 					setStandOn(_type,0);
// 				}
				_block.attri = _type;
				level->AddBlock(_block);
			}
			std::sort(level->GetVBlock().begin(),level->GetVBlock().end(),Block::less_than);
			m_pMarkUp->OutOfElem();
		}
		m_vMaps.push_back(level);

		if(m_pMarkUp->FindElem("Creature"))
		{
			m_pMarkUp->IntoElem();
			while(m_pMarkUp->FindElem("Man"))
			{
				_ID = atoi(m_pMarkUp->GetAttrib("ID").c_str());
				_Action = atoi(m_pMarkUp->GetAttrib("Action").c_str());
				_xpos = atoi(m_pMarkUp->GetAttrib("xpos").c_str());
				_ypos = atoi(m_pMarkUp->GetAttrib("ypos").c_str());
				_camp = (eCamp)(atoi(m_pMarkUp->GetAttrib("camp").c_str()));

				Character* cha = new Character;
//				MapObject* mo = new MapObject;
				cha->Init(level->GetLevel(),_ID,num,_Action,Block(_xpos,_ypos));
				//地图格子由于单位生成，属性变化移动到单位创建中完成
// 				Block* theBlock = level->GetBlock(_xpos,_ypos);
// 				if(theBlock != NULL)
// 					setOccupied((theBlock->attri),1);
				//测试移动力
				cha->SetMoveAbility(4/*,MapManager::sInstance().GetCurrentMap()*/);

				if (_camp == eCamp_Friend)
				{
					cha->SetCamp(eCamp_Friend);
					CreatureManager::sInstance().AddFriend(cha);
				}
				else if (_camp == eCamp_Enemy)
				{
					cha->SetCamp(eCamp_Enemy);
					CreatureManager::sInstance().AddEnemy(cha);
				}
				num++;
			}
			m_pMarkUp->OutOfElem();
		}	
	}

	//将所有地图文件按照level属性进行排序
	//load 完所有地图文件
	if(m_vMaps.empty())
		return false;
	std::sort(m_vMaps.begin(),m_vMaps.end(),Map::Less_than);
// 	SetCurrentLevel(m_vMaps[0]->GetLevel());
// 	GetCurrentMap()->Init();
	return true;
}

bool MapManager::LoadMap(int level)
{
	//检查是否已经加载了
	Map* theMap = GetMap(level);
	if(theMap)
		return true;

	if(m_pMarkUp==NULL)
		m_pMarkUp = new CMarkup;

	//载入地图配置
	char pBuf[MAX_PATH];
	char pathMap[MAX_PATH];
	GetCurrentDirectory(MAX_PATH,pBuf);

#ifdef _LOAD_MAP_FROM_PNG
	sprintf(pathMap,"%s\\res\\tex\\MapTex\\%d.xml",pBuf,level);
#else
	sprintf(pathMap,"%s\\res\\Maps\\%d.xml",pBuf,level);
#endif

	//不存在该地图配置
	if(_access(pathMap,0) != 0)
		return false;

	m_pMarkUp->Load(pathMap);
	Map* targetMap = new Map;
	int _level = 0;
	int _width = 0,_length = 0;
	int _ID = 0;
	int _Action = 0;
	int _xpos = 0;
	int _ypos = 0;
	eCamp _camp = eCamp_Neutral;
	int num = 0;
	m_pMarkUp->ResetMainPos();
	if(m_pMarkUp->FindElem("map"))
	{
		_level = atoi(m_pMarkUp->GetAttrib("level").c_str());
		_width = atoi(m_pMarkUp->GetAttrib("width").c_str());
		_length = atoi(m_pMarkUp->GetAttrib("length").c_str());
		if(_level!=0 && _width!=0 && _length!=0)
		{
			g_resetGlobalMapValues(_width,_length);
			targetMap->SetLevel(_level);
			targetMap->SetMapTex(level);
			targetMap->SetWidthLength(_width,_length);
			targetMap->GetTurns() = 20;	//默认20回合一关
		}
		else
			return false;
	}
	m_pMarkUp->IntoElem();
	if(m_pMarkUp->FindElem("Block"))
	{
		m_pMarkUp->IntoElem();
		int _type = 0;
		while(m_pMarkUp->FindElem("Terrain"))
		{
			_type = atoi(m_pMarkUp->GetAttrib("type").c_str());
			_xpos = atoi(m_pMarkUp->GetAttrib("xpos").c_str());
			_ypos = atoi(m_pMarkUp->GetAttrib("ypos").c_str());
			Block _block(_xpos,_ypos);
			_block.attri = _type;
			targetMap->AddBlock(_block);
		}
		std::sort(targetMap->GetVBlock().begin(),targetMap->GetVBlock().end(),Block::less_than);
		m_pMarkUp->OutOfElem();
	}
	m_vMaps.push_back(targetMap);


	//清楚以前的单位数据
	CreatureManager::sInstance().Release();
	//载入预制的单位
	if(m_pMarkUp->FindElem("Creature"))
	{
		m_pMarkUp->IntoElem();
		while(m_pMarkUp->FindElem("Man"))
		{
			_ID = atoi(m_pMarkUp->GetAttrib("ID").c_str());
			_Action = atoi(m_pMarkUp->GetAttrib("Action").c_str());
			_xpos = atoi(m_pMarkUp->GetAttrib("xpos").c_str());
			_ypos = atoi(m_pMarkUp->GetAttrib("ypos").c_str());
			_camp = (eCamp)(atoi(m_pMarkUp->GetAttrib("camp").c_str()));

			Character* cha = new Character;
			//				MapObject* mo = new MapObject;
			cha->Init(targetMap->GetLevel(),_ID,num,_Action,Block(_xpos,_ypos));
			//地图格子由于单位生成，属性变化移动到单位创建中完成
			// 				Block* theBlock = level->GetBlock(_xpos,_ypos);
			// 				if(theBlock != NULL)
			// 					setOccupied((theBlock->attri),1);
			//测试移动力
			cha->SetMoveAbility(4);

			if (_camp == eCamp_Friend)
			{
				cha->SetCamp(eCamp_Friend);
				CreatureManager::sInstance().AddFriend(cha);
			}
			else if (_camp == eCamp_Enemy)
			{
				cha->SetCamp(eCamp_Enemy);
				CreatureManager::sInstance().AddEnemy(cha);
			}
			num++;
		}
		m_pMarkUp->OutOfElem();
	}	
	return true;
}

Map* MapManager::GetMap(int level)
{
	for (int i=0;i<m_vMaps.size();i++)
	{
		if(level == m_vMaps[i]->GetLevel())
			return m_vMaps[i];
	}
	return NULL;
}

void MapManager::Render()
{
	static int oldLevel = -1;
	static Map* currentMap = NULL;
	if (oldLevel != m_nCurrentLevel)
	{
		currentMap = GetMap(m_nCurrentLevel);
		oldLevel = m_nCurrentLevel;
	}
	else
	{
		if(currentMap!=NULL)
			currentMap->Render();
	}
}

void MapManager::Update(float dt)
{
	static int oldLevel = -1;
	static Map* currentMap = NULL;
	if (oldLevel != m_nCurrentLevel)
	{
		currentMap = GetMap(m_nCurrentLevel);
		oldLevel = m_nCurrentLevel;
	}
	else
	{
		if(currentMap!=NULL)
			currentMap->Update(dt);
	}
}