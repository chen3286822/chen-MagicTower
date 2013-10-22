#pragma once
#include "commonTools.h"
#include "SkillManager.h"
#include "Scene.h"
#include "ConfigManager.h"

/*
��λ״̬�仯��
Stand --Move--> Walk ----> Stand
Stand --Attack--> Fight ----> Stand

����λ����Standʱ�����ж�����һ����None
����λ����Walkʱ�����ж�����һ������None

*/
struct BuffData
{
	int m_nType;					//buff����
	int m_nValue;				//buffֵ��С
	int m_nLastTurns;		//����ʱ��
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
//��ʾ���е�λ
class Character
{
public:
	Character(void);
	virtual ~Character(void);

	void Render();
	void Update(float delta); 

	void Init(int _Level,int _ID,int _Num,int _Action,Block _block);
	eErrorCode Move(int tarX,int tarY,bool bAllBlockInclude=false,bool bNoMoveAbilityLimit=false);	//��Ŀ���ƶ�, bAllBlockInclude :�Ƿ�������и��ӣ������Ƿ�ռ�� bNoMoveAbilityLimit:�Ƿ����ȫ��ͼ��Χ
	void Move(eDirection dir);		//�Ը���Ϊ��λ�ƶ�

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

	std::vector<Block*> CreateRange(Map* map,int range,bool bAllBlockInclude=false,bool bNoMoveAbilityLimit=false);	//bAllBlockInclude :�Ƿ�������и��ӣ������Ƿ�ռ��

	void CancelMove();	//ȡ���ƶ����ص�ԭ��λ��

	//ս�����
//	void SetTarget(int _tar){m_nTar = _tar;}
//	int GetTarget(){return m_nTar;}
//	int& GetCaster() {return m_nSrc;}
	bool CanHitTarget(Character* target);	//�Ƿ���Թ���Ŀ��
	bool CanSkillHitTarget(Character* target);	//�����Ƿ���Ի���Ŀ��
//	void GeginHit();

	//ս�����鶯��
	void Talk(int head,const char* word);//˵��
	void Wait(DWORD time);		//�ȴ�һ��ʱ��
	void Turn(int dir,DWORD time);		//ת��
	void Attack();					//����
	void Crit();						//����
	void Attacked(DWORD time);				//������
	void Defend(DWORD time);				//����
	void Hurt(DWORD time);					//���ˡ���������
	void Step(DWORD time);						//ԭ����·
	void MoveTo(int x,int y);							//�ƶ�
	void Disappear();			//��ʱ���ص�λ
	void Appear(int dir,int x,int y);		//���ص�λ����

	//����
	void TowardToAttacker(eNotification notify,Character* target,int time);
	void Attack(eNotification notify,Character* target,int time);
	void Crit(eNotification notify,Character* target,int time);
	void Attacked(eNotification notify,Character* target,int time);
	void Defend(eNotification notify,Character* target,int time);
	void Dead(eNotification notify,Character* target,int time);
	void Healed(eNotification notify,Character* target,int time);	//����������buff������
	void UseItem(eNotification notify,Character* target,int time);	//ʹ����Ʒʱ�ı���
	void ChangeColor(DWORD color){m_pAnimation->SetColor(color);}
	void ResetFrame();

	//�������
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

	//����
	std::vector<int>		GetSkillList();
	int&	GetCastSkill(){return m_nCastSkill;}

	//��Ʒ
	int&	GetUseItem(){return m_nUseItem;}
	bool	CanUseItem(Character* target);
	void	ItemEffect(Item item,bool bAdd=true);

	//װ��
	void AddEquip(int grid,int equipID);
	void RemoveEquip(int grid);
	Item GetEquip(int grid);

