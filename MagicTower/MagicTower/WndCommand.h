#ifndef WNDCOMMAND_H
#define WNDCOMMAND_H


#include "UI.h"

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

private:
	UIButton* m_pAttackButton;	//������ť
	UIButton* m_pFinishButton;	//������ť
};


#endif