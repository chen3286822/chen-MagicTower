#include "CreatureManager.h"
#include "MapManager.h"
#include "App.h"
#include "TipWnd.h"

CreatureManager::CreatureManager()
{
	m_VFriendList.clear();
	m_VEnemyList.clear();
	m_nActionCreatureNum = -1;
}

CreatureManager::~CreatureManager()
{
	m_VFriendList.clear();
	m_VEnemyList.clear();
}

void CreatureManager::Render()
{
	for (VCharacter::iterator it=m_VEnemyList.begin();it!=m_VEnemyList.end();it++)
		(*it)->Render();
	for (VCharacter::iterator it=m_VFriendList.begin();it!=m_VFriendList.end();it++)
		(*it)->Render();
}

void CreatureManager::Update(float delta)
{
	for (VCharacter::iterator it=m_VEnemyList.begin();it!=m_VEnemyList.end();it++)
		(*it)->Update(delta);
	for (VCharacter::iterator it=m_VFriendList.begin();it!=m_VFriendList.end();it++)
		(*it)->Update(delta);

	if(m_nActionCreatureNum != -1)
	{
		Character* cha = GetEnemy(m_nActionCreatureNum);
		if(cha->GetFinish())
			m_nActionCreatureNum = -1;
	}
	ShowCreatureInfo();
}

void CreatureManager::ShowCreatureInfo()
{
	Block block = App::sInstance().GetMouseBlock();
	if (block.xpos!=-1 && block.ypos!=-1)
	{
		Character* cha = GetEnemy(block.xpos,block.ypos);
		if(cha == NULL)
		{
			cha = GetFriend(block.xpos,block.ypos);
			if(cha == NULL)
			{
				TipWnd::sInstance().Clear();
				TipWnd::sInstance().SetShow(false);
				return;
			}
		}
		//得到单位，显示其信息
		char temp[256] = {0};
		TipWnd::sInstance().Clear();
		sprintf(temp," ID   : %d",cha->GetID());
		TipWnd::sInstance().AddText(temp,0xFFFFFFFF,-1,-1,MSYaHei,FontMiddle);
		sprintf(temp," 坐标 : %d , %d",cha->GetBlock().xpos,cha->GetBlock().ypos);
		TipWnd::sInstance().AddText(temp,0xFFFFFFFF,-1,-1,MSYaHei,FontMiddle);
		TipWnd::sInstance().SetShow(true);
		TipWnd::sInstance().SetPos(block.xpos,block.ypos);
	}
	else
	{
		TipWnd::sInstance().Clear();
		TipWnd::sInstance().SetShow(false);
	}

}

void CreatureManager::Strategy()
{
	//当前有单位行动中，等待其行动完成
	if(m_nActionCreatureNum != -1)
		return;

	ResetAllCreature();

	Character* enemy = GetNextEnemy();
	if(enemy == NULL)
		return;
	m_nActionCreatureNum = enemy->GetNum();
	enemy->Move((Direction)(App::sInstance().GetHGE()->Random_Int(0,4)));
}

void CreatureManager::RemoveEnemy(Character* _enemy)
{
	for (VCharacter::iterator it=m_VEnemyList.begin();it!=m_VEnemyList.end();)
	{
		if((*it)->GetID() == _enemy->GetID() && (*it)->GetNum() == _enemy->GetNum())
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
		if((*it)->GetID() == _friend->GetID() && (*it)->GetNum() == _friend->GetNum())
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
		if((*it)->GetFinish() == false)	//尚未行动
			return *it;
	}
	return NULL;	//所有人都行动过，理应等待下个回合
}

//根据地图中某格子的位置，取得该格子上的敌方
Character* CreatureManager::GetEnemy(int x,int  y)
{
	for (VCharacter::iterator it=m_VEnemyList.begin();it!=m_VEnemyList.end();it++)
	{
		if((*it)->GetBlock().xpos==x && (*it)->GetBlock().ypos==y)
			return *it;
	}
	return NULL;
}

Character* CreatureManager::GetEnemy(int num)
{
	for (VCharacter::iterator it=m_VEnemyList.begin();it!=m_VEnemyList.end();it++)
	{
		if((*it)->GetNum() == num)
			return *it;
	}
	return NULL;
}

//根据地图中某格子的位置，取得该格子上的友方
Character* CreatureManager::GetFriend(int x,int  y)
{
	for (VCharacter::iterator it=m_VFriendList.begin();it!=m_VFriendList.end();it++)
	{
		if((*it)->GetBlock().xpos==x && (*it)->GetBlock().ypos==y)
			return *it;
	}
	return NULL;
}

bool CreatureManager::ResetAllCreature()
{
	for (VCharacter::iterator it=m_VEnemyList.begin();it!=m_VEnemyList.end();it++)
	{
		if((*it)->GetFinish() == false)	//有敌方尚未行动
			return false;
	}
	for (VCharacter::iterator it=m_VFriendList.begin();it!=m_VFriendList.end();it++)
	{
		if((*it)->GetFinish() == false)	//有友方尚未行动
			return false;
	}

	for (VCharacter::iterator it=m_VEnemyList.begin();it!=m_VEnemyList.end();it++)
		(*it)->SetFinish(false);
	for (VCharacter::iterator it=m_VFriendList.begin();it!=m_VFriendList.end();it++)
		(*it)->SetFinish(false);
	return true;
}