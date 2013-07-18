#ifndef WND_SELECT_H
#define WND_SELECT_H

#include "commonTools.h"
#include "UI.h"

class Character;
class WndSelect : public UIWindow
{
public:
	enum eControlID
	{
		eControlID_ListBox = 1,
	};

	WndSelect();
	~WndSelect();

// 	virtual void Render(){}
 	virtual void Update(float dt);
	virtual void Render();
	virtual void SetRenderPositon(float _x,float _y);
	virtual void	SetBindChar(Character* bindChar);
	virtual Character*	GetBindChar(){return m_pBindChar;}
private:
	UIListBox* m_pListBox;
	std::map<int,int> m_mListItemToSkillId;
	std::map<int,int> m_mListItemToItemId;
	Character* m_pBindChar;
};

#endif