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
	std::map<int,HTEXTURE> m_mCharTex[3];		//���е�λ�Ķ���ͼƬ
	std::map<int,HTEXTURE> m_mMapTex;		//���е�ͼͼƬ
	std::map<int,HTEXTURE> m_mMap;		//���а�����ͼͼ���ͼƬ
	std::map<int,HTEXTURE> m_mUITex;		//����UI���ͼƬ
	std::map<int,HTEXTURE> m_mSkillTex;	//���м������ͼƬ
	std::map<int,HTEXTURE> m_mItemTex;	//������Ʒ���ͼƬ
	std::map<int,HTEXTURE> m_mSceneTex;//���г������ͼƬ
	std::map<int,HTEXTURE> m_mActorTex[2];	//���г�����ԱͼƬ
	std::map<int,HTEXTURE> m_mHeadTex;	//������Աͷ��
	std::map<int,blockInfo> m_mMapInfo;	//���е�ͼͼ���ڸ�ͼƬ�е�λ�ô�С��Ϣ

	void FreeTex(std::map<int,HTEXTURE>&);
	HTEXTURE LoadTexFromFile(std::string resPath,int texID);
};

