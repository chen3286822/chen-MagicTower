/*
** Lua binding: lua
** Generated automatically by tolua++-1.0.92 on 09/03/13 21:33:54.
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
}

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
  tolua_cclass(tolua_S,"Scene","Scene","",NULL);
  tolua_beginmodule(tolua_S,"Scene");
   tolua_function(tolua_S,"SetBackground",tolua_lua_Scene_SetBackground00);
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

