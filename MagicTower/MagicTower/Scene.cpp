#include "Scene.h"
#include "TexManager.h"

Actor::Actor()
{
	m_pAnim = NULL;
	m_fPosX = m_fPosY = 0;
	m_eDir = eDirection_Right;
	m_eAction = eAction_Stand;
}

Actor::~Actor()
{
	gSafeDelete(m_pAnim);
}

void Actor::sGetActionIndex(eAction action,int& index,int& frames)
{
	switch(action)
	{
	case eAction_Stand:
		index = 0;
		frames = 1;
		break;
	case eAction_Walk:
		index = 1;
		frames = 2;
		break;
	case eAction_Sit:
	case eAction_Angry:
	case eAction_Shout:
	case eAction_Cry:
	case eAction_Hand:
	case eAction_Salute:
	case eAction_Angry_Sit:
	case eAction_Shout_Sit:
	case eAction_Cry_Sit:
	case eAction_Dead:
	case eAction_Knee:
	case eAction_Ease:
	case eAction_Treat:
	case eAction_Cut:
		index = (int)action + 1;
		frames = 1;
		break;
	case eAction_Inspire:
		index = 19;
		frames = 1;
		break;
	}
}

void Actor::Render()
{
	if(m_pAnim)
	{
		if(m_eDir == eDirection_Left || m_eDir == eDirection_Right)
			m_pAnim->RenderSymmetry(m_fPosX,m_fPosY,1);
		else
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

void Actor::SetAnimTex(HTEXTURE tex1,HTEXTURE tex2)
{
	if(tex1)
	{
		if (m_pAnim)
		{
			m_pAnim->SetTexture(tex1);
		}
		else
		{
			m_pAnim = new hgeAnimation(tex1,1,8,0,0,FLOAT_PIC_SQUARE_WIDTH,FLOAT_PIC_SQUARE_HEIGHT);
			m_pAnim->SetMode(HGEANIM_LOOP|HGEANIM_FWD);
			m_pAnim->Play();
		}
		m_iTex[0] = tex1;
	}
	if(tex2)
		m_iTex[1] = tex2;
}

void Actor::SetFrames(float x1,float y1,float w,float h,int frames,int FPS)
{
	if (m_pAnim)
	{
		m_pAnim->ResetFrames(x1,y1,w,h,frames,FPS,true);
// 		m_pAnim->SetMode(HGEANIM_LOOP|HGEANIM_FWD);
// 		m_pAnim->Resume();
	}
}

void Actor::SetPos(float posX,float posY)
{
	m_fPosX = posX;
	m_fPosY = posY;
}

void Actor::SetAction(int action,int dir)
{
	//如果改变的方向和原方向是 上左 与 下右的关系，则需要更换图片
	eDirection Dir = (eDirection)dir;
	eAction Action = (eAction)action;
	if((Dir==eDirection_Down||Dir==eDirection_Right) && (m_eDir==eDirection_Up||m_eDir==eDirection_Left))
		m_pAnim->SetTexture(m_iTex[0]);
	else if((m_eDir==eDirection_Down||m_eDir==eDirection_Right) && (Dir==eDirection_Up||Dir==eDirection_Left))
		m_pAnim->SetTexture(m_iTex[1]);
	m_eDir = Dir;

	m_eAction = Action;
	int index = 0,frames = 0;
	sGetActionIndex(Action,index,frames);
	m_pAnim->ResetFrames(0,FLOAT_PIC_SQUARE_HEIGHT*index,FLOAT_PIC_SQUARE_WIDTH,FLOAT_PIC_SQUARE_HEIGHT,frames,8,true);
// 	m_pAnim->SetMode(HGEANIM_LOOP|HGEANIM_FWD);
// 	m_pAnim->Resume();
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
		HTEXTURE tex1 = 0,tex2 = 0;
		TexManager::sInstance().GetActorTex(ID,tex1,tex2);
		newActor->SetAnimTex(tex1,tex2);
		newActor->SetFrames(0,0,FLOAT_PIC_SQUARE_WIDTH,FLOAT_PIC_SQUARE_HEIGHT,1,8);
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