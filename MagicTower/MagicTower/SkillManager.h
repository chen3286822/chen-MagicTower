#ifndef SKILL_MANAGER_H
#define SKILL_MANAGER_H

#include "commonTools.h"

class Character;
struct Skill
{
	int m_nID;		//����ID
	hgeAnimation* m_pAnim;
	int m_nFrames;	//����֡��
	int m_nFPS;
	int m_nX;		
	int m_nY;		
	int m_nFrameWidth;
	int m_nFrameHeight;

	Character* m_pChar;	//���ż��ܵĵ�λ
	float m_fOffsetX;			//����ƫ��
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