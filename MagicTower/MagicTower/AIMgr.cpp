#include "AIMgr.h"
#include "Character.h"
#include "CreatureManager.h"
#include "MapManager.h"
#include "ActionProcess.h"
#include "ConfigManager.h"

AIMgr g_AIMgr;

AIMgr::AIMgr()
{
	m_pCurAI = NULL;
	m_eStrategy = eAIStrategy_AttakTarget;
}

AIMgr::~AIMgr()
{

}

void AIMgr::Init()
{
	m_pCurAI = NULL;
	m_eStrategy = eAIStrategy_AttakTarget;
}

void AIMgr::SetCurCharacter(Character* cha)
{
	//已经设置过
	if(m_pCurAI)
		return;

	m_pCurAI = cha;
	m_eStrategy = m_pCurAI->GetAIStrategy();
}

Character* AIMgr::GetCurCharacter()
{
	return m_pCurAI;
}

int AIMgr::SelectDamageSkill(Character* target,POINT attackPoint,bool ignoreAttackPoint)
{
	if(!m_pCurAI || !target)
		return -1;

	std::vector<int> skillList = m_pCurAI->GetSkillList();
	int skillDefend = target->GetSkillDefend();
	int expectedSkillDamage = 0;
	int expectedSkill = -1;
	for (std::vector<int>::iterator it=skillList.begin();it!=skillList.end();it++)
	{
		SkillInfo skill = ConfigManager::sInstance().GetSkillInfo().find(*it)->second;
		//mp不足
		if(m_pCurAI->GetMP() < skill.m_nCost)
			continue;
		//伤害型技能
		if(skill.m_nSkillType == 1)
		{
			//判断是否可以攻击到
			if(abs(target->GetBlock().xpos-attackPoint.x) + abs(target->GetBlock().ypos-attackPoint.y) <= skill.m_nCastRange || ignoreAttackPoint)
			{
				int skillDamage = skill.m_nAttack - skillDefend;
				if(skillDamage > expectedSkillDamage)
				{
					expectedSkill = skill.m_nID;
					expectedSkillDamage = skillDamage;
				}
			}
		}
	}
	return expectedSkill;
}

int AIMgr::GetDamageToTarget(Character* target,int& attack,POINT attackPoint,bool ignoreAttackPoint)
{
	if(!m_pCurAI || !target)
		return 0;

	int defend = target->GetDefend();
	float dodge = target->GetDodge();
	float crit = m_pCurAI->GetCrit();
	int attacks = m_pCurAI->GetAttack();
	int skillDefend = target->GetSkillDefend();
	//预期造成的物理伤害
	int expectedDamage = (1-dodge)*(crit*(attacks*2-defend) + (1-crit)*(attacks-defend));
	int skillID = SelectDamageSkill(target,attackPoint,ignoreAttackPoint);
	if(skillID == -1)
	{
		attack = -1;
		return expectedDamage;
	}
	SkillInfo skill = ConfigManager::sInstance().GetSkillInfo().find(skillID)->second;
	int skillDamage = skill.m_nAttack - skillDefend;
	if(skillDamage > expectedDamage)
	{
		attack = skillID;
		expectedDamage = skillDamage;
	}
	else
		attack = -1;

	return expectedDamage;
}

bool AIMgr::CanKillTarget(Character* target,int& attack,POINT attackPoint,bool ignoreAttackPoint)
{
	if(!m_pCurAI || !target)
		return false;

	int attackType = -1;
	int expectedDamage = GetDamageToTarget(target,attackType,attackPoint,ignoreAttackPoint);
	if(expectedDamage >= target->GetHP())
	{
		attack = attackType;
		return true;
	}
	return false;
}

