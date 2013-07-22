#ifndef ACTION_PROCESS_H
#define ACTION_PROCESS_H

#include "commonTools.h"
#include "Character.h"
#include "CreatureManager.h"
#include "ConfigManager.h"

typedef eNotification (Character::*LPActionFunc)(Character*,DWORD);

struct Action
{
	eNotification m_eNotify;
	Character* m_pCast;
	Character* m_pTarget;
	DWORD m_dwTime;
	Action()
	{
		m_eNotify = eNotify_Success;
		m_pCast = NULL;
		m_pTarget = NULL;
		m_dwTime = 0;
	}
	Action(eNotification notify,Character* cast,Character* target,DWORD time)
	{
		m_eNotify = notify;
		m_pCast = cast;
		m_pTarget = target;
		m_dwTime = time;
	}
	void Clear()
	{
		m_eNotify = eNotify_Success;
		m_pCast = NULL;
		m_pTarget = NULL;
		m_dwTime = 0;
	}
};

enum eActionState
{
	eActionState_PickAction = 0,
	eActionState_Process,
	eActionState_End,
};

class ActionProcess : public Singleton<ActionProcess>
{
public:
	ActionProcess()
	{
		m_lAction.clear();
		m_eActionState = eActionState_PickAction;
		m_iCurAction.Clear();
		m_nLeftTime = -1;
	}
	~ActionProcess(){}

