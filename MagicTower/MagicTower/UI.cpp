#include "UI.h"
#include "WndCommand.h"
#include "WndCharInfo.h"
#include "WndSelect.h"
#include "WndMainTitle.h"

UIWindow::UIWindow()
{
	m_pContainer = NULL;
	m_pBackGround = NULL;
	m_fPosX = m_fPosY = 0;
	m_bShow = false;
}

UIWindow::UIWindow(HTEXTURE tex, float x, float y, float w ,float h, float posX, float posY)
{
	m_pContainer = NULL;
	m_pBackGround = new hgeSprite(tex,x,y,w,h);
	m_fPosX = posX,
	m_fPosY = posY;
	m_bShow = false;
}

UIWindow::~UIWindow()
{
	gSafeDelete(m_pBackGround);
	gSafeDelete(m_pContainer);
}

void UIWindow::Render()
{
	if(m_bShow)
	{
		m_pBackGround->Render(m_fPosX,m_fPosY);
		m_pContainer->Render();
	}
}

void UIWindow::SetShow(bool _show)
{
	m_bShow = _show;
	if (m_bShow)
	{
		m_pContainer->Reset();
		m_pContainer->Enter();		
	}
	else
	{
		m_pContainer->Leave();
	}
}

bool UIWindow::IsOnControl()
{
	if (m_bShow && m_pBackGround)
	{
		float x,y;
		HGE* hge = hgeCreate(HGE_VERSION);
		hge->Input_GetMousePos(&x,&y);
		if (x>=m_fPosX && x<=(m_fPosX+m_pBackGround->GetWidth()) && y>=m_fPosY && y<=(m_fPosY+m_pBackGround->GetHeight()))
			return true;

		hge->Release();
	}
	return false;
}

UISystem::UISystem()
{
	m_pHeadWindow = NULL;
	m_mWindowCreateFunc.clear();
}

void UISystem::Init()
{
//	WndCommand* pWndCommand = new WndCommand();
//	WndCommand* pWndCommand = NULL;
//	m_mWindows[eWindowID_Command] = pWndCommand;
//	WndCharInfo* pWndCharInfo = new WndCharInfo();
//	WndCharInfo* pWndCharInfo = NULL;
//	m_mWindows[eWindowID_CharInfo] = pWndCharInfo;
//	WndSelect* pWndSelect = new WndSelect();
//	WndSelect* pWndSelect = NULL;
//	m_mWindows[eWindowID_Select] = pWndSelect;
//	WndMainTitle* pWndMainTitle = new WndMainTitle();
//	WndMainTitle* pWndMainTitle = NULL;
//	m_mWindows[eWindowID_MainTitle] = pWndMainTitle;

	ADDWINDOW(eWindowID_Command,WndCommand)
	ADDWINDOW(eWindowID_CharInfo,WndCharInfo)
	ADDWINDOW(eWindowID_Select,WndSelect)
	ADDWINDOW(eWindowID_MainTitle,WndMainTitle)

// 	for (std::map<eWindowID,UIWindow*>::iterator mit=m_mWindows.begin();mit!=m_mWindows.end();mit++)
// 		mit->second->Init();
}

void UISystem::Release()
{
	WindowNode* pWindow = m_pHeadWindow;
	while(pWindow)
	{
		if(pWindow->m_pWindow)
		{
			pWindow->m_pWindow->Release();
			gSafeDelete(pWindow->m_pWindow);
		}
		WindowNode* tempNode = pWindow;
		gSafeDelete(pWindow);
		pWindow = tempNode->m_pNext;
	}
	m_mWindowCreateFunc.clear();
}

void UISystem::Render()
{
	WindowNode* pWindow = m_pHeadWindow;
	while(pWindow)
	{
		if (pWindow->m_pWindow)
		{
			if (pWindow->m_pWindow->IsShow())
			{
				pWindow->m_pWindow->Render();
			}
		}
		pWindow = pWindow->m_pNext;
	}
}

void UISystem::Update(float dt)
{
	WindowNode* pWindow = m_pHeadWindow;
	while(pWindow)
	{
		if (pWindow->m_pWindow)
		{
			if (pWindow->m_pWindow->IsShow())
			{
				pWindow->m_pWindow->Update(dt);
			}
		}
		pWindow = pWindow->m_pNext;
	}

	ProcessMsg();
}

