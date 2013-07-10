#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

#include "commonTools.h"

struct CreatureInfo
{
	int m_nID;
	std::string m_strKind;
	int m_nTexID;
	CreatureInfo()
	{
		m_nID = -1;
		m_strKind = "";
		m_nTexID = -1;
	}
	CreatureInfo(int id,std::string kind,int texid)
	{
		m_nID = id;
		m_strKind = kind;
		m_nTexID = texid;
	}
};

struct SkillInfo
{
	int m_nID;
	int m_nFrames;
	int m_nAttackRange;
	int m_nOffX;
	int m_nOffY;
	SkillInfo()
	{
		m_nID = -1;
		m_nFrames = 0;
		m_nAttackRange = 0;
		m_nOffX = m_nOffY = 0;
	}
	SkillInfo(int id,int frames,int attackRange,int offx,int offy)
	{
		m_nID = id;
		m_nFrames = frames;
		m_nAttackRange = attackRange;
		m_nOffX = offx;
		m_nOffY = offy;
	}
};

class ConfigManager : public Singleton<ConfigManager>
{
public:
	ConfigManager()
	{
		m_mCreatureConfig.clear();
		m_mCreatureInfo.clear();
	}
	~ConfigManager(){}

	void LoadConfig()
	{
		LoadCreatureConfig();
		LoadSkillConfig();
	}

	void LoadCreatureConfig()
	{
		char pBuf[MAX_PATH];
		char pathConfig[MAX_PATH];
		GetCurrentDirectory(MAX_PATH,pBuf);
		sprintf(pathConfig,"%s\\res\\config\\Creature.txt",pBuf);
		m_mCreatureConfig = g_parseConfigFile(pathConfig);

		for (std::map<std::string,std::string>::iterator mit=m_mCreatureConfig.begin();mit!=m_mCreatureConfig.end();mit++)
		{
			char simbol[256];
			char strKey[256];
			sscanf(mit->first.c_str(),"%s %s",simbol,strKey);
			if (strcmp(simbol,"CreatureInfo")==0)
			{
				int ID = atoi(strKey);
				char kind[50];
				int texID = -1;
				sscanf(mit->second.c_str(),"%s /%d",kind,&texID);
				m_mCreatureInfo[ID] = CreatureInfo(ID,kind,texID);
			}
			else if(strcmp(simbol,"CreatureSkill")==0)
			{
				int skillNum = 0;
				skillNum = std::count(mit->second.begin(),mit->second.end(),'/') + 1;
				int* skills = new int[skillNum];
				std::string paramSkills = "%d";
				for(int i=1;i<skillNum;i++)
					paramSkills += "/%d";
			}
		}
	}
	std::map<int,CreatureInfo>& GetCreatureInfo(){return m_mCreatureInfo;}

	vector<int> GetCreatureSkill(std::string kind)
	{
		vector<int> vSkill;
		std::map<std::string, vector<int> >::iterator it = m_mCreatureSkill.find(kind);
		if(it != m_mCreatureSkill.end())
		{
			vSkill = it->second;
		}
		return vSkill;
	}

	void LoadSkillConfig()
	{
		char pBuf[MAX_PATH];
		char pathConfig[MAX_PATH];
		GetCurrentDirectory(MAX_PATH,pBuf);
		sprintf(pathConfig,"%s\\res\\config\\Skill.txt",pBuf);
		m_mSkillConfig = g_parseConfigFile(pathConfig);

		for (std::map<std::string,std::string>::iterator mit=m_mSkillConfig.begin();mit!=m_mSkillConfig.end();mit++)
		{
			char simbol[256];
			int ID = -1;
			sscanf(mit->first.c_str(),"%s %d",simbol,&ID);
			if (strcmp(simbol,"SkillInf")==0 && ID!=-1)
			{
				int frames = -1;
				int attackRange = -1;
				int offx = -1,offy = -1;
				sscanf(mit->second.c_str(),"%d/%d/%d/%d",&frames,&attackRange,&offx,&offy);
				m_mSkillInfo[ID] = SkillInfo(ID,frames,attackRange,offx,offy);
			}
		}
	}
	std::map<int,SkillInfo>& GetSkillInfo(){return m_mSkillInfo;}
private:
	std::map<std::string,std::string> m_mCreatureConfig;
	std::map<int,CreatureInfo> m_mCreatureInfo;
	std::map<std::string, vector<int> > m_mCreatureSkill;

	std::map<std::string,std::string> m_mSkillConfig;
	std::map<int,SkillInfo> m_mSkillInfo;
};
#endif