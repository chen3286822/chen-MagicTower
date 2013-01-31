#include "CreatureManager.h"

CreatureManager::CreatureManager()
{
	m_VFriendList.clear();
	m_VEnemyList.clear();
}

CreatureManager::~CreatureManager()
{
	m_VFriendList.clear();
	m_VEnemyList.clear();
}

void CreatureManager::RemoveEnemy(Character* _enemy)
{
	for (VCharacter::iterator it=m_VEnemyList.begin();it!=m_VEnemyList.end();)
	{
		if((*it)->getID() == _enemy->getID() && (*it)->getNum() == _enemy->getNum())
		{
			m_VEnemyList.erase(it);
			return;
		}
		else
			it++;
	}
}

void CreatureManager::RemoveFriend(Character* _friend)
{
	for (VCharacter::iterator it=m_VFriendList.begin();it!=m_VFriendList.end();)
	{
		if((*it)->getID() == _friend->getID() && (*it)->getNum() == _friend->getNum())
		{
			m_VFriendList.erase(it);
			return;
		}
		else
			it++;
	}
}

Character* CreatureManager::GetNextEnemy()
{

}

Character* CreatureManager::GetFriend(int x,int  y)
{

}