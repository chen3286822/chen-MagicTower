#include "TexManager.h"
#include "App.h"

TexManager::TexManager(void)
{
	m_mWalkTex.clear();
	m_mFightTex.clear();
	m_mDeadTex.clear();
	m_mDefendTex.clear();
	m_mMap.clear();
	m_mMapInfo.clear();
}


TexManager::~TexManager(void)
{
	for (std::map<int,HTEXTURE>::iterator mit=m_mWalkTex.begin();mit!=m_mWalkTex.end();mit++)
	{
		App::sInstance().GetHGE()->Texture_Free(mit->second);
	}
	for (std::map<int,HTEXTURE>::iterator mit=m_mFightTex.begin();mit!=m_mFightTex.end();mit++)
	{
		App::sInstance().GetHGE()->Texture_Free(mit->second);
	}
	for (std::map<int,HTEXTURE>::iterator mit=m_mDeadTex.begin();mit!=m_mDeadTex.end();mit++)
	{
		App::sInstance().GetHGE()->Texture_Free(mit->second);
	}
	for (std::map<int,HTEXTURE>::iterator mit=m_mDefendTex.begin();mit!=m_mDefendTex.end();mit++)
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
	g_getFiles(path,files,".png",20,true,true);

	size_t found = 0;
	int ID = 0;
	int IDEx = 0;
	for (std::map<std::string,std::string>::iterator mit=files.begin();mit!=files.end();mit++)
	{
		found = mit->second.find('.');
		if(found != 0)
		{
			char strID[10];
			strncpy(strID,mit->second.c_str(),found);
			strID[found] = '\0';
			sscanf(strID,"%d-%d",&ID,&IDEx);
			ID--;
			m_mWalkTex[ID] = App::sInstance().GetHGE()->Texture_Load(mit->first.c_str());
			if(IDEx == 1)
				m_mFightTex[ID] = App::sInstance().GetHGE()->Texture_Load(mit->first.c_str());
			else if(IDEx == 2)
				m_mDeadTex[ID] = App::sInstance().GetHGE()->Texture_Load(mit->first.c_str());
			else if(IDEx == 3)
				m_mDefendTex[ID] = App::sInstance().GetHGE()->Texture_Load(mit->first.c_str());
		}
	}
	if (m_mWalkTex.empty())
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

std::map<int,HTEXTURE> TexManager::GetTex(int _ID)
{
	std::map<int,HTEXTURE> mapTex;
	if(!m_mWalkTex.empty() && m_mWalkTex.find(_ID) != m_mWalkTex.end())
		mapTex[Walk] =  m_mWalkTex[_ID];
	if(!m_mFightTex.empty() && m_mFightTex.find(_ID) != m_mFightTex.end())
		mapTex[Fight] =  m_mFightTex[_ID];
	if(!m_mDeadTex.empty() && m_mDeadTex.find(_ID) != m_mDeadTex.end())
		mapTex[Dead] =  m_mDeadTex[_ID];
	if(!m_mDefendTex.empty() && m_mDefendTex.find(_ID) != m_mDefendTex.end())
		mapTex[Defend] =  m_mDefendTex[_ID];

	return mapTex;
}

blockInfo TexManager::GetBlock(int _type)
{
	if (!m_mMapInfo.empty() && m_mMapInfo.find(_type) != m_mMapInfo.end())
		return m_mMapInfo[_type];

	return blockInfo(0,0,0,0,0);	//没有取得
}