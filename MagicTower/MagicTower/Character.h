#pragma once
#include "commonTools.h"


//��ʾ���е�λ
class Character
{
public:
	Character(void);
	~Character(void);

	void Render();
	void Update(float delta); 

	void Init(HTEXTURE tex,int _Level,int _ID,int _Num,int _Action,Block _block);
	void Move(int tarX,int tarY);	//��Ŀ���ƶ�
	void Move(Direction dir);		//�Ը���Ϊ��λ�ƶ�

	void SetMove(bool _move){m_bCanMove = _move;}
	bool GetMove(){return m_bCanMove;}
	void SetFinish(bool _finish){m_bFinishAct = _finish;}
	bool GetFinish(){return m_bFinishAct;}
	Block&	GetBlock(){return m_iBlock;}
	void	SetMoveAbility(int _ability){m_nMoveAbility = _ability;}
	int	GetMoveAbility(){return m_nMoveAbility;}
	void SetCamp(int _camp){m_nCamp = _camp;}
	int GetCamp(){return m_nCamp;}

	int GetID(){return m_nID;}
	int GetNum(){return m_nNum;}
private:
	hgeAnimation* m_pAnimation;
	int m_nLevel;	//���ڹؿ�
	int m_nID;		//����λ����ID��
	int m_nNum;	//��������ڵ�ǰ�ؿ��ĵ�λ��
	Block m_iBlock;//���ڵ�ǰ�ؿ����ĸ�������
	float m_fXPos,m_fYPos;	//��λ��ʵ������
	float m_fStartX,m_fStartY;	//��λÿ���ж�ǰ�����λ��
	bool m_bCanMove;		//����õ�λ�Ƿ�����ƶ�
	int m_nMoveAbility;		//�����ƶ���
	bool m_bFinishAct;	//�����Ƿ��ж���
	int	m_nCamp;		//��λ��Ӫ

	Direction m_eCurMoveDir;	//��ǰ�ƶ��ķ���
	VDirection m_vPathDir;	//����·�����ƶ�����
	int m_nLeftDistance;	//ʣ����Ҫ�ƶ��ĸ�����
};


typedef std::vector<Character*> VCharacter;
