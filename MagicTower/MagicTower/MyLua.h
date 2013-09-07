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

	//�����ض��ص����нű�
	void LoadSceneScript(int level);
	
	void PopErrorMsg();

	lua_State* GetLuaState(){return m_pLuaState;}
private:
	lua_State* m_pLuaState;
};

extern MyLua g_MyLua;

#endif