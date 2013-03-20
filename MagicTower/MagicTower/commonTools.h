#ifndef COMMON_TOOLS_H
#define COMMON_TOOLS_H

#pragma warning(disable: 4244)
#pragma warning(disable: 4018)

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
#include <map>
#include <algorithm>
#include "Singleton.h"
#include "MagicTowerEdit\MagicTowerEdit\Resource.h"


#include "Markup.h"
#include <io.h>



#define LEVEL_NUM 20



#define APP_WIDTH	800
#define APP_HEIGHT	600
#define CHILDWND_OFFX	150
#define CHILDWND_OFFY	50

#define ID_CTRL_PIC_1									0x000010
#define ID_CTRL_BTN_1									0x000020
#define ID_CTRL_BTN_2									0x000021

#define FLOAT_PIC_SQUARE_WIDTH			48
#define FLOAT_PIC_SQUARE_HEIGHT			64
#define FLOAT_PIC_WIDTH							192
#define FLOAT_PIC_HEIGHT							256

#define MAP_RECT		64
#define MAP_OFF_X	10
#define MAP_OFF_Y	50
#define MAP_WIDTH_NUM	8
#define MAP_LENGTH_NUM	8
#define MAP_WIDTH		MAP_RECT*MAP_WIDTH_NUM
#define MAP_LENGTH		MAP_RECT*MAP_LENGTH_NUM	

#define KEY_NUM 36


//阵营
enum Camp
{
	Friend = 0,
	Enemy,
	Neutral,
};


struct  tagMapObject
{
	hgeSprite* spr;	//物体的贴图
	int ID;					//物体ID
	int action;			//物体动作ID
	int xpos;				//物体在地图的绘制坐标x
	int ypos;				//物体在地图的绘制坐标y
	Camp camp;		//物体所在阵营
};
typedef struct tagMapObject MapObject;


enum Direction
{
	None,
	DOWN,
	LEFT,
	RIGHT,
	UP,
};

enum LBUTTON_STATE
{
	LBUTTON_DOWN,
	LBUTTON_UP,
	LBUTTON_HOLD,
	LBUTTON_NULL
};

enum KEY_STATE
{
	KEY_DOWN,
	KEY_UP,
	KEY_HOLD,
	KEY_NULL,
};

enum FontType
{
	SongTi = 0,
	Calibri,
	MSYaHei,
	SystemFont,


	DefaultType = MSYaHei,

	SystemFontNum = SystemFont,
};

enum FontSize
{
	FontSmall = 10,
	FontMiddle = 12,
	FontBig = 14,

	FontSizeNum = 3,
};

enum
{
	Road			=		0x0000004,		//路面	0000100b
	River			=		0x0000008,		//河流	0001000b
	Mountain		=		0x000000C,		//山地	0001100b
	HillTop			=		0x0000010,		//山顶	0010000b
	CityWall		=		0x0000014,		//城墙	0010100b
	Forest			=		0x0000018,		//树林	0011000b
	Desert			=		0x000001C,		//沙漠	0011100b
	CityRoad		=		0x0000020,		//城内道路	0100000b
	City				=		0x0000024,		//城内	0100100b
};

enum		//表示地图格子的属性位
{
	CanStandOn			=		0x00000001,			//该格子是否可以站立,若格子为某些地形：山顶、城墙，则不可站立
	BeOccupied				=		0x00000002,			//该格子是否被单位占用
	WhatTerrain			=		0x0000003C,			//表示该格子是哪种地形
};

#define IsCanStandOn(a)	(((a) & CanStandOn) == CanStandOn)		//测试是否可以站立
#define setStandOn(a,b)		(a = ((b)==1 ? (a) | 0x01 : (a) & 0xFFFFFFFE))		//设定该格子是否可以站立
#define IsOccupied(a)			(IsCanStandOn(a)==0 || (IsCanStandOn(a) && (((a) & BeOccupied) == BeOccupied)))		//测试是否正在被单位占用，不可通过同样视为占用
#define setOccupied(a,b)	(a = (IsCanStandOn(a) ? ((b)==1?(a)|0x02:(a)&0xFFFFFFFD) : a))	//设定该格子是否正在被单位占用
#define getTerrain(a)			((a) & WhatTerrain)		//返回该格子的地形类别
#define setTerrain(a,b)		(a = (((a) & 0xFFFFFFC3) | (b)))	//设定该格子的地形类别
#define IsCanCross(a)		((IsOccupied(a) || (getTerrain(a) == HillTop) || (getTerrain(a) == CityWall))? 0:1)	//测试格子是否可以通过，包括单位以及地形的影响

#define gSafeDelete(X)		{	if((X)){delete (X); (X) = NULL;} }

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


KEY_STATE g_getKeyState(HGE* hge,int Key);
void g_getFiles( std::string path, std::map<std::string,std::string>& files,char* type,int maxFileNum,bool useDefaultName);
int g_getKeyNum(int Key);	//根据按键编号取得该按键在数组中的位置
void g_CTW(const char* text,wchar_t* out);	//char* to wchar_t*
LBUTTON_STATE g_getLButtonState(HGE* hge);

#endif