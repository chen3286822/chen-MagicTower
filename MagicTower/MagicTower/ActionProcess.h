#ifndef ACTION_PROCESS_H
#define ACTION_PROCESS_H

#include "commonTools.h"
#include "Character.h"
#include "CreatureManager.h"

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
	}
	~ActionProcess(){}

	void PushAction(eNotification notify,Character* cast,Character* target,DWORD time)
	{
		if(!cast || !target)
			return;
		m_lAction.push_back(Action(notify,cast,target,time));
	}
	void PopAction(){m_lAction.pop_front();}
	void TimeUp()
	{
		m_bTimeUp = true;
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
				case eNotify_BeginAttack:
					{
						bool bCrit = false;
						if (g_RandomInt(0,9) < (int)(action.m_pCast->GetCrit()*10))
							bCrit = true;
						CreatureManager::sInstance().CalculateHurt(action.m_pCast,action.m_pTarget,bCrit);
						if(bCrit)
						{
							action.m_pCast->Crit(eNotify_Crit,action.m_pTarget,1000);
							m_eActionState = eActionState_Process;
						}
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
			if(m_bTimeUp)
			{
				m_eActionState = eActionState_End;
			}
		}
		else if (m_eActionState == eActionState_End)
		{
			m_iCurAction.Clear();
			PopAction();
		}


	}
private:
	std::list<Action> m_lAction;
	Action m_iCurAction;
	eActionState m_eActionState;
	bool m_bTimeUp;
};

#endif