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
	bool SelectRoute(Character* target);
	int SelectDamageSkill(Character* target,POINT attackPoint);		//ѡ���Ŀ���˺���ߵĿ�ʩ�ż��ܣ����ؼ���id��û���򷵻�-1
	int GetDamageToTarget(Character* target,int& attack,POINT attackPoint);	//���ض�Ŀ����ɵ����Ԥ���˺�,attack��-1���ʾ����ͨ����������ֵ��ʾ�Ǽ����˺�
	bool CanKillTarget(Character* target,int& attack,POINT attackPoint);	//�Ƿ���Ի�ɱ��λ��attack��-1���ʾ����ͨ����������ֵ��ʾ�Ǽ����˺�

private:
	Character* m_pCurAI;
	eAIStrategy m_eStrategy;	//���е�λ�Ĳ���
};


extern AIMgr g_AIMgr;
#endif