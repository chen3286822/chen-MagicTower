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
	void Move(Direction dir);		//以格子为单位移动

	void SetMove(bool _move){m_bCanMove = _move;}
	bool GetMove(){return m_bCanMove;}
	void SetFinish(bool _finish){m_bFinishAct = _finish;}
	bool GetFinish(){return m_bFinishAct;}
	Block&	GetBlock(){return m_block;}

	int GetID(){return m_ID;}
	int GetNum(){return m_Num;}
private:
	hgeAnimation* m_ani;
	int m_Level;	//所在关卡
	int m_ID;		//代表单位类别的ID号
	int m_Num;	//代表出现在当前关卡的单位号
	Block m_block;//处于当前关卡的哪个格子中
	float m_xpos,m_ypos;	//单位的实际坐标
	float m_StartX,m_StartY;	//单位每次行动前的起点位置
	bool m_bCanMove;		//代表该单位是否可以移动
	int m_MoveAbility;		//代表移动力
	bool m_bFinishAct;	//代表是否行动过
//	std::vector<Block>

	Direction m_MoveDir;	//移动的方向
	int m_LeftDistance;	//剩余需要移动的格子数
};


typedef std::vector<Character*> VCharacter;
