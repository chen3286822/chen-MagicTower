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
	//�Ѿ����ù�
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
		//mp����
		if(cast->GetMP() < skill.m_nCost)
			continue;
		//�˺��ͼ���
		if(skill.m_nSkillType == 1)
		{
			//�ж��Ƿ���Թ�����
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
	//Ԥ����ɵ������˺�
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
	//��ʾ�Ƿ����ֱ�ӹ������ռ�Ŀ��
	bool bUltraTarCanHit = false;
	//���Թ������ռ�Ŀ���վλ��
	POINT attackPoint;
	Character* ultraTarget = NULL;//�ռ�Ŀ�꣬�ɵ������൱��ʤ����
	//�趨���ڵ��ռ�Ŀ��
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
	//�ռ�Ŀ�����û�У���ô�����λ����ֹͣ�ж���
	if(!ultraTarget)
	{
		m_pCurAI->SetFinish(true);
		return true;
	}
	//������Χ��û�ез���λ��ѡ�����ռ�Ŀ���ƶ�
	if (targets.empty())
	{
		SelectRoute(ultraTarget);
		//ʹ�����漼��
		//������ʱ����

		ActionProcess* process = ActionProcess::sInstancePtr();
		process->PushAction(eNotify_FinishAttack,m_pCurAI,NULL,0);
		return true;
	}
	//�����пɹ��������з����ռ�Ŀ��
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
	//�������ֱ�ӹ������ռ�Ŀ��Ļ�
	if(bUltraTarCanHit)
	{
		DWORD data = attackPoint.x + (attackPoint.y << 8);
		process->PushAction(eNotify_Walk,m_pCurAI,NULL,data);
		CreatureManager::sInstance().PreAttackAndPushAction(m_pCurAI,ultraTarget,data);
	}
	else
	{
		int attackType;
		//��ѡ����ֱ�ӻ�ɱ�ĵ�λ
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
		//�ҵ���
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
		//����û�ҵ�����Ѱ��Ԥ������˺���ߵ�
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

	//���ݲ���ȷ���ж�
	/*
	*	��������ȹ���Ŀ����ԣ����ʧ�������Ǽ���ȫ������Ŀ�굥λ�����б��е�һ����λ����
	*	A���ҳ��ƶ�+������Χ�����п��Թ������ĵз���λ��
	*	�����Ŀ�굥λ���򹥻�֮(Ϊ��������������﹥��ʱ�ж������ʹ�ü�����ʹ������˺����ֶ�)�������ж��Ƿ���������������
	*	�ĵ�λ�����򹥻�֮�����򹥻�Ԥ���˺���ߵĵ�λ
	*	B���Ҳ����з���λ����ѡ����Ŀ�굥λǰ����ǰ����ʹ�����漼�ܣ�û�������
	*	��������ȱ����Լ����ԣ�
	*	A�������Լ��Ƿ��ڵз�Ŀ�깥����Χ�����Ƿ�ᱻ��������(���ڲ�ͬ���֣���ȡ��ͬ�㷨����
	*	�������˺���ħ��ϵȡ���ͷŵ�����˺�����)����Ļ�������ѡ��·���뿪����ʹ������ħ����
	*	����κ�·�߶����������߲��������Ļ�����ִ�����ȹ���Ŀ��Ĳ���(�Ƹ�����)
	*	B���Լ����ڰ�ȫ���������ȵȴ�������λǰ����ʹ�Լ����ں���ǰ���������ȶ���Χ�����ͷ�����ħ�������
	*	��������ȱ���Ŀ����ԣ�
	*	A�����Ҫ������Ŀ���Ƿ��ڱ�������Χ���ǵĻ����Լ��Ƿ���Թ����������в�ĵ�λ�����ԵĻ�����ȥ������
	*	����ѡ����һ��в��λ��ִ���������裬�����еĻ����������в��λǰ��
	*	B������Ŀ�괦�ڰ�ȫ��Χ��ֱ�ӿ����������򱣻�Ŀ��ʩ������ħ�������ܵĻ�ֱ�Ӵ���
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
			//�����Լ��Ƿ��ڵз�Ŀ�깥����Χ
			std::vector<Block*> vLiveBlocks = CreatureManager::sInstance().GetLiveBlock(m_pCurAI,allSafe);
			if(allSafe)
			{
				//������԰�ȫ�����Ը���ǿ�����ѣ��Թ������˸���Χ����ʩ�Ź���������ħ��
				//������ʱ����
				//Ȼ�����
				process->PushAction(eNotify_FinishAttack,m_pCurAI,NULL,0);
				bResult = true;
			}
			else
			{
				if(!vLiveBlocks.empty())
				{
					//ѡ��һ�����������ƶ���ͨ��ѡ�����һ���㣬��Ϊ�����ܴ�������Լ���ԭ�����꣬�������ƶ���������
					DWORD data = vLiveBlocks.back()->xpos + (vLiveBlocks.back()->ypos << 8);
					process->PushAction(eNotify_Walk,m_pCurAI,NULL,data);
					//���Լ�ʹ�÷���������ħ��
					//������ʱ����
					process->PushAction(eNotify_FinishAttack,m_pCurAI,NULL,0);
					bResult = true;
				}
				else
				{
					//���ƶ���Χ�ڶ��ǲ���ȫ�ģ���ִ�����ȹ�������
					bResult = StrategyAttackTarget();
				}
			}
		}
		break;
	case eAIStrategy_ProtectTarget:
		{
			//Ҫ������Ŀ��
			int num = m_iStrategy.m_dwData;
			Character* target = CreatureManager::sInstance().GetCreature(num);
			if(!target || 
				((target->GetCamp()==eCamp_Enemy && m_pCurAI->GetCamp()==eCamp_Friend)||
				(target->GetCamp()==eCamp_Friend && m_pCurAI->GetCamp()==eCamp_Enemy)))
			{
				//û��Ҫ������Ŀ����߱�����Ŀ�괦�ڲ�ͬ��Ӫ(���ô���)����ô���˵�λ�Ĳ�����Ϊ���ȹ����ͣ���������λ�ж�
				m_pCurAI->SetAIStrategy(eAIStrategy_AttakTarget);
				process->PushAction(eNotify_FinishAttack,m_pCurAI,NULL,0);
				bResult = true;
			}
			else
			{
				VCharacter attackers = CreatureManager::sInstance().GetAttackersToTarget(target);
				if(attackers.empty())
				{
					//Ŀ�괦�ڰ�ȫ��Χ������Ŀ�꿿£
					SelectRoute(target);
					//��Ŀ��ʩ������ħ��
					//������ʱ����
					process->PushAction(eNotify_FinishAttack,m_pCurAI,NULL,0);
					bResult = true;
				}
				else
				{
					//����вĿ�꣬���Ȱ�����в��С��������
					gtar = target;
					std::sort(attackers.begin(),attackers.end(),DamageCompare);
					gtar = NULL;
					//ѡ�񹥻��ܹ������������в�ĵ�λ
					bool hasAttacked = false;
					for (VCharacter::iterator vit=attackers.begin();vit!=attackers.end();vit++)
					{
						POINT pt;
						if(CanHitPointOrTarget(m_pCurAI,(*vit),NULL,pt))
						{
							//���Դ򵽣��߹�ȥ����
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
		//��������AIû��ִ��
		g_debugString(__FILE__,__FUNCDNAME__,__LINE__,"AIִ�з�������");
	}
	return bResult;
}

bool AIMgr::SelectRoute(Character* target)
{
	if (!m_pCurAI)
		return false;

	//���Ƚ���λ����Ϊ�ж���ΧΪ������ͼ
	//Ȼ��Ѱ�ҵ�Ŀ���֮���·��
	//Ȼ�����������ʵ���ж�����������·���ƶ�

	//**������һ����Ҫע�⣬�����յ���һ����λռ�ݵĵ㣬��Ѱ·ʱ��Ҫ�ȰѸõ��ռ��״̬��Ϊ�գ�Ѱ·��ɺ��ٸĻ����������Ҳ���·��
	setOccupied(MapManager::sInstance().GetCurrentMap()->GetBlock(target->GetBlock().xpos,target->GetBlock().ypos)->attri,0);

	std::vector<Block*> range = m_pCurAI->CreateRange(MapManager::sInstance().GetCurrentMap(),0,false,true);
	MapManager::sInstance().GetCurrentMap()->SetSpecificRange(range);
	std::vector<Block*> path = MapManager::sInstance().GetCurrentMap()->FindPath(m_pCurAI->GetBlock().xpos,m_pCurAI->GetBlock().ypos,target->GetBlock().xpos,target->GetBlock().ypos);

	//Ѱ·��ɺ󣬽��յ�ռ��״̬�Ļ��������ⷢ���쳣
	setOccupied(MapManager::sInstance().GetCurrentMap()->GetBlock(target->GetBlock().xpos,target->GetBlock().ypos)->attri,1);

	//��ʵ��·���ϵ��յ㣬�ǿ��ǵ�λ�ƶ�������
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
	//û�е�Ŀ���·��
	if(path.empty())
		return false;

	lastBlock = path.back();
	//��·�������ǲ�������ߵ�Ŀ������ˣ���Ϊ�Ѿ���Ŀ��ռ����
	if(lastBlock->xpos == target->GetBlock().xpos && lastBlock->ypos == target->GetBlock().ypos)
	{
		path.pop_back();
		if(path.empty())
			return false;
		lastBlock = path.back();
	}

	//push walk����
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
					//�ж��Ƿ��ܻ���Ŀ��
					if(target)
					{
						if(target->GetBlock().xpos == tarX && target->GetBlock().ypos == tarY)
						{
							pt.x = (*it)->xpos;
							pt.y = (*it)->ypos;
							return true;
						}
					}
					//�ж��Ƿ��ܻ��е�
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