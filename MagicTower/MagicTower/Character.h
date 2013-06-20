#pragma once
#include "commonTools.h"


//表示所有单位
class Character
{
public:
	Character(void);
	~Character(void);

	void Render();
	void Update(float delta); 

	void Init(HTEXTURE tex,int _Level,int _ID,int _Num,int _Action,Block _block);
	void Move(int tarX,int tarY);	//向目标移动
	void Move(Direction dir);		//以格子为单位移动

	void SetMove(bool _move){m_bCanMove = _move;}
	bool GetMove(){return m_bCanMove;}
	void SetFinish(bool _finish){m_bFinishAct = _finish;}
	bool GetFinish(){return m_bFinishAct;}
	Block&	GetBlock(){return m_iBlock;}
	void	SetMoveAbility(int _ability){m_nMoveAbility = _ability;}
	int	GetMoveAbility(){return m_nMoveAbility;}
	void SetCamp(int _camp){m_nCamp = _camp;}
	int GetCamp(){return m_nCamp;}

	int GetID(){return m_nID;}
	int GetNum(){return m_nNum;}
private:
	hgeAnimation* m_pAnimation;
	int m_nLevel;	//所在关卡
	int m_nID;		//代表单位类别的ID号
	int m_nNum;	//代表出现在当前关卡的单位号
	Block m_iBlock;//处于当前关卡的哪个格子中
	float m_fXPos,m_fYPos;	//单位的实际坐标
	float m_fStartX,m_fStartY;	//单位每次行动前的起点位置
	bool m_bCanMove;		//代表该单位是否可以移动
	int m_nMoveAbility;		//代表移动力
	bool m_bFinishAct;	//代表是否行动过
	int	m_nCamp;		//单位阵营

	Direction m_eCurMoveDir;	//当前移动的方向
	VDirection m_vPathDir;	//整个路径的移动方向
	int m_nLeftDistance;	//剩余需要移动的格子数
};


typedef std::vector<Character*> VCharacter;
