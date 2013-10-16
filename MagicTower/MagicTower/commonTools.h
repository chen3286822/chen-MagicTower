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

//Walk ͼ��С
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
	eLayer_MainWnd,	//������
	eLayer_Scene,			//���鳡��
	eLayer_Fight,				//ս������
};

//ս��ʱ���鴥����
enum eTrigger
{
	eTrigger_Turns,			//�غ�������������һ���غ��Զ�����(�غ���)
	eTrigger_Location,	//�ص㴥��������ĳ�ص�󴥷�(num���ص�x��y����ָ��num��Ĭ��Ϊ�����Լ���λ����xy�ɴ���)
	eTrigger_Touch,		//�Ӵ��������Ӵ��ض���λ�󴥷�(num1��num2����ָ��num1��Ĭ��Ϊ�����Լ���λ�Ӵ�num2�ɴ���)
	eTrigger_Kill,				//��ɱ��������ɱ�ض���λ�󴥷�(num1��num2����ָ��num1��Ĭ��Ϊ�����Լ���λ��ɱnum2�ɴ���)
};

enum eActionState
{
	eActionState_PickAction = 0,
	eActionState_Process,
	eActionState_End,
};

//������
enum eErrorCode
{
	eErrorCode_Success = 0,
	eErrorCode_CannotMove,		//bCanMove Ϊtrue����
	eErrorCode_NoPath,					//û���ҵ�·��
	eErrorCode_NotStandState,	//Ѱ·ʱ����stand״̬������Ѱ·
};
//��Ӫ
enum eCamp
{
	eCamp_Friend = 0,
	eCamp_Enemy,
	eCamp_Neutral,
};

//��Ӫ�غ�
//�ѷ��͵з������ж�
enum eCampTurn
{
	eCampTurn_Friend,
	eCampTurn_Enemy,
};

//�ж��׶�
//��ѡ��λ���ڴ����׶�
//ÿ��ѡ�еĵ�λ�Դ��ƶ��׶��������׶�
//�����ڲ����׶Σ�����ͨ���Ҽ�ȡ���������ƶ��׶�
//�����׶ο��Լ̶�ת�������׶Ρ����ܽ׶Ρ�ʹ����Ʒ�׶λ���ֱ�ӽ����ж�
 enum eActionStage
 {
	 eActionStage_WaitStage = 0,	//�����׶�
	 eActionStage_MoveStage,		//�ƶ��׶�
	 eActionStage_HandleStage,	//�����׶�(ʹ����Ʒ���ͷż���)
	 eActionStage_AttackStage,	//�����׶�
	 eActionStage_ItemStage,		//ʹ����Ʒ�׶�
	 eActionStage_SkillStage,			//ʹ�ü��ܽ׶�
 };	

//��λ����״̬
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

//����ͼ��
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
//������״̬
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
//��λ��֪ͨ�¼������
enum eNotification
{
	eNotify_Success = 0,
	eNotify_TowardToAttacker,
	eNotify_Crit,
	eNotify_Attack,
	eNotify_AttackDefend,
	eNotify_CastAction,	//ʩ������
	eNotify_CastSkill,		//�ͷż���
	eNotify_Attacked,		//�����ܹ���
	eNotify_Healed,			//������������buff
	eNotify_UseItem,		//ʹ����Ʒ
	eNotify_ItemEffect,	//��Ʒ��Ч
	eNotify_Dead,
	eNotify_NoTarget,
	eNotify_ReadyToBeAttacked,
	eNotify_CannotBeAttacked,
	eNotify_FinishAttack,
};

/*
	ʮ���ͣ���λ�������ҿ��Թ�������ΧΪ1
	�����ͣ���λ��Χ8����Թ���
	��ʮ���ͣ���λ�������ҿ��Թ�������ΧΪ2
	�����ͣ���λ���Թ��������Լ���������1��Χ�����Թ���б��1��Χ

	��չ�ͽ�Ϊ��ͨ�ͷ�Χ+1
*/
//������Χ����
enum eAttackRange
{
	eAttackRange_Cross = 0,		//ʮ����
	eAttackRange_Box = 1,			//������
	eAttackRange_BigCross = 2, //��ʮ����
	eAttackRange_Arrow = 3,		//������

	eAttackRange_CrossEx = 4, //ʮ������չ
	eAttackRange_BoxEx = 5,			//��������չ
	eAttackRange_BigCrossEx = 6, //��ʮ������չ
	eAttackRange_ArrowEx = 7,		//��������չ
};
/*
	�����ͣ���λ����Χ
	ʮ���ͣ���λ�������ҿ��Թ�������ΧΪ1
	�����ͣ���λ��Χ8����Թ���
	б�����ͣ���λ��Χ8��+��Χ��������4��
	��б�����ͣ�б������������չһ��
	��ֱ���ͣ�Ŀ�굥λ�Լ������2��
	��ֱ���ͣ�Ŀ�굥λ�Լ������5��
*/
//���ܷ�Χ����
enum eSkillRange
{
	eSkillRange_Point = 1,						//������
	eSkillRange_Cross = 2,						//ʮ����
	eSkillRange_Box = 3,							//������
	eSkillRange_ObliqueBox = 4,			//б������
	eSkillRange_BigObliqueBox = 5,	//��б������
	eSkillRange_ShortLine = 6,				//��ֱ����
	eSkillRange_Line = 7,							//��ֱ����
};

