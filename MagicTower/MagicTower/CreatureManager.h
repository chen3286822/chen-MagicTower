#ifndef CREATURE_MANAGER_H
#define CREATURE_MANAGER_H

#include "commonTools.h"
#include "Character.h"

class CreatureManager : public Singleton<CreatureManager>
{
public:
	CreatureManager();
	~CreatureManager();

	void Render();
	void Update(float delta);

	void AddEnemy(Character* _enemy){m_VEnemyList.push_back(_enemy);}
	void AddFriend(Character* _friend){m_VFriendList.push_back(_friend);}
	void RemoveEnemy(Character* _enemy);
	void RemoveFriend(Character* _friend);
	void ClearEnemy(){m_VEnemyList.clear();}
	void ClearFriend(){m_VFriendList.clear();}

	Character* GetNextEnemy();
	Character* GetEnemy(int x,int  y);
	Character* GetEnemy(int num);
	Character* GetFriend(int x,int  y);

	//显示该单位的相关信息
	void ShowCreatureInfo();

	//只有当所有单位都行动过后才能重置，相当于开始下一回合
	bool ResetAllCreature();

	//AI行动策略
	void Strategy();
	int	GetCurrentActionCreature(){return m_ActionCreatureNum;}

private:
	VCharacter  m_VFriendList;
	VCharacter	m_VEnemyList;

	int m_ActionCreatureNum;	//表示当前正在行动中的单位号，没有表示为-1
};




#endif