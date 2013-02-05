#include "TexManager.h"
#include "App.h"

TexManager::TexManager(void)
{
	m_mTex.clear();
	m_mMap.clear();
	m_mMapInfo.clear();
}


TexManager::~TexManager(void)
{
	for (std::map<int,HTEXTURE>::iterator mit=m_mTex.begin();mit!=m_mTex.end();mit++)
	{
		App::sInstance().GetHGE()->Texture_Free(mit->second);
	}
	for (std::map<int,HTEXTURE>::iterator mit=m_mMap.begin();mit!=m_mMap.end();mit++)
	{
		App::sInstance().GetHGE()->Texture_Free(mit->second);
	}
}

bool TexManager::LoadTex(std::string path)
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
			m_mTex[ID] = App::sInstance().GetHGE()->Texture_Load(mit->first.c_str());
		}
	}
	if (m_mTex.empty())
	{
		return false;
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
	m_mMapInfo[Road]			=	blockInfo(96.0f,32.0f,32.0f,32.0f,m_mMap[0]);
	m_mMapInfo[CityRoad]		=	blockInfo(96.0f,64.0f,32.0f,32.0f,m_mMap[0]);
	m_mMapInfo[Forest]			=	blockInfo(96.0f,0.0f,32.0f,32.0f,m_mMap[0]);
	m_mMapInfo[City]				=	blockInfo(64.0f,192.0f,32.0f,32.0f,m_mMap[0]);
	return true;
}

HTEXTURE TexManager::GetTex(int _ID)
{
	if(!m_mTex.empty() && m_mTex.find(_ID) != m_mTex.end())
		return m_mTex[_ID];
	
	return 0;
}

blockInfo TexManager::GetBlock(int _type)
{
	if (!m_mMapInfo.empty() && m_mMapInfo.find(_type) != m_mMapInfo.end())
		return m_mMapInfo[_type];

	return blockInfo(0,0,0,0,0);	//没有取得
}