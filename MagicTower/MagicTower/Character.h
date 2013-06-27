#pragma once
#include "commonTools.h"

/*
单位状态变化：
Stand --Move--> Walk ----> Stand
Stand --Attack--> Fight ----> Stand

当单位处于Stand时，其行动方向一定是None
当单位处于Walk时，其行动方向一定不是None

*/


//表示所有单位
class Character
{
public:
	Character(void);
	~Character(void);

	void Render();
	void Update(float delta); 

	void Init(int _Level,int _ID,int _Num,int _Action,Block _block);
	eErrorCode Move(int tarX,int tarY);	//向目标移动
	void Move(eDirection dir);		//以格子为单位移动

	void SetMove(bool _move){m_bCanMove = _move;}
	bool GetMove(){return m_bCanMove;}
	void SetFinish(bool _finish){m_bFinishAct = _finish;}
	bool GetFinish(){return m_bFinishAct;}
	Block&	GetBlock(){return m_iBlock;}
	Block& GetOrigBlock(){return m_iOrigBlock;}
	void	SetMoveAbility(int _ability){m_nMoveAbility = _ability;}
	int	GetMoveAbility(){return m_nMoveAbility;}
	void SetCamp(int _camp){m_nCamp = _camp;}
	int GetCamp(){return m_nCamp;}
	void SetAttackRange(eAttackRange _attackRange){m_eAttackRange = _attackRange;}
	eAttackRange GetAttackRange(){return m_eAttackRange;}
	void SetActionStage(eActionStage _stage){m_eActionStage = _stage;}
	eActionStage GetActionStage(){return m_eActionStage;}
	eCharacterState GetCharacterState(){return m_eCharState;}
	eDirection& GetCurDirection(){return m_eCurDir;}
	eDirection& GetOrigDirection(){return m_eOrigDir;}
	int GetID(){return m_nID;}
	int GetNum(){return m_nNum;}

	void CancelMove();	//取消移动，回到原来位置

	//战斗相关
	void SetTarget(int _tar){m_nTar = _tar;}
	int GetTarget(){return m_nTar;}
	void GeginHit();
	int TowardToAttacker(int src,int dir);	//面对着攻击者
	void Attack();	//开始攻击
	void Attacked();	//开始被攻击
	void Defend(); //开始防御
private:
	hgeAnimation* m_pAnimation;
	std::map<int,HTEXTURE> m_mCharTex; //存储单位的各个图片动作
	int m_nLevel;	//所在关卡
	int m_nID;		//代表单位类别的ID号
	int m_nNum;	//代表出现在当前关卡的单位号
	Block m_iOrigBlock;	//记录单位原来处于的格子，用于右键取消行动时返回原格子
	Block m_iBlock;//处于当前关卡的哪个格子中
	float m_fXPos,m_fYPos;	//单位的实际坐标
	float m_fStartX,m_fStartY;	//单位每次行动前的起点位置
	bool m_bCanMove;		//代表该单位是否可以移动
	int m_nMoveAbility;		//代表移动力
	bool m_bFinishAct;	//代表是否行动过
	int	m_nCamp;		//单位阵营
	eActionStage m_eActionStage;	//单位所处阶段，只有轮到该单位行动时才有效

	eCharacterState m_eCharState; //单位当前的状态 
	eDirection m_eCurDir;	//当前方向
	eDirection m_eOrigDir;	//原先方向，用于右键取消行动时恢复到原方向
	LDirection m_lPathDir;	//整个路径的移动方向
	int m_nLeftDistance;	//剩余需要移动的格子数

	eAttackRange m_eAttackRange;	//单位攻击范围类型
	int m_nSrc; //攻击自己的单位
	int m_nTar;	//攻击目标
	eAttackState m_eAttackState; //攻击子状态，用于处理攻击流程
	DWORD m_dwRecordTime;	//用于动作计时
};


typedef std::vector<Character*> VCharacter;
