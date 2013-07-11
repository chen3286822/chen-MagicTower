#include "SkillManager.h"
#include "Character.h"
#include "TexManager.h"
#include "ConfigManager.h"

SkillManager::SkillManager()
{
	m_lSkill.clear();
}

SkillManager::~SkillManager()
{

}

void SkillManager::CreateSkill(int id, Character* cha)
{
	if(!cha)
		return;
	HTEXTURE skillTex = TexManager::sInstance().GetSkillTex(id);
	if(skillTex)
	{
		Skill skill;
		skill.m_nID = id;
		skill.m_pChar = cha;
		SkillInfo skillInfo = ConfigManager::sInstance().GetSkillInfo().find(id)->second;
		skill.m_nFrames = skillInfo.m_nFrames;
		skill.m_fOffsetX = skillInfo.m_nOffX;
		skill.m_fOffsetY = skillInfo.m_nOffY;
		HGE* hge = hgeCreate(HGE_VERSION);
		skill.m_nFrameWidth = hge->Texture_GetWidth(skillTex);
		skill.m_nFrameHeight = hge->Texture_GetHeight(skillTex)/skillInfo.m_nFrames;
		skill.m_pAnim = new hgeAnimation(skillTex,skill.m_nFrames,8,0,0,skill.m_nFrameWidth,skill.m_nFrameHeight);
		skill.m_pAnim->SetMode(HGEANIM_NOLOOP|HGEANIM_FWD);
		skill.m_pAnim->Play();
		m_lSkill.push_back(skill);
		hge->Release();
	}
}

void SkillManager::CreateSkill(int id, std::vector<Character*> vCha)
{
	for (VCharacter::iterator it=vCha.begin();it!=vCha.end();it++)
	{
		CreateSkill(id,*it);
	}
}

void SkillManager::Render()
{
	for (LSkill::iterator it=m_lSkill.begin();it!=m_lSkill.end();it++)
	{
		(*it).m_pAnim->Render((*it).m_pChar->GetRealX()+(*it).m_fOffsetX,(*it).m_pChar->GetRealY()+(*it).m_fOffsetY);
	}
}

void SkillManager::Update(float dt)
{
	for (LSkill::iterator it=m_lSkill.begin();it!=m_lSkill.end();)
	{
		if(!(*it).m_pAnim->IsPlaying())
		{
			gSafeDelete((*it).m_pAnim);
			it = m_lSkill.erase(it);
		}
		else
		{
			(*it).m_pAnim->Update(dt);
			it++;
		}
	}
}