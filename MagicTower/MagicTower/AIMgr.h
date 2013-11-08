#ifndef AIMGR_H
#define AIMGR_H
#include "commonTools.h"


class Character;
class AIMgr
{
public:
	AIMgr();
	~AIMgr();

	//���õ�ǰҪ����AI�ж��ĵ�λ
	void SetCurCharacter(Character* cha);
	Character* GetCurCharacter();

	bool DoAction();
	bool SelectRoute();
private:
	Character* m_pCurAI;
	eAIStrategy m_eStrategy;	//���е�λ�Ĳ���
};


extern AIMgr g_AIMgr;
#endif