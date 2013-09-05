/*
** Lua binding: lua
** Generated automatically by tolua++-1.0.92 on 09/05/13 17:41:00.
*/

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"

#include "tolua++.h"

/* Exported function */
TOLUA_API int  tolua_lua_open (lua_State* tolua_S);

#include "MagicTower/MagicTower/commonTools.h"
#include "MagicTower/MagicTower/Scene.h"
#include "MagicTower/MagicTower/LuaGlobalFunc.h"

/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"Scene");
 tolua_usertype(tolua_S,"Actor");
}

/* method: SetPos of class  Actor */
#ifndef TOLUA_DISABLE_tolua_lua_Actor_SetPos00
static int tolua_lua_Actor_SetPos00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Actor",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Actor* self = (Actor*)  tolua_tousertype(tolua_S,1,0);
  float posX = ((float)  tolua_tonumber(tolua_S,2,0));
  float posY = ((float)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetPos'", NULL);
#endif
  {
   self->SetPos(posX,posY);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetPos'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetAction of class  Actor */
#ifndef TOLUA_DISABLE_tolua_lua_Actor_SetAction00
static int tolua_lua_Actor_SetAction00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Actor",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Actor* self = (Actor*)  tolua_tousertype(tolua_S,1,0);
  int action = ((int)  tolua_tonumber(tolua_S,2,0));
  int dir = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetAction'", NULL);
#endif
  {
   self->SetAction(action,dir);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetAction'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetBackground of class  Scene */
#ifndef TOLUA_DISABLE_tolua_lua_Scene_SetBackground00
static int tolua_lua_Scene_SetBackground00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Scene",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Scene* self = (Scene*)  tolua_tousertype(tolua_S,1,0);
  int texID = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetBackground'", NULL);
#endif
  {
   self->SetBackground(texID);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetBackground'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: AddActor of class  Scene */
#ifndef TOLUA_DISABLE_tolua_lua_Scene_AddActor00
static int tolua_lua_Scene_AddActor00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Scene",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Scene* self = (Scene*)  tolua_tousertype(tolua_S,1,0);
  int ID = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'AddActor'", NULL);
#endif
  {
   self->AddActor(ID);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'AddActor'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetActor of class  Scene */
#ifndef TOLUA_DISABLE_tolua_lua_Scene_GetActor00
static int tolua_lua_Scene_GetActor00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Scene",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Scene* self = (Scene*)  tolua_tousertype(tolua_S,1,0);
  int ID = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetActor'", NULL);
#endif
  {
   Actor* tolua_ret = (Actor*)  self->GetActor(ID);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Actor");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetActor'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: PushAction of class  Scene */
#ifndef TOLUA_DISABLE_tolua_lua_Scene_PushAction00
static int tolua_lua_Scene_PushAction00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Scene",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,1,&tolua_err) ||
     !tolua_isnumber(tolua_S,6,1,&tolua_err) ||
     !tolua_isnumber(tolua_S,7,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,8,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Scene* self = (Scene*)  tolua_tousertype(tolua_S,1,0);
  int ID = ((int)  tolua_tonumber(tolua_S,2,0));
  int action = ((int)  tolua_tonumber(tolua_S,3,0));
  int dir = ((int)  tolua_tonumber(tolua_S,4,0));
  unsigned long time = (( unsigned long)  tolua_tonumber(tolua_S,5,0));
  unsigned long data = (( unsigned long)  tolua_tonumber(tolua_S,6,0));
  int num = ((int)  tolua_tonumber(tolua_S,7,-1));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'PushAction'", NULL);
#endif
  {
   self->PushAction(ID,action,dir,time,data,num);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'PushAction'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetCurrentNum of class  Scene */
#ifndef TOLUA_DISABLE_tolua_lua_Scene_GetCurrentNum00
static int tolua_lua_Scene_GetCurrentNum00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Scene",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Scene* self = (Scene*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetCurrentNum'", NULL);
#endif
  {
   int tolua_ret = (int)  self->GetCurrentNum();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetCurrentNum'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: GetScene */
#ifndef TOLUA_DISABLE_tolua_lua_GetScene00
static int tolua_lua_GetScene00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnoobj(tolua_S,1,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   Scene* tolua_ret = (Scene*)  GetScene();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Scene");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetScene'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* Open function */
TOLUA_API int tolua_lua_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
  tolua_cclass(tolua_S,"Actor","Actor","",NULL);
  tolua_beginmodule(tolua_S,"Actor");
   tolua_function(tolua_S,"SetPos",tolua_lua_Actor_SetPos00);
   tolua_function(tolua_S,"SetAction",tolua_lua_Actor_SetAction00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"Scene","Scene","",NULL);
  tolua_beginmodule(tolua_S,"Scene");
   tolua_function(tolua_S,"SetBackground",tolua_lua_Scene_SetBackground00);
   tolua_function(tolua_S,"AddActor",tolua_lua_Scene_AddActor00);
   tolua_function(tolua_S,"GetActor",tolua_lua_Scene_GetActor00);
   tolua_function(tolua_S,"PushAction",tolua_lua_Scene_PushAction00);
   tolua_function(tolua_S,"GetCurrentNum",tolua_lua_Scene_GetCurrentNum00);
  tolua_endmodule(tolua_S);
  tolua_function(tolua_S,"GetScene",tolua_lua_GetScene00);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_lua (lua_State* tolua_S) {
 return tolua_lua_open(tolua_S);
};
#endif
