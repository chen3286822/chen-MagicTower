#ifndef CREATURE_MANAGER_H
#define CREATURE_MANAGER_H

#include "commonTools.h"
#include "Character.h"
#include "ConfigManager.h"
#include "Scene.h"

struct Pair
{
	int x;
	int y;
	Pair()
	{
		x = 0;
		y = 0;
	}
	Pair(int _x,int _y)
	{
		x = _x;
		y = _y;
	}
};
typedef std::vector<Pair> VPair;
typedef std::map<eAttackRange,std::vector<int> > MAttackRange;
typedef std::map<eSkillRange,std::vector<int> > MSkillRange;

struct Items
{
	Item m_iItem;
	int m_nNum;			//物品数量
};
typedef std::list<Items> LItems;


class CreatureManager : public Singleton<CreatureManager>
{
public:
	CreatureManager();
	virtual ~CreatureManager();

	void Init();
	void Render();
	void Update(float delta);
	void Release();

	void AddEnemy(Character* _enemy){m_VEnemyList.push_back(_enemy);}
	void AddFriend(Character* _friend){m_VFriendList.push_back(_friend);}
	VCharacter::iterator RemoveEnemy(Character* _enemy);
	void RemoveFriend(Character* _friend);
	void ClearEnemy(){m_VEnemyList.clear();}
	void ClearFriend(){m_VFriendList.clear();}

	Character* GetNextEnemy();
	Character* GetEnemy(int x,int  y);
	Character* GetEnemy(int num);
	Character* GetFriend(int x,int  y);
	Character* GetFriend(int num);
	Character* GetCreature(int x,int  y);
	Character* GetCreature(int num);
	VCharacter& GetEnemy(){return m_VEnemyList;}
	VCharacter& GetFriend(){return m_VFriendList;}

	//取得当前回合信息
	eCampTurn GetCampTurn(){return m_eCampTurn;}
	//取得当前选中的单位号
	int& GetSelectNum(){return m_nSelectNum;}

	//是否单位死完
	bool IsEnemyAllDead();
	bool IsFriendAllDead();

	//显示该单位的相关信息
	void ShowCreatureInfo();
	//显示该单位移动范围
	void ShowMoveRange(Character* creature);
	//显示该单位攻击范围
	void ShowAttackRange(Character* creature);
	//显示技能释放范围
	void ShowSkillCastRange(Character* creature);
	//显示技能攻击范围
	void ShowSkillRange(int skillID);
	//显示物品释放范围
	void ShowItemCastRange(Character* creature);
	VPair GetRangePoint(){return m_vPair;}

	//只有当所有单位都行动过后才能重置，相当于开始下一回合
	bool ResetAllCreature();

	//AI行动策略
	void Strategy();
	int	GetCurrentActionCreature(){return m_nActionCreatureNum;}

	//单位间通信
	//包括攻击前通知、攻击类型通知、
	int Notify(int src,int tar,int messageID,int param);

	//提前计算攻击伤害
	void CalculateHurt(Character* cast,Character* target,bool bCrit=false);
	//计算攻击结果(命中与否)
	void CalculateResult(int src,int tar);
	//预先计算攻击并且推送动作
	void PreAttackAndPushAction(Character* cast,Character* target);
	//预先计算技能并且推送动作
	void PreSkillAndPushAction(Character* cast,Character* target);
	VCharacter&	GetSkillTargets(){return m_vSkillTargets;}
	

	//选中单位
	void SelectCreature();
	//取消选中
	void UnSelectCreature();
	//对于选中单位的处理
	void ProcessSelectCreature();
	//获得攻击类型范围
	MAttackRange& GetAttackRange(){return m_mAttackRange;}

	//物品管理，全局只维护一个物品包
	LItems& GetItems(){return m_lItems;}
	//添加一个物品
	void AddItem(int id,int num=1);
	//移除一个物品
	void RemoveItem(int id);
	//预先计算物品并且推送动作
	void PreItemAndPushAction(Character* cast,Character* target);

	//战斗剧情相关
	//暂停正常游戏流程
	void SetPause(bool pause){m_bPause = pause;}
	bool GetPause(){return m_bPause;}
	//添加剧情动作
	void AddAction(eAction action,int num,DWORD time=0,eDirection dir=eDirection_None,DWORD data=0,const char*name="",const char* word="");
	void ClearAction(){m_lActions.clear();}
	//处理动作
	void ProcessAction();
private:
	VCharacter  m_VFriendList;
	VCharacter	m_VEnemyList;
	VCharacter m_VFriendDeadList;
	VCharacter m_VEnemyDeadList;

	int m_nActionCreatureNum;	//表示当前正在行动中的单位号，没有表示为-1
	int m_nSelectNum;	//表示当前被选中的单位号

	VPair m_vPair;
	MAttackRange m_mAttackRange;
	MSkillRange m_mSkillRange;
	eCampTurn m_eCampTurn;	//当前所处回合
	VCharacter m_vSkillTargets;//技能释放目标

	LItems m_lItems;		//玩家存储的物品

	bool m_bPause;		//触发剧情，暂停流程

	std::list<NewAction> m_lActions;	//单位的战斗剧情动作列表
	eActionState m_eState; //控制列表
};




#endif