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
	//�Ѿ����ù�
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
		//mp����
		if(m_pCurAI->GetMP() < skill.m_nCost)
			continue;
		//�˺��ͼ���
		if(skill.m_nSkillType == 1)
		{
			//�ж��Ƿ���Թ�����
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
	//Ԥ����ɵ������˺�
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
	*	����κ�·�߶����������߲��������Ļ�����ִ�����ȹ���Ŀ���A����
	*	B���Լ����ڰ�ȫ���������ȵȴ�������λǰ����ʹ�Լ����ں���ǰ���������ȶ��Լ��ͷ�����ħ�������
	*	��������ȱ���Ŀ����ԣ�
	*	A�����Ҫ������Ŀ���Ƿ��ڱ�������Χ���ǵĻ����Լ��Ƿ���Թ����������в�ĵ�λ�����ԵĻ�����ȥ������
	*	����ѡ����һ��в��λ��ִ���������裬�����еĻ����������в��λǰ��
	*	B������Ŀ�괦�ڰ�ȫ��Χ��ֱ�ӿ����������򱣻�Ŀ��ʩ������ħ�������ܵĻ�ֱ�Ӵ���
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
				m_pCurAI = NULL;
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
				m_pCurAI = NULL;
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
					if(CanKillTarget((*it).m_iTarget,attackType,point) == true)
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
			//�����Լ��Ƿ��ڵз�Ŀ�깥����Χ
			std::vector<Block*> vLiveBlocks = CreatureManager::sInstance().GetLiveBlock(m_pCurAI);
			if(!vLiveBlocks.empty())
			{
				//ѡ��һ�����������ƶ���ͨ��ѡ���һ����
				ActionProcess* process = ActionProcess::sInstancePtr();
				DWORD data = vLiveBlocks[0]->xpos + (vLiveBlocks[0]->ypos << 8);
				process->PushAction(eNotify_Walk,m_pCurAI,NULL,data);
				//ʹ������ħ��
				//������ʱ����
				process->PushAction(eNotify_FinishAttack,m_pCurAI,NULL,0);
				m_pCurAI = NULL;
				return true;
			}
			else
			{
				//���ƶ���Χ���ǰ�ȫ�Ļ��߶��ǲ���ȫ�ģ���ִ�����ȹ�������
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
	for (std::vector<Block*>::reverse_iterator it=path.rbegin();it!=path.rend();it++)
	{
		int length = abs((*it)->xpos - m_pCurAI->GetBlock().xpos) + abs((*it)->ypos - m_pCurAI->GetBlock().ypos);
		if(length <= m_pCurAI->GetMoveAbility())
		{
			lastBlock = *it;
			break;
		}
	}
	//û�е�Ŀ���·��
	if(!lastBlock)
		return false;

	//push walk����
	ActionProcess* process = ActionProcess::sInstancePtr();
	DWORD data = lastBlock->xpos + (lastBlock->ypos << 8);
	process->PushAction(eNotify_Walk,m_pCurAI,NULL,data);
	return true;
}