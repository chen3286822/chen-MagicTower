#include "Scene.h"
#include "TexManager.h"
#include "UI.h"
#include "WndDialog.h"
#include "App.h"

Actor::Actor()
{
	m_pAnim = NULL;
	m_fPosX = m_fPosY = 0;
	m_eDir = eDirection_Right;
	m_eAction = eAction_Stand;
	m_fStartX = m_fStartY = 0;
	m_fBgOffsetX = m_fBgOffsetY = 0.0f;
	m_bHide = false;
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
	if(m_pAnim && !m_bHide)
	{
		if(m_eDir == eDirection_Left || m_eDir == eDirection_Right)
			m_pAnim->RenderSymmetry(m_fPosX + m_fBgOffsetX,m_fPosY + m_fBgOffsetY,1);
		else
			m_pAnim->Render(m_fPosX + m_fBgOffsetX,m_fPosY + m_fBgOffsetY);
	}
}

void Actor::Update(float dt)
{
	if (m_bHide)
		return;
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
				//矫正偏移
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
		//到达目的地
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
	if (action == eAction_Walk)
		m_pAnim->SetMode(HGEANIM_LOOP|HGEANIM_PINGPONG);
	else
 		m_pAnim->SetMode(HGEANIM_LOOP|HGEANIM_FWD);
 	m_pAnim->Resume();
}

bool Actor::IsInAction()
{
	//隐藏时属于非活动状态
	if (m_bHide)
	{
		return false;
	}
	//非移动类动作判断剩余时间
	if (m_iCurAction.m_dwTime == 0 && m_iCurAction.m_eAction!=eAction_Walk)
	{
		return false;
	}
	//移动类动作判断是否处于目的地
	if (m_iCurAction.m_eAction==eAction_Walk)
	{
		//剩余格数走完了
		if (m_iCurAction.m_dwData == 0)
		{
			return false;
		}
	}
	return true;
}

void Actor::PushAction(NewAction action)
{
	if (action.m_eAction == eAction_Hide)
	{
		m_bHide = true;
		return;
	}
	else if (action.m_eAction == eAction_Appear)
	{
		m_bHide = false;
		SetAction(eAction_Stand,action.m_eDir);
		m_iCurAction = action;
		m_iCurAction.m_eAction = eAction_Stand;	
		return;
	}
	m_iCurAction = action;
	SetAction(m_iCurAction.m_eAction,m_iCurAction.m_eDir);
}

void Actor::SetBgOffset(float x,float y)
{
	m_fBgOffsetX = x;
	m_fBgOffsetY = y;
}

Scene::Scene()
{
	m_pBackground = NULL;
	m_lVNewAction.clear();
	m_Num = 0;
	m_eState = eActionState_PickAction;
	m_fBgOffsetX = (APP_WIDTH-640)/2;
	m_fBgOffsetY = (APP_HEIGHT-400)/2-55;
	m_bShowPlaceName = false;
	m_dwShowTime = 0;
	m_strPlace.clear();

	//对话框需要提前打开，并隐藏
	WndDialog* dialog = (WndDialog*)UISystem::sInstance().PopUpWindow(eWindowID_Dialog);
	if (dialog)
	{
		dialog->SetShow(false);
	}
}

void Scene::Init()
{
	m_pBackground = new hgeSprite(0,0,0,640,400);
	m_mActors.clear();
}

Scene::~Scene()
{
	gSafeDelete(m_pBackground);
	for (std::map<int,Actor*>::iterator it=m_mActors.begin();it!=m_mActors.end();it++)
	{
		gSafeDelete(it->second);
	}
}

void Scene::Release()
{
	for (std::map<int,Actor*>::iterator it=m_mActors.begin();it!=m_mActors.end();it++)
	{
		gSafeDelete(it->second);
	}
	gSafeDelete(m_pBackground);
	m_Num = 0;
	m_eState = eActionState_PickAction;
	m_lVNewAction.clear();
	m_mActors.clear();
	m_bShowPlaceName = false;
	m_strPlace.clear();
	m_dwShowTime = 0;
}

void Scene::Render()
{
	if (m_pBackground)
	{
		m_pBackground->Render(m_fBgOffsetX,m_fBgOffsetY);
	}
	if (m_bShowPlaceName)
	{
		GfxFont* font = FontManager::sInstance().GetFont(FontAttr(eFontType_MSYaHei,eFontSize_FontHuge));
		font->SetColor(0xFF0000FF);
		wchar_t out[256];
		g_CTW(m_strPlace.c_str(),out);
		SIZE size = font->GetTextSize(out);
		font->Print(m_fBgOffsetX+(640-size.cx)/2,m_fBgOffsetY+15,"%s",m_strPlace.c_str());
	}
	for (std::map<int,Actor*>::iterator it=m_mActors.begin();it!=m_mActors.end();it++)
	{
		it->second->Render();
	}
}

