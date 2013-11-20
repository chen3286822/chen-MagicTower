#include "AIMgr.h"
#include "Character.h"
#include "CreatureManager.h"
#include "MapManager.h"
#include "ActionProcess.h"
#include "ConfigManager.h"

AIMgr g_AIMgr;

Character* gtar = NULL;
bool DamageCompare(Character* cha1,Character* cha2)
{
	POINT pt;
	pt.x = pt.y = 0;
	int attack = -1;
	int damage1 = g_AIMgr.GetDamageToTarget(cha1,gtar,attack,pt,true);
	int damage2 = g_AIMgr.GetDamageToTarget(cha2,gtar,attack,pt,true);
	return (damage1 > damage2);
}

AIMgr::AIMgr()
{
	m_pCurAI = NULL;
	m_iStrategy.m_eStrategy = eAIStrategy_AttakTarget;
}

AIMgr::~AIMgr()
{

}

void AIMgr::Init()
{
	m_pCurAI = NULL;
	m_iStrategy.m_eStrategy = eAIStrategy_AttakTarget;
}

void AIMgr::SetCurCharacter(Character* cha)
{
	//已经设置过
	if(m_pCurAI)
		return;

	m_pCurAI = cha;
	m_iStrategy = m_pCurAI->GetAIStrategy();
}

Character* AIMgr::GetCurCharacter()
{
	return m_pCurAI;
}

