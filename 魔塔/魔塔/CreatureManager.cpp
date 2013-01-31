#include "CreatureManager.h"
#include "MapManager.h"

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
	for (VCharacter::iterator it=m_VEnemyList.begin();it!=m_VEnemyList.end();it++)
	{
		if((*it)->getFinish() == false)	//尚未行动
			return *it;
	}
	return NULL;	//所有人都行动过，理应等待下个回合
}

//根据地图中某格子的位置，取得该格子上的敌方
Character* CreatureManager::GetEnemy(int x,int  y)
{
	for (VCharacter::iterator it=m_VEnemyList.begin();it!=m_VEnemyList.end();it++)
	{
		if((*it)->getBlock().xpos==x && (*it)->getBlock().ypos==y)
			return *it;
	}
	return NULL;
}

//根据地图中某格子的位置，取得该格子上的友方
Character* CreatureManager::GetFriend(int x,int  y)
{
	for (VCharacter::iterator it=m_VFriendList.begin();it!=m_VFriendList.end();it++)
	{
		if((*it)->getBlock().xpos==x && (*it)->getBlock().ypos==y)
			return *it;
	}
	return NULL;
}