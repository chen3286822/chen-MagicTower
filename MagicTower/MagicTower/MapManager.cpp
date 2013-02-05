#include "MapManager.h"
#include "TexManager.h"
#include "Character.h"
#include "CreatureManager.h"

Map::Map()
{
	m_vObjList.clear();
	m_vBlocks.clear();
	m_ilevel = 0;
	m_width = 0;
	m_length = 0;
	m_mapSpr = new hgeSprite(0,0,0,0,0);
}

Map::~Map()
{
	for (std::vector<MapObject*>::iterator it=m_vObjList.begin();it!=m_vObjList.end();it++)
	{
		gSafeDelete((*it)->spr);
		gSafeDelete(*it);
	}
	gSafeDelete(m_mapSpr);
}

void Map::Render()
{
	for (std::vector<Block>::iterator it=m_vBlocks.begin();it!=m_vBlocks.end();it++)
	{
		blockInfo _blockInfo(TexManager::sInstance().GetBlock(getTerrain(it->attri)));
		m_mapSpr->SetTexture(_blockInfo.tex);
		m_mapSpr->SetTextureRect(_blockInfo.x,_blockInfo.y,_blockInfo.width,_blockInfo.height);
		m_mapSpr->RenderStretch(MAP_OFF_X +MAP_RECT*(*it).xpos,MAP_OFF_Y+MAP_RECT*(*it).ypos,MAP_OFF_X +MAP_RECT*(*it).xpos+MAP_RECT,MAP_OFF_Y+MAP_RECT*(*it).ypos+MAP_RECT);
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

MapManager::MapManager(void)
{
	m_vMaps.clear();
	m_markUp = NULL;
	m_iCurrentLevel = 0;
}


MapManager::~MapManager(void)
{
	gSafeDelete(m_markUp);
	for (std::vector<Map*>::iterator it=m_vMaps.begin();it!=m_vMaps.end();it++)
	{
		gSafeDelete((*it));
	}
}

bool MapManager::LoadMaps(std::string path)
{
	if(m_markUp==NULL)
		m_markUp = new CMarkup;
	std::map<std::string,std::string> files;
	g_getFiles(path,files,".xml",LEVEL_NUM,true);
	
	for (std::map<std::string,std::string>::iterator mit=files.begin();mit!=files.end();mit++)
	{
		m_markUp->Load(mit->first);
		Map* level = new Map;
		int _level = 0;
		int _width = 0,_length = 0;
		int _ID = 0;
		int _Action = 0;
		int _xpos = 0;
		int _ypos = 0;
		Camp _camp = Neutral;
		int num = 0;
		m_markUp->ResetMainPos();
		if(m_markUp->FindElem("map"))
		{
			_level = atoi(m_markUp->GetAttrib("level").c_str());
			_width = atoi(m_markUp->GetAttrib("width").c_str());
			_length = atoi(m_markUp->GetAttrib("length").c_str());
			if(_level!=0 && _width!=0 && _length!=0)
			{
				level->SetLevel(_level);
				level->SetWidthLength(_width,_length);
			}
			else
				return false;
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
// 				setTerrain(_block.attri,_type);
// 				if (_type == HillTop || _type == CityWall)
// 				{
// 					setStandOn(_type,0);
// 				}
				_block.attri = _type;
				level->AddBlock(_block);
			}
			std::sort(level->GetVBlock().begin(),level->GetVBlock().end(),Block::less_than);
			m_markUp->OutOfElem();
		}
		m_vMaps.push_back(level);

		if(m_markUp->FindElem("Creature"))
		{
			m_markUp->IntoElem();
			while(m_markUp->FindElem("Man"))
			{
				_ID = atoi(m_markUp->GetAttrib("ID").c_str());
				_Action = atoi(m_markUp->GetAttrib("Action").c_str());
				_xpos = atoi(m_markUp->GetAttrib("xpos").c_str());
				_ypos = atoi(m_markUp->GetAttrib("ypos").c_str());
				_camp = (Camp)(atoi(m_markUp->GetAttrib("camp").c_str()));

				Character* cha = new Character;
				MapObject* mo = new MapObject;
				cha->Init(TexManager::sInstance().GetTex(_ID),level->GetLevel(),_ID,num,_Action,Block(_xpos,_ypos));
				//地图格子由于单位生成，属性变化移动到单位创建中完成
// 				Block* theBlock = level->GetBlock(_xpos,_ypos);
// 				if(theBlock != NULL)
// 					setOccupied((theBlock->attri),1);
				if (_camp == Friend)
				{
					CreatureManager::sInstance().AddFriend(cha);
				}
				else if (_camp == Enemy)
				{
					CreatureManager::sInstance().AddEnemy(cha);
				}
				num++;
			}
			m_markUp->OutOfElem();
		}	
	}

	//将所有地图文件按照level属性进行排序
	//load 完所有地图文件，设置当前关卡为第一关
	if(m_vMaps.empty())
		return false;
	std::sort(m_vMaps.begin(),m_vMaps.end(),Map::Less_than);
	SetCurrentLevel(m_vMaps[0]->GetLevel());
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
	if (oldLevel != m_iCurrentLevel)
	{
		currentMap = GetMap(m_iCurrentLevel);
		oldLevel = m_iCurrentLevel;
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
	if (oldLevel != m_iCurrentLevel)
	{
		currentMap = GetMap(m_iCurrentLevel);
		oldLevel = m_iCurrentLevel;
	}
	else
	{
		if(currentMap!=NULL)
			currentMap->Update();
	}
}