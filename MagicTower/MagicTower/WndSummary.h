#ifndef WND_SUMMARY_H
#define WND_SUMMARY_H

#include "UI.h"

class Character;
class WndSummary : public UIWindow
{
	CREATEWINDOW(WndSummary)
public:
	enum eControlID
	{
		eControlID_CharButton = 1,
		eControlID_ArmsButton,
		eControlID_SkillButton,
		eControlID_EquipButton,
	};

	WndSummary();
	virtual ~WndSummary();

	virtual void Render();
	virtual void Update(float dt);
	virtual void	SetRenderPositon(float _x,float _y);
	virtual bool	IsOnControl();

	void	SetBindChar(Character* bindChar);
	Character*	GetBindChar(){return m_pBindChar;}
private:
	Character* m_pBindChar;
	hgeSprite* m_pHead;		//ͷ��

	int m_nShowType;	//Ӧ����ʾ��ҳ����

	UIButton* m_pCharButton;	//�鿴����
	UIButton* m_pArmsButton;	//�鿴����
	UIButton* m_pSkillButton;		//�鿴����
	UIButton* m_pEquipButton;	//�鿴װ��
};


#endif