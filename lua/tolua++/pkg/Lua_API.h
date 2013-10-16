/*
** Lua binding: lua
** Generated automatically by tolua++-1.0.92 on 10/16/13 17:21:58.
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
#include "MagicTower/MagicTower/MapManager.h"
#include "MagicTower/MagicTower/Character.h"
#include "MagicTower/MagicTower/CreatureManager.h"

/* function to release collected object via destructor */
#ifdef __cplusplus

static int tolua_collect_tagBlock (lua_State* tolua_S)
{
 tagBlock* self = (tagBlock*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}
#endif


/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"Map");
 tolua_usertype(tolua_S,"Actor");
 tolua_usertype(tolua_S,"Scene");
 tolua_usertype(tolua_S,"CreatureManager");
 tolua_usertype(tolua_S,"tagBlock");
 tolua_usertype(tolua_S,"Character");
}

/* get function: xpos of class  tagBlock */
#ifndef TOLUA_DISABLE_tolua_get_tagBlock_xpos
static int tolua_get_tagBlock_xpos(lua_State* tolua_S)
{
  tagBlock* self = (tagBlock*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'xpos'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->xpos);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: xpos of class  tagBlock */
#ifndef TOLUA_DISABLE_tolua_set_tagBlock_xpos
static int tolua_set_tagBlock_xpos(lua_State* tolua_S)
{
  tagBlock* self = (tagBlock*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'xpos'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->xpos = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: ypos of class  tagBlock */
#ifndef TOLUA_DISABLE_tolua_get_tagBlock_ypos
static int tolua_get_tagBlock_ypos(lua_State* tolua_S)
{
  tagBlock* self = (tagBlock*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'ypos'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->ypos);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: ypos of class  tagBlock */
#ifndef TOLUA_DISABLE_tolua_set_tagBlock_ypos
static int tolua_set_tagBlock_ypos(lua_State* tolua_S)
{
  tagBlock* self = (tagBlock*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'ypos'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->ypos = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: attri of class  tagBlock */
#ifndef TOLUA_DISABLE_tolua_get_tagBlock_unsigned_attri
static int tolua_get_tagBlock_unsigned_attri(lua_State* tolua_S)
{
  tagBlock* self = (tagBlock*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'attri'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->attri);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: attri of class  tagBlock */
#ifndef TOLUA_DISABLE_tolua_set_tagBlock_unsigned_attri
static int tolua_set_tagBlock_unsigned_attri(lua_State* tolua_S)
{
  tagBlock* self = (tagBlock*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'attri'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->attri = (( unsigned long)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  tagBlock */
#ifndef TOLUA_DISABLE_tolua_lua_tagBlock_new00
static int tolua_lua_tagBlock_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"tagBlock",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int _x = ((int)  tolua_tonumber(tolua_S,2,0));
  int _y = ((int)  tolua_tonumber(tolua_S,3,0));
  {
   tagBlock* tolua_ret = (tagBlock*)  Mtolua_new((tagBlock)(_x,_y));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"tagBlock");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  tagBlock */
#ifndef TOLUA_DISABLE_tolua_lua_tagBlock_new00_local
static int tolua_lua_tagBlock_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"tagBlock",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int _x = ((int)  tolua_tonumber(tolua_S,2,0));
  int _y = ((int)  tolua_tonumber(tolua_S,3,0));
  {
   tagBlock* tolua_ret = (tagBlock*)  Mtolua_new((tagBlock)(_x,_y));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"tagBlock");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  tagBlock */
#ifndef TOLUA_DISABLE_tolua_lua_tagBlock_new01
static int tolua_lua_tagBlock_new01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"tagBlock",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  {
   tagBlock* tolua_ret = (tagBlock*)  Mtolua_new((tagBlock)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"tagBlock");
  }
 }
 return 1;
tolua_lerror:
 return tolua_lua_tagBlock_new00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  tagBlock */
#ifndef TOLUA_DISABLE_tolua_lua_tagBlock_new01_local
static int tolua_lua_tagBlock_new01_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"tagBlock",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  {
   tagBlock* tolua_ret = (tagBlock*)  Mtolua_new((tagBlock)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"tagBlock");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
tolua_lerror:
 return tolua_lua_tagBlock_new00_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

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

/* method: SetHide of class  Actor */
#ifndef TOLUA_DISABLE_tolua_lua_Actor_SetHide00
static int tolua_lua_Actor_SetHide00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Actor",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Actor* self = (Actor*)  tolua_tousertype(tolua_S,1,0);
  bool hide = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetHide'", NULL);
#endif
  {
   self->SetHide(hide);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetHide'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: IsHide of class  Actor */
#ifndef TOLUA_DISABLE_tolua_lua_Actor_IsHide00
static int tolua_lua_Actor_IsHide00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Actor",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Actor* self = (Actor*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'IsHide'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->IsHide();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IsHide'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetPlace of class  Scene */
#ifndef TOLUA_DISABLE_tolua_lua_Scene_SetPlace00
static int tolua_lua_Scene_SetPlace00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Scene",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Scene* self = (Scene*)  tolua_tousertype(tolua_S,1,0);
  const char* name = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetPlace'", NULL);
#endif
  {
   self->SetPlace(name);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetPlace'.",&tolua_err);
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
   Actor* tolua_ret = (Actor*)  self->AddActor(ID);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Actor");
  }
 }
 return 1;
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

/* method: PushWords of class  Scene */
#ifndef TOLUA_DISABLE_tolua_lua_Scene_PushWords00
static int tolua_lua_Scene_PushWords00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Scene",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isstring(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Scene* self = (Scene*)  tolua_tousertype(tolua_S,1,0);
  int head = ((int)  tolua_tonumber(tolua_S,2,0));
  const char* name = ((const char*)  tolua_tostring(tolua_S,3,0));
  const char* words = ((const char*)  tolua_tostring(tolua_S,4,0));
  int num = ((int)  tolua_tonumber(tolua_S,5,-1));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'PushWords'", NULL);
#endif
  {
   self->PushWords(head,name,words,num);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'PushWords'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetTurns of class  Map */
#ifndef TOLUA_DISABLE_tolua_lua_Map_SetTurns00
static int tolua_lua_Map_SetTurns00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Map",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Map* self = (Map*)  tolua_tousertype(tolua_S,1,0);
  int turn = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetTurns'", NULL);
#endif
  {
   self->SetTurns(turn);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetTurns'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetLevel of class  Map */
#ifndef TOLUA_DISABLE_tolua_lua_Map_GetLevel00
static int tolua_lua_Map_GetLevel00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Map",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Map* self = (Map*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetLevel'", NULL);
#endif
  {
   int tolua_ret = (int)  self->GetLevel();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetLevel'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: AddTrigger of class  Map */
#ifndef TOLUA_DISABLE_tolua_lua_Map_AddTrigger00
static int tolua_lua_Map_AddTrigger00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Map",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,6,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,7,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,8,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,9,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Map* self = (Map*)  tolua_tousertype(tolua_S,1,0);
  int triggerID = ((int)  tolua_tonumber(tolua_S,2,0));
  const char* func = ((const char*)  tolua_tostring(tolua_S,3,0));
  int turns = ((int)  tolua_tonumber(tolua_S,4,0));
  int num1 = ((int)  tolua_tonumber(tolua_S,5,0));
  int num2 = ((int)  tolua_tonumber(tolua_S,6,0));
  int x = ((int)  tolua_tonumber(tolua_S,7,0));
  int y = ((int)  tolua_tonumber(tolua_S,8,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'AddTrigger'", NULL);
#endif
  {
   self->AddTrigger(triggerID,func,turns,num1,num2,x,y);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'AddTrigger'.",&tolua_err);
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

/* function: SetCurrentMap */
#ifndef TOLUA_DISABLE_tolua_lua_SetCurrentMap00
static int tolua_lua_SetCurrentMap00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int level = ((int)  tolua_tonumber(tolua_S,1,0));
  {
   SetCurrentMap(level);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetCurrentMap'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: GetCurMap */
#ifndef TOLUA_DISABLE_tolua_lua_GetCurMap00
static int tolua_lua_GetCurMap00(lua_State* tolua_S)
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
   Map* tolua_ret = (Map*)  GetCurMap();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Map");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetCurMap'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: GetCreature */
#ifndef TOLUA_DISABLE_tolua_lua_GetCreature00
static int tolua_lua_GetCreature00(lua_State* tolua_S)
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
   CreatureManager* tolua_ret = (CreatureManager*)  GetCreature();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"CreatureManager");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetCreature'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: GetChar */
#ifndef TOLUA_DISABLE_tolua_lua_GetChar00
static int tolua_lua_GetChar00(lua_State* tolua_S)
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
   Character* tolua_ret = (Character*)  GetChar();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Character");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetChar'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: MyMessageBox */
#ifndef TOLUA_DISABLE_tolua_lua_MyMessageBox00
static int tolua_lua_MyMessageBox00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const char* msg = ((const char*)  tolua_tostring(tolua_S,1,0));
  {
   MyMessageBox(msg);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'MyMessageBox'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Init of class  Character */
#ifndef TOLUA_DISABLE_tolua_lua_Character_Init00
static int tolua_lua_Character_Init00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Character",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,6,&tolua_err) || !tolua_isusertype(tolua_S,6,"tagBlock",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Character* self = (Character*)  tolua_tousertype(tolua_S,1,0);
  int _Level = ((int)  tolua_tonumber(tolua_S,2,0));
  int _ID = ((int)  tolua_tonumber(tolua_S,3,0));
  int _Num = ((int)  tolua_tonumber(tolua_S,4,0));
  int _Action = ((int)  tolua_tonumber(tolua_S,5,0));
  struct tagBlock _block = *(( struct tagBlock*)  tolua_tousertype(tolua_S,6,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Init'", NULL);
#endif
  {
   self->Init(_Level,_ID,_Num,_Action,_block);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Init'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetMoveAbility of class  Character */
#ifndef TOLUA_DISABLE_tolua_lua_Character_SetMoveAbility00
static int tolua_lua_Character_SetMoveAbility00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Character",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"Map",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Character* self = (Character*)  tolua_tousertype(tolua_S,1,0);
  int _ability = ((int)  tolua_tonumber(tolua_S,2,0));
  Map* map = ((Map*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetMoveAbility'", NULL);
#endif
  {
   self->SetMoveAbility(_ability,map);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetMoveAbility'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetCamp of class  Character */
#ifndef TOLUA_DISABLE_tolua_lua_Character_SetCamp00
static int tolua_lua_Character_SetCamp00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Character",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Character* self = (Character*)  tolua_tousertype(tolua_S,1,0);
  int _camp = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetCamp'", NULL);
#endif
  {
   self->SetCamp(_camp);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetCamp'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetName of class  Character */
#ifndef TOLUA_DISABLE_tolua_lua_Character_SetName00
static int tolua_lua_Character_SetName00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Character",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Character* self = (Character*)  tolua_tousertype(tolua_S,1,0);
  const char* name = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetName'", NULL);
#endif
  {
   self->SetName(name);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetName'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Talk of class  Character */
#ifndef TOLUA_DISABLE_tolua_lua_Character_Talk00
static int tolua_lua_Character_Talk00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Character",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isstring(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Character* self = (Character*)  tolua_tousertype(tolua_S,1,0);
  int head = ((int)  tolua_tonumber(tolua_S,2,0));
  const char* name = ((const char*)  tolua_tostring(tolua_S,3,0));
  const char* word = ((const char*)  tolua_tostring(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Talk'", NULL);
#endif
  {
   self->Talk(head,name,word);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Talk'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Turn of class  Character */
#ifndef TOLUA_DISABLE_tolua_lua_Character_Turn00
static int tolua_lua_Character_Turn00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Character",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Character* self = (Character*)  tolua_tousertype(tolua_S,1,0);
  int dir = ((int)  tolua_tonumber(tolua_S,2,0));
  unsigned long time = (( unsigned long)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Turn'", NULL);
#endif
  {
   self->Turn(dir,time);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Turn'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Attack of class  Character */
#ifndef TOLUA_DISABLE_tolua_lua_Character_Attack00
static int tolua_lua_Character_Attack00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Character",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Character* self = (Character*)  tolua_tousertype(tolua_S,1,0);
  unsigned long time = (( unsigned long)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Attack'", NULL);
#endif
  {
   self->Attack(time);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Attack'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Crit of class  Character */
#ifndef TOLUA_DISABLE_tolua_lua_Character_Crit00
static int tolua_lua_Character_Crit00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Character",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Character* self = (Character*)  tolua_tousertype(tolua_S,1,0);
  unsigned long time = (( unsigned long)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Crit'", NULL);
#endif
  {
   self->Crit(time);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Crit'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Attacked of class  Character */
#ifndef TOLUA_DISABLE_tolua_lua_Character_Attacked00
static int tolua_lua_Character_Attacked00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Character",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Character* self = (Character*)  tolua_tousertype(tolua_S,1,0);
  unsigned long time = (( unsigned long)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Attacked'", NULL);
#endif
  {
   self->Attacked(time);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Attacked'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Defend of class  Character */
#ifndef TOLUA_DISABLE_tolua_lua_Character_Defend00
static int tolua_lua_Character_Defend00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Character",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Character* self = (Character*)  tolua_tousertype(tolua_S,1,0);
  unsigned long time = (( unsigned long)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Defend'", NULL);
#endif
  {
   self->Defend(time);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Defend'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Hurt of class  Character */
#ifndef TOLUA_DISABLE_tolua_lua_Character_Hurt00
static int tolua_lua_Character_Hurt00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Character",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Character* self = (Character*)  tolua_tousertype(tolua_S,1,0);
  unsigned long time = (( unsigned long)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Hurt'", NULL);
#endif
  {
   self->Hurt(time);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Hurt'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Step of class  Character */
#ifndef TOLUA_DISABLE_tolua_lua_Character_Step00
static int tolua_lua_Character_Step00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Character",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Character* self = (Character*)  tolua_tousertype(tolua_S,1,0);
  unsigned long time = (( unsigned long)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Step'", NULL);
#endif
  {
   self->Step(time);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Step'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: MoveTo of class  Character */
#ifndef TOLUA_DISABLE_tolua_lua_Character_MoveTo00
static int tolua_lua_Character_MoveTo00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Character",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Character* self = (Character*)  tolua_tousertype(tolua_S,1,0);
  int x = ((int)  tolua_tonumber(tolua_S,2,0));
  int y = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'MoveTo'", NULL);
#endif
  {
   self->MoveTo(x,y);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'MoveTo'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: AddEnemy of class  CreatureManager */
#ifndef TOLUA_DISABLE_tolua_lua_CreatureManager_AddEnemy00
static int tolua_lua_CreatureManager_AddEnemy00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CreatureManager",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Character",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CreatureManager* self = (CreatureManager*)  tolua_tousertype(tolua_S,1,0);
  Character* _enemy = ((Character*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'AddEnemy'", NULL);
#endif
  {
   self->AddEnemy(_enemy);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'AddEnemy'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: AddFriend of class  CreatureManager */
#ifndef TOLUA_DISABLE_tolua_lua_CreatureManager_AddFriend00
static int tolua_lua_CreatureManager_AddFriend00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CreatureManager",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"Character",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CreatureManager* self = (CreatureManager*)  tolua_tousertype(tolua_S,1,0);
  Character* _friend = ((Character*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'AddFriend'", NULL);
#endif
  {
   self->AddFriend(_friend);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'AddFriend'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetEnemy of class  CreatureManager */
#ifndef TOLUA_DISABLE_tolua_lua_CreatureManager_GetEnemy00
static int tolua_lua_CreatureManager_GetEnemy00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CreatureManager",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CreatureManager* self = (CreatureManager*)  tolua_tousertype(tolua_S,1,0);
  int x = ((int)  tolua_tonumber(tolua_S,2,0));
  int y = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetEnemy'", NULL);
#endif
  {
   Character* tolua_ret = (Character*)  self->GetEnemy(x,y);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Character");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetEnemy'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetEnemy of class  CreatureManager */
#ifndef TOLUA_DISABLE_tolua_lua_CreatureManager_GetEnemy01
static int tolua_lua_CreatureManager_GetEnemy01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CreatureManager",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  CreatureManager* self = (CreatureManager*)  tolua_tousertype(tolua_S,1,0);
  int num = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetEnemy'", NULL);
#endif
  {
   Character* tolua_ret = (Character*)  self->GetEnemy(num);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Character");
  }
 }
 return 1;
tolua_lerror:
 return tolua_lua_CreatureManager_GetEnemy00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetFriend of class  CreatureManager */
#ifndef TOLUA_DISABLE_tolua_lua_CreatureManager_GetFriend00
static int tolua_lua_CreatureManager_GetFriend00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CreatureManager",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CreatureManager* self = (CreatureManager*)  tolua_tousertype(tolua_S,1,0);
  int x = ((int)  tolua_tonumber(tolua_S,2,0));
  int y = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetFriend'", NULL);
#endif
  {
   Character* tolua_ret = (Character*)  self->GetFriend(x,y);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Character");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetFriend'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetFriend of class  CreatureManager */
#ifndef TOLUA_DISABLE_tolua_lua_CreatureManager_GetFriend01
static int tolua_lua_CreatureManager_GetFriend01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CreatureManager",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  CreatureManager* self = (CreatureManager*)  tolua_tousertype(tolua_S,1,0);
  int num = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetFriend'", NULL);
#endif
  {
   Character* tolua_ret = (Character*)  self->GetFriend(num);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Character");
  }
 }
 return 1;
tolua_lerror:
 return tolua_lua_CreatureManager_GetFriend00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetCreature of class  CreatureManager */
#ifndef TOLUA_DISABLE_tolua_lua_CreatureManager_GetCreature00
static int tolua_lua_CreatureManager_GetCreature00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CreatureManager",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CreatureManager* self = (CreatureManager*)  tolua_tousertype(tolua_S,1,0);
  int x = ((int)  tolua_tonumber(tolua_S,2,0));
  int y = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetCreature'", NULL);
#endif
  {
   Character* tolua_ret = (Character*)  self->GetCreature(x,y);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Character");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetCreature'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetCreature of class  CreatureManager */
#ifndef TOLUA_DISABLE_tolua_lua_CreatureManager_GetCreature01
static int tolua_lua_CreatureManager_GetCreature01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CreatureManager",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  CreatureManager* self = (CreatureManager*)  tolua_tousertype(tolua_S,1,0);
  int num = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetCreature'", NULL);
#endif
  {
   Character* tolua_ret = (Character*)  self->GetCreature(num);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Character");
  }
 }
 return 1;
tolua_lerror:
 return tolua_lua_CreatureManager_GetCreature00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetPause of class  CreatureManager */
#ifndef TOLUA_DISABLE_tolua_lua_CreatureManager_SetPause00
static int tolua_lua_CreatureManager_SetPause00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CreatureManager",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CreatureManager* self = (CreatureManager*)  tolua_tousertype(tolua_S,1,0);
  bool pause = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetPause'", NULL);
#endif
  {
   self->SetPause(pause);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetPause'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetPause of class  CreatureManager */
#ifndef TOLUA_DISABLE_tolua_lua_CreatureManager_GetPause00
static int tolua_lua_CreatureManager_GetPause00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CreatureManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CreatureManager* self = (CreatureManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetPause'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->GetPause();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetPause'.",&tolua_err);
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
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"tagBlock","tagBlock","",tolua_collect_tagBlock);
  #else
  tolua_cclass(tolua_S,"tagBlock","tagBlock","",NULL);
  #endif
  tolua_beginmodule(tolua_S,"tagBlock");
   tolua_variable(tolua_S,"xpos",tolua_get_tagBlock_xpos,tolua_set_tagBlock_xpos);
   tolua_variable(tolua_S,"ypos",tolua_get_tagBlock_ypos,tolua_set_tagBlock_ypos);
   tolua_variable(tolua_S,"attri",tolua_get_tagBlock_unsigned_attri,tolua_set_tagBlock_unsigned_attri);
   tolua_function(tolua_S,"new",tolua_lua_tagBlock_new00);
   tolua_function(tolua_S,"new_local",tolua_lua_tagBlock_new00_local);
   tolua_function(tolua_S,".call",tolua_lua_tagBlock_new00_local);
   tolua_function(tolua_S,"new",tolua_lua_tagBlock_new01);
   tolua_function(tolua_S,"new_local",tolua_lua_tagBlock_new01_local);
   tolua_function(tolua_S,".call",tolua_lua_tagBlock_new01_local);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"Actor","Actor","",NULL);
  tolua_beginmodule(tolua_S,"Actor");
   tolua_function(tolua_S,"SetPos",tolua_lua_Actor_SetPos00);
   tolua_function(tolua_S,"SetAction",tolua_lua_Actor_SetAction00);
   tolua_function(tolua_S,"SetHide",tolua_lua_Actor_SetHide00);
   tolua_function(tolua_S,"IsHide",tolua_lua_Actor_IsHide00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"Scene","Scene","",NULL);
  tolua_beginmodule(tolua_S,"Scene");
   tolua_function(tolua_S,"SetPlace",tolua_lua_Scene_SetPlace00);
   tolua_function(tolua_S,"SetBackground",tolua_lua_Scene_SetBackground00);
   tolua_function(tolua_S,"AddActor",tolua_lua_Scene_AddActor00);
   tolua_function(tolua_S,"GetActor",tolua_lua_Scene_GetActor00);
   tolua_function(tolua_S,"PushAction",tolua_lua_Scene_PushAction00);
   tolua_function(tolua_S,"GetCurrentNum",tolua_lua_Scene_GetCurrentNum00);
   tolua_function(tolua_S,"PushWords",tolua_lua_Scene_PushWords00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"Map","Map","",NULL);
  tolua_beginmodule(tolua_S,"Map");
   tolua_function(tolua_S,"SetTurns",tolua_lua_Map_SetTurns00);
   tolua_function(tolua_S,"GetLevel",tolua_lua_Map_GetLevel00);
   tolua_function(tolua_S,"AddTrigger",tolua_lua_Map_AddTrigger00);
  tolua_endmodule(tolua_S);
  tolua_function(tolua_S,"GetScene",tolua_lua_GetScene00);
  tolua_function(tolua_S,"SetCurrentMap",tolua_lua_SetCurrentMap00);
  tolua_function(tolua_S,"GetCurMap",tolua_lua_GetCurMap00);
  tolua_function(tolua_S,"GetCreature",tolua_lua_GetCreature00);
  tolua_function(tolua_S,"GetChar",tolua_lua_GetChar00);
  tolua_function(tolua_S,"MyMessageBox",tolua_lua_MyMessageBox00);
  tolua_cclass(tolua_S,"Character","Character","",NULL);
  tolua_beginmodule(tolua_S,"Character");
   tolua_function(tolua_S,"Init",tolua_lua_Character_Init00);
   tolua_function(tolua_S,"SetMoveAbility",tolua_lua_Character_SetMoveAbility00);
   tolua_function(tolua_S,"SetCamp",tolua_lua_Character_SetCamp00);
   tolua_function(tolua_S,"SetName",tolua_lua_Character_SetName00);
   tolua_function(tolua_S,"Talk",tolua_lua_Character_Talk00);
   tolua_function(tolua_S,"Turn",tolua_lua_Character_Turn00);
   tolua_function(tolua_S,"Attack",tolua_lua_Character_Attack00);
   tolua_function(tolua_S,"Crit",tolua_lua_Character_Crit00);
   tolua_function(tolua_S,"Attacked",tolua_lua_Character_Attacked00);
   tolua_function(tolua_S,"Defend",tolua_lua_Character_Defend00);
   tolua_function(tolua_S,"Hurt",tolua_lua_Character_Hurt00);
   tolua_function(tolua_S,"Step",tolua_lua_Character_Step00);
   tolua_function(tolua_S,"MoveTo",tolua_lua_Character_MoveTo00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"CreatureManager","CreatureManager","",NULL);
  tolua_beginmodule(tolua_S,"CreatureManager");
   tolua_function(tolua_S,"AddEnemy",tolua_lua_CreatureManager_AddEnemy00);
   tolua_function(tolua_S,"AddFriend",tolua_lua_CreatureManager_AddFriend00);
   tolua_function(tolua_S,"GetEnemy",tolua_lua_CreatureManager_GetEnemy00);
   tolua_function(tolua_S,"GetEnemy",tolua_lua_CreatureManager_GetEnemy01);
   tolua_function(tolua_S,"GetFriend",tolua_lua_CreatureManager_GetFriend00);
   tolua_function(tolua_S,"GetFriend",tolua_lua_CreatureManager_GetFriend01);
   tolua_function(tolua_S,"GetCreature",tolua_lua_CreatureManager_GetCreature00);
   tolua_function(tolua_S,"GetCreature",tolua_lua_CreatureManager_GetCreature01);
   tolua_function(tolua_S,"SetPause",tolua_lua_CreatureManager_SetPause00);
   tolua_function(tolua_S,"GetPause",tolua_lua_CreatureManager_GetPause00);
  tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_lua (lua_State* tolua_S) {
 return tolua_lua_open(tolua_S);
};
#endif

