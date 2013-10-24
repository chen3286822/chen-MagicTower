#include "MyLua.h"
#include "Lua_API.h"
#include "LuaGlobalFunc.h"

MyLua::MyLua()
{
	m_pLuaState = lua_open();
	luaL_openlibs(m_pLuaState);
	luaopen_lua(m_pLuaState);
	Lua_RegisterFunc(m_pLuaState);
	m_mScriptLoaded.clear();
}

MyLua::~MyLua()
{
	lua_close(m_pLuaState);
}

bool MyLua::LoadFile(const char* file,bool bAlert)
{
	int iRet = luaL_dofile(m_pLuaState,file);
	if (iRet != 0)
	{
		if(bAlert)
		{
			char str[256];
			sprintf(str,"载入lua文件%s失败",file);
			g_debugString(__FILE__,__FUNCTION__,__LINE__,str);
		}
		return false;
	}
	return true;
}

bool MyLua::LoadString(const char* str,bool bAlert)
{
	int iRet = luaL_dostring(m_pLuaState,str);
	if (iRet != 0)
	{
		if (bAlert)
		{
			char strTemp[1024];
			sprintf(strTemp,"载入字符串%s失败",str);
			g_debugString(__FILE__,__FUNCTION__,__LINE__,strTemp);
		}
		return false;
	}
	return true;
}

bool MyLua::RunFunc(bool bAlert,const char* func,const char* format,...)
{
	int iRet = 0;
	lua_getglobal(m_pLuaState,func);

	va_list arg_list;
	va_start(arg_list,format);
	char* pFormat = (char*)format;
	while(*pFormat != '\0')
	{
		if('%' == *pFormat)
		{
			++pFormat;
			switch(*pFormat)
			{
			case 'f':
				lua_pushnumber(m_pLuaState,va_arg(arg_list,double));
				break;
			case 'd':
			case 'i':
				lua_pushnumber(m_pLuaState,va_arg(arg_list,int));
				break;
			case 's':
				lua_pushstring(m_pLuaState,va_arg(arg_list,char*));
				break;
			case 'z':
				lua_pushlightuserdata(m_pLuaState,va_arg(arg_list,void*));
				break;
			default:
				break;
			}
			iRet++;
		}
		++pFormat;
	}
	va_end(arg_list);
	iRet = lua_pcall(m_pLuaState,iRet,LUA_MULTRET,0);
	if (iRet != 0)
	{
		if(bAlert)
		{
			char temp[256];
			sprintf(temp,"调用脚本函数%s错误",func);
			g_debugString(__FILE__,__FUNCTION__,__LINE__,temp);
		}
		return false;
	}
	return  true;
}

void MyLua::LoadScript(int level)
{
	char pBuf[MAX_PATH];
	char pathConfig[MAX_PATH];
	GetCurrentDirectory(MAX_PATH,pBuf);
	for (int i=0;i<3;i++)
	{
		//每关3个脚本文件：战前脚本、战斗脚本、战后脚本
		sprintf(pathConfig,"%s\\res\\script\\%d_%d.lua",pBuf,level,i);
		if (_access(pathConfig,0) == 0)
		{
			luaL_dofile(m_pLuaState,pathConfig);
			m_mScriptLoaded[level*100+i] = true;
		}	
	}
}

bool MyLua::LoadSpecifyScriptOrNot(int level,eScript script)
{
	char pBuf[MAX_PATH];
	char pathConfig[MAX_PATH];
	GetCurrentDirectory(MAX_PATH,pBuf);
	switch(script)
	{
	case eScript_PreScene:
	case eScript_PreFight:
	case eScript_PostScene:
		sprintf(pathConfig,"%s\\res\\script\\%d_%d.lua",pBuf,level,(int)script);
		break;
	default:
		return false;
	}
	if (_access(pathConfig,0) == 0)
	{
		if(m_mScriptLoaded.find(level*100+(int)script) != m_mScriptLoaded.end())
		{
			if(m_mScriptLoaded[level*100+(int)script] == true)
				return true;
		}
		luaL_dofile(m_pLuaState,pathConfig);
		m_mScriptLoaded[level*100+(int)script] = true;
		return true;
	}
	return false;
}

void MyLua::PopErrorMsg()
{
	int top = lua_gettop(m_pLuaState);
	const char* errMsg = lua_tostring(m_pLuaState,-1);
	MessageBox(NULL,errMsg,"Error!",MB_OK);
}

MyLua g_MyLua;