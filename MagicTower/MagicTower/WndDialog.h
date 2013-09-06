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
	int m_nID; //�Ի�����
	hgeSprite* m_pHead;		//ͷ��
	std::string m_strName;	//����
	std::list<std::string> m_lWords;	//�洢��һ���λ�����Ҫ�ֶ����ʾ
	std::string m_strCurWord;	//��ǰ������ʾ�Ļ�
	int m_nNextWordIndex; //ָʾ��һ����Ҫ��ʾ�ڼ�����
	DWORD m_dwTime;	//ÿ������ʾ�ļ��ʱ��
	DWORD m_dwPastTime;	//ÿ�λ���ʱ������ʱ��
	DWORD m_dwWordShowTime;	//ÿ�仰��ʾ��֮�������ʾ�ĳ���ʱ��
	DWORD m_dwWordShowPastTime;	//ÿ�仰��ʾ��֮�󾭹���ʱ��
};

#endif