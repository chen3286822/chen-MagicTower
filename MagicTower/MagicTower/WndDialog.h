#ifndef WND_DIALOG_H
#define WND_DIALOG_H

#include "UI.h"

class WndDialog : public UIWindow
{
	CREATEWINDOW(WndDialog)
public:
	WndDialog();
	~WndDialog();

	virtual void Render();
	void	SetBindChar(Character* bindChar);
	void SetHead(int ID);
	void SetName(std::string name);
private:
	Character* m_pBindChar;
	hgeSprite* m_pHead;		//ͷ��
	std::string m_strName;	//����
	std::vector<std::string> m_vWords;
};

#endif