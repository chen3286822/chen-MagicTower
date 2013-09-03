#ifndef SCENE_H
#define SCENE_H

/*
	��Ϸ�Զ���������
*/
#include "commonTools.h"

class Scene
{
public:
	Scene();
	~Scene();

	void Render();
	void Update(float dt);

	void SetBackground(int texID);

private:
	hgeSprite* m_pBackground;
};

#endif