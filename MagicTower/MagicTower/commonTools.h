#ifndef COMMON_TOOLS_H
#define COMMON_TOOLS_H

#pragma warning(disable: 4244)
#pragma warning(disable: 4018)
#pragma warning(disable: 4996)

//#define MARKUP_STL
#include <windows.h>
//#ifdef _WINDOWS_
//#undef _WINDOWS_
 #include <atlbase.h>
//#endif


#include <CommDlg.h>
#include <Shlobj.h.>
#include <hge.h>
#include <hgesprite.h>
#include <hgeanim.h>
#include <hgeguictrls.h>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <list>
#include <map>
#include <algorithm>
#include "Singleton.h"
#include "MagicTowerEdit\MagicTowerEdit\Resource.h"
#include "vld.h"

#include "Markup.h"
#include <io.h>

extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

#define LEVEL_NUM 20



#define APP_WIDTH	800
#define APP_HEIGHT	600
#define CHILDWND_OFFX	150
#define CHILDWND_OFFY	50

#define ID_CTRL_PIC_1									0x000010
#define ID_CTRL_BTN_1									0x000020
#define ID_CTRL_BTN_2									0x000021

//Walk 图大小
#define FLOAT_PIC_SQUARE_WIDTH			48
#define FLOAT_PIC_SQUARE_HEIGHT			64
#define FLOAT_PIC_WIDTH							192
#define FLOAT_PIC_HEIGHT							256
#define Tex_Walk_Width								48
#define Tex_Walk_Length							528
#define Tex_Attack_Width							64
#define	 Tex_Attack_Length							768
#define	 Tex_Defend_Width						48
#define Tex_Defend_Length						240

#define MAP_RECT		64
#define MAP_OFF_X	10
#define MAP_OFF_Y	50
#define MAP_WIDTH_NUM	8
#define MAP_LENGTH_NUM	8
#define MAP_WIDTH		MAP_RECT*MAP_WIDTH_NUM
#define MAP_LENGTH		MAP_RECT*MAP_LENGTH_NUM	

#define KEY_NUM 36

#define MAX_DISTANCE  100000

enum eLayer
{
	eLayer_MainWnd,	//主界面
	eLayer_Scene,			//剧情场景
	eLayer_Fight,				//战斗场景
};

//战斗时剧情触发器
enum eTrigger
{
	eTrigger_Turns,			//回合数触发：到达一定回合自动触发(回合数)
	eTrigger_Location,	//地点触发：到达某地点后触发(num，地点x，y，不指定num则默认为所有自己单位到达xy可触发)
	eTrigger_Touch,		//接触触发：接触特定单位后触发(num1，num2，不指定num1则默认为所有自己单位接触num2可触发)
	eTrigger_Kill,				//击杀触发：击杀特定单位后触发(num1，num2，不指定num1则默认为所有自己单位击杀num2可触发)
};

enum eActionState
{
	eActionState_PickAction = 0,
	eActionState_Process,
	eActionState_End,
};

//错误码
enum eErrorCode
{
	eErrorCode_Success = 0,
	eErrorCode_CannotMove,		//bCanMove 为true导致
	eErrorCode_NoPath,					//没有找到路径
	eErrorCode_NotStandState,	//寻路时不是stand状态，不可寻路
};
//阵营
enum eCamp
{
	eCamp_Friend = 0,
	eCamp_Enemy,
	eCamp_Neutral,
};

//阵营回合
//友方和敌方轮流行动
enum eCampTurn
{
	eCampTurn_Friend,
	eCampTurn_Enemy,
};

//行动阶段
//待选单位处于待命阶段
//每个选中的单位皆从移动阶段至操作阶段
//当处于操作阶段，可以通过右键取消回退至移动阶段
//操作阶段可以继而转到攻击阶段、技能阶段、使用物品阶段或者直接结束行动
 enum eActionStage
 {
	 eActionStage_WaitStage = 0,	//待命阶段
	 eActionStage_MoveStage,		//移动阶段
	 eActionStage_HandleStage,	//操作阶段(使用物品，释放技能)
	 eActionStage_AttackStage,	//攻击阶段
	 eActionStage_ItemStage,		//使用物品阶段
	 eActionStage_SkillStage,			//使用技能阶段
 };	

//单位动作状态
enum eCharacterState
{
	eCharacterState_Stand = -1,
	eCharacterState_Walk = 0,
	eCharacterState_Fight = 1,
	eCharacterState_Dead = 2,
	eCharacterState_Defense = 3,
	eCharacterState_Attacked = 4,
	eCharacterState_Healed = 5,
	eCharacterState_UseItem = 6,
	eCharacterState_Wait = 7,
	eCharacterState_Turn = 8,
	eCharacterState_Attack = 9,
	eCharacterState_Crit = 10,
	eCharacterState_Hurt = 11,
	eCharacterState_Step = 12,
};

