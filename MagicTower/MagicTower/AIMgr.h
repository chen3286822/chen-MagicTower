#ifndef AIMGR_H
#define AIMGR_H
#include "commonTools.h"

class Character;
bool DamageCompare(Character* cha1,Character* cha2);
class AIMgr
{
public:
	AIMgr();
	~AIMgr();

	void Init();
	//设置当前要进行AI行动的单位
	void SetCurCharacter(Character* cha);
	Character* GetCurCharacter();

	//AI主循环
	bool DoAction();
	//选择一条向目标移动的路径并执行移动
	bool SelectRoute(Character* target);

	//以下函数给出cast，是为了让这些函数作为通用函数使用，即无需设定AI单位
	//选择对目标伤害最高的可施放技能，返回技能id，没有则返回-1
	int SelectDamageSkill(Character* cast,Character* target,POINT attackPoint,bool ignoreAttackPoint=false);		
	//返回对目标造成的最高预期伤害,attack是-1则表示是普通攻击，其他值表示是技能伤害
	int GetDamageToTarget(Character* cast,Character* target,int& attack,POINT attackPoint,bool ignoreAttackPoint=false);	
	//是否可以击杀单位，attack是-1则表示是普通攻击，其他值表示是技能伤害,ignoreAttackPoint为true则表示只是为了测试伤害够不够，而不考虑实际是否打得到目标
	bool CanKillTarget(Character* cast,Character* target,int& attack,POINT attackPoint,bool ignoreAttackPoint=false);	
	//测试单位在其移动范围内是否能打到目标点或是另一个单位,pt 返回可以击中时cast的位置
	bool CanHitPointOrTarget(Character* cast,Character* target,Block* point,POINT& pt);
private:
	Character* m_pCurAI;
	AIStrategy m_iStrategy;	//现行单位的策略

	bool StrategyAttackTarget();
};


extern AIMgr g_AIMgr;
#endif