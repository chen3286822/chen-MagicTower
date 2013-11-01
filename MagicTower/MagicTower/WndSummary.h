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
		eControlID_ListBox,
		eControlID_SkillRange,
	};

	WndSummary();
	virtual ~WndSummary();

	virtual void Render();
	virtual void Update(float dt);
	virtual void	SetRenderPositon(float _x=-1,float _y=-1);
	virtual bool	IsOnControl();
	virtual void OnMouseOver(float x,float y);

	void	SetBindChar(Character* bindChar);
	Character*	GetBindChar(){return m_pBindChar;}
private:
	Character* m_pBindChar;
	hgeSprite* m_pHead;		//头像

	hgeAnimation* m_pAnim;	//兵种
	hgeSprite* m_pIcons[6];	//6项属性的图标
	hgeSprite* m_pWeaponGrid; //武器栏
	hgeSprite* m_pClothGrid;		//盔甲栏
	hgeSprite* m_pEquipGrid;		//装备栏
	hgeSprite* m_pEquip[eEquipGrid_TotalEquip];	//装备图标 

	UIListBox* m_pListBox;	//技能列表
	std::map<int,int> m_mListItemToSkillId;
	hgeSprite* m_pSkillRange;	//描述技能范围

	int m_nShowType;	//应该显示哪页内容

	UIButton* m_pCharButton;	//查看人物
	UIButton* m_pArmsButton;	//查看兵种
	UIButton* m_pSkillButton;		//查看技能
	static std::string m_sStrAttr[6];		//属性描述
};


#endif