#pragma once
#include "commonTools.h"

class Map
{
public:
	Map();
	~Map();
	
	void Render();
	void Update();

	inline void AddObject(MapObject* mo){m_vObjList.push_back(mo);}
	inline void AddBlock(Block _block){m_vBlocks.push_back(_block);}
	inline void SetLevel(int _level){m_ilevel = _level;}
	inline int	GetLevel(){return m_ilevel;}
	inline void SetWidthLength(int _width,int _length){m_width = _width; m_length = _length;}
	inline void GetWidthLength(int& _width,int& _length){_width = m_width; _length = m_length;}

	//����λ�ڵ�ͼ��λ��ȡ�õ�ǰλ�õ�����͵�ͼ��
	MapObject* GetObject(int x,int y);
	Block* GetBlock(int x,int y);

	inline std::vector<MapObject*>& GetVObject(){return m_vObjList;}
	inline std::vector<Block>&	GetVBlock(){return m_vBlocks;}

	//Ϊ��֧��stl��sort�㷨
	static bool Less_than( Map* &m1, Map* &m2) {return m1->m_ilevel < m2->m_ilevel;}

private:
	std::vector<MapObject*> m_vObjList;	//�����ͼ���������,һЩ��̬����
	std::vector<Block> m_vBlocks; //�����ͼ�����и���
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
	Map* GetMap(int level);
	inline Map* GetCurrentMap(){return GetMap(m_iCurrentLevel);}
	void Render();
	void Update();

private:
	std::vector<Map*> m_vMaps;
	int m_iCurrentLevel;

	CMarkup*	m_markUp;

	inline void SetCurrentLevel(int _level){m_iCurrentLevel = _level;}
};

