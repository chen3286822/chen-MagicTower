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
	std::map<int,HTEXTURE> m_mWalkTex;		//���е�λ���ж�ͼƬ
	std::map<int,HTEXTURE> m_mFightTex;		//���е�λ��ս��ͼƬ
	std::map<int,HTEXTURE> m_mDeadTex;		//���е�λ������ͼƬ
	std::map<int,HTEXTURE> m_mDefendTex;		//���е�λ�ķ���ͼƬ
	std::map<int,HTEXTURE> m_mMap;		//���а�����ͼͼ���ͼƬ
	std::map<int,HTEXTURE> m_mUITex;		//����UI���ͼƬ
	std::map<int,blockInfo> m_mMapInfo;	//���е�ͼͼ���ڸ�ͼƬ�е�λ�ô�С��Ϣ
};

