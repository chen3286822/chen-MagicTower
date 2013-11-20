#ifndef WND_CHAR_INFO_H
#define WND_CHAR_INFO_H

#include "UI.h"

class Character;
class WndCharInfo : public UIWindow
{
	CREATEWINDOW(WndCharInfo)
public:
	WndCharInfo();
	virtual ~WndCharInfo();

	virtual void Render();

	void	SetBindChar(Character* bindChar);
	Character*	GetBindChar(){return m_pBindChar;}
	virtual void					SetRenderPositon(float _x=-1,float _y=-1);
private:
	Character* m_pBindChar;
	hgeSprite* m_pHead;		//Í·Ïñ
};



#endif