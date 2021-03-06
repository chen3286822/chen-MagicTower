#ifndef WNDCOMMAND_H
#define WNDCOMMAND_H


#include "UI.h"

class Character;
//指令窗口
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
	UIButton* m_pAttackButton;	//攻击按钮
	UIButton* m_pFinishButton;	//待命按钮

	//绑定该面板的角色
	Character* m_pChar;
};


#endif