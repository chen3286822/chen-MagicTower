#pragma once
#include "commonTools.h"


//表示所有单位
class Character
{
public:
	Character(void);
	~Character(void);

	void render();
	void update(); 

	void init(HTEXTURE tex,int _ID,int _Num,Block _block);
	void move(Direction dir);		//以格子为单位移动

	void setMove(bool _move){m_bCanMove = _move;}
	bool getMove(){return m_bCanMove;}
private:
	hgeAnimation* m_ani;
	int m_ID;		//代表单位类别的ID号
	int m_Num;	//代表出现在当前关卡的单位号
	Block m_block;//处于当前关卡的哪个格子中
	float m_xpos,m_ypos;	//单位的实际坐标
	bool m_bCanMove;		//代表该单位是否可以移动
	int m_MoveAbility;		//代表移动力
//	std::vector<Block>

	Direction m_MoveDir;	//移动的方向
	int m_LeftDistance;	//剩余需要移动的格子数
};

