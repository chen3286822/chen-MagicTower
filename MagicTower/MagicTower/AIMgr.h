#ifndef AIMGR_H
#define AIMGR_H


class Character;
class AIMgr
{
public:
	AIMgr();
	~AIMgr();

	//���õ�ǰҪ����AI�ж��ĵ�λ
	void SetCurCharacter(Character* cha);
	Character* GetCurCharacter();

	bool SelectRoute();
private:
	Character* m_pCurAI;
};


extern AIMgr g_AIMgr;
#endif