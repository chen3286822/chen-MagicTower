#pragma once
#include "commonTools.h"

struct blockInfo
{
	float m_fX,m_fY;
	float m_fWidth;
	float m_fHeight;
	HTEXTURE m_iTex;

	blockInfo()
	{
		m_fX = 0;
		m_fY = 0;
		m_fWidth = 0;
		m_fHeight = 0;
		m_iTex = 0;
	}

	blockInfo(float _x,float _y,float _width,float _height,HTEXTURE _tex)
	{
		m_fX = _x;
		m_fY = _y;
		m_fWidth = _width;
		m_fHeight = _height;
		m_iTex = _tex;
	}
};

class TexManager :public Singleton<TexManager>
{
public:
	TexManager(void);
	virtual ~TexManager(void);

	bool LoadTex();

	bool LoadMapTex();
	bool LoadCharTex();
	bool LoadMapBlock();
	bool LoadUI();
	bool LoadSkill();
	bool LoadItem();
	bool LoadScene();
	bool LoadActor();
	bool LoadHead();

	std::map<int,HTEXTURE> GetTex(int _ID);
	HTEXTURE GetMapTex(int _ID);
	HTEXTURE GetUITex(int UIID);
	HTEXTURE GetSkillTex(int _ID);
	HTEXTURE GetItemTex(int _ID);
	HTEXTURE GetSceneTex(int _ID);
	void GetActorTex(int _ID,HTEXTURE& tex1,HTEXTURE& tex2);
	HTEXTURE GetHeadTex(int _ID);
	blockInfo	GetBlock(int _type);

private:
	std::map<int,HTEXTURE> m_mCharTex[3];		//所有单位的动作图片
	std::map<int,HTEXTURE> m_mMapTex;		//所有地图图片
	std::map<int,HTEXTURE> m_mMap;		//所有包含地图图块的图片
	std::map<int,HTEXTURE> m_mUITex;		//所有UI相关图片
	std::map<int,HTEXTURE> m_mSkillTex;	//所有技能相关图片
	std::map<int,HTEXTURE> m_mItemTex;	//所有物品相关图片
	std::map<int,HTEXTURE> m_mSceneTex;//所有场景相关图片
	std::map<int,HTEXTURE> m_mActorTex[2];	//所有场景演员图片
	std::map<int,HTEXTURE> m_mHeadTex;	//所有演员头像
	std::map<int,blockInfo> m_mMapInfo;	//所有地图图块在各图片中的位置大小信息

	void FreeTex(std::map<int,HTEXTURE>&);
	HTEXTURE LoadTexFromFile(std::string resPath,int texID);
};

