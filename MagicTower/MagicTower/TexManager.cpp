#include "TexManager.h"
#include "App.h"

TexManager::TexManager(void)
{
	for(int i=0;i<eActionTex_Num;i++)
		m_mCharTex[i].clear();
	m_mMap.clear();
	m_mMapInfo.clear();
}


TexManager::~TexManager(void)
{
	for(int i=0;i<eActionTex_Num;i++)
	{
		for (std::map<int,HTEXTURE>::iterator mit=m_mCharTex[i].begin();mit!=m_mCharTex[i].end();mit++)
		{
			App::sInstance().GetHGE()->Texture_Free(mit->second);
		}
	}

	for (std::map<int,HTEXTURE>::iterator mit=m_mMap.begin();mit!=m_mMap.end();mit++)
	{
		App::sInstance().GetHGE()->Texture_Free(mit->second);
	}
}

bool TexManager::LoadTex(std::string path)
{
	std::string realPath[eActionTex_Num];
	realPath[eActionTex_Walk] = path + "\\walk";
	realPath[eActionTex_Attack] = path + "\\attack";
	realPath[eActionTex_Defend] = path + "\\defend";

	std::map<std::string,std::string> files[eActionTex_Num];
	for(int i=0;i<eActionTex_Num;i++)
		g_getFiles(realPath[i],files[i],".png",50,true,true);

	size_t found = 0;
	int ID = 0;
	int IDEx = 0;
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
				sscanf(strID,"%d-%d",&ID,&IDEx);
				ID--;
				if(IDEx == 1)
					m_mCharTex[i][ID] = App::sInstance().GetHGE()->Texture_Load(mit->first.c_str());
				ID = 0;
				IDEx = 0;
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

bool TexManager::LoadMap(std::string path)
{
	std::map<std::string,std::string> files;
	g_getFiles(path,files,".png",50,true);

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

bool TexManager::LoadUI(std::string path)
{
	std::map<std::string,std::string> files;
	g_getFiles(path,files,".png",50,true);
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

std::map<int,HTEXTURE> TexManager::GetTex(int _ID)
{
	std::map<int,HTEXTURE> mapTex;
	for (int i=0;i<eActionTex_Num;i++)
	{
		if(!m_mCharTex[i].empty() && m_mCharTex[i].find(_ID) != m_mCharTex[i].end())
			mapTex[i] =  m_mCharTex[i][_ID];
	}

	return mapTex;
}

blockInfo TexManager::GetBlock(int _type)
{
	if (!m_mMapInfo.empty() && m_mMapInfo.find(_type) != m_mMapInfo.end())
		return m_mMapInfo[_type];

	return blockInfo(0,0,0,0,0);	//没有取得
}