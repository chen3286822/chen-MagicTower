#include "MapManager.h"
#include "TexManager.h"
#include "Character.h"
#include "CreatureManager.h"

Map::Map()
{
	m_vObjList.clear();
	m_vBlocks.clear();
	m_nlevel = 0;
	m_nWidth = 0;
	m_nLength = 0;
	m_pMapSpr = new hgeSprite(0,0,0,0,0);
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
	m_iPathFinder.Init();
}

void Map::Render()
{
	for (std::vector<Block>::iterator it=m_vBlocks.begin();it!=m_vBlocks.end();it++)
	{
		blockInfo _blockInfo(TexManager::sInstance().GetBlock(getTerrain(it->attri)));
		m_pMapSpr->SetTexture(_blockInfo.m_iTex);
		m_pMapSpr->SetTextureRect(_blockInfo.m_fX,_blockInfo.m_fY,_blockInfo.m_fWidth,_blockInfo.m_fHeight);
		m_pMapSpr->RenderStretch(MAP_OFF_X +MAP_RECT*(*it).xpos,MAP_OFF_Y+MAP_RECT*(*it).ypos,MAP_OFF_X +MAP_RECT*(*it).xpos+MAP_RECT,MAP_OFF_Y+MAP_RECT*(*it).ypos+MAP_RECT);
	}

	for (std::vector<MapObject*>::iterator it=m_vObjList.begin();it!=m_vObjList.end();it++)
	{
		(*it)->spr->Render((MAP_RECT-FLOAT_PIC_SQUARE_WIDTH)/2+MAP_OFF_X +MAP_RECT*(*it)->xpos,MAP_OFF_Y+MAP_RECT*(*it)->ypos);
	}
}

void Map::Update()
{

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
	if(x < 0 || x >= MAP_WIDTH_NUM || y < 0 || y >= MAP_LENGTH_NUM)
		return NULL;
	return &(m_vBlocks[x+y*MAP_WIDTH_NUM]);
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


std::vector<Block*> Map::FindPath(int startX,int startY,int endX,int endY)
{
	std::vector<Block*> vPath;
	
	//调用A* 算法得到路径
	m_iPathFinder.UpdateMap();
	m_iPathFinder.Run(startX,startY,endX,endY);
	list<pNode> path = m_iPathFinder.GetPath();
	if (!path.empty())
	{
		for (list<pNode>::iterator it=path.begin();it!=path.end();it++)
		{
			Block* pathBlock = GetBlock((*it)->postionX,(*it)->postionY);
			if(!pathBlock)
				vPath.push_back(pathBlock);
			else
			{
				//路线有问题，此时需要清空路线
				vPath.clear();
			}
		}
	}

	return vPath;
}

MapManager::MapManager(void)
{
	m_vMaps.clear();
	m_pMarkUp = NULL;
	m_nCurrentLevel = 0;
}


MapManager::~MapManager(void)
{
	gSafeDelete(m_pMarkUp);
	for (std::vector<Map*>::iterator it=m_vMaps.begin();it!=m_vMaps.end();it++)
	{
		gSafeDelete((*it));
	}
}

bool MapManager::LoadMaps(std::string path)
{
	if(m_pMarkUp==NULL)
		m_pMarkUp = new CMarkup;
	std::map<std::string,std::string> files;
	g_getFiles(path,files,".xml",LEVEL_NUM,true);
	
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
		Camp _camp = Neutral;
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
				_camp = (Camp)(atoi(m_pMarkUp->GetAttrib("camp").c_str()));

				Character* cha = new Character;
//				MapObject* mo = new MapObject;
				cha->Init(TexManager::sInstance().GetTex(_ID),level->GetLevel(),_ID,num,_Action,Block(_xpos,_ypos));
				//地图格子由于单位生成，属性变化移动到单位创建中完成
// 				Block* theBlock = level->GetBlock(_xpos,_ypos);
// 				if(theBlock != NULL)
// 					setOccupied((theBlock->attri),1);
				//测试移动力
				cha->SetMoveAbility(4);

				if (_camp == Friend)
				{
					cha->SetCamp(Friend);
					CreatureManager::sInstance().AddFriend(cha);
				}
				else if (_camp == Enemy)
				{
					cha->SetCamp(Enemy);
					CreatureManager::sInstance().AddEnemy(cha);
				}
				num++;
			}
			m_pMarkUp->OutOfElem();
		}	
	}

	//将所有地图文件按照level属性进行排序
	//load 完所有地图文件，设置当前关卡为第一关并初始化
	if(m_vMaps.empty())
		return false;
	std::sort(m_vMaps.begin(),m_vMaps.end(),Map::Less_than);
	SetCurrentLevel(m_vMaps[0]->GetLevel());
	GetCurrentMap()->Init();
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

void MapManager::Update()
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
			currentMap->Update();
	}
}