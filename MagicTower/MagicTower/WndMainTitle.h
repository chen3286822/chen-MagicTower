#ifndef WND_MAIN_TITLE_H
#define WND_MAIN_TITLE_H

#include "UI.h"

class WndMainTitle : public UIWindow
{
	CREATEWINDOW(WndMainTitle)
public:
	enum eControlID
	{
		eControlID_NewGameButton = 1,
		eControlID_LoadGameButton,
		eControlID_SetButton,
		eControlID_QuitButton,
	};

	WndMainTitle();
	virtual ~WndMainTitle();

	virtual void Update(float dt);
	virtual void	SetRenderPositon(float _x,float _y);
	virtual bool	IsOnControl();
	virtual void Render();

private:
	UIButton* m_pNewGame;
	UIButton* m_pLoadGame;
	UIButton* m_pSet;
	UIButton* m_pQuit;

	hgeSprite* m_pLogo;
};

#endif