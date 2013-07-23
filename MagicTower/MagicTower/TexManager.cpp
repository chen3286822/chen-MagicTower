#include "TexManager.h"
#include "App.h"
#include "ConfigManager.h"

TexManager::TexManager(void)
{
	for(int i=0;i<eActionTex_Num;i++)
		m_mCharTex[i].clear();
	m_mMap.clear();
	m_mMapInfo.clear();
	m_mUITex.clear();
	m_mSkillTex.clear();
	m_mItemTex.clear();
}


TexManager::~TexManager(void)
{
	for(int i=0;i<eActionTex_Num;i++)
	{
		FreeTex(m_mCharTex[i]);
	}
	FreeTex(m_mMap);
	FreeTex(m_mUITex);
	FreeTex(m_mSkillTex);
	FreeTex(m_mItemTex);
}

void TexManager::FreeTex(std::map<int,HTEXTURE>& mTex)
{
	for (std::map<int,HTEXTURE>::iterator mit=mTex.begin();mit!=mTex.end();mit++)
	{
		App::sInstance().GetHGE()->Texture_Free(mit->second);
	}
}

bool TexManager::LoadTex()
{
//	bool bCharTex = LoadCharTex();
	bool bCharTex = true;
	bool bMapTex = LoadMap();
//	bool bUITex = LoadUI();
	bool bUITex = true;
//	bool bSkillTex = LoadSkill();
	bool bSkillTex = true;
//	bool bItemTex = LoadItem();
	bool bItemTex = true;

	return (bCharTex && bMapTex && bUITex && bSkillTex && bItemTex);
}

bool TexManager::LoadCharTex()
{
	for(int i=0;i<eActionTex_Num;i++)
		m_mCharTex[i].clear();

	char pBuf[MAX_PATH];
	char pathTex[MAX_PATH];
	GetCurrentDirectory(MAX_PATH,pBuf);
	sprintf(pathTex,"%s\\res\\tex",pBuf);
	std::string path = pathTex;

	std::string realPath[eActionTex_Num];
	realPath[eActionTex_Walk] = path + "\\walk";
	realPath[eActionTex_Attack] = path + "\\attack";
	realPath[eActionTex_Defend] = path + "\\defend";

	std::map<std::string,std::string> files[eActionTex_Num];
	for(int i=0;i<eActionTex_Num;i++)
		g_getFiles(realPath[i],files[i],".png",50,true,true);

	size_t found = 0;
	int ID = 0;
	for (int i=0;i<eActionTex_Num;i++)
	{
		for (std::map<std::string,std::string>::iterator mit=files[i].begin();mit!=files[i].end();mit++)
		{
			found = mit->second.find('.');
			if(found != 0)
			{
				char strID[10];
				strncpy(strID,mit->second.c_str(),found);
				strID[found] = '\0';
				ID = atoi(strID);
				m_mCharTex[i][ID] = App::sInstance().GetHGE()->Texture_Load(mit->first.c_str());
			}
		}
	}

	for(int i=0;i<eActionTex_Num;i++)
	{
		if (m_mCharTex[i].empty())
		{
			return false;
		}
	}


	return true;
}

bool TexManager::LoadMap()
{
	m_mMap.clear();
	m_mMapInfo.clear();

	char pBuf[MAX_PATH];
	char pathTex[MAX_PATH];
	GetCurrentDirectory(MAX_PATH,pBuf);
	sprintf(pathTex,"%s\\res\\map",pBuf);

	std::map<std::string,std::string> files;
	g_getFiles(pathTex,files,".png",50,true);

	size_t found = 0;
	int ID = 0;
	for (std::map<std::string,std::string>::iterator mit=files.begin();mit!=files.end();mit++)
	{
		found = mit->second.find('.');
		if(found != 0)
		{
			char strID[10];
			strncpy(strID,mit->second.c_str(),found);
			strID[found] = '\0';
			ID = atoi(strID) - 1;
			//载入地图
			m_mMap[ID] = App::sInstance().GetHGE()->Texture_Load(mit->first.c_str());
		}
	}
	if (m_mMap.empty())
	{
		return false;
	}
	//载入地图中图块信息
	m_mMapInfo[eTerrain_Road]			=	blockInfo(96.0f,32.0f,32.0f,32.0f,m_mMap[0]);
	m_mMapInfo[eTerrain_CityRoad]		=	blockInfo(96.0f,64.0f,32.0f,32.0f,m_mMap[0]);
	m_mMapInfo[eTerrain_Forest]			=	blockInfo(96.0f,0.0f,32.0f,32.0f,m_mMap[0]);
	m_mMapInfo[eTerrain_City]				=	blockInfo(64.0f,192.0f,32.0f,32.0f,m_mMap[0]);
	return true;
}