	void PushAction(eNotification notify,Character* cast,Character* target,DWORD time)
	{
		if(!cast || !target)
			return;
		m_lAction.push_back(Action(notify,cast,target,time));

		//test
		char temp[10];
		sprintf(temp,"%d ",notify);
		OutputDebugString(temp);
	}
	void PopAction(){m_lAction.pop_front();}
	void TimeUp(DWORD leftTime)
	{
		m_nLeftTime = leftTime;
	}
	void Update()
	{
		if (m_eActionState == eActionState_PickAction)
		{
			if(!m_lAction.empty())
			{
				Action action = m_lAction.front();
				switch (action.m_eNotify)
				{
				case eNotify_TowardToAttacker:
					{
						action.m_pCast->TowardToAttacker(eNotify_TowardToAttacker,action.m_pTarget,action.m_dwTime);
						if(action.m_dwTime == 0)
							m_eActionState = eActionState_End;
						else
							m_eActionState = eActionState_Process;
					}
					break;
				case eNotify_Crit:
					{
						action.m_pCast->Crit(eNotify_Crit,action.m_pTarget,action.m_dwTime);
						m_eActionState = eActionState_Process;
					}
					break;
				case eNotify_Attack:
					{
						action.m_pCast->Attack(eNotify_Attack,action.m_pTarget,0);
						action.m_pTarget->Attacked(eNotify_Attack,action.m_pCast,action.m_dwTime);

						m_eActionState = eActionState_Process;
					}
					break;
				case eNotify_AttackDefend:
					{
						action.m_pCast->Attack(eNotify_Attack,action.m_pTarget,0);
						action.m_pTarget->Defend(eNotify_Attack,action.m_pCast,action.m_dwTime);

						m_eActionState = eActionState_Process;
					}
					break;
				case eNotify_Dead:
					{
						action.m_pTarget->Dead(eNotify_Dead,action.m_pCast,action.m_dwTime);
						m_eActionState = eActionState_Process;
					}
					break;
				case eNotify_FinishAttack:
					{
						m_eActionState = eActionState_End;
					}
					break;
				case eNotify_CastAction:
					{
						action.m_pCast->Attack(eNotify_Attack,action.m_pTarget,action.m_dwTime);
						//action.m_pTarget->Attacked(eNotify_Attack,action.m_pCast,0);

						m_eActionState = eActionState_Process;
					}
					break;
				case eNotify_CastSkill:
					{
						SkillManager::sInstance().CreateSkill(action.m_pCast->GetCastSkill(),CreatureManager::sInstance().GetSkillTargets());

						m_eActionState = eActionState_Process;
					}
					break;
				case eNotify_Attacked:
					{
						action.m_pTarget->Attacked(eNotify_Attack,action.m_pCast,action.m_dwTime);

						m_eActionState = eActionState_Process;
					}
					break;
				case eNotify_Healed:
					{
						action.m_pTarget->Healed(eNotify_Attack,action.m_pCast,action.m_dwTime);

						m_eActionState = eActionState_Process;
					}
					break;
				case eNotify_UseItem:
					{
						action.m_pCast->UseItem(eNotify_UseItem,action.m_pTarget,action.m_dwTime);

						m_eActionState = eActionState_Process;
					}
					break;
				case eNotify_ItemEffect:
					{
						action.m_pTarget->Healed(eNotify_ItemEffect,action.m_pCast,action.m_dwTime);

						m_eActionState = eActionState_Process;
					}
					break;
				default:
					break;
				}

				
				m_iCurAction = action;
			}
		}
		else if (m_eActionState == eActionState_Process)
		{
			Action action = m_lAction.front();
			switch (action.m_eNotify)
			{
			case eNotify_Crit:
				{
					int nTempColor = (int)(255*m_nLeftTime/1000);
					DWORD dwCritColor = (nTempColor + (nTempColor << 8)) | 0xFFFF0000;
					action.m_pCast->ChangeColor(dwCritColor);
				}
				break;
			default:
				break;
			}
			if(m_nLeftTime==0)
			{
				m_nLeftTime = -1;
				m_eActionState = eActionState_End;
			}
		}
		else if (m_eActionState == eActionState_End)
		{
			Action action = m_lAction.front();
			switch (action.m_eNotify)
			{
			case eNotify_TowardToAttacker:
				{
					action.m_pCast->GetCharacterState() = eCharacterState_Stand;
				}
				break;
			case eNotify_Crit:
				{
					action.m_pCast->ChangeColor(0xFFFFFFFF);
				}
				break;
			case eNotify_Attack:
				{
					action.m_pCast->ResetFrame();
					action.m_pTarget->ResetFrame();
					action.m_pTarget->GetHP() -= action.m_pTarget->GetPreHurt();
					action.m_pTarget->GetPreHurt() = 0;
				}
				break;
			case eNotify_AttackDefend:
				{
					action.m_pCast->ResetFrame();
					action.m_pTarget->ResetFrame();
				}
				break;
			case eNotify_Dead:
				{
					action.m_pTarget->GetDead() = true;
				}
				break;
			case eNotify_FinishAttack:
				{
					action.m_pCast->SetFinish(true);
					action.m_pCast->GetCastSkill() = -1;
					action.m_pCast->GetUseItem() = -1;
				}
				break;
			case eNotify_CastAction:
				{
					action.m_pCast->ResetFrame();	
				}
				break;
			case eNotify_CastSkill:
				{
					SkillInfo skill = ConfigManager::sInstance().GetSkillInfo().find(action.m_pCast->GetCastSkill())->second;
					action.m_pCast->GetMP() -= skill.m_nCost;
				}
				break;
			case eNotify_Attacked:
				{
					action.m_pTarget->ResetFrame();
					action.m_pTarget->GetHP() -= action.m_pTarget->GetPreHurt();
					action.m_pTarget->GetPreHurt() = 0;
				}
				break;
			case eNotify_Healed:
				{
					SkillInfo skill = ConfigManager::sInstance().GetSkillInfo().find(action.m_pCast->GetCastSkill())->second;
					action.m_pTarget->ResetFrame();
					if(skill.m_nSkillType == eSkillType_Heal)
						action.m_pTarget->GetHP() += action.m_pTarget->GetPreHurt();
					else if (skill.m_nSkillType == eSkillType_Buff)
					{
						int buffType = action.m_pTarget->GetPreHurt()/1000;
						int buffValue = action.m_pTarget->GetPreHurt()%1000;
						action.m_pTarget->GetBuffData().push_back(BuffData(buffType,buffValue,skill.m_nLastTurn));
						action.m_pTarget->SetAttributeValue(buffType,buffValue);
					}
					action.m_pTarget->GetPreHurt() = 0;
				}
				break;
			case eNotify_UseItem:
				{
					action.m_pCast->ResetFrame();
					CreatureManager::sInstance().RemoveItem(action.m_pCast->GetUseItem());
				}
				break;
			case eNotify_ItemEffect:
				{
					action.m_pTarget->ResetFrame();
					//物品生效
					Item item = ConfigManager::sInstance().GetItemInfo().find(action.m_pCast->GetUseItem())->second;
					action.m_pTarget->ItemEffect(item);
					action.m_pCast->GetUseItem() = -1;
				}
				break;
			default:
				break;
			}
			m_iCurAction.Clear();
			m_eActionState = eActionState_PickAction;
			PopAction();
		}


	}
private:
	std::list<Action> m_lAction;
	Action m_iCurAction;
	eActionState m_eActionState;
	int m_nLeftTime; 
};

#endif