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
	};


	WndCommand(HTEXTURE,float,float,float,float,float,float);
	~WndCommand();

	virtual void Update(float dt);
	virtual void	SetRenderPositon(float _x,float _y);
	virtual bool	IsOnControl();

	void	SetBindChar(Character* bindChar);
	Character*	GetBindChar();
private:
	UIButton* m_pAttackButton;	//������ť
	UIButton* m_pFinishButton;	//������ť

	//�󶨸����Ľ�ɫ
	Character* m_pChar;
};


#endif