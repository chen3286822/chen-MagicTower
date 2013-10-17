#ifndef WND_SUMMARY_H
#define WND_SUMMARY_H

#include "UI.h"

class Character;
class WndSummary : public UIWindow
{
	CREATEWINDOW(WndSummary)
public:
	enum eControlID
	{
		eControlID_CharButton = 1,
		eControlID_ArmsButton,
		eControlID_SkillButton,
		eControlID_EquipButton,
	};

	WndSummary();
	virtual ~WndSummary();

	virtual void Render();
	virtual void Update(float dt);
	virtual void	SetRenderPositon(float _x,float _y);
	virtual bool	IsOnControl();

	void	SetBindChar(Character* bindChar);
	Character*	GetBindChar(){return m_pBindChar;}
private:
	Character* m_pBindChar;
	hgeSprite* m_pHead;		//头像

	int m_nShowType;	//应该显示哪页内容

	UIButton* m_pCharButton;	//查看人物
	UIButton* m_pArmsButton;	//查看兵种
	UIButton* m_pSkillButton;		//查看技能
	UIButton* m_pEquipButton;	//查看装备
};


#endif