int AIMgr::SelectDamageSkill(Character* cast,Character* target,POINT attackPoint,bool ignoreAttackPoint)
{
	if(!cast || !target)
		return -1;

	std::vector<int> skillList = cast->GetSkillList();
	int skillDefend = target->GetSkillDefend();
	int expectedSkillDamage = 0;
	int expectedSkill = -1;
	for (std::vector<int>::iterator it=skillList.begin();it!=skillList.end();it++)
	{
		SkillInfo skill = ConfigManager::sInstance().GetSkillInfo().find(*it)->second;
		//mp不足
		if(cast->GetMP() < skill.m_nCost)
			continue;
		//伤害型技能
		if(skill.m_nSkillType == 1)
		{
			//判断是否可以攻击到
			if(ignoreAttackPoint || abs(target->GetBlock().xpos-attackPoint.x) + abs(target->GetBlock().ypos-attackPoint.y) <= skill.m_nCastRange)
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

int AIMgr::GetDamageToTarget(Character* cast,Character* target,int& attack,POINT attackPoint,bool ignoreAttackPoint)
{
	if(!cast || !target)
		return 0;

	int defend = target->GetDefend();
	float dodge = target->GetDodge();
	float crit = cast->GetCrit();
	int attacks = cast->GetAttack();
	int skillDefend = target->GetSkillDefend();
	//预期造成的物理伤害
	int expectedDamage = (1-dodge)*(crit*(attacks*2-defend) + (1-crit)*(attacks-defend));
	int skillID = SelectDamageSkill(cast,target,attackPoint,ignoreAttackPoint);
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

bool AIMgr::CanKillTarget(Character* cast,Character* target,int& attack,POINT attackPoint,bool ignoreAttackPoint)
{
	if(!cast || !target)
		return false;

	int attackType = -1;
	int expectedDamage = GetDamageToTarget(cast,target,attackType,attackPoint,ignoreAttackPoint);
	if(expectedDamage >= target->GetHP())
	{
		attack = attackType;
		return true;
	}
	return false;
}

bool AIMgr::StrategyAttackTarget()
{
	Map* curMap = MapManager::sInstance().GetCurrentMap();
	if(!curMap)
		return false;

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
			if(CanKillTarget(m_pCurAI,(*it).m_iTarget,attackType,point) == true)
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
				int expectedDamage = GetDamageToTarget(m_pCurAI,(*it).m_iTarget,attackType,point);
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
	return true;
}

bool AIMgr::DoAction()
{
	bool bResult = false;
	if(!m_pCurAI)
		return bResult;

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
	*	如果任何路线都可能死或者不会致死的话，则执行优先攻击目标的策略(破釜沉舟)
	*	B、自己处于安全区域，则优先等待其他单位前进，使自己处于后方再前进，并优先对周围队友释放增益魔法后待命
	*	如果是优先保护目标策略：
	*	A、检查要保护的目标是否处于被攻击范围，是的话，自己是否可以攻击到最具威胁的单位，可以的话，过去攻击，
	*	否则选择下一威胁单位，执行上述步骤，都不行的话，向最具威胁单位前进
	*	B、保护目标处于安全范围则直接靠近，优先向保护目标施放增益魔法，不能的话直接待命
	*/
	ActionProcess* process = ActionProcess::sInstancePtr();
	switch(m_iStrategy.m_eStrategy)
	{
	case eAIStrategy_AttakTarget:
		{
			bResult = StrategyAttackTarget();
		}
		break;
	case eAIStrategy_ProtectSelf:
		{
			bool allSafe = false;
			//查找自己是否处于敌方目标攻击范围
			std::vector<Block*> vLiveBlocks = CreatureManager::sInstance().GetLiveBlock(m_pCurAI,allSafe);
			if(allSafe)
			{
				//由于相对安全，可以辅助强化队友，以攻击敌人给周围队友施放攻击性增益魔法
				//这里暂时不用
				//然后待命
				process->PushAction(eNotify_FinishAttack,m_pCurAI,NULL,0);
				bResult = true;
			}
			else
			{
				if(!vLiveBlocks.empty())
				{
					//选择一个生存点进行移动，通常选择最后一个点，因为这个点很大可能是自己的原来坐标，淡定不移动才是王道
					DWORD data = vLiveBlocks.back()->xpos + (vLiveBlocks.back()->ypos << 8);
					process->PushAction(eNotify_Walk,m_pCurAI,NULL,data);
					//给自己使用防御性增益魔法
					//这里暂时不用
					process->PushAction(eNotify_FinishAttack,m_pCurAI,NULL,0);
					bResult = true;
				}
				else
				{
					//在移动范围内都是不安全的，故执行优先攻击策略
					bResult = StrategyAttackTarget();
				}
			}
		}
		break;
	case eAIStrategy_ProtectTarget:
		{
			//要保护的目标
			int num = m_iStrategy.m_dwData;
			Character* target = CreatureManager::sInstance().GetCreature(num);
			if(!target || 
				((target->GetCamp()==eCamp_Enemy && m_pCurAI->GetCamp()==eCamp_Friend)||
				(target->GetCamp()==eCamp_Friend && m_pCurAI->GetCamp()==eCamp_Enemy)))
			{
				//没有要保护的目标或者保护的目标处于不同阵营(设置错误)，那么将此单位的策略设为优先攻击型，并结束单位行动
				m_pCurAI->SetAIStrategy(eAIStrategy_AttakTarget);
				process->PushAction(eNotify_FinishAttack,m_pCurAI,NULL,0);
				bResult = true;
			}
			else
			{
				VCharacter attackers = CreatureManager::sInstance().GetAttackersToTarget(target);
				if(attackers.empty())
				{
					//目标处于安全范围，则向目标靠拢
					SelectRoute(target);
					//向目标施放增益魔法
					//这里暂时不做
					process->PushAction(eNotify_FinishAttack,m_pCurAI,NULL,0);
					bResult = true;
				}
				else
				{
					//有威胁目标，首先按照威胁大小进行排序
					gtar = target;
					std::sort(attackers.begin(),attackers.end(),DamageCompare);
					gtar = NULL;
					//选择攻击能攻击到的最具威胁的单位
					bool hasAttacked = false;
					for (VCharacter::iterator vit=attackers.begin();vit!=attackers.end();vit++)
					{
						POINT pt;
						if(CanHitPointOrTarget(m_pCurAI,(*vit),NULL,pt))
						{
							//可以打到，走过去攻击
							DWORD data = pt.x + (pt.y << 8);
							process->PushAction(eNotify_Walk,m_pCurAI,NULL,data);
							
							int attack = -1;
							GetDamageToTarget(m_pCurAI,(*vit),attack,pt);
							if(attack == -1)
								CreatureManager::sInstance().PreAttackAndPushAction(m_pCurAI,(*vit),data);
							else
							{
								m_pCurAI->GetCastSkill() = attack;
								CreatureManager::sInstance().PreSkillAndPushAction(m_pCurAI,(*vit),data);
							}
							bResult = true;
							hasAttacked = true;
							break;
						}
					}
					if(!hasAttacked)
					{
						SelectRoute(attackers.front());
						process->PushAction(eNotify_FinishAttack,m_pCurAI,NULL,0);
						bResult = true;
					}
				}
			}
		}
		break;
	}
	if(bResult)
		m_pCurAI = NULL;
	else
	{
		//发生错误，AI没有执行
		g_debugString(__FILE__,__FUNCDNAME__,__LINE__,"AI执行发生错误");
	}
	return bResult;
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
	for (std::vector<Block*>::iterator it=path.begin();it!=path.end();)
	{
		int length = abs((*it)->xpos - m_pCurAI->GetBlock().xpos) + abs((*it)->ypos - m_pCurAI->GetBlock().ypos);
		if(length > m_pCurAI->GetMoveAbility())
		{
			it = path.erase(it);
		}
		else
			it++;
	}
	//没有到目标的路径
	if(path.empty())
		return false;

	lastBlock = path.back();
	//有路径，但是不能真的走到目标点上了，因为已经被目标占据了
	if(lastBlock->xpos == target->GetBlock().xpos && lastBlock->ypos == target->GetBlock().ypos)
	{
		path.pop_back();
		if(path.empty())
			return false;
		lastBlock = path.back();
	}

	//push walk动作
	ActionProcess* process = ActionProcess::sInstancePtr();
	DWORD data = lastBlock->xpos + (lastBlock->ypos << 8);
	process->PushAction(eNotify_Walk,m_pCurAI,NULL,data);
	return true;
}

bool AIMgr::CanHitPointOrTarget(Character* cast,Character* target,Block* point,POINT& pt)
{
	if(!cast)
		return false;

	bool surveyMap[MAX_MAP_WIDTH_NUM][MAX_MAP_LENGTH_NUM];
	memset(surveyMap,false,sizeof(bool)*MAX_MAP_WIDTH_NUM*MAX_MAP_LENGTH_NUM);


	std::vector<Block*> range = cast->GetMoveRange();
	for (std::vector<Block*>::iterator it=range.begin();it!=range.end();it++)
	{
		int tarX = 0,tarY = 0;
		VPair vPairPoint = CreatureManager::sInstance().GetRangePoint();
		for (MAttackRange::iterator mit=CreatureManager::sInstance().GetAttackRange().begin();mit!=CreatureManager::sInstance().GetAttackRange().end();mit++)
		{
			if(mit->first == cast->GetAttackRange())
			{
				for (vector<int>::iterator it2=mit->second.begin();it2!=mit->second.end();it2++)
				{
					tarX = vPairPoint[*it2].x + (*it)->xpos;
					tarY = vPairPoint[*it2].y + (*it)->ypos;

					if(surveyMap[tarX][tarY])
						continue;
					//判断是否能击中目标
					if(target)
					{
						if(target->GetBlock().xpos == tarX && target->GetBlock().ypos == tarY)
						{
							pt.x = (*it)->xpos;
							pt.y = (*it)->ypos;
							return true;
						}
					}
					//判断是否能击中点
					else
					{
						if(point->xpos == tarX && point->ypos == tarY)
						{
							pt.x = (*it)->xpos;
							pt.y = (*it)->ypos;
							return true;
						}
					}
					surveyMap[tarX][tarY] = true;
				}
			}
		}
	}
	
	return false;
}