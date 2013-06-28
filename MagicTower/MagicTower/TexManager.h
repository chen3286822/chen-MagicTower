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
	~TexManager(void);

	bool LoadTex(std::string path);
	bool LoadMap(std::string path);
	bool LoadUI(std::string path);

	std::map<int,HTEXTURE> GetTex(int _ID);
	std::map<int,HTEXTURE> GetTex(){return m_mUITex;}
	blockInfo	GetBlock(int _type);

private:
	std::map<int,HTEXTURE> m_mWalkTex;		//所有单位的行动图片
	std::map<int,HTEXTURE> m_mFightTex;		//所有单位的战斗图片
	std::map<int,HTEXTURE> m_mDeadTex;		//所有单位的死亡图片
	std::map<int,HTEXTURE> m_mDefendTex;		//所有单位的防御图片
	std::map<int,HTEXTURE> m_mMap;		//所有包含地图图块的图片
	std::map<int,HTEXTURE> m_mUITex;		//所有UI相关图片
	std::map<int,blockInfo> m_mMapInfo;	//所有地图图块在各图片中的位置大小信息
};

