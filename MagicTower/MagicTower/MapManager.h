#pragma once
#include "commonTools.h"
#include "AStar.h"

//������
struct Trigger
{
	eTrigger m_eTrigger;
	int m_nTurns;
	int m_nNum1;	//��Ϊ�ص㴥����num���Ӵ���ɱ������num1
	int m_nNum2;	//��Ϊ��ɱ�Ӵ�������num2
	int m_nPosX;
	int m_nPosY;
	bool m_bEffective;	//�Ƿ���Ч
	string m_strFunc;	//����ʱ���õĺ�����
};

//ʤ����������
struct Victory
{
	eVictoryCondition m_eCondition;	//ʤ������
	std::vector<int> m_vData;		//�����ض���λ�洢�ĵ�λNum�����ߵ����ض�����洢�����꣬��x��y�ߵ��ֽ����
	int m_nNum;		//�����ض������������ض���λ��
};

//ʧ����������
struct Failed
{
	eFailCondition m_eConditoin;	//ʧ������
	std::vector<int> m_vNum;	//����ʧ�ܵĵ�λ��
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

	//����λ�ڵ�ͼ��λ��ȡ�õ�ǰλ�õ�����͵�ͼ��
	MapObject* GetObject(int x,int y);
	Block* GetBlock(int x,int y);

	inline std::vector<MapObject*>& GetVObject(){return m_vObjList;}
	inline std::vector<Block>&	GetVBlock(){return m_vBlocks;}

	std::vector<Block*> FindPath(int startX,int startY,int endX,int endY);
	void SetSpecificRange(std::vector<Block*>);
	std::vector<Block*>& GetSpecificRange(){return m_vSpecificRange;}

	void SetBlockOccupied(int xpos,int ypos);
	bool GetBlockOccupied(int xpos,int ypos);

	bool GetCreatureStandOn(int xpos,int ypos);	//�Ƿ��е�λ�����棬�൱��ȥ���˸��ӱ�����ռ�õ�����

	//Ϊ��֧��stl��sort�㷨
	static bool Less_than( Map* &m1, Map* &m2) {return m1->m_nlevel < m2->m_nlevel;}

	//ʤ������
	void SetVictoryCondition(int condition,int data=-1,int num=-1);	//����ʤ������������ʤ����Ϣ������������ε��ã�����һ�£��򸽴���Ϣ��ϲ�,numָ�ض���λ��
	Victory GetVictoryCondition(){return m_iVictory;}
	void SetVictory(bool bVictory){m_bVictory = bVictory;}
	bool GetVictory(){return m_bVictory;}
	bool CheckVictory(eVictoryCondition condition,int data);		//����Ƿ�ʤ��
	//ʧ������
	void SetFailCondition(int condition,int num);	//����ʧ��������numΪ����ʧ�ܵĵ�λ��
	Failed GetFailCondition(){return m_iFailed;}
	void SetFailed(bool bFailed){m_bFailed = bFailed;}
	bool GetFailed(){return m_bFailed;}
	bool CheckFail(eFailCondition condition,int num);	//����Ƿ�ʧ��


	//���������
	//��Ӵ�������������������Ҫ����Ϊ-1
	void AddTrigger(int triggerID,const char* func,int turns,int num1,int num2,int x,int y);
	void ClearTrigger();
	bool IsTriggerTurns(int turns);
	bool IsTriggerLocation(int num);
	bool IsTriggerTouch(int num);
	bool IsTriggerKill(int num);

private:
	std::vector<MapObject*> m_vObjList;	//�����ͼ���������,һЩ��̬����
	std::vector<Block> m_vBlocks; //�����ͼ�����и���
	int m_nlevel;
	int m_nWidth;
	int m_nLength;
	int m_nTurns;		//���ƻغ���
	int m_nCurTurn;	//��ǰ�غ���
	bool m_bShowTurns;	//�Ƿ���ʾ��ǰ�غ���
	int m_nShowTurnTime;	//��ʾ�غ����ĳ���ʱ��
	bool m_bShowCampTurn;	//�Ƿ���ʾ��Ӫ�غ�
	int m_nShowCampTurnTime;	//��ʾ��Ӫ�ĳ���ʱ��
	eCampTurn m_eCurCampTurn;	//��ǰ��Ӫ

	hgeSprite* m_pMapSpr;
	int m_nOffX;	//��ǰ��ʾ�ĵ�ͼ����ƫ��
	int m_nOffY;	//��ǰ��ʾ�ĵ�ͼ����ƫ��
	DWORD m_dwMapMoveTime[4];	//ͨ������ƶ���ͼ�ĸ�����ʱ��ʱ��������ֹ��ͼһ���ƶ�����
	AStar m_iPathFinder;
	std::vector<Block*>	m_vSpecificRange;	//�޶���Ѱ·��Χ 

	Victory m_iVictory;		//���ص�ʤ������
	bool m_bVictory;			//�Ƿ��Ѿ�ʤ��
	bool m_bFailed;			//�Ƿ��Ѿ�ʧ��
	Failed m_iFailed;			//���ص�ʧ������

	std::vector<Trigger> m_vTriggers;	//����ͼ�����Ĵ�����
};

class MapManager : public Singleton<MapManager>
{
public:
	MapManager(void);
	virtual ~MapManager(void);

	//�˺��������⣬��ʱ��Ӧʹ��
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

