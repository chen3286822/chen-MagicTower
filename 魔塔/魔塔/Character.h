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
private:
	hgeAnimation* m_ani;
	int m_ID;		//����λ����ID��
	int m_Num;	//��������ڵ�ǰ�ؿ��ĵ�λ��
	Block m_block;//���ڵ�ǰ�ؿ����ĸ�������
	int m_xpos,m_ypos;	//���ڵ�ǰ�ؿ����ĸ�������
	bool m_bCanMove;		//����õ�λ�Ƿ�����ƶ�
	int m_MoveAbility;		//�����ƶ���
//	std::vector<Block>
};

