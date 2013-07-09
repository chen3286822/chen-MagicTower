#ifndef WND_SELECT_H
#define WND_SELECT_H

#include "commonTools.h"
#include "UI.h"

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
	virtual void SetRenderPositon(float _x,float _y);
private:
	UIListBox* m_pListBox;
};

#endif