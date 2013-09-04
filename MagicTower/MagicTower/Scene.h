#ifndef SCENE_H
#define SCENE_H

/*
	游戏自动场景控制
*/
#include "commonTools.h"

enum eAction
{
	eAction_Stand,		//站立
	eAction_Walk,			//走路
	eAction_Sit,				//坐下
	eAction_Angry,		//生气
	eAction_Shout,		//大喊
	eAction_Cry,			//哭泣
	eAction_Hand,			//伸手
	eAction_Salute,		//敬礼
	eAction_Angry_Sit,	//生气(坐下)
	eAction_Shout_Sit,	//大喊(坐下)
	eAction_Cry_Sit,		//哭泣(坐下)
	eAction_Dead,			//死亡
	eAction_Knee,			//跪下
	eAction_Ease,			//悠闲
	eAction_Treat,			//威胁
	eAction_Cut,			//砍下
	eAction_Inspire,		//鼓舞
};
//场景中的演员
class Actor
{
public:
	Actor();
	~Actor();

	static void sGetActionIndex(eAction action,int& index,int& frames);

	void Render();
	void Update(float dt);
	//每个演员的所有动作是由正反两张图组成的
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