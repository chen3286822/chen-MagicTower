#ifndef LUA_GLOBAL_FUNC_H
#define LUA_GLOBAL_FUNC_H

/*
	这里列出所有暴露给Lua脚本的接口
*/
#include "commonTools.h"
#include "CreatureManager.h"

/*
	取得几个数之间最大的值
	max = Max(a,b,c,...)
*/
static int Max(lua_State* L);

/*
	移动指定num号的单位至指定位置
	Move(num,posX,posY)
*/
static int Move(lua_State* L);

void Lua_RegisterFunc();
#endif