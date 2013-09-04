#include "Scene.h"
#include "TexManager.h"

Actor::Actor()
{
	m_pAnim = NULL;
	m_fPosX = m_fPosY = 0;
}

Actor::~Actor()
{
	gSafeDelete(m_pAnim);
}

void Actor::Render()
{
	if(m_pAnim)
	{
		m_pAnim->Render(m_fPosX,m_fPosY);
	}
}

void Actor::Update(float dt)
{
	if (m_pAnim)
	{
		m_pAnim->Update(dt);
	}
}

void Actor::SetAnimTex(HTEXTURE tex)
{
	if(tex)
	{
		if (m_pAnim)
		{
			m_pAnim->SetTexture(tex);
		}
		else
		{
			m_pAnim = new hgeAnimation(tex,1,8,0,0,48,64);
		}
	}

}

void Actor::SetFrames(float x1,float y1,float w,float h,int frames,int FPS)
{
	if (m_pAnim)
	{
		m_pAnim->ResetFrames(x1,y1,w,h,frames,FPS,true);
	}
}

void Actor::SetPos(float posX,float posY)
{
	m_fPosX = posX;
	m_fPosY = posY;
}

Scene::Scene()
{
	m_pBackground = new hgeSprite(0,0,0,640,400);
}

Scene::~Scene()
{
	gSafeDelete(m_pBackground);
	for (std::map<int,Actor*>::iterator it=m_mActors.begin();it!=m_mActors.end();it++)
	{
		gSafeDelete(it->second);
	}
}

void Scene::Render()
{
	if (m_pBackground)
	{
		m_pBackground->Render((APP_WIDTH-640)/2,(APP_HEIGHT-400)/2);
	}
	for (std::map<int,Actor*>::iterator it=m_mActors.begin();it!=m_mActors.end();it++)
	{
		it->second->Render();
	}
}

void Scene::Update(float dt)
{
	for (std::map<int,Actor*>::iterator it=m_mActors.begin();it!=m_mActors.end();it++)
	{
		it->second->Update(dt);
	}
}

void Scene::SetBackground(int texID)
{
	HTEXTURE tex = TexManager::sInstance().GetSceneTex(texID);
	if (tex)
	{
		m_pBackground->SetTexture(tex);
	}
}

void Scene::AddActor(int ID)
{
	std::map<int,Actor*>::iterator it = m_mActors.find(ID);
	if (it == m_mActors.end())
	{
		Actor* newActor = new Actor;
		newActor->SetAnimTex(TexManager::sInstance().GetActorTex(ID));
		newActor->SetFrames(0,0,48,64,1,8);
		m_mActors[ID] = newActor;
	}
}

Actor* Scene::GetActor(int ID)
{
	std::map<int,Actor*>::iterator it = m_mActors.find(ID);
	if (it != m_mActors.end())
	{
		return (it->second);
	}
	return NULL;
}