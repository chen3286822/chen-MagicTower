#ifndef LUA_GLOBAL_FUNC_H
#define LUA_GLOBAL_FUNC_H

/*
	�����г����б�¶��Lua�ű��Ľӿ�
*/
#include "commonTools.h"
#include "CreatureManager.h"

/*
	ȡ�ü�����֮������ֵ
	max = Max(a,b,c,...)
*/
static int Max(lua_State* L);

/*
	�ƶ�ָ��num�ŵĵ�λ��ָ��λ��
	Move(num,posX,posY)
*/
static int Move(lua_State* L);

void Lua_RegisterFunc();
#endif