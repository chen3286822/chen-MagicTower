#include "AIMgr.h"
#include "Character.h"

AIMgr g_AIMgr;

AIMgr::AIMgr()
{
	m_pCurAI = NULL;
}

AIMgr::~AIMgr()
{

}

void AIMgr::SetCurCharacter(Character* cha)
{
	//已经设置过
	if(m_pCurAI)
		return;

	m_pCurAI = cha;
}

Character* AIMgr::GetCurCharacter()
{
	return m_pCurAI;
}

bool AIMgr::SelectRoute()
{

	return true;
}