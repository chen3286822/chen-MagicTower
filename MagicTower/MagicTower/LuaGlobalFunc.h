#ifndef LUA_GLOBAL_FUNC_H
#define LUA_GLOBAL_FUNC_H

/*
	这里列出所有暴露给Lua脚本的接口
*/
#include "commonTools.h"
#include "CreatureManager.h"

//取得几个数之间最大的值
static int Max(lua_State* L)
{
	int n = lua_gettop(L);
	if(n <= 0)
	{
		lua_pushstring(L,"参数个数不正确");
		lua_error(L);
	}
	float fMax = 0;
	if(!lua_isnumber(L,1))
	{
		lua_pushstring(L,"参数类型不正确");
		lua_error(L);
	}
	fMax = lua_tonumber(L,1);
	for (int i=2;i<=n;i++)
	{
		if(!lua_isnumber(L,i))
		{
			lua_pushstring(L,"参数类型不正确");
			lua_error(L);
		}
		float fNext = lua_tonumber(L,i);
		if(fMax < fNext)
			fMax = fNext;
	}
	lua_pushnumber(L,fMax);
	return 1;
}

static int Move(lua_State* L)
{
	if(!lua_isnumber(L,1) ||
		!lua_isnumber(L,2) ||
		!lua_isnumber(L,3)) 
	{
		lua_pushstring(L,"参数类型不正确");
		lua_error(L);
	}
	int num = lua_tonumber(L,1);
	int tarX = lua_tonumber(L,2);
	int tarY = lua_tonumber(L,3);

	Character* player = CreatureManager::sInstance().GetCreature(num);
	if (player)
	{
		player->Move(tarX,tarY);
		//脚本移动，不需要记录先前位置，因为一般该移动属于剧情移动，不可以返回原位置
		player->GetOrigBlock().xpos = player->GetBlock().xpos;
		player->GetOrigBlock().ypos = player->GetBlock().ypos;
		player->GetOrigDirection() = player->GetCurDirection();
	}
	
	return 0;
}

void Lua_RegisterFunc()
{
	lua_register(g_pLua,"Max",Max);
	lua_register(g_pLua,"Move",Move);
}
#endif