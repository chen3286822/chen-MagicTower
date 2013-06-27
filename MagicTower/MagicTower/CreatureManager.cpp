#include "CreatureManager.h"
#include "MapManager.h"
#include "App.h"
#include "TipWnd.h"

CreatureManager::CreatureManager()
{
	m_VFriendList.clear();
	m_VEnemyList.clear();
	m_nActionCreatureNum = -1;
	m_nSelectNum = -1;
}

CreatureManager::~CreatureManager()
{
	m_VFriendList.clear();
	m_VEnemyList.clear();
}

void CreatureManager::Init()
{
	m_mAttackRange[eAttackRange_Cross].push_back(Pair(0,-1));
	m_mAttackRange[eAttackRange_Cross].push_back(Pair(0,1));
	m_mAttackRange[eAttackRange_Cross].push_back(Pair(-1,0));
	m_mAttackRange[eAttackRange_Cross].push_back(Pair(1,0));

	m_mAttackRange[eAttackRange_Box].push_back(Pair(1,0));
	m_mAttackRange[eAttackRange_Box].push_back(Pair(0,1));
	m_mAttackRange[eAttackRange_Box].push_back(Pair(-1,0));
	m_mAttackRange[eAttackRange_Box].push_back(Pair(0,-1));
	m_mAttackRange[eAttackRange_Box].push_back(Pair(-1,-1));
	m_mAttackRange[eAttackRange_Box].push_back(Pair(-1,1));
	m_mAttackRange[eAttackRange_Box].push_back(Pair(1,-1));
	m_mAttackRange[eAttackRange_Box].push_back(Pair(1,1));

	m_mAttackRange[eAttackRange_BigCross].push_back(Pair(0,-1));
	m_mAttackRange[eAttackRange_BigCross].push_back(Pair(0,1));
	m_mAttackRange[eAttackRange_BigCross].push_back(Pair(-1,0));
	m_mAttackRange[eAttackRange_BigCross].push_back(Pair(1,0));
	m_mAttackRange[eAttackRange_BigCross].push_back(Pair(0,-2));
	m_mAttackRange[eAttackRange_BigCross].push_back(Pair(0,2));
	m_mAttackRange[eAttackRange_BigCross].push_back(Pair(-2,0));
	m_mAttackRange[eAttackRange_BigCross].push_back(Pair(2,0));

	m_mAttackRange[eAttackRange_Arrow].push_back(Pair(0,-2));
	m_mAttackRange[eAttackRange_Arrow].push_back(Pair(0,2));
	m_mAttackRange[eAttackRange_Arrow].push_back(Pair(-2,0));
	m_mAttackRange[eAttackRange_Arrow].push_back(Pair(2,0));
	m_mAttackRange[eAttackRange_Arrow].push_back(Pair(-1,-1));
	m_mAttackRange[eAttackRange_Arrow].push_back(Pair(-1,1));
	m_mAttackRange[eAttackRange_Arrow].push_back(Pair(1,-1));
	m_mAttackRange[eAttackRange_Arrow].push_back(Pair(1,1));

	m_eCampTurn = eCampTurn_Friend;
}

void CreatureManager::Release()
{
	for (VCharacter::iterator it=m_VEnemyList.begin();it!=m_VEnemyList.end();it++)
		gSafeDelete(*it);
	for (VCharacter::iterator it=m_VFriendList.begin();it!=m_VFriendList.end();it++)
		gSafeDelete(*it);
}

void CreatureManager::Render()
{
	ProcessSelectCreature();

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

		//测试用
		if (cha->GetCamp() == eCamp_Enemy && cha->GetActionStage() == eActionStage_AttackStage)
		{
			m_nActionCreatureNum = -1;
			cha->SetFinish(true);
		}
	}

	SelectCreature();
	UnSelectCreature();
	ShowCreatureInfo();
}

void CreatureManager::ShowMoveRange(Character* creature)
{
	if (creature)
	{
		int moveAbility = creature->GetMoveAbility();	
		Block charBlock = creature->GetBlock();
		Map* currentMap = MapManager::sInstance().GetCurrentMap();
		int mapWidth = 0,mapLength = 0;
		currentMap->GetWidthLength(mapWidth,mapLength);
		int offX = 0,offY = 0;
		DWORD color = 0;
		if(creature->GetCamp() == eCamp_Friend)
			color = 0x4F3737DF;
		else if(creature->GetCamp() == eCamp_Enemy)
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
							//画方格表示可以移动
							App::sInstance().DrawSmallRect(Block(i,j),color);
						}
					}
				}
			}
		}
	}
}

