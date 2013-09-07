#ifndef MYLUA_H
#define MYLUA_H

#include "commonTools.h"

class MyLua
{
public:
	MyLua();
	~MyLua();

	void Init(){}
	void Release(){}

	void LoadFile(const char* file);
	void LoadString(const char* str);
	void RunFunc(const char* func);

	//载入特定关的所有脚本
	void LoadSceneScript(int level);
	
	void PopErrorMsg();

	lua_State* GetLuaState(){return m_pLuaState;}
private:
	lua_State* m_pLuaState;
};

extern MyLua g_MyLua;

#endif