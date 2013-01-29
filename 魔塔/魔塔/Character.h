#pragma once
#include "commonTools.h"


//��ʾ���е�λ
class Character
{
public:
	Character(void);
	~Character(void);

	void render();
	void update(); 

	void init(HTEXTURE tex,int _ID,int _Num,Block _block);
	void move(Direction dir);		//�Ը���Ϊ��λ�ƶ�

	void setMove(bool _move){m_bCanMove = _move;}
	bool getMove(){return m_bCanMove;}
private:
	hgeAnimation* m_ani;
	int m_ID;		//����λ����ID��
	int m_Num;	//��������ڵ�ǰ�ؿ��ĵ�λ��
	Block m_block;//���ڵ�ǰ�ؿ����ĸ�������
	float m_xpos,m_ypos;	//��λ��ʵ������
	bool m_bCanMove;		//����õ�λ�Ƿ�����ƶ�
	int m_MoveAbility;		//�����ƶ���
//	std::vector<Block>

	Direction m_MoveDir;	//�ƶ��ķ���
	int m_LeftDistance;	//ʣ����Ҫ�ƶ��ĸ�����
};

