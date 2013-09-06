#ifndef WND_DIALOG_H
#define WND_DIALOG_H

#include "UI.h"

class WndDialog : public UIWindow
{
	CREATEWINDOW(WndDialog)
public:
	WndDialog();
	~WndDialog();

	virtual void Update(float dt);
	virtual void Render();
	void	SetBindChar(Character* bindChar);
	bool SetHead(int ID);
	void SetName(std::string name);
	void PushWords(int ID,const char* name,const char* word);
	bool IsFinishWords();
private:
	Character* m_pBindChar;
	int m_nID; //对话的人
	hgeSprite* m_pHead;		//头像
	std::string m_strName;	//名字
	std::list<std::string> m_lWords;	//存储的一整段话，需要分多次显示
	std::string m_strCurWord;	//当前正在显示的话
	int m_nNextWordIndex; //指示下一个需要显示第几个字
	DWORD m_dwTime;	//每个字显示的间隔时间
	DWORD m_dwPastTime;	//每次换字时经过的时间
	DWORD m_dwWordShowTime;	//每句话显示完之后继续显示的持续时间
	DWORD m_dwWordShowPastTime;	//每句话显示完之后经过的时间
};

#endif