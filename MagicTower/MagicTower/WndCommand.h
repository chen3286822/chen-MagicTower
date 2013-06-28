#ifndef WNDCOMMAND_H
#define WNDCOMMAND_H


#include "UI.h"

//Ö¸Áî´°¿Ú
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
	UIButton* m_pAttackButton;	//¹¥»÷°´Å¥
	UIButton* m_pFinishButton;	//´ýÃü°´Å¥
};


#endif