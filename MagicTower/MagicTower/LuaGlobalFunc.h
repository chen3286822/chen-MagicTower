#ifndef LUA_GLOBAL_FUNC_H
#define LUA_GLOBAL_FUNC_H

/*
	这里列出所有暴露给Lua脚本的接口
*/
#include "commonTools.h"
#include "CreatureManager.h"
#include "MapManager.h"
#include "Character.h"
class Scene;

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

void Lua_RegisterFunc(lua_State* L);
///////////////////////////////////////////////////////////////////////
/*
	取得全局Scene的指针
*/
Scene* GetScene();

/*
	设置当前关卡
*/
void SetCurrentMap(int level);

/*
	取得当前关卡地图的指针
*/
Map* GetCurMap();

/*
	取得单位管理器
*/
CreatureManager* GetCreature();

/*
	生成一个单位
*/
Character* GetChar();
#endif