bool AIMgr::DoAction()
{
	if(!m_pCurAI)
		return false;

	//根据策略确定行动
	/*
	*	如果是优先攻击目标策略（如果失败条件是己方全死，则目标单位就是列表中第一个单位）：
	*	A、找出移动+攻击范围内所有可以攻击到的敌方单位，
	*	如果有目标单位，则攻击之(为了性能起见，这里攻击时判断如果能使用技能则使用最高伤害的手段)；否则判断是否有期望攻击致死
	*	的单位，有则攻击之；否则攻击预期伤害最高的单位
	*	B、找不到敌方单位，则选择向目标单位前进，前进后使用增益技能，没有则待命
	*	如果是优先保护自己策略：
	*	A、查找自己是否处于敌方目标攻击范围，且是否会被攻击致死(对于不同兵种，采取不同算法计算
	*	被攻击伤害，魔法系取能释放的最高伤害技能)，会的话，优先选择路线离开，并使用增益魔法；
	*	如果任何路线都可能死或者不会致死的话，则执行优先攻击目标的A策略
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
			//攻击范围内没有敌方单位，选择向终极目标移动
			if (targets.empty())
			{
				SelectRoute(ultraTarget);
				//使用增益技能
				//这里暂时不用
				
				ActionProcess* process = ActionProcess::sInstancePtr();
				process->PushAction(eNotify_FinishAttack,m_pCurAI,NULL,0);
				m_pCurAI = NULL;
				return true;
			}
			//在所有可攻击敌人中发现终极目标
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
			ActionProcess* process = ActionProcess::sInstancePtr();
			//如果可以直接攻击到终极目标的话
			if(bUltraTarCanHit)
			{
				DWORD data = attackPoint.x + (attackPoint.y << 8);
				process->PushAction(eNotify_Walk,m_pCurAI,NULL,data);
				CreatureManager::sInstance().PreAttackAndPushAction(m_pCurAI,ultraTarget,data);
			}
			else
			{
				int attackType;
				//挑选可以直接击杀的单位
				for (VAttackTarget::iterator it=targets.begin();it!=targets.end();it++)
				{
					attackType = -1;
					POINT point;
					point.x = (*it).m_iAttakPoint.x;
					point.y = (*it).m_iAttakPoint.y;
					if(CanKillTarget((*it).m_iTarget,attackType,point) == true)
					{
						bUltraTarCanHit = true;
						attackPoint.x = (*it).m_iAttakPoint.x;
						attackPoint.y = (*it).m_iAttakPoint.y;
						ultraTarget = (*it).m_iTarget;
						break;
					}
				}
				//找到了
				if(bUltraTarCanHit)
				{
					DWORD data = attackPoint.x + (attackPoint.y << 8);
					process->PushAction(eNotify_Walk,m_pCurAI,NULL,data);
					if(attackType == -1)
						CreatureManager::sInstance().PreAttackAndPushAction(m_pCurAI,ultraTarget,data);
					else
					{
						m_pCurAI->GetCastSkill() = attackType;
						CreatureManager::sInstance().PreSkillAndPushAction(m_pCurAI,ultraTarget,data);
					}
				}
				//依旧没找到，则寻找预期造成伤害最高的
				else
				{
					int attackType;
					int expectedHigeDamage = 0;
					for (VAttackTarget::iterator it=targets.begin();it!=targets.end();it++)
					{
						POINT point;
						point.x = (*it).m_iAttakPoint.x;
						point.y = (*it).m_iAttakPoint.y;
						attackType = -1;
						int expectedDamage = GetDamageToTarget((*it).m_iTarget,attackType,point);
						if(expectedHigeDamage < expectedDamage)
						{
							expectedHigeDamage = expectedDamage;
							attackPoint.x = (*it).m_iAttakPoint.x;
							attackPoint.y = (*it).m_iAttakPoint.y;
							ultraTarget = (*it).m_iTarget;
						}
					}
					DWORD data = attackPoint.x + (attackPoint.y << 8);
					process->PushAction(eNotify_Walk,m_pCurAI,NULL,data);
					if(attackType == -1)
						CreatureManager::sInstance().PreAttackAndPushAction(m_pCurAI,ultraTarget,data);
					else
					{
						m_pCurAI->GetCastSkill() = attackType;
						CreatureManager::sInstance().PreSkillAndPushAction(m_pCurAI,ultraTarget,data);
					}
				}
			}
		}
		break;
	case eAIStrategy_ProtectSelf:
		{
			//查找自己是否处于敌方目标攻击范围
			std::vector<Block*> vLiveBlocks = CreatureManager::sInstance().GetLiveBlock(m_pCurAI);
			if(!vLiveBlocks.empty())
			{
				//选择一个生存点进行移动，通常选择第一个点
				ActionProcess* process = ActionProcess::sInstancePtr();
				DWORD data = vLiveBlocks[0]->xpos + (vLiveBlocks[0]->ypos << 8);
				process->PushAction(eNotify_Walk,m_pCurAI,NULL,data);
				//使用增益魔法
				//这里暂时不用
				process->PushAction(eNotify_FinishAttack,m_pCurAI,NULL,0);
				m_pCurAI = NULL;
				return true;
			}
			else
			{
				//在移动范围内是安全的或者都是不安全的，故执行优先攻击策略
			}
		}
		break;
	case eAIStrategy_ProtectTarget:
		{

		}
		break;
	}
	return true;
}

bool AIMgr::SelectRoute(Character* target)
{
	if (!m_pCurAI)
		return false;

	//首先将单位假象为行动范围为整个地图
	//然后寻找到目标点之间的路径
	//然后根据自身真实的行动能力，向着路径移动

	//**这里有一点需要注意，就是终点是一个单位占据的点，故寻路时需要先把该点的占据状态改为空，寻路完成后再改回来，否则找不到路径
	setOccupied(MapManager::sInstance().GetCurrentMap()->GetBlock(target->GetBlock().xpos,target->GetBlock().ypos)->attri,0);

	std::vector<Block*> range = m_pCurAI->CreateRange(MapManager::sInstance().GetCurrentMap(),0,false,true);
	MapManager::sInstance().GetCurrentMap()->SetSpecificRange(range);
	std::vector<Block*> path = MapManager::sInstance().GetCurrentMap()->FindPath(m_pCurAI->GetBlock().xpos,m_pCurAI->GetBlock().ypos,target->GetBlock().xpos,target->GetBlock().ypos);

	//寻路完成后，将终点占据状态改回来，以免发生异常
	setOccupied(MapManager::sInstance().GetCurrentMap()->GetBlock(target->GetBlock().xpos,target->GetBlock().ypos)->attri,1);

	//真实的路径上的终点，是考虑单位移动能力的
	Block* lastBlock = NULL;
	for (std::vector<Block*>::reverse_iterator it=path.rbegin();it!=path.rend();it++)
	{
		int length = abs((*it)->xpos - m_pCurAI->GetBlock().xpos) + abs((*it)->ypos - m_pCurAI->GetBlock().ypos);
		if(length <= m_pCurAI->GetMoveAbility())
		{
			lastBlock = *it;
			break;
		}
	}
	//没有到目标的路径
	if(!lastBlock)
		return false;

	//push walk动作
	ActionProcess* process = ActionProcess::sInstancePtr();
	DWORD data = lastBlock->xpos + (lastBlock->ypos << 8);
	process->PushAction(eNotify_Walk,m_pCurAI,NULL,data);
	return true;
}