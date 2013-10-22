#pragma once
#include "commonTools.h"
#include "SkillManager.h"
#include "Scene.h"
#include "ConfigManager.h"

/*
单位状态变化：
Stand --Move--> Walk ----> Stand
Stand --Attack--> Fight ----> Stand

当单位处于Stand时，其行动方向一定是None
当单位处于Walk时，其行动方向一定不是None

*/
struct BuffData
{
	int m_nType;					//buff种类
	int m_nValue;				//buff值大小
	int m_nLastTurns;		//持续时间
	BuffData()
	{
		m_nType = -1;
		m_nValue = -1;
		m_nLastTurns = 0;
	}
	BuffData(int type,int value,int lastTurns)
	{
		m_nType = type;
		m_nValue = value;
		m_nLastTurns = lastTurns;
	}
};

struct Item;
class Map;
//表示所有单位
class Character
{
public:
	Character(void);
	virtual ~Character(void);

	void Render();
	void Update(float delta); 

	void Init(int _Level,int _ID,int _Num,int _Action,Block _block);
	eErrorCode Move(int tarX,int tarY,bool bAllBlockInclude=false,bool bNoMoveAbilityLimit=false);	//向目标移动, bAllBlockInclude :是否包含所有格子，不管是否被占据 bNoMoveAbilityLimit:是否采用全地图范围
	void Move(eDirection dir);		//以格子为单位移动

	void SetMove(bool _move){m_bCanMove = _move;}
	bool GetMove(){return m_bCanMove;}
	void SetFinish(bool _finish);
	bool GetFinish(){return m_bFinishAct;}
	Block&	GetBlock(){return m_iBlock;}
	Block& GetOrigBlock(){return m_iOrigBlock;}
	float GetRealX(){return m_fXPos;}
	float GetRealY(){return m_fYPos;}
	void	SetMoveAbility(int _ability,Map* map);
	int	GetMoveAbility(){return m_nMoveAbility;}
	void SetCamp(int _camp){m_nCamp = _camp;}
	int GetCamp(){return m_nCamp;}
	void SetAttackRange(eAttackRange _attackRange){m_eAttackRange = _attackRange;}
	eAttackRange GetAttackRange(){return m_eAttackRange;}
	void SetActionStage(eActionStage _stage){m_eActionStage = _stage;}
	eActionStage GetActionStage(){return m_eActionStage;}
	eCharacterState& GetCharacterState(){return m_eCharState;}
	eDirection& GetCurDirection(){return m_eCurDir;}
	eDirection& GetOrigDirection(){return m_eOrigDir;}
	int GetID(){return m_nID;}
	int GetNum(){return m_nNum;}
	bool GetHide(){return m_bHide;}
	void SetHide(bool hide){m_bHide = hide;}

	std::vector<Block*> CreateRange(Map* map,int range,bool bAllBlockInclude=false,bool bNoMoveAbilityLimit=false);	//bAllBlockInclude :是否包含所有格子，不管是否被占据

	void CancelMove();	//取消移动，回到原来位置

	//战斗相关
//	void SetTarget(int _tar){m_nTar = _tar;}
//	int GetTarget(){return m_nTar;}
//	int& GetCaster() {return m_nSrc;}
	bool CanHitTarget(Character* target);	//是否可以攻击目标
	bool CanSkillHitTarget(Character* target);	//技能是否可以击中目标
//	void GeginHit();

	//战斗剧情动作
	void Talk(int head,const char* word);//说话
	void Wait(DWORD time);		//等待一段时间
	void Turn(int dir,DWORD time);		//转向
	void Attack();					//攻击
	void Crit();						//暴击
	void Attacked(DWORD time);				//被攻击
	void Defend(DWORD time);				//防御
	void Hurt(DWORD time);					//受伤、死亡动画
	void Step(DWORD time);						//原地走路
	void MoveTo(int x,int y);							//移动
	void Disappear();			//暂时隐藏单位
	void Appear(int dir,int x,int y);		//隐藏单位出现

	//动作
	void TowardToAttacker(eNotification notify,Character* target,int time);
	void Attack(eNotification notify,Character* target,int time);
	void Crit(eNotification notify,Character* target,int time);
	void Attacked(eNotification notify,Character* target,int time);
	void Defend(eNotification notify,Character* target,int time);
	void Dead(eNotification notify,Character* target,int time);
	void Healed(eNotification notify,Character* target,int time);	//被治愈，上buff，升级
	void UseItem(eNotification notify,Character* target,int time);	//使用物品时的表现
	void ChangeColor(DWORD color){m_pAnimation->SetColor(color);}
	void ResetFrame();

