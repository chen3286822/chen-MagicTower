#include "CreatureManager.h"
#include "ConfigManager.h"
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
	m_vPair.clear();
	m_mAttackRange.clear();
	m_mSkillRange.clear();

	m_vPair.push_back(Pair(0,0));	/*0*/			
	m_vPair.push_back(Pair(1,0));	/*1*/				m_vPair.push_back(Pair(1,-1));		/*2*/
	m_vPair.push_back(Pair(0,-1));	/*3*/				m_vPair.push_back(Pair(-1,-1));	/*4*/
	m_vPair.push_back(Pair(-1,0));	/*5*/				m_vPair.push_back(Pair(-1,1));		/*6*/
	m_vPair.push_back(Pair(0,1));	/*7*/				m_vPair.push_back(Pair(1,1));		/*8*/
	m_vPair.push_back(Pair(2,0));	/*9*/				m_vPair.push_back(Pair(2,-1));		/*10*/
	m_vPair.push_back(Pair(0,-2));	/*11*/			m_vPair.push_back(Pair(1,-2));		/*12*/
	m_vPair.push_back(Pair(-2,0));	/*13*/			m_vPair.push_back(Pair(-1,-2));	/*14*/
	m_vPair.push_back(Pair(0,2));	/*15*/			m_vPair.push_back(Pair(-2,-1));	/*16*/
	m_vPair.push_back(Pair(-2,1));	/*17*/			m_vPair.push_back(Pair(3,0));		/*18*/
	m_vPair.push_back(Pair(-1,2));	/*19*/			m_vPair.push_back(Pair(0,-3));		/*20*/
	m_vPair.push_back(Pair(1,2));	/*21*/			m_vPair.push_back(Pair(-3,0));		/*22*/
	m_vPair.push_back(Pair(2,1));	/*23*/			m_vPair.push_back(Pair(0,3));		/*24*/
	m_vPair.push_back(Pair(4,0));	/*25*/			m_vPair.push_back(Pair(-4,0));		/*26*/
	m_vPair.push_back(Pair(5,0));	/*27*/			m_vPair.push_back(Pair(-5,0));		/*28*/
	m_vPair.push_back(Pair(0,-4));	/*29*/			m_vPair.push_back(Pair(0,4));		/*30*/
	m_vPair.push_back(Pair(0,-5));	/*31*/			m_vPair.push_back(Pair(0,5));		/*32*/
	
	int neAttackRange_Cross[] = {1,3,5,7};
	m_mAttackRange[eAttackRange_Cross].assign(neAttackRange_Cross,neAttackRange_Cross+4);

	int neAttackRange_Box[] = {1,2,3,4,5,6,7,8};
	m_mAttackRange[eAttackRange_Box].assign(neAttackRange_Box,neAttackRange_Box+8);

	int neAttackRange_BigCross[] = {1,3,5,7,9,11,13,15};
	m_mAttackRange[eAttackRange_BigCross].assign(neAttackRange_BigCross,neAttackRange_BigCross+8);

	int neAttackRange_Arrow[] = {2,4,6,8,9,11,13,15};
	m_mAttackRange[eAttackRange_Arrow].assign(neAttackRange_Arrow,neAttackRange_Arrow+8);

	int neSkillRange_Point[] = {0};
	m_mSkillRange[eSkillRange_Point].assign(neSkillRange_Point,neSkillRange_Point+1);
	int neSkillRange_Cross[] = {0,1,3,5,7};
	m_mSkillRange[eSkillRange_Cross].assign(neSkillRange_Cross,neSkillRange_Cross+5);
	int neSkillRange_Box[] = {0,1,2,3,4,5,6,7,8};
	m_mSkillRange[eSkillRange_Box].assign(neSkillRange_Box,neSkillRange_Box+9);
	int neSkillRange_ObliqueBox[] = {0,1,2,3,4,5,6,7,8,9,11,13,15};
	m_mSkillRange[eSkillRange_ObliqueBox].assign(neSkillRange_ObliqueBox,neSkillRange_ObliqueBox+13);
	int neSkillRange_BigObliqueBox[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24};
	m_mSkillRange[eSkillRange_BigObliqueBox].assign(neSkillRange_BigObliqueBox,neSkillRange_BigObliqueBox+25);

	//直线型比较特殊，需要判断方向
	int neSkillRange_ShortLine[] = {0,7,15,0,3,11,0,5,13,0,1,9};
	m_mSkillRange[eSkillRange_ShortLine].assign(neSkillRange_ShortLine,neSkillRange_ShortLine+12);
	int neSkillRange_Line[] = {0,7,15,24,30,32,0,3,11,20,29,31,0,5,13,22,26,28,0,1,9,18,25,27};
	m_mSkillRange[eSkillRange_Line].assign(neSkillRange_Line,neSkillRange_Line+24);

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

	if( !(UISystem::sInstance().IsInAnyControl()))
	{
		if (g_getLButtonState(App::sInstance().GetHGE()) == eLButtonState_Up)
			SelectCreature();
		if(g_getRButtonState(App::sInstance().GetHGE()) == eRButtonState_Up)
			UnSelectCreature();
	}

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
		std::vector<Block*> range = creature->CreateRange(MapManager::sInstance().GetCurrentMap(),creature->GetMoveAbility());
		for (std::vector<Block*>::iterator it=range.begin();it!=range.end();it++)
		{
			//画方格表示可以移动
			App::sInstance().DrawSmallRect(Block((*it)->xpos,(*it)->ypos),color);
		}
	}
}