enum eSkillType
{
	eSkillType_Hurt = 1,		//�˺��ͼ���
	eSkillType_Heal = 2,		//�����ͼ���
	eSkillType_Buff = 3,			//buff�ͼ���
};

//��������
enum eAttackType
{
	eAttackType_Normal = 0, //��ͨ����
	eAttackType_Distance,		//Զ�̹���
	eAttackType_Magic,			//ħ������
};

struct  tagMapObject
{
	hgeSprite* spr;	//�������ͼ
	int ID;					//����ID
	int action;			//���嶯��ID
	int xpos;				//�����ڵ�ͼ�Ļ�������x
	int ypos;				//�����ڵ�ͼ�Ļ�������y
	eCamp camp;		//����������Ӫ
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
	eTerrain_Road			=		0x0000004,		//·��	0000100b
	eTerrain_River			=		0x0000008,		//����	0001000b
	eTerrain_Mountain		=		0x000000C,		//ɽ��	0001100b
	eTerrain_HillTop			=		0x0000010,		//ɽ��	0010000b
	eTerrain_CityWall		=		0x0000014,		//��ǽ	0010100b
	eTerrain_Forest			=		0x0000018,		//����	0011000b
	eTerrain_Desert			=		0x000001C,		//ɳĮ	0011100b
	eTerrain_CityRoad		=		0x0000020,		//���ڵ�·	0100000b
	eTerrain_City				=		0x0000024,		//����	0100100b
};

enum eBlockAttribute		//��ʾ��ͼ���ӵ�����λ
{
	eBlockAttribute_CanStandOn			=		0x00000001,			//�ø����Ƿ����վ��,������ΪĳЩ���Σ�ɽ������ǽ���򲻿�վ��
	eBlockAttribute_BeOccupied				=		0x00000002,			//�ø����Ƿ񱻵�λռ��
	eBlockAttribute_WhatTerrain			=		0x0000003C,			//��ʾ�ø��������ֵ���
};

#define IsCanStandOn(a)	(((a) & eBlockAttribute_CanStandOn) == eBlockAttribute_CanStandOn)		//�����Ƿ����վ��
#define setStandOn(a,b)		(a = ((b)==1 ? (a) | 0x01 : (a) & 0xFFFFFFFE))		//�趨�ø����Ƿ����վ��
#define IsOccupied(a)			(IsCanStandOn(a)==0 || (IsCanStandOn(a) && (((a) & eBlockAttribute_BeOccupied) == eBlockAttribute_BeOccupied)))		//�����Ƿ����ڱ���λռ�ã�����ͨ��ͬ����Ϊռ��
#define setOccupied(a,b)	(a = (IsCanStandOn(a) ? ((b)==1?(a)|0x02:(a)&0xFFFFFFFD) : a))	//�趨�ø����Ƿ����ڱ���λռ��
#define getTerrain(a)			((a) & eBlockAttribute_WhatTerrain)		//���ظø��ӵĵ������
#define setTerrain(a,b)		(a = (((a) & 0xFFFFFFC3) | (b)))	//�趨�ø��ӵĵ������
#define IsCanCross(a)		((IsOccupied(a) || (getTerrain(a) == eTerrain_HillTop) || (getTerrain(a) == eTerrain_CityWall))? 0:1)	//���Ը����Ƿ����ͨ����������λ�Լ����ε�Ӱ��

#define gSafeDelete(X)			{	if((X)){delete (X); (X) = NULL;} }
#define gSafeDeleteArray(X) {	if((X)){delete[] (X); (X) = NULL;}}

struct tagBlock
{
	int xpos;		//��ͼ������ӱ��
	int ypos;		//��ͼ������ӱ��
	DWORD	attri;	//���ӵ�����

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
int g_getKeyNum(int Key);	//���ݰ������ȡ�øð����������е�λ��
void g_CTW(const char* text,wchar_t* out);	//char* to wchar_t*
int g_getPointToPointValueByIndex(int** array,int n,int i,int j);
eLButtonState g_getLButtonState(HGE* hge);
eRButtonState g_getRButtonState(HGE* hge);
int g_getTerrainCost(int terrain);	//���ݵ���ȡ������
int g_RandomInt(int start,int end);	//��start ��end��ѡȡ���������Χ[start,end]
void g_debugString(char* file,char* func,int line,char* msg="");	//�������

#endif