bool TexManager::LoadUI()
{
	m_mUITex.clear();

	char pBuf[MAX_PATH];
	char pathTex[MAX_PATH];
	GetCurrentDirectory(MAX_PATH,pBuf);
	sprintf(pathTex,"%s\\res\\UI",pBuf);

	std::map<std::string,std::string> files;
	g_getFiles(pathTex,files,".png",50,true);
	size_t found = 0;
	int ID = 0;
	for (std::map<std::string,std::string>::iterator mit=files.begin();mit!=files.end();mit++)
	{
		found = mit->second.find('.');
		if(found != 0)
		{
			char strID[10];
			strncpy(strID,mit->second.c_str(),found);
			strID[found] = '\0';
			ID = atoi(strID);
			m_mUITex[ID] = App::sInstance().GetHGE()->Texture_Load(mit->first.c_str());
		}
	}
	if (m_mUITex.empty())
	{
		return false;
	}
	return true;
}

bool TexManager::LoadSkill()
{
	m_mSkillTex.clear();

	char pBuf[MAX_PATH];
	char pathTex[MAX_PATH];
	GetCurrentDirectory(MAX_PATH,pBuf);
	sprintf(pathTex,"%s\\res\\tex\\skill",pBuf);

	std::map<std::string,std::string> files;
	g_getFiles(pathTex,files,".png",50,true);
	size_t found = 0;
	int ID = 0;
	for (std::map<std::string,std::string>::iterator mit=files.begin();mit!=files.end();mit++)
	{
		found = mit->second.find('.');
		if(found != 0)
		{
			char strID[10];
			strncpy(strID,mit->second.c_str(),found);
			strID[found] = '\0';
			ID = atoi(strID) - 1;
			m_mSkillTex[ID] = App::sInstance().GetHGE()->Texture_Load(mit->first.c_str());
		}
	}
	if (m_mSkillTex.empty())
	{
		return false;
	}
	return true;
}

bool TexManager::LoadItem()
{
	m_mItemTex.clear();

	char pBuf[MAX_PATH];
	char pathTex[MAX_PATH];
	GetCurrentDirectory(MAX_PATH,pBuf);
	sprintf(pathTex,"%s\\res\\tex\\item",pBuf);

	std::map<std::string,std::string> files;
	g_getFiles(pathTex,files,".png",104,true);
	size_t found = 0;
	int ID = 0;
	for (std::map<std::string,std::string>::iterator mit=files.begin();mit!=files.end();mit++)
	{
		found = mit->second.find('.');
		if(found != 0)
		{
			char strID[10];
			strncpy(strID,mit->second.c_str(),found);
			strID[found] = '\0';
			ID = atoi(strID) - 1;
			m_mItemTex[ID] = App::sInstance().GetHGE()->Texture_Load(mit->first.c_str());
		}
	}
	if (m_mItemTex.empty())
	{
		return false;
	}
	return true;
}

HTEXTURE TexManager::LoadTexFromFile(std::string resPath,int texID)
{
	char pBuf[MAX_PATH];
	char pathTex[MAX_PATH];
	GetCurrentDirectory(MAX_PATH,pBuf);
	sprintf(pathTex,"%s\\%s\\%d.png",pBuf,resPath.c_str(),texID);

	return App::sInstance().GetHGE()->Texture_Load(pathTex);
}

