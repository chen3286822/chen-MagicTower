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
	void Move(Direction dir);		//�Ը���Ϊ��λ�ƶ�

	void SetMove(bool _move){m_bCanMove = _move;}
	bool GetMove(){return m_bCanMove;}
	void SetFinish(bool _finish){m_bFinishAct = _finish;}
	bool GetFinish(){return m_bFinishAct;}
	Block&	GetBlock(){return m_block;}

	int GetID(){return m_ID;}
	int GetNum(){return m_Num;}
private:
	hgeAnimation* m_ani;
	int m_Level;	//���ڹؿ�
	int m_ID;		//����λ����ID��
	int m_Num;	//��������ڵ�ǰ�ؿ��ĵ�λ��
	Block m_block;//���ڵ�ǰ�ؿ����ĸ�������
	float m_xpos,m_ypos;	//��λ��ʵ������
	float m_StartX,m_StartY;	//��λÿ���ж�ǰ�����λ��
	bool m_bCanMove;		//����õ�λ�Ƿ�����ƶ�
	int m_MoveAbility;		//�����ƶ���
	bool m_bFinishAct;	//�����Ƿ��ж���
//	std::vector<Block>

	Direction m_MoveDir;	//�ƶ��ķ���
	int m_LeftDistance;	//ʣ����Ҫ�ƶ��ĸ�����
};


typedef std::vector<Character*> VCharacter;
