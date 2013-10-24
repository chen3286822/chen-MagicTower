#ifndef MYLUA_H
#define MYLUA_H

#include "commonTools.h"

enum eScript
{
	eScript_PreScene,
	eScript_PreFight,
	eScript_PostScene,
};

class MyLua
{
public:
	MyLua();
	virtual ~MyLua();

	void Init(){}
	void Release(){}

	bool LoadFile(const char* file,bool bAlert=false);
	bool LoadString(const char* str,bool bAlert=false);
	bool RunFunc(bool bAlert,const char* func,const char* format,...);

	//�����ض��ص����нű�
	void LoadScript(int level);
	//����Ƿ���ڽű������������
	bool LoadSpecifyScriptOrNot(int level,eScript script);
	
	void PopErrorMsg();

	lua_State* GetLuaState(){return m_pLuaState;}
private:
	lua_State* m_pLuaState;
	std::map<int,bool> m_mScriptLoaded;	//��¼lua�ļ��Ƿ����, level*100+script���� ����ĳһ��lua�ļ����ñȵ�һ�ص�PreScene����1*100+1 = 101
};

extern MyLua g_MyLua;

#endif