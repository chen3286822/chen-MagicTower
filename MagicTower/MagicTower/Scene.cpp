#include "Scene.h"
#include "TexManager.h"

Scene::Scene()
{
	m_pBackground = new hgeSprite(0,0,0,640,400);
}

Scene::~Scene()
{

}

void Scene::Render()
{
	if (m_pBackground)
	{
		m_pBackground->Render(0,0);
	}
}

void Scene::Update(float dt)
{

}

void Scene::SetBackground(int texID)
{
	HTEXTURE tex = TexManager::sInstance().GetSceneTex(texID);
	if (tex)
	{
		m_pBackground->SetTexture(tex);
	}
}