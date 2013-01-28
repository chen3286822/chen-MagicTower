#ifndef	APPLICATION_H
#define APPLICATION_H

#include "commonTools.h"

class PictureController;

class Application : public Singleton<Application>
{
public:
	Application(void);
	~Application(void);

	bool systemInit(HWND hwnd);
	void loadResource();
	void run();
	void freeResource();
	void cleanUp();

	bool appUpdate();
	bool appRender();

	inline void setNext(bool _next){m_goNext = _next;}
	inline bool getNext(){return m_goNext;}
	inline void setLast(bool  _last){m_goLast = _last;}
	inline bool getLast(){return m_goLast;}
	inline HGE* Application::getHGE(){return hge;}

	void				setMouseTex(hgeSprite* spr);
	inline void	setObjectTag(int tag,int action=0){m_tag = tag;m_action = action;}		//��ÿ�����嶼��һ��Ψһ��int�ͱ�ǩ,ͬʱ���輸���������
	CMarkup*	getMarkup(){return m_markUp;}
	inline void	setMarkup(CMarkup* _markup){m_markUp = _markup;}

	bool	saveMapToXml();
	bool	loadMapFromXml();

	inline void	clearCfg(){m_config.clear();}
	Config& getConfig(){return m_config;}

private:
	HGE* hge;
	hgeGUI* gui;
	hgeGUIButton* nextButton;
	hgeGUIButton* lastButton;
	HTEXTURE tex;
	PictureController* pic;
	 hgeVertex m_MapVertex[4];

	bool m_goNext;
	bool m_goLast;

	hgeSprite* m_mouseSpr;

	int m_tag;
	int m_action;

	std::vector<Block> m_Blocks;	//�洢��ͼ�����е��θ���
	std::vector<MapObject*> m_vMapObject;	//�洢��ʾ�ڵ�ͼ�ϵ�����

	CMarkup*	m_markUp;	//xml parser

	Config m_config;	//�ؿ�����

private:
	void drawMapLine();
	void drawMap();
	void drawMouseTex();
	void drawSmallRect(float x,float y);

	void updateMap();

	void resetMap();
};

#endif