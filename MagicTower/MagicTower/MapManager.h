#pragma once
#include "commonTools.h"
#include "AStar.h"

class Map
{
public:
	Map();
	~Map();

	void Init();
	void Render();
	void Update();

	inline void AddObject(MapObject* mo){m_vObjList.push_back(mo);}
	inline void AddBlock(Block _block){m_vBlocks.push_back(_block);}
	inline void SetLevel(int _level){m_nlevel = _level;}
	inline int	GetLevel(){return m_nlevel;}
	inline void SetWidthLength(int _width,int _length){m_nWidth = _width; m_nLength = _length;}
	inline void GetWidthLength(int& _width,int& _length){_width = m_nWidth; _length = m_nLength;}

	//根据位于地图的位置取得当前位置的物件和地图块
	MapObject* GetObject(int x,int y);
	Block* GetBlock(int x,int y);

	inline std::vector<MapObject*>& GetVObject(){return m_vObjList;}
	inline std::vector<Block>&	GetVBlock(){return m_vBlocks;}

	std::vector<Block*> FindPath(int startX,int startY,int endX,int endY);
	void SetSpecificRange(std::vector<Block*>);
	std::vector<Block*>& GetSpecificRange(){return m_vSpecificRange;}

	void SetBlockOccupied(int xpos,int ypos);
	bool GetBlockOccupied(int xpos,int ypos);

	//为了支持stl的sort算法
	static bool Less_than( Map* &m1, Map* &m2) {return m1->m_nlevel < m2->m_nlevel;}

private:
	std::vector<MapObject*> m_vObjList;	//代表地图上所有物件,一些静态物体
	std::vector<Block> m_vBlocks; //代表地图上所有格子
	int m_nlevel;
	int m_nWidth;
	int m_nLength;

	hgeSprite* m_pMapSpr;
	AStar m_iPathFinder;
	std::vector<Block*>	m_vSpecificRange;	//限定的寻路范围 
};

class MapManager : public Singleton<MapManager>
{
public:
	MapManager(void);
	~MapManager(void);

	bool LoadMaps();
	Map* GetMap(int level);
	inline Map* GetCurrentMap(){return GetMap(m_nCurrentLevel);}
	void Render();
	void Update();

private:
	std::vector<Map*> m_vMaps;
	int m_nCurrentLevel;

	CMarkup*	m_pMarkUp;

	inline void SetCurrentLevel(int _level){m_nCurrentLevel = _level;}
};

