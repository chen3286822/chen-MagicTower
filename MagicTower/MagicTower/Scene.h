#ifndef SCENE_H
#define SCENE_H

/*
	游戏自动场景控制
*/
#include "commonTools.h"

enum eAction
{
	eAction_Stand,		//站立
	eAction_Walk,			//走路
	eAction_Sit,				//坐下
	eAction_Angry,		//生气
	eAction_Shout,		//大喊
	eAction_Cry,			//哭泣
	eAction_Hand,			//伸手
	eAction_Salute,		//敬礼
	eAction_Angry_Sit,	//生气(坐下)
	eAction_Shout_Sit,	//大喊(坐下)
	eAction_Cry_Sit,		//哭泣(坐下)
	eAction_Dead,			//死亡
	eAction_Knee,			//跪下
	eAction_Ease,			//悠闲
	eAction_Treat,			//威胁
	eAction_Cut,			//砍下
	eAction_Inspire,		//鼓舞

	//特殊动作
	eAction_Talk,			//说话，此动作不属于人物动作
	eAction_Hide,			//隐藏人物动作
	eAction_Appear,		//出现人物动作
};

struct NewAction
{
	int m_nID;
	eAction m_eAction;
	eDirection m_eDir;
	DWORD m_dwTime;
	DWORD m_dwData;
	int m_nNum;
	std::string m_strName;
	std::string m_strWords;
	NewAction()
	{
		m_nID = 0;
		m_eAction = eAction_Stand;
		m_eDir = eDirection_Right;
		m_dwTime = 0;
		m_dwData = 0;
		m_nNum = -1;
		m_strName = "";
		m_strWords = "";
	}
	NewAction(int id,eAction action,eDirection dir,DWORD time,DWORD data,int num)
	{
		m_nID = id;
		m_eAction = action;
		m_eDir = dir;
		m_dwTime = time;
		m_dwData = data;
		m_nNum = num;
	}
};
//场景中的演员
class Actor
{
public:
	Actor();
	~Actor();

	static void sGetActionIndex(eAction action,int& index,int& frames);

	void Render();
	void Update(float dt);
	//每个演员的所有动作是由正反两张图组成的
	void SetAnimTex(HTEXTURE tex1,HTEXTURE tex2);
	void SetFrames(float x1,float y1,float w,float h,int frames,int FPS);

	void SetPos(float posX,float posY);
	void SetAction(int action,int dir);
	void SetHide(bool hide){m_bHide = hide;}
	bool IsHide(){return m_bHide;}
	
	//是否处于行动中
	bool IsInAction();
	void PushAction(NewAction action);
	void SetBgOffset(float x,float y);
private:
	hgeAnimation* m_pAnim;
	HTEXTURE m_iTex[2];
	float m_fPosX;
	float m_fPosY;
	float m_fStartX;
	float m_fStartY;
	eDirection m_eDir;
	eAction m_eAction;
	NewAction m_iCurAction;
	float m_fBgOffsetX;	//背景图绘制偏移
	float m_fBgOffsetY;
	bool m_bHide;
};

//push walk动作，需要填入步数和方向参数
//步数是把场景按照斜十字方向分为了10*10的小格子
//每个格子代表一步
class Scene : public Singleton<Scene>
{
public:
	Scene();
	~Scene();

	void Render();
	void Update(float dt);

	void Release();

	void SetPlace(const char* name);
	void SetBackground(int texID);
	Actor* AddActor(int ID);
	Actor* GetActor(int ID);
	//给指定角色添加指定动作
	//动作序号表示该动作执行的顺序，如果几个动作序号相同
	//则表示是同时进行的动作，但是要等到这几个角色都处于可行动时才可以执行
	void PushAction(int ID,int action,int dir,DWORD time=0,DWORD data=0,int num=-1);
	int GetCurrentNum(){return m_Num;}

	//推送一个说话动作，此动作和演员动作共享行动队列
	void PushWords(int head,const char* name,const char* words,int num=-1);
private:
	hgeSprite* m_pBackground;
	std::map<int,Actor*> m_mActors;
	int m_Num;	//记录当前动作的序号
	std::list<std::vector<NewAction>> m_lVNewAction;
	eActionState m_eState;
	float m_fBgOffsetX;	//背景图绘制偏移
	float m_fBgOffsetY;
	std::string m_strPlace;	//场景地点
	bool m_bShowPlaceName;	//是否显示场景名称
	DWORD m_dwShowTime;	//场景名称显示时间
};

#endif