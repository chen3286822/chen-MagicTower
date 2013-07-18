#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

#include "commonTools.h"
#include <sstream>

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
	int m_nID;								//����ID
	int m_nFrames;					//��Ч֡��
	int m_nAttackRange;			//������Χ
	int m_nOffX;							//���ƺ���ƫ��
	int m_nOffY;							//��������ƫ��
	int m_nIcon;							//ͼ���
	int m_nCastRange;				//�ͷŷ�Χ
	int m_nCost;							//����MP
	int m_nSkillType;					//��������
	int m_nAttack;						//������
	int m_nLastTurn;					//�����غ���
	std::string m_strName;		//��������
	SkillInfo()
	{
		m_nID = -1;
		m_nFrames = 0;
		m_nAttackRange = 0;
		m_nOffX = m_nOffY = 0;
		m_nIcon = -1;
		m_nCastRange = 0;
		m_nCost = 0;
		m_nSkillType = 0;
		m_nAttack = 0;
		m_nLastTurn = 0;
		m_strName = "";
	}
	SkillInfo(int id,int frames,int attackRange,int offx,int offy,int icon,int castRange,int cost,int skillType,int attack,int lastTurn,std::string name)
	{
		m_nID = id;
		m_nFrames = frames;
		m_nAttackRange = attackRange;
		m_nOffX = offx;
		m_nOffY = offy;
		m_nIcon = icon;
		m_nCastRange = castRange;
		m_nCost = cost;
		m_nSkillType = skillType;
		m_nAttack = attack;
		m_nLastTurn = lastTurn;
		m_strName = name;
	}
};

struct Item
{
	std::string m_strName;					//��Ʒ����
	int m_nType;										//��Ʒ����
	std::map<int,int> m_mEffect;		//��ƷЧ��
	Item()
	{
		m_strName = "";
		m_nType = -1;
		m_mEffect.clear();
	}
	Item(std::string name,int type,std::map<int,int> effect)
	{
		m_strName = name;
		m_nType = type;
		m_mEffect = effect;
	}
};

class ConfigManager : public Singleton<ConfigManager>
{
public:
	ConfigManager()
	{
		m_mCreatureConfig.clear();
		m_mCreatureInfo.clear();
		m_mItemConfig.clear();
		m_mItemInfo.clear();
		m_mSkillConfig.clear();
		m_mSkillInfo.clear();
	}
	~ConfigManager(){}

	void LoadConfig()
	{
		LoadCreatureConfig();
		LoadSkillConfig();
		LoadItemConfig();
	}

	void LoadCreatureConfig()
	{
		m_mCreatureInfo.clear();
		m_mCreatureSkill.clear();

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
				std::stringstream ssteam(mit->second);
				int skillNum = 0;
				skillNum = std::count(mit->second.begin(),mit->second.end(),'/') + 1;
				int* skillID = new int[skillNum];
				char cTemp;
				for (int i=0;i<skillNum;i++)
				{
					ssteam >> skillID[i] >> cTemp;
					m_mCreatureSkill[strKey].push_back(skillID[i]);
				}

// 				int searchPos = 0;
// 				for (int i=0;i<skillNum;i++)
// 				{
// 					int skillID = -1;
// 					int pos = mit->second.find('/',searchPos);
// 					if(pos == std::string::npos)
// 						skillID = atoi(mit->second.c_str()+searchPos);
// 					else
// 					{
// 						skillID = atoi((mit->second).substr(searchPos,pos).c_str());
// 						searchPos = pos + 1;
// 					}
// 					m_mCreatureSkill[strKey].push_back(skillID);
// 				}
			}
		}
	}
	std::map<int,CreatureInfo>& GetCreatureInfo(){return m_mCreatureInfo;}

	std::vector<int> GetCreatureSkill(std::string kind)
	{
		std::vector<int> vSkill;
		std::map<std::string, std::vector<int> >::iterator it = m_mCreatureSkill.find(kind);
		if(it != m_mCreatureSkill.end())
		{
			vSkill = it->second;
		}
		return vSkill;
	}

	void LoadSkillConfig()
	{
		m_mSkillInfo.clear();
		m_mSkillConfig.clear();


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
			if (strcmp(simbol,"SkillInfo")==0 && ID!=-1)
			{
				int frames = -1;
				int attackRange = -1;
				int offx = -1,offy = -1;
				int icon = -1;
				int castRange = -1;
				int cost = 0;
				int skillType = 0;
				int attack = 0;
				int lastTurn = 0;
				std::string name;
				sscanf(mit->second.c_str(),"%d/%d/%d/%d/%d/%d/%d/%d/%d/%d/%s",&frames,&attackRange,&offx,&offy,&icon,&castRange,&cost,&skillType,&attack,&lastTurn,name.c_str());
				m_mSkillInfo[ID] = SkillInfo(ID,frames,attackRange,offx,offy,icon,castRange,cost,skillType,attack,lastTurn,name.c_str());
			}
		}
	}
	std::map<int,SkillInfo>& GetSkillInfo(){return m_mSkillInfo;}

	void LoadItemConfig()
	{
		m_mItemInfo.clear();
		m_mItemConfig.clear();

		char pBuf[MAX_PATH];
		char pathConfig[MAX_PATH];
		GetCurrentDirectory(MAX_PATH,pBuf);
		sprintf(pathConfig,"%s\\res\\config\\Item.txt",pBuf);
		m_mItemConfig = g_parseConfigFile(pathConfig);

		for (std::map<std::string,std::string>::iterator mit=m_mItemConfig.begin();mit!=m_mItemConfig.end();mit++)
		{
			char simbol[256];
			int ID = -1;
			sscanf(mit->first.c_str(),"%s %d",simbol,&ID);
			if (strcmp(simbol,"ItemInfo")==0 && ID!=-1)
			{
				std::string name = "";
				int type = -1;
				int num = 0;
				std::stringstream ssteam(mit->second);
				char cTemp;
				ssteam >> name >> cTemp >> type >> cTemp >> num >> cTemp;
				int effectType = -1;
				int effectValue = -1;
				std::map<int,int> effect;
				for (int i=0;i<num;i++)
				{
					ssteam >> effectType >> cTemp >> effectValue >> cTemp;
					effect[effectType] = effectValue;
				}
				m_mItemInfo[ID] = Item(name,type,effect);
			}
		}
	}
private:
	std::map<std::string,std::string> m_mCreatureConfig;
	std::map<int,CreatureInfo> m_mCreatureInfo;
	std::map<std::string, std::vector<int> > m_mCreatureSkill;
	std::map<std::string,std::string> m_mItemConfig;
	std::map<int,Item> m_mItemInfo;

	std::map<std::string,std::string> m_mSkillConfig;
	std::map<int,SkillInfo> m_mSkillInfo;
};
#endif