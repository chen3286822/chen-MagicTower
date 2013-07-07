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

class ConfigManager : public Singleton<ConfigManager>
{
public:
	ConfigManager()
	{
		m_mCreatureConfig.clear();
		m_mCreatureInfo.clear();
	}
	~ConfigManager(){}

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
			int ID = -1;
			sscanf(mit->first.c_str(),"%s %d",simbol,&ID);
			if (strcmp(simbol,"CreatureInfo")==0 && ID!=-1)
			{
				std::string kind;
				int texID = -1;
				int slash = -1;
				slash = mit->second.find('/');
				if(slash!=-1)
				{
					kind = mit->second.substr(0,slash);
					texID = atoi(mit->second.substr(slash+1,mit->second.length()-slash).c_str());
				}
				if(texID != -1)
					m_mCreatureInfo[ID] = CreatureInfo(ID,kind,texID);
			}
		}
	}
	std::map<int,CreatureInfo>& GetCreatureInfo(){return m_mCreatureInfo;}

private:
	std::map<std::string,std::string> m_mCreatureConfig;
	std::map<int,CreatureInfo> m_mCreatureInfo;
};
#endif