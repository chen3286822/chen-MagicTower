#ifndef SCENE_H
#define SCENE_H

/*
	游戏自动场景控制
*/
#include "commonTools.h"


//场景中的演员
class Actor
{
public:
	Actor();
	~Actor();

	void Render();
	void Update(float dt);
	void SetAnimTex(HTEXTURE tex);
	void SetFrames(float x1,float y1,float w,float h,int frames,int FPS);

	void SetPos(float posX,float posY);
private:
	hgeAnimation* m_pAnim;
	float m_fPosX;
	float m_fPosY;
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