HTEXTURE TexManager::GetSkillTex(int _ID)
{
	for (std::map<int,HTEXTURE>::iterator it=m_mSkillTex.begin();it!=m_mSkillTex.end();it++)
	{
		if(_ID == it->first)
			return it->second;
	}
	//没有找到，从文件载入
	HTEXTURE skillTex = LoadTexFromFile("res\\tex\\skill",_ID+1);
	if(skillTex)
	{
		m_mSkillTex[_ID] = skillTex;
		return skillTex;
	}
	else
	{
		g_debugString(__FILE__,__FUNCTION__,__LINE__,"载入技能图片失败");
		exit(0);
	}
	return 0;
}

HTEXTURE TexManager::GetItemTex(int _ID)
{
	for (std::map<int,HTEXTURE>::iterator it=m_mItemTex.begin();it!=m_mItemTex.end();it++)
	{
		if(_ID == it->first)
			return it->second;
	}
	//没有找到，从文件载入
	HTEXTURE itemTex = LoadTexFromFile("res\\tex\\item",_ID+1);
	if(itemTex)
	{
		m_mItemTex[_ID] = itemTex;
		return itemTex;
	}
	else
	{
		g_debugString(__FILE__,__FUNCTION__,__LINE__,"载入物品图片失败");
		exit(0);
	}
	return 0;
}

HTEXTURE TexManager::GetUITex(int UIID)
{
	for (std::map<int,HTEXTURE>::iterator it=m_mUITex.begin();it!=m_mUITex.end();it++)
	{
		if(UIID == it->first)
			return it->second;
	}
	//没有找到，从文件载入
	HTEXTURE UITex = LoadTexFromFile("res\\UI",UIID);
	if(UITex)
	{
		m_mUITex[UIID] = UITex;
		return UITex;
	}
	else
	{
		g_debugString(__FILE__,__FUNCTION__,__LINE__,"载入UI图片失败");
		exit(0);
	}
	return 0;
}

std::map<int,HTEXTURE> TexManager::GetTex(int _ID)
{
	int texID = -1;
	std::map<int,CreatureInfo>& creatureInfo = ConfigManager::sInstance().GetCreatureInfo();
	for (std::map<int,CreatureInfo>::iterator it=creatureInfo.begin();it!=creatureInfo.end();it++)
	{
		if(it->first == _ID)
		{
			texID = it->second.m_nTexID;
			break;
		}
	}
	std::map<int,HTEXTURE> mapTex;
	if(texID!=-1)
	{
		for (int i=0;i<eActionTex_Num;i++)
		{
			if(!m_mCharTex[i].empty() && m_mCharTex[i].find(texID) != m_mCharTex[i].end())
				mapTex[i] =  m_mCharTex[i][texID];
		}
		if (mapTex.empty())		//没有读取到，从文件中尝试载入
		{
			HTEXTURE attackTex = LoadTexFromFile("res\\tex\\attack",texID);
			HTEXTURE walkTex = LoadTexFromFile("res\\tex\\walk",texID);
			HTEXTURE defendTex = LoadTexFromFile("res\\tex\\defend",texID);
			if(attackTex && walkTex && defendTex)
			{
				m_mCharTex[eActionTex_Walk][texID] = walkTex;
				m_mCharTex[eActionTex_Attack][texID] = attackTex;
				m_mCharTex[eActionTex_Defend][texID] = defendTex;
				for (int i=0;i<eActionTex_Num;i++)
					mapTex[i] = m_mCharTex[i][texID];
			}
			else	//载入失败
			{
				g_debugString(__FILE__,__FUNCTION__,__LINE__,"载入角色图片失败");
				exit(0);
			}
		}
	}


	return mapTex;
}

blockInfo TexManager::GetBlock(int _type)
{
	if (!m_mMapInfo.empty() && m_mMapInfo.find(_type) != m_mMapInfo.end())
		return m_mMapInfo[_type];

	return blockInfo(0,0,0,0,0);	//没有取得
}