#include "Scene.h"
#include "TexManager.h"
#include "UI.h"
#include "WndDialog.h"

Actor::Actor()
{
	m_pAnim = NULL;
	m_fPosX = m_fPosY = 0;
	m_eDir = eDirection_Right;
	m_eAction = eAction_Stand;
	m_fStartX = m_fStartY = 0;
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
	if (m_iCurAction.m_dwTime > 0)
	{
		int pastTime = (int)(dt*1000);
		if(m_iCurAction.m_dwTime > pastTime)
			m_iCurAction.m_dwTime -= pastTime;
		else
			m_iCurAction.m_dwTime = 0;
	}
	if (m_iCurAction.m_eAction == eAction_Walk)
	{
		if (m_iCurAction.m_dwData > 0)
		{
			if (abs(m_fPosX-m_fStartX)>=64 && abs(m_fPosY-m_fStartY)>=40)
			{
				m_iCurAction.m_dwData -= 1;
				//����ƫ��
				if (m_iCurAction.m_eDir == eDirection_Down)
				{
					m_fPosX = m_fStartX-64;
					m_fPosY = m_fStartY+40;
				}
				else if (m_iCurAction.m_eDir == eDirection_Up)
				{
					m_fPosX = m_fStartX+64;
					m_fPosY = m_fStartY-40;
				}
				else if (m_iCurAction.m_eDir == eDirection_Left)
				{
					m_fPosX = m_fStartX-64;
					m_fPosY = m_fStartY-40;
				}
				else if (m_iCurAction.m_eDir == eDirection_Right)
				{
					m_fPosX = m_fStartX+64;
					m_fPosY = m_fStartY+40;
				}
				 m_fStartX = m_fPosX;
				 m_fStartY = m_fPosY;
			}
			if (m_iCurAction.m_eDir == eDirection_Down)
			{
				m_fPosX -= dt*64;
				m_fPosY += dt*40;
			}
			else if (m_iCurAction.m_eDir == eDirection_Up)
			{
				m_fPosX += dt*64;
				m_fPosY -= dt*40;
			}
			else if (m_iCurAction.m_eDir == eDirection_Left)
			{
				m_fPosX -= dt*64;
				m_fPosY -= dt*40;
			}
			else if (m_iCurAction.m_eDir == eDirection_Right)
			{
				m_fPosX += dt*64;
				m_fPosY += dt*40;
			}
		}
		//����Ŀ�ĵ�
		else
		{
			SetAction(eAction_Stand,m_iCurAction.m_eDir);
			m_iCurAction.m_eAction = eAction_Stand;
			m_iCurAction.m_dwTime = 0;
		}
	}
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
	m_fStartX = posX;
	m_fStartY = posY;
}

void Actor::SetAction(int action,int dir)
{
	//����ı�ķ����ԭ������ ���� �� ���ҵĹ�ϵ������Ҫ����ͼƬ
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
	if (action == eAction_Walk)
		m_pAnim->SetMode(HGEANIM_LOOP|HGEANIM_PINGPONG);
	else
 		m_pAnim->SetMode(HGEANIM_LOOP|HGEANIM_FWD);
 	m_pAnim->Resume();
}

bool Actor::IsInAction()
{
	//���ƶ��ද���ж�ʣ��ʱ��
	if (m_iCurAction.m_dwTime == 0 && m_iCurAction.m_eAction!=eAction_Walk)
	{
		return false;
	}
	//�ƶ��ද���ж��Ƿ���Ŀ�ĵ�
	if (m_iCurAction.m_eAction==eAction_Walk)
	{
		//ʣ�����������
		if (m_iCurAction.m_dwData == 0)
		{
			return false;
		}
	}
	return true;
}

void Actor::PushAction(NewAction action)
{
	m_iCurAction = action;
	SetAction(m_iCurAction.m_eAction,m_iCurAction.m_eDir);
}

Scene::Scene()
{
	m_pBackground = new hgeSprite(0,0,0,640,400);
	m_lVNewAction.clear();
	m_Num = 0;
	m_eState = eActionState_PickAction;

	WndDialog* dialog = (WndDialog*)UISystem::sInstance().PopUpWindow(eWindowID_Dialog);
	if (dialog)
	{
		dialog->SetName("�ܲ�");
		dialog->SetHead(1);
	}
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
		m_pBackground->Render((APP_WIDTH-640)/2,(APP_HEIGHT-400)/2-55);
	}
	for (std::map<int,Actor*>::iterator it=m_mActors.begin();it!=m_mActors.end();it++)
	{
		it->second->Render();
	}
}

void Scene::Update(float dt)
{
	if (m_eState == eActionState_PickAction)
	{
		if (!m_lVNewAction.empty())
		{
			std::vector<NewAction> vAction = m_lVNewAction.front();
			bool bCanPushAction = true;
			for (int i=0;i<vAction.size();i++)
			{
				if(GetActor(vAction[i].m_nID)->IsInAction())
				{	
					bCanPushAction = false;
					break;
				}
			}
			//�������Ͷ�������Ա
			if (bCanPushAction)
			{
				for (int i=0;i<vAction.size();i++)
				{
					GetActor(vAction[i].m_nID)->PushAction(vAction[i]);
				}
				m_eState = eActionState_Process;
			}
		}
	}
	else if (m_eState == eActionState_Process)
	{
		std::vector<NewAction> vAction = m_lVNewAction.front();
		bool bActionOver = true;
		for (int i=0;i<vAction.size();i++)
		{
			if(GetActor(vAction[i].m_nID)->IsInAction())
			{	
				bActionOver = false;
				break;
			}
		}
		if (bActionOver)
		{
			m_eState = eActionState_End;
		}
	}
	else if (m_eState == eActionState_End)
	{
		m_lVNewAction.pop_front();
		m_eState = eActionState_PickAction;
	}

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

void Scene::PushAction(int ID,int action,int dir,DWORD time,DWORD data,int num)
{
	int aNum = 0;
	if(num == -1 || num==m_Num)
	{
		//����ţ�һ���µĶ���
		aNum = m_Num;
		m_Num++;
		//�˴���ֵnum��Ϊ������push�������ж�
		if(num!=-1)
			num = m_Num;
	}
	else if(num < m_Num)
	{
		//������ǰ����ţ�˵���ö���Ҳ��ǰ�Ķ����ǲ��е�
		aNum = num;
	}
	else
		//�������
		return;
	NewAction aAction(ID,(eAction)action,(eDirection)dir,time,data,aNum);
	if(num == -1 || num==m_Num)
	{
		std::vector<NewAction> vAction;
		vAction.push_back(aAction);
		m_lVNewAction.push_back(vAction);
	}
	else
	{
		int index=0;
		for (std::list<std::vector<NewAction>>::iterator it=m_lVNewAction.begin();it!=m_lVNewAction.end();it++,index++)
		{
			if(index == num)
			{
				it->push_back(aAction);
			}
		}
	}
}
