#ifndef AIMGR_H
#define AIMGR_H
#include "commonTools.h"


class Character;
class AIMgr
{
public:
	AIMgr();
	~AIMgr();

	void Init();
	//���õ�ǰҪ����AI�ж��ĵ�λ
	void SetCurCharacter(Character* cha);
	Character* GetCurCharacter();

	bool DoAction();
	bool SelectRoute(Character* target);
	int SelectDamageSkill(Character* target,POINT attackPoint,bool ignoreAttackPoint=false);		//ѡ���Ŀ���˺���ߵĿ�ʩ�ż��ܣ����ؼ���id��û���򷵻�-1
	int GetDamageToTarget(Character* target,int& attack,POINT attackPoint,bool ignoreAttackPoint=false);	//���ض�Ŀ����ɵ����Ԥ���˺�,attack��-1���ʾ����ͨ����������ֵ��ʾ�Ǽ����˺�
	bool CanKillTarget(Character* target,int& attack,POINT attackPoint,bool ignoreAttackPoint=false);	//�Ƿ���Ի�ɱ��λ��attack��-1���ʾ����ͨ����������ֵ��ʾ�Ǽ����˺�,ignoreAttackPointΪtrue���ʾֻ��Ϊ�˲����˺�����������������ʵ���Ƿ��õ�Ŀ��

private:
	Character* m_pCurAI;
	eAIStrategy m_eStrategy;	//���е�λ�Ĳ���
};


extern AIMgr g_AIMgr;
#endif