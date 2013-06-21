#include "CreatureManager.h"
#include "MapManager.h"
#include "App.h"
#include "TipWnd.h"

CreatureManager::CreatureManager()
{
	m_VFriendList.clear();
	m_VEnemyList.clear();
	m_nActionCreatureNum = -1;
	m_nSelectNum = 1;
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

	if(m_nSelectNum >= 0)
		ShowMoveRange();
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

void CreatureManager::ShowMoveRange()
{
	Character* selectChar = GetCreature(m_nSelectNum);
	if (selectChar)
	{
		int moveAbility = selectChar->GetMoveAbility();	
		Block charBlock = selectChar->GetBlock();
		Map* currentMap = MapManager::sInstance().GetCurrentMap();
		int mapWidth = 0,mapLength = 0;
		currentMap->GetWidthLength(mapWidth,mapLength);
		int offX = 0,offY = 0;
		DWORD color = 0;
		if(selectChar->GetCamp() == Friend)
			color = 0x4F3737DF;
		else if(selectChar->GetCamp() == Enemy)
			color = 0x4FEB2323;
		for (int i=charBlock.xpos-moveAbility;i<=charBlock.xpos+moveAbility;i++)
		{
			if(i >= 0 && i< mapWidth)
			{
				for (int j=charBlock.ypos-moveAbility;j<=charBlock.ypos+moveAbility;j++)
				{
					if (j >= 0 && j < mapLength)
					{
						offX = abs(i - charBlock.xpos);
						offY = abs(j - charBlock.ypos);
						if((offX + offY > moveAbility) || (i==charBlock.xpos && j==charBlock.ypos))
							continue;

						if (!currentMap->GetBlockOccupied(i,j))
						{
							//�������ʾ�����ƶ�
							App::sInstance().DrawSmallRect(Block(i,j),color);
						}
					}
				}
			}
		}
	}
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
		//�õ���λ����ʾ����Ϣ
		char temp[256] = {0};
		TipWnd::sInstance().Clear();
		sprintf(temp," ID   : %d",cha->GetID());
		TipWnd::sInstance().AddText(temp,0xFFFFFFFF,-1,-1,MSYaHei,FontMiddle);
		sprintf(temp," ���� : %d , %d",cha->GetBlock().xpos,cha->GetBlock().ypos);
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
	//��ǰ�е�λ�ж��У��ȴ����ж����
	if(m_nActionCreatureNum != -1)
		return;

	ResetAllCreature();

	Character* enemy = GetNextEnemy();
	if(enemy == NULL)
		return;
	m_nActionCreatureNum = enemy->GetNum();
	m_nSelectNum = m_nActionCreatureNum;
	int xMove = App::sInstance().GetHGE()->Random_Int(0,enemy->GetMoveAbility());
	int yMove = enemy->GetMoveAbility() - xMove;
	int xDir = (App::sInstance().GetHGE()->Random_Int(0,1)==0)?-1:1;
	int yDir = (App::sInstance().GetHGE()->Random_Int(0,1)==0)?-1:1;
	enemy->Move(enemy->GetBlock().xpos+xMove*xDir,enemy->GetBlock().ypos+yMove*yDir);
//	enemy->Move((Direction)(App::sInstance().GetHGE()->Random_Int(0,4)));
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
		if((*it)->GetFinish() == false)	//��δ�ж�
			return *it;
	}
	return NULL;	//�����˶��ж�������Ӧ�ȴ��¸��غ�
}

//���ݵ�ͼ��ĳ���ӵ�λ�ã�ȡ�øø����ϵĵз�
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

//���ݵ�ͼ��ĳ���ӵ�λ�ã�ȡ�øø����ϵ��ѷ�
Character* CreatureManager::GetFriend(int x,int  y)
{
	for (VCharacter::iterator it=m_VFriendList.begin();it!=m_VFriendList.end();it++)
	{
		if((*it)->GetBlock().xpos==x && (*it)->GetBlock().ypos==y)
			return *it;
	}
	return NULL;
}

Character* CreatureManager::GetFriend(int num)
{
	for (VCharacter::iterator it=m_VFriendList.begin();it!=m_VFriendList.end();it++)
	{
		if((*it)->GetNum() == num)
			return *it;
	}
	return NULL;
}

Character* CreatureManager::GetCreature(int x,int  y)
{
	Character* temp = GetEnemy(x,y);
	if(temp == NULL)
		temp = GetFriend(x,y);

	return temp;
}

Character* CreatureManager::GetCreature(int num)
{
	Character* temp = GetEnemy(num);
	if(temp == NULL)
		temp = GetFriend(num);

	return temp;
}

bool CreatureManager::ResetAllCreature()
{
	for (VCharacter::iterator it=m_VEnemyList.begin();it!=m_VEnemyList.end();it++)
	{
		if((*it)->GetFinish() == false)	//�ез���δ�ж�
			return false;
	}
	for (VCharacter::iterator it=m_VFriendList.begin();it!=m_VFriendList.end();it++)
	{
		if((*it)->GetFinish() == false)	//���ѷ���δ�ж�
			return false;
	}

	for (VCharacter::iterator it=m_VEnemyList.begin();it!=m_VEnemyList.end();it++)
		(*it)->SetFinish(false);
	for (VCharacter::iterator it=m_VFriendList.begin();it!=m_VFriendList.end();it++)
		(*it)->SetFinish(false);
	return true;
}