//动作图包
enum eActionTex
{
	eActionTex_Walk = 0,
	eActionTex_Attack = 1,
	eActionTex_Defend = 2,

	eActionTex_Num,

	eActionTex_Stand = eActionTex_Walk,
	eActionTex_Dead = eActionTex_Walk,
	eActionTex_Attacked = eActionTex_Defend,
	eActionTex_LevelUp = eActionTex_Defend,

};
//攻击子状态
enum eAttackState
{
	eAttackState_Waiting = 1,
	eAttackState_Ready,
	eAttackState_Criting,
	eAttackState_Attacking,
	eAttackState_Attackeding,
	eAttackState_Defending,
	eAttackState_Dead,
};
//单位间通知事件及结果
enum eNotification
{
	eNotify_Success = 0,
	eNotify_TowardToAttacker,
	eNotify_Crit,
	eNotify_Attack,
	eNotify_AttackDefend,
	eNotify_CastAction,	//施法动作
	eNotify_CastSkill,		//释放技能
	eNotify_Attacked,		//被技能攻击
	eNotify_Healed,			//被治愈或者上buff
	eNotify_UseItem,		//使用物品
	eNotify_ItemEffect,	//物品生效
	eNotify_Dead,
	eNotify_NoTarget,
	eNotify_ReadyToBeAttacked,
	eNotify_CannotBeAttacked,
	eNotify_FinishAttack,
};

/*
	十字型：单位上下左右可以攻击，范围为1
	方块型：单位周围8格可以攻击
	大十字型：单位上下左右可以攻击，范围为2
	弓箭型：单位可以攻击距离自己上下左右1格范围，可以攻击斜向1格范围

	扩展型皆为普通型范围+1
*/
//攻击范围类型
enum eAttackRange
{
	eAttackRange_Cross = 0,		//十字型
	eAttackRange_Box = 1,			//方块型
	eAttackRange_BigCross = 2, //大十字型
	eAttackRange_Arrow = 3,		//弓箭型

	eAttackRange_CrossEx = 4, //十字型扩展
	eAttackRange_BoxEx = 5,			//方块型扩展
	eAttackRange_BigCrossEx = 6, //大十字型扩展
	eAttackRange_ArrowEx = 7,		//弓箭型扩展
};
/*
	单点型：单位自身范围
	十字型：单位上下左右可以攻击，范围为1
	方块型：单位周围8格可以攻击
	斜方块型：单位周围8格+外围上下左右4格
	大斜方块型：斜方块再向外扩展一格
	短直线型：目标单位以及其身后2格
	长直线型：目标单位以及其身后5格
*/
//技能范围类型
enum eSkillRange
{
	eSkillRange_Point = 1,						//单点型
	eSkillRange_Cross = 2,						//十字型
	eSkillRange_Box = 3,							//方块型
	eSkillRange_ObliqueBox = 4,			//斜方块型
	eSkillRange_BigObliqueBox = 5,	//大斜方块型
	eSkillRange_ShortLine = 6,				//短直线型
	eSkillRange_Line = 7,							//长直线型
};

enum eSkillType
{
	eSkillType_Hurt = 1,		//伤害型技能
	eSkillType_Heal = 2,		//治愈型技能
	eSkillType_Buff = 3,			//buff型技能
};

//攻击类型
enum eAttackType
{
	eAttackType_Normal = 0, //普通攻击
	eAttackType_Distance,		//远程攻击
	eAttackType_Magic,			//魔法攻击
};

struct  tagMapObject
{
	hgeSprite* spr;	//物体的贴图
	int ID;					//物体ID
	int action;			//物体动作ID
	int xpos;				//物体在地图的绘制坐标x
	int ypos;				//物体在地图的绘制坐标y
	eCamp camp;		//物体所在阵营
};
typedef struct tagMapObject MapObject;


enum eDirection
{
	eDirection_None,
	eDirection_Down,
	eDirection_Up,
	eDirection_Left,
	eDirection_Right,

	eDirection_Num,
	eDirection_End = eDirection_Num,
};
typedef std::list<eDirection> LDirection;

enum eLButtonState
{
	eLButtonState_Down,
	eLButtonState_Up,
	eLButtonState_Hold,
	eLButtonState_Null
};

enum eRButtonState
{
	eRButtonState_Down,
	eRButtonState_Up,
	eRButtonState_Hold,
	eRButtonState_Null
};

enum eKeyState
{
	eKeyState_Down,
	eKeyState_Up,
	eKeyState_Hold,
	eKeyState_Null,
};

enum eFontType
{
	eFontType_SongTi = 0,
	eFontType_Calibri,
	eFontType_MSYaHei,
	eFontType_SystemFont,


	eFontType_DefaultType = eFontType_MSYaHei,

	eFontType_SystemFontNum = eFontType_SystemFont,
};

