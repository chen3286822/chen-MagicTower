#pragma once
#include "commonTools.h"

/*
��λ״̬�仯��
Stand --Move--> Walk ----> Stand
Stand --Attack--> Fight ----> Stand

����λ����Standʱ�����ж�����һ����None
����λ����Walkʱ�����ж�����һ������None

*/


//��ʾ���е�λ
class Character
{
public:
	Character(void);
	~Character(void);

	void Render();
	void Update(float delta); 

	void Init(int _Level,int _ID,int _Num,int _Action,Block _block);
	eErrorCode Move(int tarX,int tarY);	//��Ŀ���ƶ�
	void Move(eDirection dir);		//�Ը���Ϊ��λ�ƶ�

	void SetMove(bool _move){m_bCanMove = _move;}
	bool GetMove(){return m_bCanMove;}
	void SetFinish(bool _finish){m_bFinishAct = _finish;}
	bool GetFinish(){return m_bFinishAct;}
	Block&	GetBlock(){return m_iBlock;}
	Block& GetOrigBlock(){return m_iOrigBlock;}
	void	SetMoveAbility(int _ability){m_nMoveAbility = _ability;}
	int	GetMoveAbility(){return m_nMoveAbility;}
	void SetCamp(int _camp){m_nCamp = _camp;}
	int GetCamp(){return m_nCamp;}
	void SetAttackRange(eAttackRange _attackRange){m_eAttackRange = _attackRange;}
	eAttackRange GetAttackRange(){return m_eAttackRange;}
	void SetActionStage(eActionStage _stage){m_eActionStage = _stage;}
	eActionStage GetActionStage(){return m_eActionStage;}
	eCharacterState GetCharacterState(){return m_eCharState;}
	eDirection& GetCurDirection(){return m_eCurDir;}
	eDirection& GetOrigDirection(){return m_eOrigDir;}
	int GetID(){return m_nID;}
	int GetNum(){return m_nNum;}

	void CancelMove();	//ȡ���ƶ����ص�ԭ��λ��

	//ս�����
	void SetTarget(int _tar){m_nTar = _tar;}
	int GetTarget(){return m_nTar;}
	void GeginHit();
	int TowardToAttacker(int src,int dir);	//����Ź�����
	void Attack();	//��ʼ����
	void Attacked();	//��ʼ������
	void Defend(); //��ʼ����
private:
	hgeAnimation* m_pAnimation;
	std::map<int,HTEXTURE> m_mCharTex; //�洢��λ�ĸ���ͼƬ����
	int m_nLevel;	//���ڹؿ�
	int m_nID;		//����λ����ID��
	int m_nNum;	//��������ڵ�ǰ�ؿ��ĵ�λ��
	Block m_iOrigBlock;	//��¼��λԭ�����ڵĸ��ӣ������Ҽ�ȡ���ж�ʱ����ԭ����
	Block m_iBlock;//���ڵ�ǰ�ؿ����ĸ�������
	float m_fXPos,m_fYPos;	//��λ��ʵ������
	float m_fStartX,m_fStartY;	//��λÿ���ж�ǰ�����λ��
	bool m_bCanMove;		//����õ�λ�Ƿ�����ƶ�
	int m_nMoveAbility;		//�����ƶ���
	bool m_bFinishAct;	//�����Ƿ��ж���
	int	m_nCamp;		//��λ��Ӫ
	eActionStage m_eActionStage;	//��λ�����׶Σ�ֻ���ֵ��õ�λ�ж�ʱ����Ч

	eCharacterState m_eCharState; //��λ��ǰ��״̬ 
	eDirection m_eCurDir;	//��ǰ����
	eDirection m_eOrigDir;	//ԭ�ȷ��������Ҽ�ȡ���ж�ʱ�ָ���ԭ����
	LDirection m_lPathDir;	//����·�����ƶ�����
	int m_nLeftDistance;	//ʣ����Ҫ�ƶ��ĸ�����

	eAttackRange m_eAttackRange;	//��λ������Χ����
	int m_nSrc; //�����Լ��ĵ�λ
	int m_nTar;	//����Ŀ��
	eAttackState m_eAttackState; //������״̬�����ڴ���������
	DWORD m_dwRecordTime;	//���ڶ�����ʱ
};


typedef std::vector<Character*> VCharacter;
