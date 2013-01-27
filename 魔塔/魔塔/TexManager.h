#pragma once
#include "commonTools.h"

struct blockInfo
{
	float x,y;
	float width;
	float height;
	HTEXTURE tex;

	blockInfo()
	{
		x = 0;
		y = 0;
		width = 0;
		height = 0;
		tex = 0;
	}

	blockInfo(float _x,float _y,float _width,float _height,HTEXTURE _tex)
	{
		x = _x;
		y = _y;
		width = _width;
		height = _height;
		tex = _tex;
	}
};

class TexManager :public Singleton<TexManager>
{
public:
	TexManager(void);
	~TexManager(void);

	bool LoadTex(std::string path);
	bool LoadMap(std::string path);

	HTEXTURE getTex(int _ID);
	blockInfo	getBlock(int _type);

private:
	std::map<int,HTEXTURE> m_mTex;		//���е�λ��ͼƬ
	std::map<int,HTEXTURE> m_mMap;		//���а�����ͼͼ���ͼƬ
	std::map<int,blockInfo> m_mMapInfo;	//���е�ͼͼ���ڸ�ͼƬ�е�λ�ô�С��Ϣ
};