	//属性相关
	int& GetForce(){return m_nForce;}
	int& GetIntelligence(){return m_nIntelligence;}
	int& GetCharm(){return m_nCharm;}
	const char* GetIntro(){return m_strIntro.c_str();}
	void SetIntro(const char* intro){m_strIntro = intro;}
	void SetHead(int head){m_nHead = head;}
	int GetHead(){return m_nHead;}
	void SetName(const char* name){m_strName = name;}
	std::string&	GetName(){return m_strName;}
	std::string&	GetKind(){return m_strKind;}
	eAttackType& GetAttackType(){return m_eAttackType;}
	int&	GetAttack(){return m_nAttack;}
	int&	GetDefend(){return m_nDefend;}
	int&	GetSkillDamage(){return m_nSkillDamage;}
	int&	GetSkillDefend(){return m_nSkillDefend;}
	float&	GetCrit(){return m_fCrit;}
	float&	GetDodge(){return m_fDodge;}
	int&	GetHP(){return m_nHP;}
	int&	GetHPMax(){return m_nHPMax;}
	int&	GetMPMax(){return m_nMPMax;}
	int&	GetMP(){return m_nMP;}
	int&	GetLevel(){return m_nLevel;}
	int&	GetExp(){return m_nExp;}
	int&	GetExpTotal(){return m_nExpTotal;}
	int&	GetPreHurt(){return m_nPreHurt;}
	bool& GetDead(){return m_bDead;}
	bool& GetCounter(){return m_bCounter;}
	void SetAttributeValue(int type,int value);
	std::vector<BuffData>& GetBuffData(){return m_vBuffData;}
	void RemoveBuff();

	//技能
	std::vector<int>		GetSkillList();
	int&	GetCastSkill(){return m_nCastSkill;}

	//物品
	int&	GetUseItem(){return m_nUseItem;}
	bool	CanUseItem(Character* target);
	void	ItemEffect(Item item,bool bAdd=true);

	//装备
	void AddEquip(int grid,int equipID);
	void RemoveEquip(int grid);
	Item GetEquip(int grid);

	//剧情动作相关
	void PushAction(NewAction action);
	NewAction GetAction(){return m_iAction;}
	bool IsInAction();
private:
	hgeAnimation* m_pAnimation;
	std::map<int,HTEXTURE> m_mCharTex; //存储单位的各个图片动作
	int m_nMapLevel;	//所在关卡
	int m_nID;		//代表单位类别的ID号
	int m_nNum;	//代表出现在当前关卡的单位号
	Block m_iOrigBlock;	//记录单位原来处于的格子，用于右键取消行动时返回原格子
	Block m_iBlock;//处于当前关卡的哪个格子中
	float m_fXPos,m_fYPos;	//单位的实际坐标
	float m_fStartX,m_fStartY;	//单位每次行动前的起点位置
	bool m_bCanMove;		//代表该单位是否可以移动
	int m_nMoveAbility;		//代表移动力
	bool m_bFinishAct;	//代表是否行动过
	int	m_nCamp;		//单位阵营
	bool m_bHide;		//是否隐藏
	eActionStage m_eActionStage;	//单位所处阶段，只有轮到该单位行动时才有效

	eCharacterState m_eCharState; //单位当前的状态 
	eDirection m_eCurDir;	//当前方向
	eDirection m_eOrigDir;	//原先方向，用于右键取消行动时恢复到原方向
	LDirection m_lPathDir;	//整个路径的移动方向
	int m_nLeftDistance;	//剩余需要移动的格子数

	eAttackRange m_eAttackRange;	//单位攻击范围类型
//	int m_nSrc; //攻击自己的单位
//	int m_nTar;	//攻击目标
	eAttackState m_eAttackState; //攻击子状态，用于处理攻击流程
//	DWORD m_dwRecordTime;	//用于动作计时
	int m_nActionTime;		//动作需要时间
	eNotification m_eNotify;			//动作通知类型

	//人物属性
	int m_nIntelligence;		//智力
	int m_nForce;				//武力
	int m_nCharm;				//魅力
	int m_nHead;		//头像编号
	std::string m_strName;	//名称
	std::string m_strIntro;	//人物介绍，限制一定字数

	//单位属性
	std::string m_strKind;		//兵种
	eAttackType m_eAttackType;		//攻击类型
	int m_nAttack;		//物理攻击
	int m_nDefend;	//防御力
	int m_nSkillDamage;	//法术攻击
	int m_nSkillDefend; //法术防御
	float m_fCrit;			//暴击概率
	float m_fDodge;	//闪避概率(防御概率)
	int m_nHP;				//生命值
	int m_nHPMax;		//最大生命值
	int m_nMP;				//魔法值
	int m_nMPMax;	//最大魔法值
	int m_nLevel;		//等级
	int m_nExp;			//当前经验值
	int m_nExpTotal;	//当前升级所需总经验值
	bool	m_bDead;	//是否死亡
	bool	m_bCounter;	//是否可以反击
	std::vector<BuffData> m_vBuffData;	//保存单位获得的buff数据，以便buff结束时恢复
	int m_nPreHurt;	//下一次攻击准备打出的伤害，提前计算出以便后面扣除

	//技能列表
	std::vector<int> m_vDefaultSkillList;	//单位默认技能列表，是兵种预设技能
	std::vector<int> m_vNewSkillList;		//单位新增技能列表，如学习技能书获得
	eDirection GetTexDir(eDirection dir);
	int m_nCastSkill;		//准备释放的技能id

	//物品
	int m_nUseItem;		//准备使用的物品id
	hgeSprite* m_pItemSpr;	//使用物品的图片
	float m_fItemRiseHeight;	//物品绘制上升的高度
	int m_nDrawItem;	//是否绘制物品 0表示不绘制 1,2表示绘制上升和绘制下降
	float m_fDrawItemX;	//绘制物品的坐标
	float m_fDrawItemY;
	Character* m_pItemTarget;	//物品使用目标

	//剧情动作
	NewAction m_iAction; //剧情动作

	//装备
	Item m_iEquip[eEquipGrid_TotalEquip];
};


typedef std::vector<Character*> VCharacter;
