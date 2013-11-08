#ifndef AIMGR_H
#define AIMGR_H
#include "commonTools.h"


class Character;
class AIMgr
{
public:
	AIMgr();
	~AIMgr();

	//设置当前要进行AI行动的单位
	void SetCurCharacter(Character* cha);
	Character* GetCurCharacter();

	bool DoAction();
	bool SelectRoute();
private:
	Character* m_pCurAI;
	eAIStrategy m_eStrategy;	//现行单位的策略
};


extern AIMgr g_AIMgr;
#endif