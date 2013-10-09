#ifndef LUA_GLOBAL_FUNC_H
#define LUA_GLOBAL_FUNC_H

/*
	�����г����б�¶��Lua�ű��Ľӿ�
*/
#include "commonTools.h"
#include "CreatureManager.h"
#include "MapManager.h"
#include "Character.h"
class Scene;

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

void Lua_RegisterFunc(lua_State* L);
///////////////////////////////////////////////////////////////////////
/*
	ȡ��ȫ��Scene��ָ��
*/
Scene* GetScene();

/*
	���õ�ǰ�ؿ�
*/
void SetCurrentMap(int level);

/*
	ȡ�õ�ǰ�ؿ���ͼ��ָ��
*/
Map* GetCurMap();

/*
	ȡ�õ�λ������
*/
CreatureManager* GetCreature();

/*
	����һ����λ
*/
Character* GetChar();
#endif