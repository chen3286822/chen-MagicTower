#ifndef AIMGR_H
#define AIMGR_H


class Character;
class AIMgr
{
public:
	AIMgr();
	~AIMgr();

	//设置当前要进行AI行动的单位
	void SetCurCharacter(Character* cha);
	Character* GetCurCharacter();

	bool SelectRoute();
private:
	Character* m_pCurAI;
};


extern AIMgr g_AIMgr;
#endif