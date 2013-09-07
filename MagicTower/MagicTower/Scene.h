#ifndef SCENE_H
#define SCENE_H

/*
	��Ϸ�Զ���������
*/
#include "commonTools.h"

enum eAction
{
	eAction_Stand,		//վ��
	eAction_Walk,			//��·
	eAction_Sit,				//����
	eAction_Angry,		//����
	eAction_Shout,		//��
	eAction_Cry,			//����
	eAction_Hand,			//����
	eAction_Salute,		//����
	eAction_Angry_Sit,	//����(����)
	eAction_Shout_Sit,	//��(����)
	eAction_Cry_Sit,		//����(����)
	eAction_Dead,			//����
	eAction_Knee,			//����
	eAction_Ease,			//����
	eAction_Treat,			//��в
	eAction_Cut,			//����
	eAction_Inspire,		//����

	//���⶯��
	eAction_Talk,			//˵�����˶������������ﶯ��
	eAction_Hide,			//�������ﶯ��
	eAction_Appear,		//�������ﶯ��
};

struct NewAction
{
	int m_nID;
	eAction m_eAction;
	eDirection m_eDir;
	DWORD m_dwTime;
	DWORD m_dwData;
	int m_nNum;
	std::string m_strName;
	std::string m_strWords;
	NewAction()
	{
		m_nID = 0;
		m_eAction = eAction_Stand;
		m_eDir = eDirection_Right;
		m_dwTime = 0;
		m_dwData = 0;
		m_nNum = -1;
		m_strName = "";
		m_strWords = "";
	}
	NewAction(int id,eAction action,eDirection dir,DWORD time,DWORD data,int num)
	{
		m_nID = id;
		m_eAction = action;
		m_eDir = dir;
		m_dwTime = time;
		m_dwData = data;
		m_nNum = num;
	}
};
//�����е���Ա
class Actor
{
public:
	Actor();
	~Actor();

	static void sGetActionIndex(eAction action,int& index,int& frames);

	void Render();
	void Update(float dt);
	//ÿ����Ա�����ж���������������ͼ��ɵ�
	void SetAnimTex(HTEXTURE tex1,HTEXTURE tex2);
	void SetFrames(float x1,float y1,float w,float h,int frames,int FPS);

	void SetPos(float posX,float posY);
	void SetAction(int action,int dir);
	void SetHide(bool hide){m_bHide = hide;}
	bool IsHide(){return m_bHide;}
	
	//�Ƿ����ж���
	bool IsInAction();
	void PushAction(NewAction action);
	void SetBgOffset(float x,float y);
private:
	hgeAnimation* m_pAnim;
	HTEXTURE m_iTex[2];
	float m_fPosX;
	float m_fPosY;
	float m_fStartX;
	float m_fStartY;
	eDirection m_eDir;
	eAction m_eAction;
	NewAction m_iCurAction;
	float m_fBgOffsetX;	//����ͼ����ƫ��
	float m_fBgOffsetY;
	bool m_bHide;
};

//push walk��������Ҫ���벽���ͷ������
//�����ǰѳ�������бʮ�ַ����Ϊ��10*10��С����
//ÿ�����Ӵ���һ��
class Scene : public Singleton<Scene>
{
public:
	Scene();
	~Scene();

	void Render();
	void Update(float dt);

	void Release();

	void SetPlace(const char* name);
	void SetBackground(int texID);
	Actor* AddActor(int ID);
	Actor* GetActor(int ID);
	//��ָ����ɫ���ָ������
	//������ű�ʾ�ö���ִ�е�˳������������������ͬ
	//���ʾ��ͬʱ���еĶ���������Ҫ�ȵ��⼸����ɫ�����ڿ��ж�ʱ�ſ���ִ��
	void PushAction(int ID,int action,int dir,DWORD time=0,DWORD data=0,int num=-1);
	int GetCurrentNum(){return m_Num;}

	//����һ��˵���������˶�������Ա���������ж�����
	void PushWords(int head,const char* name,const char* words,int num=-1);
private:
	hgeSprite* m_pBackground;
	std::map<int,Actor*> m_mActors;
	int m_Num;	//��¼��ǰ���������
	std::list<std::vector<NewAction>> m_lVNewAction;
	eActionState m_eState;
	float m_fBgOffsetX;	//����ͼ����ƫ��
	float m_fBgOffsetY;
	std::string m_strPlace;	//�����ص�
	bool m_bShowPlaceName;	//�Ƿ���ʾ��������
	DWORD m_dwShowTime;	//����������ʾʱ��
};

#endif