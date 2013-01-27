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
private:
	hgeAnimation* m_ani;
	int m_ID;		//代表单位类别的ID号
	int m_Num;	//代表出现在当前关卡的单位号
	Block m_block;//处于当前关卡的哪个格子中
	int m_xpos,m_ypos;	//处于当前关卡的哪个格子中
	bool m_bCanMove;		//代表该单位是否可以移动
	int m_MoveAbility;		//代表移动力
//	std::vector<Block>
};