enum eFontSize
{
	eFontSize_FontSmall = 10,
	eFontSize_FontMiddle = 12,
	eFontSize_FontBig = 14,
	eFontSize_FontTitle = 16,
	eFontSize_FontSuper = 18,
	eFontSize_FontHuge = 50,

	eFontSize_FontSizeNum = 6,
};

enum eTerrain
{
	eTerrain_Road			=		0x0000004,		//路面	0000100b
	eTerrain_River			=		0x0000008,		//河流	0001000b
	eTerrain_Mountain		=		0x000000C,		//山地	0001100b
	eTerrain_HillTop			=		0x0000010,		//山顶	0010000b
	eTerrain_CityWall		=		0x0000014,		//城墙	0010100b
	eTerrain_Forest			=		0x0000018,		//树林	0011000b
	eTerrain_Desert			=		0x000001C,		//沙漠	0011100b
	eTerrain_CityRoad		=		0x0000020,		//城内道路	0100000b
	eTerrain_City				=		0x0000024,		//城内	0100100b
};

enum eBlockAttribute		//表示地图格子的属性位
{
	eBlockAttribute_CanStandOn			=		0x00000001,			//该格子是否可以站立,若格子为某些地形：山顶、城墙，则不可站立
	eBlockAttribute_BeOccupied				=		0x00000002,			//该格子是否被单位占用
	eBlockAttribute_WhatTerrain			=		0x0000003C,			//表示该格子是哪种地形
};

#define IsCanStandOn(a)	(((a) & eBlockAttribute_CanStandOn) == eBlockAttribute_CanStandOn)		//测试是否可以站立
#define setStandOn(a,b)		(a = ((b)==1 ? (a) | 0x01 : (a) & 0xFFFFFFFE))		//设定该格子是否可以站立
#define IsOccupied(a)			(IsCanStandOn(a)==0 || (IsCanStandOn(a) && (((a) & eBlockAttribute_BeOccupied) == eBlockAttribute_BeOccupied)))		//测试是否正在被单位占用，不可通过同样视为占用
#define setOccupied(a,b)	(a = (IsCanStandOn(a) ? ((b)==1?(a)|0x02:(a)&0xFFFFFFFD) : a))	//设定该格子是否正在被单位占用
#define getTerrain(a)			((a) & eBlockAttribute_WhatTerrain)		//返回该格子的地形类别
#define setTerrain(a,b)		(a = (((a) & 0xFFFFFFC3) | (b)))	//设定该格子的地形类别
#define IsCanCross(a)		((IsOccupied(a) || (getTerrain(a) == eTerrain_HillTop) || (getTerrain(a) == eTerrain_CityWall))? 0:1)	//测试格子是否可以通过，包括单位以及地形的影响

#define gSafeDelete(X)			{	if((X)){delete (X); (X) = NULL;} }
#define gSafeDeleteArray(X) {	if((X)){delete[] (X); (X) = NULL;}}

struct tagBlock
{
	int xpos;		//地图横向格子编号
	int ypos;		//地图纵向格子编号
	DWORD	attri;	//格子的属性

	tagBlock(int _x,int _y)
	{
		xpos = _x;
		ypos = _y;
		attri = 0x0;
	}
	tagBlock()
	{
		xpos = 0;
		ypos = 0;
		attri = 0x0;
	}
	tagBlock& operator=(tagBlock& block)
	{
		xpos = block.xpos;
		ypos = block.ypos;
		attri = block.attri;
		return *this;
	}
	static bool less_than( tagBlock &b1, tagBlock &b2)
	{
		return b1.ypos*MAP_WIDTH_NUM+b1.xpos < b2.ypos*MAP_WIDTH_NUM+b2.xpos;
	}
};
typedef struct tagBlock Block;

struct tagConfig
{
	int level;
	tagConfig()
	{
		clear();
	}
	void clear()
	{
		level = -1;
	}
};
typedef struct tagConfig Config;


eKeyState g_getKeyState(HGE* hge,int Key);
std::map<std::string,std::string> g_parseConfigFile(const std::string file);
void g_getFiles( std::string path, std::map<std::string,std::string>& files,char* type,int maxFileNum,bool useDefaultName,bool isCharacter=false);
int g_getKeyNum(int Key);	//根据按键编号取得该按键在数组中的位置
void g_CTW(const char* text,wchar_t* out);	//char* to wchar_t*
int g_getPointToPointValueByIndex(int** array,int n,int i,int j);
eLButtonState g_getLButtonState(HGE* hge);
eRButtonState g_getRButtonState(HGE* hge);
int g_getTerrainCost(int terrain);	//根据地形取得消耗
int g_RandomInt(int start,int end);	//从start 到end中选取随机数，范围[start,end]
void g_debugString(char* file,char* func,int line,char* msg="");	//错误输出

#endif