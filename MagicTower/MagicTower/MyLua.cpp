#include "MyLua.h"
#include "Lua_API.h"
#include "LuaGlobalFunc.h"

MyLua::MyLua()
{
	m_pLuaState = lua_open();
	luaL_openlibs(m_pLuaState);
	luaopen_lua(m_pLuaState);
	Lua_RegisterFunc(m_pLuaState);
}

MyLua::~MyLua()
{
	lua_close(m_pLuaState);
}

void MyLua::LoadFile(const char* file)
{
	int iRet = luaL_dofile(m_pLuaState,file);
	if (iRet != 0)
	{
		char str[256];
		sprintf(str,"‘ÿ»ÎluaŒƒº˛%s ß∞‹",file);
		g_debugString(__FILE__,__FUNCTION__,__LINE__,str);
	}
}

void MyLua::LoadString(const char* str)
{
	int iRet = luaL_dostring(m_pLuaState,str);
	if (iRet != 0)
	{
		char strTemp[1024];
		sprintf(strTemp,"‘ÿ»Î◊÷∑˚¥Æ%s ß∞‹",str);
		g_debugString(__FILE__,__FUNCTION__,__LINE__,strTemp);
	}
}

void MyLua::RunFunc(const char* func)
{

}

void MyLua::LoadSceneScript(int level)
{
	char pBuf[MAX_PATH];
	char pathConfig[MAX_PATH];
	GetCurrentDirectory(MAX_PATH,pBuf);
	for (int i=0;i<3;i++)
	{
		sprintf(pathConfig,"%s\\res\\script\\%d_%d.lua",pBuf,level,i);
		if (_access(pBuf,0) == 0)
		{
			luaL_dofile(m_pLuaState,pathConfig);
		}	
	}
}

void MyLua::PopErrorMsg()
{
	int top = lua_gettop(m_pLuaState);
	const char* errMsg = lua_tostring(m_pLuaState,-1);
	MessageBox(NULL,errMsg,"Error!",MB_OK);
}

MyLua g_MyLua;