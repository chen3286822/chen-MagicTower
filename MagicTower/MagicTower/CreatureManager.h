#ifndef CREATURE_MANAGER_H
#define CREATURE_MANAGER_H

#include "commonTools.h"
#include "Character.h"
#include "ConfigManager.h"
#include "Scene.h"

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
typedef std::vector<Pair> VPair;
typedef std::map<eAttackRange,std::vector<int> > MAttackRange;
typedef std::map<eSkillRange,std::vector<int> > MSkillRange;

struct Items
{
	Item m_iItem;
	int m_nNum;			//��Ʒ����
};
typedef std::list<Items> LItems;


class CreatureManager : public Singleton<CreatureManager>
{
public:
	CreatureManager();
	virtual ~CreatureManager();

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
	VCharacter& GetEnemy(){return m_VEnemyList;}
	VCharacter& GetFriend(){return m_VFriendList;}

	//ȡ�õ�ǰ�غ���Ϣ
	eCampTurn GetCampTurn(){return m_eCampTurn;}
	//ȡ�õ�ǰѡ�еĵ�λ��
	int& GetSelectNum(){return m_nSelectNum;}

	//�Ƿ�λ����
	bool IsEnemyAllDead();
	bool IsFriendAllDead();

	//��ʾ�õ�λ�������Ϣ
	void ShowCreatureInfo();
	//��ʾ�õ�λ�ƶ���Χ
	void ShowMoveRange(Character* creature);
	//��ʾ�õ�λ������Χ
	void ShowAttackRange(Character* creature);
	//��ʾ�����ͷŷ�Χ
	void ShowSkillCastRange(Character* creature);
	//��ʾ���ܹ�����Χ
	void ShowSkillRange(int skillID);
	//��ʾ��Ʒ�ͷŷ�Χ
	void ShowItemCastRange(Character* creature);
	VPair GetRangePoint(){return m_vPair;}

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
	//Ԥ�ȼ��㹥���������Ͷ���
	void PreAttackAndPushAction(Character* cast,Character* target);
	//Ԥ�ȼ��㼼�ܲ������Ͷ���
	void PreSkillAndPushAction(Character* cast,Character* target);
	VCharacter&	GetSkillTargets(){return m_vSkillTargets;}
	

	//ѡ�е�λ
	void SelectCreature();
	//ȡ��ѡ��
	void UnSelectCreature();
	//����ѡ�е�λ�Ĵ���
	void ProcessSelectCreature();
	//��ù������ͷ�Χ
	MAttackRange& GetAttackRange(){return m_mAttackRange;}

	//��Ʒ����ȫ��ֻά��һ����Ʒ��
	LItems& GetItems(){return m_lItems;}
	//���һ����Ʒ
	void AddItem(int id,int num=1);
	//�Ƴ�һ����Ʒ
	void RemoveItem(int id);
	//Ԥ�ȼ�����Ʒ�������Ͷ���
	void PreItemAndPushAction(Character* cast,Character* target);

	//ս���������
	//��ͣ������Ϸ����
	void SetPause(bool pause){m_bPause = pause;}
	bool GetPause(){return m_bPause;}
	//��Ӿ��鶯��
	void AddAction(eAction action,int num,DWORD time=0,eDirection dir=eDirection_None,DWORD data=0,const char*name="",const char* word="");
	void ClearAction(){m_lActions.clear();}
	//������
	void ProcessAction();
private:
	VCharacter  m_VFriendList;
	VCharacter	m_VEnemyList;
	VCharacter m_VFriendDeadList;
	VCharacter m_VEnemyDeadList;

	int m_nActionCreatureNum;	//��ʾ��ǰ�����ж��еĵ�λ�ţ�û�б�ʾΪ-1
	int m_nSelectNum;	//��ʾ��ǰ��ѡ�еĵ�λ��

	VPair m_vPair;
	MAttackRange m_mAttackRange;
	MSkillRange m_mSkillRange;
	eCampTurn m_eCampTurn;	//��ǰ�����غ�
	VCharacter m_vSkillTargets;//�����ͷ�Ŀ��

	LItems m_lItems;		//��Ҵ洢����Ʒ

	bool m_bPause;		//�������飬��ͣ����

	std::list<NewAction> m_lActions;	//��λ��ս�����鶯���б�
	eActionState m_eState; //�����б�
};




#endif