void CreatureManager::ShowSkillCastRange(Character* creature)
{
	if (creature)
	{
		DWORD color = 0x4FEB2323;
		std::vector<Block*> range = creature->CreateRange(MapManager::sInstance().GetCurrentMap(),ConfigManager::sInstance().GetSkillInfo().find(creature->GetCastSkill())->second.m_nCastRange,true);
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
				for (vector<int>::iterator it=mit->second.begin();it!=mit->second.end();it++)
				{
					offX = m_vPair[*it].x + charBlock.xpos;
					offY = m_vPair[*it].y + charBlock.ypos;
					if (offX >= 0 && offX < mapWidth && offY >= 0 && offY < mapLength)
					{					
						App::sInstance().DrawBox(MAP_OFF_X + offX*MAP_RECT,MAP_OFF_Y + offY*MAP_RECT,0xBFFF0000,8,MAP_RECT,MAP_RECT);
					}
				}
			}
		}
	}
}

void CreatureManager::ShowSkillRange(int skillID)
{
	SkillInfo skillInfo = ConfigManager::sInstance().GetSkillInfo().find(skillID)->second;
	Map* currentMap = MapManager::sInstance().GetCurrentMap();
	int mapWidth = 0,mapLength = 0;
	currentMap->GetWidthLength(mapWidth,mapLength);
	int offX = 0,offY = 0;
	Block mouseBlock = App::sInstance().GetMouseBlock();
	DWORD color = 0xBFFF0000;
	if(skillInfo.m_nSkillType == eSkillType_Heal || skillInfo.m_nSkillType == eSkillType_Buff)
		color = 0xBF00FF00;

	for (MSkillRange::iterator mit=m_mSkillRange.begin();mit!=m_mSkillRange.end();mit++)
	{
		if(mit->first == skillInfo.m_nAttackRange)
		{
			//直线型特殊处理，只显示一个格子
			if(mit->first == eSkillRange_ShortLine || mit->first == eSkillRange_Line)
			{
				offX = mouseBlock.xpos;
				offY = mouseBlock.ypos;
				App::sInstance().DrawBox(MAP_OFF_X + offX*MAP_RECT,MAP_OFF_Y + offY*MAP_RECT,color,8,MAP_RECT,MAP_RECT);
			}
			else
			{
				for (vector<int>::iterator it=mit->second.begin();it!=mit->second.end();it++)
				{
					offX = m_vPair[*it].x + mouseBlock.xpos;
					offY = m_vPair[*it].y + mouseBlock.ypos;
					if (offX >= 0 && offX < mapWidth && offY >= 0 && offY < mapLength)
					{					
						App::sInstance().DrawBox(MAP_OFF_X + offX*MAP_RECT,MAP_OFF_Y + offY*MAP_RECT,color,8,MAP_RECT,MAP_RECT);
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

				UIWindow* charInfoWindow = UISystem::sInstance().GetWindow(eWindowID_CharInfo);
				if(charInfoWindow->IsShow())
					charInfoWindow->SetShow(false);
				return;
			}
		}
		UIWindow* commandWindow = UISystem::sInstance().GetWindow(eWindowID_Command);
		if(commandWindow && commandWindow->IsShow())
		{
			return;
		}
		if (commandWindow->GetBindChar() && commandWindow->GetBindChar()->GetActionStage() == eActionStage_AttackStage)
		{
			if(commandWindow->GetBindChar()->CanHitTarget(cha))
			{
				UIWindow* charInfoWindow = UISystem::sInstance().GetWindow(eWindowID_CharInfo);
				if(charInfoWindow)
				{
					charInfoWindow->SetShow(true);
					charInfoWindow->SetBindChar(cha);
				}			
			}
			return;
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
		sprintf(temp," 攻击力：%d",cha->GetAttack());
		TipWnd::sInstance().AddText(temp,0xFFFFFFFF,-1,-1,eFontType_MSYaHei,eFontSize_FontMiddle);
		TipWnd::sInstance().SetShow(true);
		TipWnd::sInstance().SetPos(block.xpos,block.ypos);
	}
	else
	{
		UIWindow* charInfoWindow = UISystem::sInstance().GetWindow(eWindowID_CharInfo);
		if(charInfoWindow->IsShow())
			charInfoWindow->SetShow(false);

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
		MapManager::sInstance().GetCurrentMap()->GoIntoTurn(m_eCampTurn);
	}

	for (VCharacter::iterator it=m_VEnemyList.begin();it!=m_VEnemyList.end();it++)
	{
		if((*it)->GetFinish() == false)	//有敌方尚未行动
			return false;
	}
	//敌方行动完，进入下一回合
	if (m_eCampTurn == eCampTurn_Enemy)
	{
		m_eCampTurn = eCampTurn_Friend;
		MapManager::sInstance().GetCurrentMap()->AddTurn();
		MapManager::sInstance().GetCurrentMap()->GoIntoTurn(m_eCampTurn);
	}

	for (VCharacter::iterator it=m_VEnemyList.begin();it!=m_VEnemyList.end();it++)
	{
			(*it)->SetFinish(false);
			(*it)->SetActionStage(eActionStage_WaitStage);
			(*it)->ChangeColor(0xFFFFFFFF);

			//更新buff
			(*it)->RemoveBuff();
	}
	for (VCharacter::iterator it=m_VFriendList.begin();it!=m_VFriendList.end();it++)
	{
		(*it)->SetFinish(false);
		(*it)->SetActionStage(eActionStage_WaitStage);
		(*it)->ChangeColor(0xFFFFFFFF);

		//更新buff
		(*it)->RemoveBuff();
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
		target->GetPreHurt() = 0;

	int hurt = cast->GetAttack() - target->GetDefend();
	if(bCrit)
		hurt += cast->GetAttack();
	if(hurt > 0)
		target->GetPreHurt() = hurt;
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

void CreatureManager::PreAttackAndPushAction(Character* cast,Character* target)
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
		target->GetPreHurt() = 0;

	//目标是否死亡
	bool bDead = false;
	if(target->GetHP() <= target->GetPreHurt())
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

		if(bCounter)
		{
			//反击是否暴击
			if (g_RandomInt(0,9) < (int)(target->GetCrit()*10))
				bCrit2 = true;

			//进行攻击伤害预存
			CalculateHurt(target,cast,bCrit2);

			//计算是否命中
			if(g_RandomInt(0,9) >= (int)(cast->GetDodge()*10))
				bhit2 = true;
			else
				cast->GetPreHurt() = 0;

			//cast是否死亡
			if(cast->GetPreHurt() >= cast->GetHP())
				bDead2 = true;
		}
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
	if(!bDead2)
		process->PushAction(eNotify_FinishAttack,cast,target,0);
}

void CreatureManager::PreSkillAndPushAction(Character* cast,Character* target)
{
	if(!cast || !target)
		return;

	//找到技能范围内的所有敌人
	m_vSkillTargets.clear();
	SkillInfo skill = ConfigManager::sInstance().GetSkillInfo().find(cast->GetCastSkill())->second;
	VCharacter vTar;
	int tarX = 0,tarY = 0;
	for (MSkillRange::iterator mit=m_mSkillRange.begin();mit!=m_mSkillRange.end();mit++)
	{
		if(mit->first == skill.m_nCastRange)
		{
			if(skill.m_nCastRange != eSkillRange_ShortLine || skill.m_nCastRange!= eSkillRange_Line)
			{
				for (vector<int>::iterator it=mit->second.begin();it!=mit->second.end();it++)
				{
					tarX = m_vPair[*it].x + target->GetBlock().xpos;
					tarY = m_vPair[*it].y + target->GetBlock().ypos;
					Character* otherChar = NULL;
					if(skill.m_nSkillType == eSkillType_Hurt)
						otherChar = GetEnemy(tarX,tarY);
					else if(skill.m_nSkillType == eSkillType_Heal || skill.m_nSkillType == eSkillType_Buff)
						otherChar = GetFriend(tarX,tarY);
					if(otherChar!=NULL)
						vTar.push_back(otherChar);
				}
			}
			else
			{
				//确定双方位置
				eDirection dir = eDirection_None;
				Block& block = target->GetBlock();
				if (block.xpos < cast->GetBlock().xpos)
					dir = eDirection_Left;
				else if(block.xpos > cast->GetBlock().xpos)
					dir = eDirection_Right;
				else if(block.ypos < cast->GetBlock().ypos)
					dir = eDirection_Up;
				else
					dir = eDirection_Down;

				int num = ((skill.m_nCastRange==eSkillRange_ShortLine)?12:24);
				for(int i=num/4*(dir-1);i<num/4*(dir-1)+4;i++)
				{
					tarX = m_vPair[mit->second[i]].x + target->GetBlock().xpos;
					tarY = m_vPair[mit->second[i]].y + target->GetBlock().ypos;
					Character* otherChar = NULL;
					if(skill.m_nSkillType == eSkillType_Hurt)
						otherChar = GetEnemy(tarX,tarY);
					else if(skill.m_nSkillType == eSkillType_Heal || skill.m_nSkillType == eSkillType_Buff)
						otherChar = GetFriend(tarX,tarY);
					if(otherChar!=NULL)
						vTar.push_back(otherChar);
				}
			}
		}
	}
	m_vSkillTargets = vTar;

	ActionProcess* process = ActionProcess::sInstancePtr();
	process->PushAction(eNotify_TowardToAttacker,cast,target,0);
	process->PushAction(eNotify_CastAction,cast,target,500);
	process->PushAction(eNotify_CastSkill,cast,target,0);

	cast->GetPreHurt() = 0;
	if(skill.m_nSkillType == eSkillType_Hurt)
	{
		for(VCharacter::iterator it=vTar.begin();it!=vTar.end();it++)
		{
			(*it)->GetPreHurt() = skill.m_nAttack;
			bool bDead = false;
			if((*it)->GetHP() <= (*it)->GetPreHurt())
				bDead = true;

			process->PushAction(eNotify_Attacked,cast,(*it),500);
			if(bDead)
				process->PushAction(eNotify_Dead,cast,(*it),300);
		}
	}
	else if(skill.m_nSkillType == eSkillType_Heal || skill.m_nSkillType == eSkillType_Buff)
	{
		for(VCharacter::iterator it=vTar.begin();it!=vTar.end();it++)
		{
			(*it)->GetPreHurt() = skill.m_nAttack;
			process->PushAction(eNotify_Healed,cast,(*it),500);
		}
	}

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
							if(selectChar->GetActionStage()== eActionStage_MoveStage)
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
							else if (selectChar->GetActionStage()== eActionStage_SkillStage)
							{
								//对自己释放技能
								if(selectChar->CanSkillHitTarget(selectChar))
								{
									m_nSelectNum = -1;
									PreSkillAndPushAction(selectChar,selectChar);
									return;
								}
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
								if(lastChar->GetActionStage() == eActionStage_SkillStage)
								{
									//对友方释放技能
									if(lastChar->CanSkillHitTarget(selectChar))
									{
										m_nSelectNum = -1;
										PreSkillAndPushAction(lastChar,selectChar);
										return;
									}
								}
								else if(lastChar->GetFinish() && !selectChar->GetFinish())
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
							g_debugString(__FILE__,__FUNCTION__,__LINE__);
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
									//隐藏单位信息窗口
									UIWindow* charInfoWindow = UISystem::sInstance().GetWindow(eWindowID_CharInfo);
									if(charInfoWindow && charInfoWindow->IsShow())
									{
										charInfoWindow->SetShow(false);
										UIWindow* commandWnd = UISystem::sInstance().GetWindow(eWindowID_Command);
										if(commandWnd)
											commandWnd->SetBindChar(NULL);
									}	

									m_nSelectNum = -1;
									//lastChar->GeginHit();
									//进行预计算
									PreAttackAndPushAction(lastChar,selectChar);
									return;
								}
							}
							else if (lastChar->GetActionStage() == eActionStage_SkillStage)
							{
								if(lastChar->CanSkillHitTarget(selectChar))
								{
									m_nSelectNum = -1;
									PreSkillAndPushAction(lastChar,selectChar);
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
			//点中地面
			else
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

void CreatureManager::UnSelectCreature()
{
	if (m_nSelectNum >= 0)
	{
		int nLastSelect = m_nSelectNum;
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
			else if(stage == eActionStage_AttackStage || stage == eActionStage_ItemStage || stage == eActionStage_SkillStage)
			{
				lastChar->GetCastSkill() = -1;
				lastChar->SetActionStage(eActionStage_HandleStage);
				//打开操作界面
				UIWindow* commandWindow = UISystem::sInstance().GetWindow(eWindowID_Command);
				if(commandWindow)
				{
					commandWindow->SetShow(true);
					commandWindow->SetBindChar(lastChar);
				}
				//关闭技能界面
				UIWindow* selectWndow = UISystem::sInstance().GetWindow(eWindowID_Select);
				if(selectWndow)
				{
					selectWndow->SetShow(false);
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

void CreatureManager::ProcessSelectCreature()
{
	if(m_nSelectNum >= 0)
	{
		Character* selectChar = GetCreature(m_nSelectNum);
		if((selectChar->GetCamp() == eCamp_Enemy) || (selectChar->GetCamp()==eCamp_Friend && selectChar->GetCharacterState()==eCharacterState_Stand && selectChar->GetActionStage() ==eActionStage_MoveStage))
			ShowMoveRange(selectChar);
		if(selectChar->GetCamp() == eCamp_Enemy || (selectChar->GetCamp()==eCamp_Friend && (selectChar->GetActionStage()==eActionStage_AttackStage)))
			ShowAttackRange(selectChar);
		if(selectChar->GetCamp()==eCamp_Friend && selectChar->GetActionStage()==eActionStage_SkillStage && selectChar->GetCastSkill()!=-1)
			ShowSkillCastRange(selectChar);

		if(selectChar->GetCastSkill() != -1)
		{
			ShowSkillRange(selectChar->GetCastSkill());
		}
	}
	

}