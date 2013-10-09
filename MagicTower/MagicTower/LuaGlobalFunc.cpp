#include "LuaGlobalFunc.h"
#include "Scene.h"

static int Max(lua_State* L)
{
	int n = lua_gettop(L);
	if(n <= 0)
	{
		luaL_error(L,"������������ȷ");
	}
	float fMax = 0;
	if(!lua_isnumber(L,1))
	{
		luaL_error(L,"�������Ͳ���ȷ");
	}
	fMax = lua_tonumber(L,1);
	for (int i=2;i<=n;i++)
	{
		if(!lua_isnumber(L,i))
		{
			luaL_error(L,"�������Ͳ���ȷ");
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
		luaL_error(L,"�������Ͳ���ȷ");
	}
	int num = lua_tonumber(L,1);
	int tarX = lua_tonumber(L,2);
	int tarY = lua_tonumber(L,3);

	Character* player = CreatureManager::sInstance().GetCreature(num);
	if (player)
	{
		player->Move(tarX,tarY);
		//�ű��ƶ�������Ҫ��¼��ǰλ�ã���Ϊһ����ƶ����ھ����ƶ��������Է���ԭλ��
		player->GetOrigBlock().xpos = player->GetBlock().xpos;
		player->GetOrigBlock().ypos = player->GetBlock().ypos;
		player->GetOrigDirection() = player->GetCurDirection();
	}

	return 0;
}

void Lua_RegisterFunc(lua_State* L)
{
	lua_register(L,"Max",Max);
	lua_register(L,"Move",Move);
}

Scene* GetScene()
{
	return (Scene::sInstancePtr());
}

void SetCurrentMap(int level)
{
	MapManager::sInstance().SetCurrentLevel(level);
	MapManager::sInstance().GetCurrentMap()->Init();
}

Map* GetCurMap()
{
	return (MapManager::sInstance().GetCurrentMap());
}

CreatureManager* GetCreature()
{
	return (CreatureManager::sInstancePtr());
}

Character* GetChar()
{
	return (new Character);
}