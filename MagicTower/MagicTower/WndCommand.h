#ifndef WNDCOMMAND_H
#define WNDCOMMAND_H


#include "UI.h"

class Character;
//ָ���
class WndCommand : public UIWindow
{
public:
	enum eControlID
	{
		eControlID_AttackButton = 1,
		eControlID_FinishButton,
		eControlID_SkillButton,
	};


	WndCommand();
	~WndCommand();

	virtual void Update(float dt);
	virtual void	SetRenderPositon(float _x,float _y);
	virtual bool	IsOnControl();

	void	SetBindChar(Character* bindChar);
	Character*	GetBindChar();
private:
	UIButton* m_pAttackButton;	//������ť
	UIButton* m_pFinishButton;	//������ť
	UIButton* m_pSkillButton;		//���ܰ�ť

	//�󶨸����Ľ�ɫ
	Character* m_pChar;
};


#endif