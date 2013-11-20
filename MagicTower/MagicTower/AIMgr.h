#ifndef AIMGR_H
#define AIMGR_H
#include "commonTools.h"

class Character;
bool DamageCompare(Character* cha1,Character* cha2);
class AIMgr
{
public:
	AIMgr();
	~AIMgr();

	void Init();
	//���õ�ǰҪ����AI�ж��ĵ�λ
	void SetCurCharacter(Character* cha);
	Character* GetCurCharacter();

	//AI��ѭ��
	bool DoAction();
	//ѡ��һ����Ŀ���ƶ���·����ִ���ƶ�
	bool SelectRoute(Character* target);

	//���º�������cast����Ϊ������Щ������Ϊͨ�ú���ʹ�ã��������趨AI��λ
	//ѡ���Ŀ���˺���ߵĿ�ʩ�ż��ܣ����ؼ���id��û���򷵻�-1
	int SelectDamageSkill(Character* cast,Character* target,POINT attackPoint,bool ignoreAttackPoint=false);		
	//���ض�Ŀ����ɵ����Ԥ���˺�,attack��-1���ʾ����ͨ����������ֵ��ʾ�Ǽ����˺�
	int GetDamageToTarget(Character* cast,Character* target,int& attack,POINT attackPoint,bool ignoreAttackPoint=false);	
	//�Ƿ���Ի�ɱ��λ��attack��-1���ʾ����ͨ����������ֵ��ʾ�Ǽ����˺�,ignoreAttackPointΪtrue���ʾֻ��Ϊ�˲����˺�����������������ʵ���Ƿ��õ�Ŀ��
	bool CanKillTarget(Character* cast,Character* target,int& attack,POINT attackPoint,bool ignoreAttackPoint=false);	
	//���Ե�λ�����ƶ���Χ���Ƿ��ܴ�Ŀ��������һ����λ,pt ���ؿ��Ի���ʱcast��λ��
	bool CanHitPointOrTarget(Character* cast,Character* target,Block* point,POINT& pt);
private:
	Character* m_pCurAI;
	AIStrategy m_iStrategy;	//���е�λ�Ĳ���

	bool StrategyAttackTarget();
};


extern AIMgr g_AIMgr;
#endif