UIWindow* UISystem::GetWindow(eWindowID windowID)
{
	WindowNode* pWindow = m_pHeadWindow;
	while(pWindow)
	{
		if (pWindow->m_eWindowID == windowID)
			return pWindow->m_pWindow;
		pWindow = pWindow->m_pNext;
	}
	return NULL;
}

bool UISystem::IsInAnyControl()
{
	WindowNode* pWindow = m_pHeadWindow;
	while(pWindow)
	{
		if(pWindow->m_pWindow)
		{
			if(pWindow->m_pWindow->IsOnControl())
			{
				return true;
			}
		}
		pWindow = pWindow->m_pNext;
	}
	return false;
}

UIWindow* UISystem::PopUpWindow(eWindowID windowID)
{
	WindowNode* pWindow = m_pHeadWindow;
	while(pWindow)
	{
		if(pWindow->m_eWindowID == windowID)
		{
			if(pWindow->m_pWindow)
			{
				pWindow->m_pWindow->SetShow(true);
			}
			return pWindow->m_pWindow;
		}
		else
			pWindow = pWindow->m_pNext;
	}
	//窗口未生成
	if (!pWindow)
	{
		UIWindow* window = NULL;
		std::map<eWindowID,LPCreateWindow>::iterator mit = m_mWindowCreateFunc.find(windowID);
		if(mit!=m_mWindowCreateFunc.end())
		{
			window = mit->second();
			window->Init();
			window->SetShow(true);
			window->GetID() = windowID;
			WindowNode* newWindow = new WindowNode;
			newWindow->m_eWindowID = windowID;
			newWindow->m_pNext = NULL;
			newWindow->m_pWindow = window;
			WindowNode* tempNode = m_pHeadWindow;
			if(!m_pHeadWindow)
				m_pHeadWindow = newWindow;
			else
			{
				while(tempNode)
				{
					if(tempNode->m_pNext == NULL)
					{
						tempNode->m_pNext = newWindow;
						break;
					}
					tempNode = tempNode->m_pNext;
				}
			}
		}
		return window;
	}
	return NULL;
}

void UISystem::CloseWindow(eWindowID windowID)
{
	//先隐藏窗口，然后加入消息队列中等待删除窗口
	UIWindow* window = GetWindow(windowID);
	if (window)
	{
		window->SetShow(false);
		MsgWnd(eMessage_CloseWindow,window->GetID());
	}
}

void UISystem::RemoveWindow(eWindowID windowID)
{
	WindowNode* pWindow = m_pHeadWindow;
	WindowNode* pLastWindow = pWindow;
	while(pWindow)
	{
		if(pWindow->m_eWindowID == windowID)
		{
			if(pWindow->m_pWindow)
			{
				//删除窗口
				pWindow->m_pWindow->Release();
				gSafeDelete(pWindow->m_pWindow);
			}
			//将该窗口节点从列表中移除
			if(pWindow == m_pHeadWindow)
			{
				WindowNode* pTempNode = m_pHeadWindow->m_pNext;
				gSafeDelete(pWindow);
				m_pHeadWindow = pTempNode;
			}
			else
			{
				pLastWindow->m_pNext = pWindow->m_pNext;
				gSafeDelete(pWindow);
			}
			break;
		}
		else
		{
			pLastWindow = pWindow;
			pWindow = pWindow->m_pNext;
		}
	}
}

void UISystem::MsgWnd(eMessage eMsgID,eWindowID eWndID,DWORD dwData)
{
	MsgNode newNode;
	newNode.m_eMsgID = eMsgID;
	newNode.m_eWndID = eWndID;
	newNode.m_dwData = dwData;

	m_lMsgList.push_back(newNode);
}

void UISystem::ProcessMsg()
{
	while(!m_lMsgList.empty())
	{
		MsgNode tempNode = m_lMsgList.front();
		switch(tempNode.m_eMsgID)
		{
		case eMessage_CloseWindow:
			{
				RemoveWindow(tempNode.m_eWndID);
			}
			break;
		default:
			break;
		}
		m_lMsgList.pop_front();
	}
}