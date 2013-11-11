#pragma once
#include "commonTools.h"
#include "AStar.h"

//触发器
struct Trigger
{
	eTrigger m_eTrigger;
	int m_nTurns;
	int m_nNum1;	//作为地点触发的num，接触击杀触发的num1
	int m_nNum2;	//作为击杀接触触发的num2
	int m_nPosX;
	int m_nPosY;
	bool m_bEffective;	//是否有效
	string m_strFunc;	//触发时调用的函数名
};

//胜利条件描述
struct Victory
{
	eVictoryCondition m_eCondition;	//胜利类型
	std::vector<int> m_vData;		//击败特定单位存储的单位Num，或者到达特定区域存储的坐标，用x，y高低字节组合
	int m_nNum;		//到达特定区域条件的特定单位号
};

//失败条件描述
struct Failed
{
	eFailCondition m_eConditoin;	//失败类型
	std::vector<int> m_vNum;	//导致失败的单位号
};

class Map
{
public:
	Map();
	~Map();

	void Init();
	void Release();
	void Render();
	void Update(float dt);

	void SetMapTex(int level);
	inline int GetOffX(){return m_nOffX;}
	inline int GetOffY(){return m_nOffY;}
	void SetOffXY(int x,int y);
	RECT GetMapRect();
	inline void AddObject(MapObject* mo){m_vObjList.push_back(mo);}
	inline void AddBlock(Block _block){m_vBlocks.push_back(_block);}
	inline void SetLevel(int _level){m_nlevel = _level;}
	inline int	GetLevel(){return m_nlevel;}
	inline void SetWidthLength(int _width,int _length){m_nWidth = _width; m_nLength = _length;}
	inline void GetWidthLength(int& _width,int& _length){_width = m_nWidth; _length = m_nLength;}
	inline void SetTurns(int turn){m_nTurns = turn;}
	inline int&	GetTurns(){return m_nTurns;}
	inline int&	GetCurrentTurn(){return m_nCurTurn;}

	void RenderTitle();
	void AddTurn();
	void GoIntoTurn(eCampTurn turn);
	bool IsShowTitle();

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

	bool GetCreatureStandOn(int xpos,int ypos);	//是否有单位在上面，相当于去除了格子本身不可占用的条件

	//为了支持stl的sort算法
	static bool Less_than( Map* &m1, Map* &m2) {return m1->m_nlevel < m2->m_nlevel;}

	//胜利条件
	void SetVictoryCondition(int condition,int data=-1,int num=-1);	//设置胜利条件，附带胜利信息，如果连续两次调用，类型一致，则附带信息会合并,num指特定单位号
	Victory GetVictoryCondition(){return m_iVictory;}
	void SetVictory(bool bVictory){m_bVictory = bVictory;}
	bool GetVictory(){return m_bVictory;}
	bool CheckVictory(eVictoryCondition condition,int data);		//检查是否胜利
	//失败条件
	void SetFailCondition(int condition,int num);	//设置失败条件，num为导致失败的单位号
	Failed GetFailCondition(){return m_iFailed;}
	void SetFailed(bool bFailed){m_bFailed = bFailed;}
	bool GetFailed(){return m_bFailed;}
	bool CheckFail(eFailCondition condition,int num);	//检查是否失败


	//触发器相关
	//添加触发器，触发条件不需要的设为-1
	void AddTrigger(int triggerID,const char* func,int turns,int num1,int num2,int x,int y);
	void ClearTrigger();
	bool IsTriggerTurns(int turns);
	bool IsTriggerLocation(int num);
	bool IsTriggerTouch(int num);
	bool IsTriggerKill(int num);

private:
	std::vector<MapObject*> m_vObjList;	//代表地图上所有物件,一些静态物体
	std::vector<Block> m_vBlocks; //代表地图上所有格子
	int m_nlevel;
	int m_nWidth;
	int m_nLength;
	int m_nTurns;		//限制回合数
	int m_nCurTurn;	//当前回合数
	bool m_bShowTurns;	//是否显示当前回合数
	int m_nShowTurnTime;	//显示回合数的持续时间
	bool m_bShowCampTurn;	//是否显示阵营回合
	int m_nShowCampTurnTime;	//显示阵营的持续时间
	eCampTurn m_eCurCampTurn;	//当前阵营

	hgeSprite* m_pMapSpr;
	int m_nOffX;	//当前显示的地图格子偏移
	int m_nOffY;	//当前显示的地图格子偏移
	DWORD m_dwMapMoveTime[4];	//通过鼠标移动地图四个方向时的时间间隔，防止地图一下移动过多
	AStar m_iPathFinder;
	std::vector<Block*>	m_vSpecificRange;	//限定的寻路范围 

	Victory m_iVictory;		//本关的胜利条件
	bool m_bVictory;			//是否已经胜利
	bool m_bFailed;			//是否已经失败
	Failed m_iFailed;			//本关的失败条件

	std::vector<Trigger> m_vTriggers;	//本地图包含的触发器
};

class MapManager : public Singleton<MapManager>
{
public:
	MapManager(void);
	virtual ~MapManager(void);

	//此函数有问题，暂时不应使用
	bool LoadMaps();

	bool LoadMap(int level);
	Map* GetMap(int level);
	inline void SetCurrentLevel(int _level){m_nCurrentLevel = _level;}
	inline Map* GetCurrentMap(){return GetMap(m_nCurrentLevel);}
	void Render();
	void Update(float dt);
	void Release();

private:
	std::vector<Map*> m_vMaps;
	int m_nCurrentLevel;

	CMarkup*	m_pMarkUp;


};

