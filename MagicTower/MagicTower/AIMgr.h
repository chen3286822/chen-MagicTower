#ifndef AIMGR_H
#define AIMGR_H
#include "commonTools.h"


class Character;
class AIMgr
{
public:
	AIMgr();
	~AIMgr();

	void Init();
	//设置当前要进行AI行动的单位
	void SetCurCharacter(Character* cha);
	Character* GetCurCharacter();

	bool DoAction();
	bool SelectRoute(Character* target);
	int SelectDamageSkill(Character* target,POINT attackPoint,bool ignoreAttackPoint=false);		//选择对目标伤害最高的可施放技能，返回技能id，没有则返回-1
	int GetDamageToTarget(Character* target,int& attack,POINT attackPoint,bool ignoreAttackPoint=false);	//返回对目标造成的最高预期伤害,attack是-1则表示是普通攻击，其他值表示是技能伤害
	bool CanKillTarget(Character* target,int& attack,POINT attackPoint,bool ignoreAttackPoint=false);	//是否可以击杀单位，attack是-1则表示是普通攻击，其他值表示是技能伤害,ignoreAttackPoint为true则表示只是为了测试伤害够不够，而不考虑实际是否打得到目标

private:
	Character* m_pCurAI;
	eAIStrategy m_eStrategy;	//现行单位的策略
};


extern AIMgr g_AIMgr;
#endif