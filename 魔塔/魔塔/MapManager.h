#pragma once
#include "commonTools.h"

class Map
{
public:
	Map();
	~Map();
	
	void render();
	void update();

	inline void addObject(MapObject* mo){m_vObjList.push_back(mo);}
	inline void addBlock(Block _block){m_vBlocks.push_back(_block);}
	inline void setLevel(int _level){m_ilevel = _level;}
	inline int	getLevel(){return m_ilevel;}
	inline void setWidthLength(int _width,int _length){m_width = _width; m_length = _length;}
	inline void getWidthLength(int& _width,int& _length){_width = m_width; _length = m_length;}

	//为了支持stl的sort算法
	static bool less_than( Map* &m1, Map* &m2) {return m1->m_ilevel < m2->m_ilevel;}

private:
	std::vector<MapObject*> m_vObjList;	//代表地图上所有单位
	std::vector<Block> m_vBlocks; //代表地图上所有格子
	int m_ilevel;
	int m_width;
	int m_length;

	hgeSprite* m_mapSpr;
};

class MapManager : public Singleton<MapManager>
{
public:
	MapManager(void);
	~MapManager(void);

	bool LoadMaps(std::string path);
	void render();
	void update();

private:
	std::vector<Map*> m_vMaps;
	int m_iCurrentLevel;

	CMarkup*	m_markUp;

	inline void setCurrentLevel(int _level){m_iCurrentLevel = _level;}
};

