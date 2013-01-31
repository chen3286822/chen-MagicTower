#ifndef CREATURE_MANAGER_H
#define CREATURE_MANAGER_H

#include "Singleton.h"
#include "Character.h"

class CreatureManager : public Singleton<CreatureManager>
{
public:
	CreatureManager();
	~CreatureManager();

	void AddEnemy(Character* _enemy){m_VEnemyList.push_back(_enemy);}
	void AddFriend(Character* _friend){m_VFriendList.push_back(_friend);}
	void RemoveEnemy(Character* _enemy);
	void RemoveFriend(Character* _friend);
	void ClearEnemy(){m_VEnemyList.clear();}
	void ClearFriend(){m_VFriendList.clear();}

	Character* GetNextEnemy();
	Character* GetFriend(int x,int  y);

private:
	VCharacter  m_VFriendList;
	VCharacter	m_VEnemyList;
};




#endif