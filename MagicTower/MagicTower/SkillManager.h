#ifndef SKILL_MANAGER_H
#define SKILL_MANAGER_H

#include "commonTools.h"

class Character;
struct Skill
{
	int m_nID;		//技能ID
	hgeAnimation* m_pAnim;
	int m_nFrames;	//技能帧数
	int m_nFPS;
	int m_nX;		
	int m_nY;		
	int m_nFrameWidth;
	int m_nFrameHeight;

	Character* m_pChar;	//播放技能的单位
	float m_fOffsetX;			//绘制偏移
	float m_fOffsetY;			//
};
typedef std::list<Skill> LSkill;


class SkillManager :  public Singleton<SkillManager>
{
public:
	SkillManager();
	~SkillManager();

	void CreateSkill(int id, Character* cha);
	void CreateSkill(int id, std::vector<Character*> vCha);

	void Render();
private:
	LSkill m_lSkill;
};

#endif