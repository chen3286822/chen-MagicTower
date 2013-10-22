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
	};

	WndSummary();
	virtual ~WndSummary();

	virtual void Render();
	virtual void Update(float dt);
	virtual void	SetRenderPositon(float _x,float _y);
	virtual bool	IsOnControl();
	virtual void OnMouseOver(float x,float y);

	void	SetBindChar(Character* bindChar);
	Character*	GetBindChar(){return m_pBindChar;}
private:
	Character* m_pBindChar;
	hgeSprite* m_pHead;		//ͷ��

	hgeAnimation* m_pAnim;	//����
	hgeSprite* m_pIcons[6];	//6�����Ե�ͼ��
	hgeSprite* m_pWeaponGrid; //������
	hgeSprite* m_pClothGrid;		//������
	hgeSprite* m_pEquipGrid;		//װ����
	hgeSprite* m_pEquip[eEquipGrid_TotalEquip];	//װ��ͼ�� 

	int m_nShowType;	//Ӧ����ʾ��ҳ����

	UIButton* m_pCharButton;	//�鿴����
	UIButton* m_pArmsButton;	//�鿴����
	UIButton* m_pSkillButton;		//�鿴����
	static std::string m_sStrAttr[6];		//��������
};


#endif