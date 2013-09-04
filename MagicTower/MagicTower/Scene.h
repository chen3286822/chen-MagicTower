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
private:
	hgeAnimation* m_pAnim;
	HTEXTURE m_iTex[2];
	float m_fPosX;
	float m_fPosY;
	eDirection m_eDir;
	eAction m_eAction;
};

class Scene : public Singleton<Scene>
{
public:
	Scene();
	~Scene();

	void Render();
	void Update(float dt);

	void SetBackground(int texID);
	void AddActor(int ID);
	Actor* GetActor(int ID);
private:
	hgeSprite* m_pBackground;
	std::map<int,Actor*> m_mActors;
};

#endif