void Scene::Update(float dt)
{
	if (m_bShowPlaceName)
	{
		if(m_dwShowTime > 2000)
			m_bShowPlaceName = false;
		else
			m_dwShowTime += (int)(dt*1000);
		return;
	}
	if (m_eState == eActionState_PickAction)
	{
		if (!m_lVNewAction.empty())
		{
			std::vector<NewAction> vAction = m_lVNewAction.front();
			bool bCanPushAction = true;
			for (int i=0;i<vAction.size();i++)
			{
				if (vAction[i].m_nID!=-1)
				{
					Actor* actor = GetActor(vAction[i].m_nID);
					if(actor)
					{
						if(actor->IsInAction())
						{	
							bCanPushAction = false;
							break;
						}
					}
					else
					{
						g_debugString(__FILE__,__FUNCTION__,__LINE__,"场景中动作有错误，找不到对应演员");
						exit(0);
					}
				}
			}
			//是否有对话未完成
			if (bCanPushAction)
			{
				WndDialog* dialog = (WndDialog*)UISystem::sInstance().GetWindow(eWindowID_Dialog);
				if(dialog && !dialog->IsFinishWords())
					bCanPushAction = false;
			}
			//可以推送动作给演员
			if (bCanPushAction)
			{
				for (int i=0;i<vAction.size();i++)
				{
					if(vAction[i].m_nID != -1)
					{
						Actor* actor = GetActor(vAction[i].m_nID);
						if(actor)
							actor->PushAction(vAction[i]);
						else
						{
							g_debugString(__FILE__,__FUNCTION__,__LINE__,"推送动作有错误，找不到对应演员");
							exit(0);
						}
					}
					else
					{
						//推送对话动作给对话框
						if (vAction[i].m_eAction == eAction_Talk)
						{
							WndDialog* dialog = (WndDialog*)UISystem::sInstance().GetWindow(eWindowID_Dialog);
							if (dialog)
							{
								dialog->SetShow(true);
								dialog->PushWords(vAction[i].m_dwData,vAction[i].m_strName.c_str(),vAction[i].m_strWords.c_str());
							}
						}
					}
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
			if (vAction[i].m_nID!=-1)
			{
				Actor* actor = GetActor(vAction[i].m_nID);
				if (actor && actor->IsInAction())
				{
					bActionOver = false;
					break;
				}
			}
		}
		if (bActionOver)
		{
			WndDialog* dialog = (WndDialog*)UISystem::sInstance().GetWindow(eWindowID_Dialog);
			if(dialog && !dialog->IsFinishWords())
				bActionOver = false;
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
		//场景结束
		if (m_lVNewAction.empty())
		{
			App::sInstance().StartNextScene();
		}
	}

	for (std::map<int,Actor*>::iterator it=m_mActors.begin();it!=m_mActors.end();it++)
	{
		it->second->Update(dt);
	}
}

void Scene::SetPlace(const char* name)
{
	m_strPlace = name;
	m_bShowPlaceName = true;
	m_dwShowTime = 0;
}

void Scene::SetBackground(int texID)
{
	HTEXTURE tex = TexManager::sInstance().GetSceneTex(texID);
	if (tex)
	{
		m_pBackground->SetTexture(tex);
	}
}

Actor* Scene::AddActor(int ID)
{
	std::map<int,Actor*>::iterator it = m_mActors.find(ID);
	if (it == m_mActors.end())
	{
		Actor* newActor = new Actor;
		HTEXTURE tex1 = 0,tex2 = 0;
		TexManager::sInstance().GetActorTex(ID,tex1,tex2);
		newActor->SetAnimTex(tex1,tex2);
		newActor->SetFrames(0,0,FLOAT_PIC_SQUARE_WIDTH,FLOAT_PIC_SQUARE_HEIGHT,1,8);
		newActor->SetBgOffset(m_fBgOffsetX,m_fBgOffsetY);
		m_mActors[ID] = newActor;
		return m_mActors[ID];
	}
	return NULL;
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
		//新序号，一个新的动作
		aNum = m_Num;
		m_Num++;
		//此处赋值num是为了下面push动作的判断
		if(num!=-1)
			num = m_Num;
	}
	else if(num < m_Num)
	{
		//采用以前的序号，说明该动作也以前的动作是并行的
		aNum = num;
	}
	else
		//错误序号
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

void Scene::PushWords(int head,const char* name,const char* words,int num)
{
	//检查合法性
	if (name == NULL || words == NULL)
		return;

	int aNum = 0;
	if(num == -1 || num==m_Num)
	{
		//新序号，一个新的动作
		aNum = m_Num;
		m_Num++;
		//此处赋值num是为了下面push动作的判断
		if(num!=-1)
			num = m_Num;
	}
	else if(num < m_Num)
	{
		//采用以前的序号，说明该动作也以前的动作是并行的
		aNum = num;
	}
	else
		//错误序号
		return;
	NewAction aAction(-1,eAction_Talk,eDirection_None,0,head,aNum);
	aAction.m_strName = name;
	aAction.m_strWords = words;
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