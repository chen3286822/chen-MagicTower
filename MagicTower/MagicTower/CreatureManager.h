#ifndef CREATURE_MANAGER_H
#define CREATURE_MANAGER_H

#include "commonTools.h"
#include "Character.h"

class CreatureManager : public Singleton<CreatureManager>
{
public:
	CreatureManager();
	~CreatureManager();

	void Render();
	void Update(float delta);

	void AddEnemy(Character* _enemy){m_VEnemyList.push_back(_enemy);}
	void AddFriend(Character* _friend){m_VFriendList.push_back(_friend);}
	void RemoveEnemy(Character* _enemy);
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
	void ShowMoveRange();
	//��ʾ�õ�λ������Χ
	void ShowAttackRange();

	//ֻ�е����е�λ���ж�����������ã��൱�ڿ�ʼ��һ�غ�
	bool ResetAllCreature();

	//AI�ж�����
	void Strategy();
	int	GetCurrentActionCreature(){return m_nActionCreatureNum;}

	//��λ��ͨ��
	//��������ǰ֪ͨ����������֪ͨ��
	int Notify(int src,int tar,int messageID,int param);

	//���㹥�����
	void CalculateResult(int src,int tar);

	//ѡ�е�λ
	void SelectCreature();
	//ȡ��ѡ��
	void UnSelectCreature();
	//����ѡ�е�λ�Ĵ���
	void ProcessSelectCreature();
private:
	VCharacter  m_VFriendList;
	VCharacter	m_VEnemyList;

	int m_nActionCreatureNum;	//��ʾ��ǰ�����ж��еĵ�λ�ţ�û�б�ʾΪ-1
	int m_nSelectNum;	//��ʾ��ǰ��ѡ�еĵ�λ��
};




#endif