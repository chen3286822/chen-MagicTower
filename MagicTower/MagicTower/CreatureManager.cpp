#include "CreatureManager.h"
#include "MapManager.h"
#include "App.h"
#include "TipWnd.h"
#include "UI.h"
#include "ActionProcess.h"

CreatureManager::CreatureManager()
{
	m_VFriendList.clear();
	m_VEnemyList.clear();
	m_VEnemyDeadList.clear();
	m_VFriendDeadList.clear();
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
	{
		if((*it)->GetDead() != true)
			(*it)->Render();
	}
	for (VCharacter::iterator it=m_VFriendList.begin();it!=m_VFriendList.end();it++)
	{
		if((*it)->GetDead() != true)
			(*it)->Render();
	}
}

void CreatureManager::Update(float delta)
{
	for (VCharacter::iterator it=m_VEnemyList.begin();it!=m_VEnemyList.end();it++)
	{
		if((*it)->GetDead() == true)
			m_VEnemyDeadList.push_back(*it);
		else
			(*it)->Update(delta);
	}
	for (VCharacter::iterator it=m_VFriendList.begin();it!=m_VFriendList.end();it++)
	{
		if((*it)->GetDead() == true)
			m_VFriendDeadList.push_back(*it);
		else
			(*it)->Update(delta);
	}

	//移除死亡单位
	for (int i=0;i<m_VEnemyDeadList.size();i++)
	{
		RemoveEnemy(m_VEnemyDeadList[i]);
	}
	m_VEnemyDeadList.clear();
	for (int i=0;i<m_VFriendDeadList.size();i++)
	{
		RemoveFriend(m_VFriendDeadList[i]);
	}
	m_VFriendDeadList.clear();

	if(m_nActionCreatureNum != -1)
	{
		Character* cha = GetEnemy(m_nActionCreatureNum);
		if(cha->GetFinish())
			m_nActionCreatureNum = -1;

		//测试用
		if (cha->GetCamp() == eCamp_Enemy && cha->GetActionStage() == eActionStage_HandleStage)
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
		DWORD color = 0;
		if(creature->GetCamp() == eCamp_Friend)
			color = 0x4F3737DF;
		else if(creature->GetCamp() == eCamp_Enemy)
			color = 0x4FEB2323;
		std::vector<Block*> range = creature->CreateMoveRange(MapManager::sInstance().GetCurrentMap());
		for (std::vector<Block*>::iterator it=range.begin();it!=range.end();it++)
		{
			//画方格表示可以移动
			App::sInstance().DrawSmallRect(Block((*it)->xpos,(*it)->ypos),color);
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
		sprintf(temp," 生命值：%d",cha->GetHP());
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
			int xMove = g_RandomInt(0,enemy->GetMoveAbility());
			int yMove = enemy->GetMoveAbility() - xMove;
			int xDir = (g_RandomInt(0,1)==0)?-1:1;
			int yDir = (g_RandomInt(0,1)==0)?-1:1;
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

VCharacter::iterator CreatureManager::RemoveEnemy(Character* _enemy)
{
	for (VCharacter::iterator it=m_VEnemyList.begin();it!=m_VEnemyList.end();)
	{
		if((*it)->GetID() == _enemy->GetID() && (*it)->GetNum() == _enemy->GetNum())
		{
			//所占格子属性变更
			setOccupied(MapManager::sInstance().GetCurrentMap()->GetBlock(_enemy->GetBlock().xpos,_enemy->GetBlock().ypos)->attri,0);
			gSafeDelete(*it);
			m_VEnemyList.erase(it);
			return it;
		}
		else
			it++;
	}
	return m_VEnemyList.end();
}

void CreatureManager::RemoveFriend(Character* _friend)
{
	for (VCharacter::iterator it=m_VFriendList.begin();it!=m_VFriendList.end();)
	{
		if((*it)->GetID() == _friend->GetID() && (*it)->GetNum() == _friend->GetNum())
		{
			//所占格子属性变更
			setOccupied(MapManager::sInstance().GetCurrentMap()->GetBlock(_friend->GetBlock().xpos,_friend->GetBlock().ypos)->attri,0);
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
	{
		m_eCampTurn = eCampTurn_Enemy;
		m_nSelectNum = -1;
	}

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
/*
int CreatureManager::Notify(int src,int tar,int messageID,int param)
{
	int result = -1;
	Character* target = GetCreature(tar);
	if (!target)
		result = eNotify_NoTarget;

	switch(messageID)
	{
	case eNotify_TowardToAttacker:
		result = target->TowardToAttacker(src);
		break;
	case eNotify_ReadyToBeAttacked:
		{
			//计算是否暴击
			bool bCrit = false;
			if (g_RandomInt(0,9) < (int)(target->GetCrit()*10))
				bCrit = true;

			Character* _self = GetCreature(src);
			if(_self)
				CalculateHurt(target,_self,bCrit);

			if(!bCrit)
				target->Attack();
			else
				target->Crit();

			result = eNotify_Success;
			break;
		}
	case eNotify_FinishAttack:
		{
			target->SetFinish(true);
		}
		break;
	default:
		break;
	}

	return result;
}*/

void CreatureManager::CalculateHurt(Character* cast,Character* target,bool bCrit)
{
	if(!cast || !target)
		cast->GetPreHurt() = 0;

	int hurt = cast->GetAttack() - target->GetDefend();
	if(bCrit)
		hurt += cast->GetAttack();
	if(hurt > 0)
		cast->GetPreHurt() = hurt;
}

// void CreatureManager::CalculateResult(int src,int tar)
// {
// 	Character* cast = GetCreature(src);
// 	Character* target = GetCreature(tar);
// 	//首先计算命中与否
// 	bool bhit = false;
// 	if(g_RandomInt(0,9) >= (int)(target->GetDodge()*10))
// 		bhit = true;
// 
// 
// 	target->GetCounter() = true;
// 	if(bhit)
// 	{
// 		target->GetHP() -= cast->GetPreHurt();
// 		cast->GetPreHurt() = 0;
// 
// 		target->Attacked();
// 	}
// 	else
// 		target->Defend();
// }

void CreatureManager::PreCalculateAndPushAction(Character* cast,Character* target)
{
	if(!cast || !target)
		return;
	cast->GetPreHurt() = 0;
	target->GetPreHurt() = 0;
	//首先计算是否暴击
	//计算是否暴击
	bool bCrit = false;
	if (g_RandomInt(0,9) < (int)(cast->GetCrit()*10))
		bCrit = true;

	//进行攻击伤害预存
	CalculateHurt(cast,target,bCrit);

	//计算是否命中
	bool bhit = false;
	if(g_RandomInt(0,9) >= (int)(target->GetDodge()*10))
		bhit = true;
	else 
		cast->GetPreHurt() = 0;

	//目标是否死亡
	bool bDead = false;
	if(target->GetHP() <= cast->GetPreHurt())
		bDead = true;

	bool bCounter = false;
	bool bCrit2 = false;
	bool bhit2 = false;
	bool bDead2 = false;
	if(!bDead)
	{
		//计算是否反击
		if(target->GetCounter())
		{
			//是否处于反击范围
			if(target->CanHitTarget(cast))
			{
				bCounter = true;
			}
		}

		//反击是否暴击
		if (g_RandomInt(0,9) < (int)(target->GetCrit()*10))
			bCrit2 = true;

		//进行攻击伤害预存
		CalculateHurt(target,cast,bCrit2);

		//计算是否命中
		if(g_RandomInt(0,9) >= (int)(cast->GetDodge()*10))
			bhit2 = true;
		else
			target->GetPreHurt() = 0;

		//cast是否死亡
		if(target->GetPreHurt() >= cast->GetHP())
			bDead2 = true;
	}


	//推送动作
	ActionProcess* process = ActionProcess::sInstancePtr();
	process->PushAction(eNotify_TowardToAttacker,cast,target,0);
	process->PushAction(eNotify_TowardToAttacker,target,cast,200);
	if(bCrit)
		process->PushAction(eNotify_Crit,cast,target,1000);
	if(bhit)
		process->PushAction(eNotify_Attack,cast,target,500);
	else
		process->PushAction(eNotify_AttackDefend,cast,target,500);
	if(bDead)
		process->PushAction(eNotify_Dead,cast,target,300);
	else
	{
		if(bCounter)
		{
			if(bCrit2)
				process->PushAction(eNotify_Crit,target,cast,1000);
			if(bhit2)
				process->PushAction(eNotify_Attack,target,cast,500);
			else
				process->PushAction(eNotify_AttackDefend,target,cast,500);
			if(bDead2)
				process->PushAction(eNotify_Dead,target,cast,300);
		}
	}
	//结束行动
	process->PushAction(eNotify_FinishAttack,cast,target,0);
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
				//需要拦截点击操作界面的消息
				if (g_getLButtonState(App::sInstance().GetHGE()) == eLButtonState_Up && !(UISystem::sInstance().GetWindow(eWindowID_Command)->IsOnControl()))
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
						else if(nLastSelect == selectChar->GetNum())
						{
							if(!selectChar->GetFinish())
							{
								//跳过移动阶段，进入操作阶段
								selectChar->SetActionStage(eActionStage_HandleStage);
								//打开操作界面
								UIWindow* commandWindow = UISystem::sInstance().GetWindow(eWindowID_Command);
								if(commandWindow)
								{
									commandWindow->SetShow(true);
									commandWindow->SetBindChar(selectChar);
								}
							}
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
									if(lastChar->GetFinish() && !selectChar->GetFinish())
									{
										m_nSelectNum = selectChar->GetNum();
										selectChar->SetActionStage(eActionStage_MoveStage);
									}
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
									if(lastChar->CanHitTarget(selectChar))
									{
//										lastChar->SetTarget(selectChar->GetNum());
										m_nSelectNum = -1;
										//lastChar->GeginHit();
										//进行预计算
										PreCalculateAndPushAction(lastChar,selectChar);
										return;
									}
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
				if (g_getLButtonState(App::sInstance().GetHGE()) == eLButtonState_Up && !(UISystem::sInstance().GetWindow(eWindowID_Command)->IsOnControl()))
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
			if(lastChar!=NULL && lastChar->GetCamp()==eCamp_Friend && !(lastChar->GetFinish()))
			{
				//返回至原位置
				eActionStage stage = lastChar->GetActionStage();
				if(stage == eActionStage_HandleStage)
				{
					lastChar->SetActionStage(eActionStage_MoveStage);
					lastChar->CancelMove();
					//关闭操作界面
					UIWindow* commandWindow = UISystem::sInstance().GetWindow(eWindowID_Command);
					if(commandWindow)
					{
						commandWindow->SetShow(false);
					}
				}
				//攻击阶段、技能阶段、使用物品阶段可以返回至操作阶段
				else if(stage == eActionStage_AttackStage || stage == eActionStage_GoodStage || stage == eActionStage_SkillStage)
				{
					lastChar->SetActionStage(eActionStage_HandleStage);
					//打开操作界面
					UIWindow* commandWindow = UISystem::sInstance().GetWindow(eWindowID_Command);
					if(commandWindow)
					{
						commandWindow->SetShow(true);
						commandWindow->SetBindChar(lastChar);
					}
				}
				//右键取消选中的友方，需要重置行动阶段，处于移动中的单位不可以当时取消
				else if(stage == eActionStage_MoveStage && lastChar->GetCharacterState()==eCharacterState_Stand)	
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
		if(selectChar->GetCamp() == eCamp_Enemy || (selectChar->GetCamp()==eCamp_Friend && (selectChar->GetActionStage()==eActionStage_AttackStage || selectChar->GetActionStage()==eActionStage_SkillStage)))
			ShowAttackRange(selectChar);
	}
	
}