void CreatureManager::ShowAttackRange(Character* creature)
{
	if (creature)
	{
		eAttackRange attackRange = creature->GetAttackRange();	
		Block charBlock = creature->GetBlock();
		Map* currentMap = MapManager::sInstance().GetCurrentMap();
		int mapWidth = 0,mapLength = 0;
		currentMap->GetWidthLength(mapWidth,mapLength);
		int offX = 0,offY = 0;

		for (MAttackRange::iterator mit=m_mAttackRange.begin();mit!=m_mAttackRange.end();mit++)
		{
			if(mit->first == attackRange)
			{
				for (vector<Pair>::iterator it=mit->second.begin();it!=mit->second.end();it++)
				{
					offX = it->x + charBlock.xpos;
					offY = it->y + charBlock.ypos;
					if (offX >= 0 && offX < mapWidth && offY >= 0 && offY < mapLength)
					{
						App::sInstance().DrawBox(Block(offX,offY),0xBFFF0000);
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
		//得到单位，显示其信息
		char temp[256] = {0};
		TipWnd::sInstance().Clear();
		sprintf(temp," ID   : %d",cha->GetID());
		TipWnd::sInstance().AddText(temp,0xFFFFFFFF,-1,-1,eFontType_MSYaHei,eFontSize_FontMiddle);
		sprintf(temp," 坐标 : %d , %d",cha->GetBlock().xpos,cha->GetBlock().ypos);
		TipWnd::sInstance().AddText(temp,0xFFFFFFFF,-1,-1,eFontType_MSYaHei,eFontSize_FontMiddle);
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

	if (m_eCampTurn == eCampTurn_Enemy)
	{
		Character* enemy = GetNextEnemy();
		if(enemy == NULL)
			return;
		if(enemy->GetActionStage() == eActionStage_WaitStage)
		{
			enemy->SetActionStage(eActionStage_MoveStage);
			m_nActionCreatureNum = enemy->GetNum();
			//m_nSelectNum = m_nActionCreatureNum;
			int xMove = App::sInstance().GetHGE()->Random_Int(0,enemy->GetMoveAbility());
			int yMove = enemy->GetMoveAbility() - xMove;
			int xDir = (App::sInstance().GetHGE()->Random_Int(0,1)==0)?-1:1;
			int yDir = (App::sInstance().GetHGE()->Random_Int(0,1)==0)?-1:1;
			eErrorCode errorCode = enemy->Move(enemy->GetBlock().xpos+xMove*xDir,enemy->GetBlock().ypos+yMove*yDir);

			//测试
			if (errorCode != eErrorCode_Success)
			{
				enemy->SetFinish(true);
			}
		}

	}
//	enemy->Move((Direction)(App::sInstance().GetHGE()->Random_Int(0,4)));
}

void CreatureManager::RemoveEnemy(Character* _enemy)
{
	for (VCharacter::iterator it=m_VEnemyList.begin();it!=m_VEnemyList.end();)
	{
		if((*it)->GetID() == _enemy->GetID() && (*it)->GetNum() == _enemy->GetNum())
		{
			gSafeDelete(*it);
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
			gSafeDelete(*it);
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
	for (VCharacter::iterator it=m_VFriendList.begin();it!=m_VFriendList.end();it++)
	{
		if((*it)->GetFinish() == false)	//有友方尚未行动
			return false;
	}
	//友方行动完，进入敌方回合
	if (m_eCampTurn == eCampTurn_Friend)
		m_eCampTurn = eCampTurn_Enemy;

	for (VCharacter::iterator it=m_VEnemyList.begin();it!=m_VEnemyList.end();it++)
	{
		if((*it)->GetFinish() == false)	//有敌方尚未行动
			return false;
	}
	//敌方行动完，进入下一回合
	if (m_eCampTurn == eCampTurn_Enemy)
		m_eCampTurn = eCampTurn_Friend;
	

	for (VCharacter::iterator it=m_VEnemyList.begin();it!=m_VEnemyList.end();it++)
	{
			(*it)->SetFinish(false);
			(*it)->SetActionStage(eActionStage_WaitStage);
	}
	for (VCharacter::iterator it=m_VFriendList.begin();it!=m_VFriendList.end();it++)
	{
		(*it)->SetFinish(false);
		(*it)->SetActionStage(eActionStage_WaitStage);
	}
	return true;
}

int CreatureManager::Notify(int src,int tar,int messageID,int param)
{
	int result = -1;
	Character* target = GetCreature(tar);
	if (!target)
		result = eNotify_NoTarget;

	switch(messageID)
	{
	case eNotify_TowardToAttacker:
		result = target->TowardToAttacker(src,param);
		break;
	case eNotify_ReadyToBeAttacked:
		{
			target->Attack();
			result = eNotify_Success;
		}
		break;
	default:
		break;
	}

	return result;
}

void CreatureManager::CalculateResult(int src,int tar)
{
	//测试，让target 受伤害
	Character* target = GetCreature(tar);
	target->Attacked();
}

void CreatureManager::SelectCreature()
{
	//友方回合才可以进行选择操作，包括查看敌方行动范围
	if (m_eCampTurn == eCampTurn_Friend)
	{
		Block mouseBlock = App::sInstance().GetMouseBlock();
		Character* selectChar = NULL;
		if (mouseBlock.xpos!=-1 && mouseBlock.ypos!=-1)
		{
			selectChar = GetCreature(mouseBlock.xpos,mouseBlock.ypos);
			//选中单位
			if(selectChar!=NULL)
			{
				if (g_getLButtonState(App::sInstance().GetHGE()) == eLButtonState_Up)
				{
					int nLastSelect = m_nSelectNum;
					if (selectChar->GetCamp() == eCamp_Friend )
					{
						//上次没有选择单位
						if(nLastSelect == -1)
						{
							//选中的未行动友方进入移动阶段
							if(!selectChar->GetFinish())
							{
								m_nSelectNum = selectChar->GetNum();
								selectChar->SetActionStage(eActionStage_MoveStage);
							}
						}
						//连续两次点击同一友方单位
						else if(nLastSelect == m_nSelectNum)
						{
							//过滤移动阶段，进入攻击阶段
							selectChar->SetActionStage(eActionStage_AttackStage);
						}
						//其他情况
						else
						{
							Character* lastChar = GetCreature(nLastSelect);
							if(lastChar != NULL)
							{
								//上次点的是别的友方
								if(lastChar->GetCamp() == eCamp_Friend)
								{
// 									//上次的友方返回至待命阶段
// 									lastChar->SetActionStage(eActionStage_WaitStage);
// 									selectChar->SetActionStage(eActionStage_MoveStage);
								}
								else if(lastChar->GetCamp() == eCamp_Enemy)
								{
									if(!selectChar->GetFinish())
									{
										m_nSelectNum = selectChar->GetNum();
										selectChar->SetActionStage(eActionStage_MoveStage);
									}
								}
							}
							else 
								//这里是错误分支，不可以运行到这里
								return;
						}
					}
					else if (selectChar->GetCamp() == eCamp_Enemy)
					{
						Character* lastChar = GetCreature(nLastSelect);
						if(lastChar!=NULL) 
						{
							if(lastChar->GetCamp()==eCamp_Friend)
							{
								if(lastChar->GetActionStage() == eActionStage_AttackStage)
								{
									//判断是否可以攻击到选中单位
									//这里直接攻击方便测试
									lastChar->SetTarget(selectChar->GetNum());
									lastChar->GeginHit();
									//lastChar->SetFinish(true);
									m_nSelectNum = -1;
								}
							}
							else if (lastChar->GetCamp() == eCamp_Enemy)
							{
								m_nSelectNum = selectChar->GetNum();
							}
						}
						else
						{
							m_nSelectNum = selectChar->GetNum();
						}
					}
				}
			}
			//点中地面
			else
			{
				if (g_getLButtonState(App::sInstance().GetHGE()) == eLButtonState_Up)
				{
					if (m_nSelectNum>=0)
					{
						selectChar = GetCreature(m_nSelectNum);
						if (selectChar!= NULL)
						{
							if(selectChar->GetCamp() == eCamp_Friend && selectChar->GetActionStage() == eActionStage_MoveStage)
							{
								//判断是否可以移动过去
								int length = abs(mouseBlock.xpos - selectChar->GetBlock().xpos) + abs(mouseBlock.ypos - selectChar->GetBlock().ypos);
								//超过移动范围
								if(length > selectChar->GetMoveAbility())
									return;

								//移动过去，记录原始位置
								selectChar->GetOrigBlock().xpos = selectChar->GetBlock().xpos;
								selectChar->GetOrigBlock().ypos = selectChar->GetBlock().ypos;
								selectChar->GetOrigDirection() = selectChar->GetCurDirection();
								selectChar->Move(mouseBlock.xpos,mouseBlock.ypos);
							}
						}
					}
				}
			}
		}
	}
}

void CreatureManager::UnSelectCreature()
{
	if (m_nSelectNum >= 0)
	{
		int nLastSelect = m_nSelectNum;
		if (g_getRButtonState(App::sInstance().GetHGE()) == eRButtonState_Up)
		{
			Character* lastChar = GetCreature(nLastSelect);
			if(lastChar!=NULL && lastChar->GetCamp()==eCamp_Friend)
			{
				//返回至原位置
				if(lastChar->GetActionStage() == eActionStage_AttackStage)
				{
					lastChar->SetActionStage(eActionStage_MoveStage);
					lastChar->CancelMove();
				}
				//右键取消选中的友方，需要重置行动阶段，处于移动中的单位不可以当时取消
				else if(lastChar->GetActionStage() == eActionStage_MoveStage && lastChar->GetCharacterState()==eCharacterState_Stand)	
				{
					lastChar->SetActionStage(eActionStage_WaitStage);
					m_nSelectNum = -1;
				}
			}
			else
				m_nSelectNum = -1;
		}
	}
}

void CreatureManager::ProcessSelectCreature()
{
	if(m_nSelectNum >= 0)
	{
		Character* selectChar = GetCreature(m_nSelectNum);
		if((selectChar->GetCamp() == eCamp_Enemy) || (selectChar->GetCamp()==eCamp_Friend && selectChar->GetCharacterState()==eCharacterState_Stand && selectChar->GetActionStage() ==eActionStage_MoveStage))
			ShowMoveRange(selectChar);
		if(selectChar->GetCamp() == eCamp_Enemy || (selectChar->GetCamp()==eCamp_Friend && selectChar->GetActionStage()==eActionStage_AttackStage))
			ShowAttackRange(selectChar);
	}
	
}