	//���鶯�����
	void PushAction(NewAction action);
	NewAction GetAction(){return m_iAction;}
	bool IsInAction();
private:
	hgeAnimation* m_pAnimation;
	std::map<int,HTEXTURE> m_mCharTex; //�洢��λ�ĸ���ͼƬ����
	int m_nMapLevel;	//���ڹؿ�
	int m_nID;		//����λ����ID��
	int m_nNum;	//��������ڵ�ǰ�ؿ��ĵ�λ��
	Block m_iOrigBlock;	//��¼��λԭ�����ڵĸ��ӣ������Ҽ�ȡ���ж�ʱ����ԭ����
	Block m_iBlock;//���ڵ�ǰ�ؿ����ĸ�������
	float m_fXPos,m_fYPos;	//��λ��ʵ������
	float m_fStartX,m_fStartY;	//��λÿ���ж�ǰ�����λ��
	bool m_bCanMove;		//����õ�λ�Ƿ�����ƶ�
	int m_nMoveAbility;		//�����ƶ���
	bool m_bFinishAct;	//�����Ƿ��ж���
	int	m_nCamp;		//��λ��Ӫ
	bool m_bHide;		//�Ƿ�����
	eActionStage m_eActionStage;	//��λ�����׶Σ�ֻ���ֵ��õ�λ�ж�ʱ����Ч

	eCharacterState m_eCharState; //��λ��ǰ��״̬ 
	eDirection m_eCurDir;	//��ǰ����
	eDirection m_eOrigDir;	//ԭ�ȷ��������Ҽ�ȡ���ж�ʱ�ָ���ԭ����
	LDirection m_lPathDir;	//����·�����ƶ�����
	int m_nLeftDistance;	//ʣ����Ҫ�ƶ��ĸ�����

	eAttackRange m_eAttackRange;	//��λ������Χ����
//	int m_nSrc; //�����Լ��ĵ�λ
//	int m_nTar;	//����Ŀ��
	eAttackState m_eAttackState; //������״̬�����ڴ���������
//	DWORD m_dwRecordTime;	//���ڶ�����ʱ
	int m_nActionTime;		//������Ҫʱ��
	eNotification m_eNotify;			//����֪ͨ����

	//��������
	int m_nIntelligence;		//����
	int m_nForce;				//����
	int m_nCharm;				//����
	int m_nHead;		//ͷ����
	std::string m_strName;	//����
	std::string m_strIntro;	//������ܣ�����һ������

	//��λ����
	std::string m_strKind;		//����
	eAttackType m_eAttackType;		//��������
	int m_nAttack;		//������
	int m_nDefend;	//������
	int m_nSkillDamage;	//��������
	int m_nSkillDefend; //��������
	float m_fCrit;			//��������
	float m_fDodge;	//���ܸ���(��������)
	int m_nHP;				//����ֵ
	int m_nHPMax;		//�������ֵ
	int m_nMP;				//ħ��ֵ
	int m_nMPMax;	//���ħ��ֵ
	int m_nLevel;		//�ȼ�
	int m_nExp;			//��ǰ����ֵ
	int m_nExpTotal;	//��ǰ���������ܾ���ֵ
	bool	m_bDead;	//�Ƿ�����
	bool	m_bCounter;	//�Ƿ���Է���
	std::vector<BuffData> m_vBuffData;	//���浥λ��õ�buff���ݣ��Ա�buff����ʱ�ָ�
	int m_nPreHurt;	//��һ�ι���׼��������˺�����ǰ������Ա����۳�

	//�����б�
	std::vector<int> m_vDefaultSkillList;	//��λĬ�ϼ����б��Ǳ���Ԥ�輼��
	std::vector<int> m_vNewSkillList;		//��λ���������б���ѧϰ��������
	eDirection GetTexDir(eDirection dir);
	int m_nCastSkill;		//׼���ͷŵļ���id

	//��Ʒ
	int m_nUseItem;		//׼��ʹ�õ���Ʒid
	hgeSprite* m_pItemSpr;	//ʹ����Ʒ��ͼƬ
	float m_fItemRiseHeight;	//��Ʒ���������ĸ߶�
	int m_nDrawItem;	//�Ƿ������Ʒ 0��ʾ������ 1,2��ʾ���������ͻ����½�
	float m_fDrawItemX;	//������Ʒ������
	float m_fDrawItemY;
	Character* m_pItemTarget;	//��Ʒʹ��Ŀ��

	//���鶯��
	NewAction m_iAction; //���鶯��

	//װ��
	Item m_iEquip[eEquipGrid_TotalEquip];
};


typedef std::vector<Character*> VCharacter;
