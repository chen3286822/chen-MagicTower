#ifndef WND_CHAR_INFO_H
#define WND_CHAR_INFO_H

#include "UI.h"

class Character;
class WndCharInfo : public UIWindow
{
public:
	WndCharInfo(HTEXTURE,float,float,float,float,float,float);
	~WndCharInfo();

	virtual void Render();

	void	SetBindChar(Character* bindChar);
	Character*	GetBindChar(){return m_pBindChar;}
private:
	Character* m_pBindChar;
	hgeSprite* m_pHead;		//Í·Ïñ
};



#endif