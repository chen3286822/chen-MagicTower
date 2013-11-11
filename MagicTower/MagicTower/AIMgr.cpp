#include "AIMgr.h"
#include "Character.h"
#include "CreatureManager.h"
#include "MapManager.h"

AIMgr g_AIMgr;

AIMgr::AIMgr()
{
	m_pCurAI = NULL;
	m_eStrategy = eAIStrategy_AttakTarget;
}

AIMgr::~AIMgr()
{

}

void AIMgr::SetCurCharacter(Character* cha)
{
	//已经设置过
	if(m_pCurAI)
		return;

	m_pCurAI = cha;
}

Character* AIMgr::GetCurCharacter()
{
	return m_pCurAI;
}

bool AIMgr::DoAction()
{
	if(!m_pCurAI)
		return false;

	//根据策略确定行动
	/*
	*	如果是优先攻击目标策略（如果失败条件是己方全死，则目标单位就是列表中第一个单位）：
	*	A、找出移动+攻击范围内所以可以攻击到的敌方单位，
	*	如果有目标单位，则攻击之(为了性能起见，这里攻击时判断如果能使用技能则使用最高伤害的手段)；否则判断是否有期望攻击致死
	*	的单位，有则攻击之；否则攻击克制其兵种的单位；否则攻击血量最低单位
	*	B、找不到敌方单位，则选择向目标单位前进，前进后使用增益技能，没有则待命
	*	如果是优先保护自己策略：
	*	A、查找自己是否处于敌方目标攻击范围，且是否会被攻击致死(对于不同兵种，采取不同算法计算
	*	被攻击伤害，魔法系取能释放的最高伤害技能)，会的话，优先选择路线离开，并使用增益魔法；
	*	不会致死的话，则执行优先攻击目标的A策略
	*	B、自己处于安全区域，则优先等待其他单位前进，使自己处于后方再前进，并优先对自己释放增益魔法后待命
	*	如果是优先保护目标策略：
	*	A、检查要保护的目标是否处于被攻击范围，是的话，自己是否可以攻击到最具威胁的单位，可以的话，过去攻击，
	*	否则选择下一威胁单位，执行上述步骤，都不行的话，向最具威胁单位前进
	*	B、保护目标处于安全范围则直接靠近，优先向保护目标施放增益魔法，不能的话直接待命
	*/
	Map* curMap = MapManager::sInstance().GetCurrentMap();
	if(!curMap)
		return false;

	switch(m_eStrategy)
	{
	case eAIStrategy_AttakTarget:
		{
			VAttackTarget targets = CreatureManager::sInstance().GetAttackTarget(m_pCurAI);
			Failed failCondition = curMap->GetFailCondition();
			//表示是否可以直接攻击到终极目标
			bool bUltraTarCanHit = false;
			//可以攻击到终极目标的站位点
			POINT attackPoint;
			Character* ultraTarget = NULL;//终极目标，干掉他就相当于胜利了
			//设定现在的终极目标
			if(failCondition.m_eConditoin == eFailCondition_TargetDead)
			{
				if(!failCondition.m_vNum.empty())
					ultraTarget = CreatureManager::sInstance().GetCreature(failCondition.m_vNum.front());
			}
			else if(failCondition.m_eConditoin == eFailCondition_AllFriendsDead)
			{
				if(!targets.empty())
					ultraTarget = targets.front().m_iTarget;
				else
				{
					if(m_pCurAI->GetCamp() == eCamp_Enemy)
					{
						VCharacter& friendList = CreatureManager::sInstance().GetFriend();
						if(!friendList.empty())
							ultraTarget = friendList.front();
					}
					else if(m_pCurAI->GetCamp() == eCamp_Friend)
					{
						VCharacter& enemyList = CreatureManager::sInstance().GetEnemy();
						if(!enemyList.empty())
							ultraTarget = enemyList.front();
					}
				}
			}
			//终极目标根本没有，那么这个单位可以停止行动了
			if(!ultraTarget)
			{
				m_pCurAI->SetFinish(true);
				m_pCurAI = NULL;
				return true;
			}
			for (VAttackTarget::iterator it=targets.begin();it!=targets.end();it++)
			{
				if((*it).m_iTarget->GetNum() == ultraTarget->GetNum())
				{
					bUltraTarCanHit = true;
					attackPoint.x = (*it).m_iAttakPoint.x;
					attackPoint.y = (*it).m_iAttakPoint.y;
					break;
				}
			}
			//如果可以直接攻击到终极目标的话
			if(bUltraTarCanHit)
			{

			}
		}
		break;
	case eAIStrategy_ProtectSelf:
		{

		}
		break;
	case eAIStrategy_ProtectTarget:
		{

		}
		break;
	}
	return true;
}

bool AIMgr::SelectRoute()
{
	if (!m_pCurAI)
		return false;

	
	return true;
}