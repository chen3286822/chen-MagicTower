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

	//载入特定关的所有脚本
	void LoadScript(int level);
	//检查是否存在脚本，存在则加载
	bool LoadSpecifyScriptOrNot(int level,eScript script);
	
	void PopErrorMsg();

	lua_State* GetLuaState(){return m_pLuaState;}
private:
	lua_State* m_pLuaState;
	std::map<int,bool> m_mScriptLoaded;	//记录lua文件是否加载, level*100+script类型 代表某一个lua文件，好比第一关的PreScene就是1*100+1 = 101
};

extern MyLua g_MyLua;

#endif