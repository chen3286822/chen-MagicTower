#ifndef CREATURE_MANAGER_H
#define CREATURE_MANAGER_H

#include "commonTools.h"
#include "Character.h"

struct Pair
{
	int x;
	int y;
	Pair()
	{
		x = 0;
		y = 0;
	}
	Pair(int _x,int _y)
	{
		x = _x;
		y = _y;
	}
};
typedef std::map<eAttackRange,std::vector<Pair> > MAttackRange;


class CreatureManager : public Singleton<CreatureManager>
{
public:
	CreatureManager();
	~CreatureManager();

	void Init();
	void Render();
	void Update(float delta);
	void Release();

	void AddEnemy(Character* _enemy){m_VEnemyList.push_back(_enemy);}
	void AddFriend(Character* _friend){m_VFriendList.push_back(_friend);}
	VCharacter::iterator RemoveEnemy(Character* _enemy);
	void RemoveFriend(Character* _friend);
	void ClearEnemy(){m_VEnemyList.clear();}
	void ClearFriend(){m_VFriendList.clear();}

	Character* GetNextEnemy();
	Character* GetEnemy(int x,int  y);
	Character* GetEnemy(int num);
	Character* GetFriend(int x,int  y);
	Character* GetFriend(int num);
	Character* GetCreature(int x,int  y);
	Character* GetCreature(int num);

	//��ʾ�õ�λ�������Ϣ
	void ShowCreatureInfo();
	//��ʾ�õ�λ�ƶ���Χ
	void ShowMoveRange(Character* creature);
	//��ʾ�õ�λ������Χ
	void ShowAttackRange(Character* creature);

	//ֻ�е����е�λ���ж�����������ã��൱�ڿ�ʼ��һ�غ�
	bool ResetAllCreature();

	//AI�ж�����
	void Strategy();
	int	GetCurrentActionCreature(){return m_nActionCreatureNum;}

	//��λ��ͨ��
	//��������ǰ֪ͨ����������֪ͨ��
	int Notify(int src,int tar,int messageID,int param);

	//��ǰ���㹥���˺�
	void CalculateHurt(Character* cast,Character* target,bool bCrit=false);
	//���㹥�����(�������)
	void CalculateResult(int src,int tar);
	//Ԥ�ȼ��㲢�����Ͷ���
	void PreCalculateAndPushAction(Character* cast,Character* target);

	//ѡ�е�λ
	void SelectCreature();
	//ȡ��ѡ��
	void UnSelectCreature();
	//����ѡ�е�λ�Ĵ���
	void ProcessSelectCreature();
	//��ù������ͷ�Χ
	MAttackRange& GetAttackRange(){return m_mAttackRange;}
private:
	VCharacter  m_VFriendList;
	VCharacter	m_VEnemyList;
	VCharacter m_VFriendDeadList;
	VCharacter m_VEnemyDeadList;

	int m_nActionCreatureNum;	//��ʾ��ǰ�����ж��еĵ�λ�ţ�û�б�ʾΪ-1
	int m_nSelectNum;	//��ʾ��ǰ��ѡ�еĵ�λ��

	MAttackRange m_mAttackRange;
	eCampTurn m_eCampTurn;	//��ǰ�����غ�
};




#endif