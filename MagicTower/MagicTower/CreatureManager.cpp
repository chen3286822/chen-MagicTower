#include "CreatureManager.h"
#include "MapManager.h"
#include "App.h"
#include "TipWnd.h"
#include "UI.h"
#include "ActionProcess.h"
#include "WndDialog.h"
#include "WndSummary.h"
#include "ActionProcess.h"
#include "AIMgr.h"

CreatureManager::CreatureManager()
{
	m_VFriendList.clear();
	m_VEnemyList.clear();
	m_VEnemyDeadList.clear();
	m_VFriendDeadList.clear();
	m_nActionCreatureNum = -1;
	m_nSelectNum = -1;
}

CreatureManager::~CreatureManager()
{
	m_VFriendList.clear();
	m_VEnemyList.clear();
}

void CreatureManager::Init()
{
	m_vPair.clear();
	m_mAttackRange.clear();
	m_mSkillRange.clear();
	m_bPause = false;
	m_eState = eActionState_PickAction;
	m_lActions.clear();

	m_vPair.push_back(Pair(0,0));	/*0*/			
	m_vPair.push_back(Pair(1,0));	/*1*/				m_vPair.push_back(Pair(1,-1));		/*2*/
	m_vPair.push_back(Pair(0,-1));	/*3*/				m_vPair.push_back(Pair(-1,-1));	/*4*/
	m_vPair.push_back(Pair(-1,0));	/*5*/				m_vPair.push_back(Pair(-1,1));		/*6*/
	m_vPair.push_back(Pair(0,1));	/*7*/				m_vPair.push_back(Pair(1,1));		/*8*/
	m_vPair.push_back(Pair(2,0));	/*9*/				m_vPair.push_back(Pair(2,-1));		/*10*/
	m_vPair.push_back(Pair(0,-2));	/*11*/			m_vPair.push_back(Pair(1,-2));		/*12*/
	m_vPair.push_back(Pair(-2,0));	/*13*/			m_vPair.push_back(Pair(-1,-2));	/*14*/
	m_vPair.push_back(Pair(0,2));	/*15*/			m_vPair.push_back(Pair(-2,-1));	/*16*/
	m_vPair.push_back(Pair(-2,1));	/*17*/			m_vPair.push_back(Pair(3,0));		/*18*/
	m_vPair.push_back(Pair(-1,2));	/*19*/			m_vPair.push_back(Pair(0,-3));		/*20*/
	m_vPair.push_back(Pair(1,2));	/*21*/			m_vPair.push_back(Pair(-3,0));		/*22*/
	m_vPair.push_back(Pair(2,1));	/*23*/			m_vPair.push_back(Pair(0,3));		/*24*/
	m_vPair.push_back(Pair(4,0));	/*25*/			m_vPair.push_back(Pair(-4,0));		/*26*/
	m_vPair.push_back(Pair(5,0));	/*27*/			m_vPair.push_back(Pair(-5,0));		/*28*/
	m_vPair.push_back(Pair(0,-4));	/*29*/			m_vPair.push_back(Pair(0,4));		/*30*/
	m_vPair.push_back(Pair(0,-5));	/*31*/			m_vPair.push_back(Pair(0,5));		/*32*/
	
	int neAttackRange_Cross[] = {1,3,5,7};
	m_mAttackRange[eAttackRange_Cross].assign(neAttackRange_Cross,neAttackRange_Cross+4);

	int neAttackRange_Box[] = {1,2,3,4,5,6,7,8};
	m_mAttackRange[eAttackRange_Box].assign(neAttackRange_Box,neAttackRange_Box+8);

	int neAttackRange_BigCross[] = {1,3,5,7,9,11,13,15};
	m_mAttackRange[eAttackRange_BigCross].assign(neAttackRange_BigCross,neAttackRange_BigCross+8);

	int neAttackRange_Arrow[] = {2,4,6,8,9,11,13,15};
	m_mAttackRange[eAttackRange_Arrow].assign(neAttackRange_Arrow,neAttackRange_Arrow+8);

	int neAttackRange_CrossEx[] = {1,3,5,7,2,4,6,8,9,11,14,15};
	m_mAttackRange[eAttackRange_CrossEx].assign(neAttackRange_CrossEx,neAttackRange_CrossEx+12);

	int neAttackRange_BoxEx[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,19,21,23};
	m_mAttackRange[eAttackRange_BoxEx].assign(neAttackRange_BoxEx,neAttackRange_BoxEx+20);

	int neAttackRange_BigCrossEx[] = {1,3,5,7,9,11,13,15,18,20,22,24};
	m_mAttackRange[eAttackRange_BigCrossEx].assign(neAttackRange_BigCrossEx,neAttackRange_BigCrossEx+12);

	int neAttackRange_ArrowEx[] = {2,4,6,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24};
	m_mAttackRange[eAttackRange_ArrowEx].assign(neAttackRange_ArrowEx,neAttackRange_ArrowEx+20);

	int neSkillRange_Point[] = {0};
	m_mSkillRange[eSkillRange_Point].assign(neSkillRange_Point,neSkillRange_Point+1);
	int neSkillRange_Cross[] = {0,1,3,5,7};
	m_mSkillRange[eSkillRange_Cross].assign(neSkillRange_Cross,neSkillRange_Cross+5);
	int neSkillRange_Box[] = {0,1,2,3,4,5,6,7,8};
	m_mSkillRange[eSkillRange_Box].assign(neSkillRange_Box,neSkillRange_Box+9);
	int neSkillRange_ObliqueBox[] = {0,1,2,3,4,5,6,7,8,9,11,13,15};
	m_mSkillRange[eSkillRange_ObliqueBox].assign(neSkillRange_ObliqueBox,neSkillRange_ObliqueBox+13);
	int neSkillRange_BigObliqueBox[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24};
	m_mSkillRange[eSkillRange_BigObliqueBox].assign(neSkillRange_BigObliqueBox,neSkillRange_BigObliqueBox+25);

	//ֱ���ͱȽ����⣬��Ҫ�жϷ���
	int neSkillRange_ShortLine[] = {0,7,15,0,3,11,0,5,13,0,1,9};
	m_mSkillRange[eSkillRange_ShortLine].assign(neSkillRange_ShortLine,neSkillRange_ShortLine+12);
	int neSkillRange_Line[] = {0,7,15,24,30,32,0,3,11,20,29,31,0,5,13,22,26,28,0,1,9,18,25,27};
	m_mSkillRange[eSkillRange_Line].assign(neSkillRange_Line,neSkillRange_Line+24);

	m_eCampTurn = eCampTurn_Friend;
	m_vSkillTargets.clear();
	m_lItems.clear();
	m_nActionCreatureNum = -1;
	m_nSelectNum = -1;
}

void CreatureManager::Release()
{
	for (VCharacter::iterator it=m_VEnemyList.begin();it!=m_VEnemyList.end();it++)
		gSafeDelete(*it);
	for (VCharacter::iterator it=m_VFriendList.begin();it!=m_VFriendList.end();it++)
		gSafeDelete(*it);
	m_VEnemyList.clear();
	m_VFriendList.clear();
}

void CreatureManager::Render()
{
	ProcessSelectCreature();

	for (VCharacter::iterator it=m_VEnemyList.begin();it!=m_VEnemyList.end();it++)
	{
		if((*it)->GetDead() != true && (*it)->GetHide() != true)
			(*it)->Render();
	}
	for (VCharacter::iterator it=m_VFriendList.begin();it!=m_VFriendList.end();it++)
	{
		if((*it)->GetDead() != true && (*it)->GetHide() != true)
			(*it)->Render();
	}
}

void CreatureManager::Update(float delta)
{
	ProcessAction();

	//���Ǵ�����ͣ�У���ʧ���ˣ��򷵻�������
	if (!m_bPause && MapManager::sInstance().GetCurrentMap()->GetFailed() == true)
	{
		//���ʣ��Ķ���
		ActionProcess::sInstance().ClearAction();
		//��������
		MapManager::sInstance().GetCurrentMap()->Release();
		CreatureManager::sInstance().Release();
		MapManager::sInstance().Release();
		//�ر�С��ͼ
		App::sInstance().ShutDownSmallMap();
		App::sInstance().StartMainWnd();
		return;
	}

	//���Ǵ�����ͣ�У���ʤ���ˣ�����ת��ս�����
	if (!m_bPause && MapManager::sInstance().GetCurrentMap()->GetVictory() == true)
	{
		//���ʣ��Ķ���
		ActionProcess::sInstance().ClearAction();
		App::sInstance().SetLayer(eLayer_Scene,true);
		App::sInstance().StartNextScene();
		//�ر�С��ͼ
		App::sInstance().ShutDownSmallMap();
	}

	for (VCharacter::iterator it=m_VEnemyList.begin();it!=m_VEnemyList.end();it++)
	{
		if((*it)->GetDead() == true)
			m_VEnemyDeadList.push_back(*it);
		else if((*it)->GetHide() != true)
			(*it)->Update(delta);
	}
	for (VCharacter::iterator it=m_VFriendList.begin();it!=m_VFriendList.end();it++)
	{
		if((*it)->GetDead() == true)
			m_VFriendDeadList.push_back(*it);
		else if((*it)->GetHide() != true)
			(*it)->Update(delta);
	}

	//�Ƴ�������λ
	for (int i=0;i<m_VEnemyDeadList.size();i++)
	{
		RemoveEnemy(m_VEnemyDeadList[i]);
	}
	m_VEnemyDeadList.clear();
	for (int i=0;i<m_VFriendDeadList.size();i++)
	{
		RemoveFriend(m_VFriendDeadList[i]);
	}
	m_VFriendDeadList.clear();

	if(m_nActionCreatureNum != -1)
	{
		Character* cha = GetEnemy(m_nActionCreatureNum);
		if(!cha || cha->GetFinish())
			m_nActionCreatureNum = -1;

// 		//������
// 		if (cha->GetCamp() == eCamp_Enemy && cha->GetActionStage() == eActionStage_HandleStage)
// 		{
// 			m_nActionCreatureNum = -1;
// 			cha->SetFinish(true);
// 		}
	}

	if( !(UISystem::sInstance().IsInAnyControl()))
	{
		if (g_getLButtonState(App::sInstance().GetHGE()) == eLButtonState_Up)
			SelectCreature();
		if(g_getRButtonState(App::sInstance().GetHGE()) == eRButtonState_Up)
			UnSelectCreature();
	}

	ShowCreatureInfo();
}

void CreatureManager::ShowMoveRange(Character* creature)
{
	if (creature)
	{
		DWORD color = 0;
		if(creature->GetCamp() == eCamp_Friend)
			color = 0x4F3737DF;
		else if(creature->GetCamp() == eCamp_Enemy)
			color = 0x4FEB2323;
		std::vector<Block*> range = creature->CreateRange(MapManager::sInstance().GetCurrentMap(),creature->GetMoveAbility());
		//���˵������ƶ���Χ����ʵ���Ϲ���ȥ�ĵ㣬����ǽ�ڲ��ĵ㣬�м䱻ǽ���		
		for (std::vector<Block*>::iterator vit=range.begin();vit!=range.end();)
		{
			MapManager::sInstance().GetCurrentMap()->SetSpecificRange(range);
			vector<Block*> path = MapManager::sInstance().GetCurrentMap()->FindPath(creature->GetBlock().xpos,creature->GetBlock().ypos,(*vit)->xpos,(*vit)->ypos);
			if(path.empty())
				vit = range.erase(vit);
			else
				vit++;
		}
		for (std::vector<Block*>::iterator it=range.begin();it!=range.end();it++)
		{
			//�������ʾ�����ƶ�
			App::sInstance().DrawSmallRect(Block((*it)->xpos-MapManager::sInstance().GetCurrentMap()->GetOffX(),(*it)->ypos-MapManager::sInstance().GetCurrentMap()->GetOffY()),color);
		}
	}
}

void CreatureManager::ShowSkillCastRange(Character* creature)
{
	if (creature)
	{
		DWORD color = 0x4FEB2323;
		std::vector<Block*> range = creature->CreateRange(MapManager::sInstance().GetCurrentMap(),ConfigManager::sInstance().GetSkillInfo().find(creature->GetCastSkill())->second.m_nCastRange,true);
		for (std::vector<Block*>::iterator it=range.begin();it!=range.end();it++)
		{
			App::sInstance().DrawSmallRect(Block((*it)->xpos-MapManager::sInstance().GetCurrentMap()->GetOffX(),(*it)->ypos-MapManager::sInstance().GetCurrentMap()->GetOffY()),color);
		}
	}
}

void CreatureManager::ShowItemCastRange(Character* creature)
{
	if (creature)
	{
		DWORD color = 0x4FEB2323;
		std::vector<Block*> range = creature->CreateRange(MapManager::sInstance().GetCurrentMap(),1,true);
		for (std::vector<Block*>::iterator it=range.begin();it!=range.end();it++)
		{
			App::sInstance().DrawSmallRect(Block((*it)->xpos-MapManager::sInstance().GetCurrentMap()->GetOffX(),(*it)->ypos-MapManager::sInstance().GetCurrentMap()->GetOffY()),color);
		}
	}
}

void CreatureManager::ShowAttackRange(Character* creature)
{
	if (creature)
	{
		eAttackRange attackRange = creature->GetAttackRange();	
		Block charBlock = creature->GetBlock();
		Map* currentMap = MapManager::sInstance().GetCurrentMap();
		int mapWidth = 0,mapLength = 0;
		currentMap->GetWidthLength(mapWidth,mapLength);
		int offX = 0,offY = 0;

		for (MAttackRange::iterator mit=m_mAttackRange.begin();mit!=m_mAttackRange.end();mit++)
		{
			if(mit->first == attackRange)
			{
				for (vector<int>::iterator it=mit->second.begin();it!=mit->second.end();it++)
				{
					int mapOffx = 0,mapOffy = 0;
					if (currentMap)
					{
						mapOffx = currentMap->GetOffX();
						mapOffy = currentMap->GetOffY();
					}
					offX = m_vPair[*it].x + charBlock.xpos - mapOffx;
					offY = m_vPair[*it].y + charBlock.ypos - mapOffy;
					if (offX >= 0 && offX < mapWidth && offY >= 0 && offY < mapLength)
					{					
						App::sInstance().DrawBox(MAP_OFF_X + offX*MAP_RECT+4,MAP_OFF_Y + offY*MAP_RECT+4,0xBFFF0000,8,MAP_RECT-8,MAP_RECT-8);
					}
				}
			}
		}
	}
}

void CreatureManager::ShowSkillRange(int skillID)
{
	SkillInfo skillInfo = ConfigManager::sInstance().GetSkillInfo().find(skillID)->second;
	Map* currentMap = MapManager::sInstance().GetCurrentMap();
	int mapWidth = 0,mapLength = 0;
	currentMap->GetWidthLength(mapWidth,mapLength);
	int offX = 0,offY = 0;
	Block mouseBlock = App::sInstance().GetMouseBlock();
	DWORD color = 0xBFFF0000;
	if(skillInfo.m_nSkillType == eSkillType_Heal || skillInfo.m_nSkillType == eSkillType_Buff)
		color = 0xBF00FF00;

	for (MSkillRange::iterator mit=m_mSkillRange.begin();mit!=m_mSkillRange.end();mit++)
	{
		if(mit->first == skillInfo.m_nAttackRange)
		{
			//ֱ�������⴦��ֻ��ʾһ������
			if(mit->first == eSkillRange_ShortLine || mit->first == eSkillRange_Line)
			{
				offX = mouseBlock.xpos - currentMap->GetOffX();
				offY = mouseBlock.ypos - currentMap->GetOffY();
				App::sInstance().DrawBox(MAP_OFF_X + offX*MAP_RECT+4,MAP_OFF_Y + offY*MAP_RECT+4,color,8,MAP_RECT-8,MAP_RECT-8);
			}
			else
			{
				for (vector<int>::iterator it=mit->second.begin();it!=mit->second.end();it++)
				{
					offX = m_vPair[*it].x + mouseBlock.xpos - currentMap->GetOffX();
					offY = m_vPair[*it].y + mouseBlock.ypos - currentMap->GetOffY();
					if (offX >= 0 && offX < mapWidth && offY >= 0 && offY < mapLength)
					{					
						App::sInstance().DrawBox(MAP_OFF_X + offX*MAP_RECT+4,MAP_OFF_Y + offY*MAP_RECT+4,color,8,MAP_RECT-8,MAP_RECT-8);
					}
				}
			}
		}
	}
}

void CreatureManager::ShowCreatureInfo()
{
	Block block = App::sInstance().GetMouseBlock();
	if (block.xpos!=-1 && block.ypos!=-1)
	{
		Character* cha = GetEnemy(block.xpos,block.ypos);
		if(cha == NULL)
		{
			cha = GetFriend(block.xpos,block.ypos);
			if(cha == NULL)
			{
				TipWnd::sInstance().Clear();
				TipWnd::sInstance().SetShow(false);

// 				UIWindow* charInfoWindow = UISystem::sInstance().GetWindow(eWindowID_CharInfo);
// 				if(charInfoWindow && charInfoWindow->IsShow())
// 					charInfoWindow->SetShow(false);
				UISystem::sInstance().CloseWindow(eWindowID_CharInfo);
				return;
			}
		}
		UIWindow* commandWindow = UISystem::sInstance().GetWindow(eWindowID_Command);
		if(commandWindow && commandWindow->IsShow())
		{
			return;
		}
		if (m_nSelectNum!=-1)
		{
			Character* selectChar = GetCreature(m_nSelectNum);
			if(selectChar)
			{
				if(selectChar->GetActionStage() == eActionStage_AttackStage && selectChar->CanHitTarget(cha))
				{
					UIWindow* charInfoWindow = UISystem::sInstance().PopUpWindow(eWindowID_CharInfo);
					if(charInfoWindow)
						charInfoWindow->SetBindChar(cha);
					return;
				}
			}
		}
		//�õ���λ����ʾ����Ϣ
		char temp[256] = {0};
		TipWnd::sInstance().Clear();
		sprintf(temp," ID   : %d",cha->GetID());
		TipWnd::sInstance().AddText(temp,0xFFFFFFFF,-1,-1,eFontType_MSYaHei,eFontSize_FontMiddle);
		sprintf(temp," Num   : %d",cha->GetNum());
		TipWnd::sInstance().AddText(temp,0xFFFFFFFF,-1,-1,eFontType_MSYaHei,eFontSize_FontMiddle);
		sprintf(temp," ���� : %d , %d",cha->GetBlock().xpos,cha->GetBlock().ypos);
		TipWnd::sInstance().AddText(temp,0xFFFFFFFF,-1,-1,eFontType_MSYaHei,eFontSize_FontMiddle);
		sprintf(temp," ����ֵ��%d",cha->GetHP());
		TipWnd::sInstance().AddText(temp,0xFFFFFFFF,-1,-1,eFontType_MSYaHei,eFontSize_FontMiddle);
		sprintf(temp," ��������%d",cha->GetAttack());
		TipWnd::sInstance().AddText(temp,0xFFFFFFFF,-1,-1,eFontType_MSYaHei,eFontSize_FontMiddle);
		TipWnd::sInstance().SetShow(true);
		Map* theMap = MapManager::sInstance().GetCurrentMap();
		int offx = 0,offy = 0;
		if (theMap)
		{
			offx = theMap->GetOffX();
			offy = theMap->GetOffY();
		}
		TipWnd::sInstance().SetPos(block.xpos-offx,block.ypos-offy);
	}
	else
	{
// 		UIWindow* charInfoWindow = UISystem::sInstance().GetWindow(eWindowID_CharInfo);
// 		if(charInfoWindow && charInfoWindow->IsShow())
// 			charInfoWindow->SetShow(false);
		UISystem::sInstance().CloseWindow(eWindowID_CharInfo);

		TipWnd::sInstance().Clear();
		TipWnd::sInstance().SetShow(false);
	}

}

void CreatureManager::Strategy()
{
	if(m_bPause)
		return;

	//��ǰ�е�λ�ж��У��ȴ����ж����
	if(m_nActionCreatureNum != -1)
		return;

	ResetAllCreature();

	if (m_eCampTurn == eCampTurn_Enemy)
	{
		Character* enemy = GetNextEnemy();
		if(enemy == NULL)
			return;
		if(enemy->GetActionStage() == eActionStage_WaitStage)
		{
			g_AIMgr.SetCurCharacter(enemy);
			g_AIMgr.DoAction();
// 			enemy->SetActionStage(eActionStage_MoveStage);
 			m_nActionCreatureNum = enemy->GetNum();
// 			//m_nSelectNum = m_nActionCreatureNum;
// 			int xMove = g_RandomInt(0,enemy->GetMoveAbility());
// 			int yMove = enemy->GetMoveAbility() - xMove;
// 			int xDir = (g_RandomInt(0,1)==0)?-1:1;
// 			int yDir = (g_RandomInt(0,1)==0)?-1:1;
// 			eErrorCode errorCode = enemy->Move(enemy->GetBlock().xpos+xMove*xDir,enemy->GetBlock().ypos+yMove*yDir);
// 
// 			//����
// 			if (errorCode != eErrorCode_Success)
// 			{
// 				enemy->SetFinish(true);
// 			}
// 			DWORD data = enemy->GetBlock().xpos+xMove*xDir + ((enemy->GetBlock().ypos+yMove*yDir) << 8);
// 			ActionProcess* process = ActionProcess::sInstancePtr();
// 			process->PushAction(eNotify_Walk,enemy,NULL,data);
// 			process->PushAction(eNotify_FinishAttack,enemy,NULL,0);
		}

	}
//	enemy->Move((Direction)(App::sInstance().GetHGE()->Random_Int(0,4)));
}

VCharacter::iterator CreatureManager::RemoveEnemy(Character* _enemy)
{
	for (VCharacter::iterator it=m_VEnemyList.begin();it!=m_VEnemyList.end();)
	{
		if((*it)->GetID() == _enemy->GetID() && (*it)->GetNum() == _enemy->GetNum())
		{
			//��ռ�������Ա��
			setOccupied(MapManager::sInstance().GetCurrentMap()->GetBlock(_enemy->GetBlock().xpos,_enemy->GetBlock().ypos)->attri,0);
			gSafeDelete(*it);
			m_VEnemyList.erase(it);
			return it;
		}
		else
			it++;
	}
	return m_VEnemyList.end();
}

void CreatureManager::RemoveFriend(Character* _friend)
{
	for (VCharacter::iterator it=m_VFriendList.begin();it!=m_VFriendList.end();)
	{
		if((*it)->GetID() == _friend->GetID() && (*it)->GetNum() == _friend->GetNum())
		{
			//��ռ�������Ա��
			setOccupied(MapManager::sInstance().GetCurrentMap()->GetBlock(_friend->GetBlock().xpos,_friend->GetBlock().ypos)->attri,0);
			gSafeDelete(*it);
			m_VFriendList.erase(it);
			return;
		}
		else
			it++;
	}
}

Character* CreatureManager::GetNextEnemy()
{
	for (VCharacter::iterator it=m_VEnemyList.begin();it!=m_VEnemyList.end();it++)
	{
		if((*it)->GetFinish() == false && (*it)->GetHide()==false)	//��δ�ж�
			return *it;
	}
	return NULL;	//�����˶��ж�������Ӧ�ȴ��¸��غ�
}

//���ݵ�ͼ��ĳ���ӵ�λ�ã�ȡ�øø����ϵĵз�
Character* CreatureManager::GetEnemy(int x,int  y)
{
	for (VCharacter::iterator it=m_VEnemyList.begin();it!=m_VEnemyList.end();it++)
	{
		if((*it)->GetBlock().xpos==x && (*it)->GetBlock().ypos==y)
			return *it;
	}
	return NULL;
}

Character* CreatureManager::GetEnemy(int num)
{
	for (VCharacter::iterator it=m_VEnemyList.begin();it!=m_VEnemyList.end();it++)
	{
		if((*it)->GetNum() == num)
			return *it;
	}
	return NULL;
}

//���ݵ�ͼ��ĳ���ӵ�λ�ã�ȡ�øø����ϵ��ѷ�
Character* CreatureManager::GetFriend(int x,int  y)
{
	for (VCharacter::iterator it=m_VFriendList.begin();it!=m_VFriendList.end();it++)
	{
		if((*it)->GetBlock().xpos==x && (*it)->GetBlock().ypos==y)
			return *it;
	}
	return NULL;
}

Character* CreatureManager::GetFriend(int num)
{
	for (VCharacter::iterator it=m_VFriendList.begin();it!=m_VFriendList.end();it++)
	{
		if((*it)->GetNum() == num)
			return *it;
	}
	return NULL;
}

Character* CreatureManager::GetCreature(int x,int  y)
{
	Character* temp = GetEnemy(x,y);
	if(temp == NULL)
		temp = GetFriend(x,y);

	return temp;
}

Character* CreatureManager::GetCreature(int num)
{
	Character* temp = GetEnemy(num);
	if(temp == NULL)
		temp = GetFriend(num);

	return temp;
}

bool CreatureManager::IsEnemyAllDead()
{
	if(m_VEnemyList.empty())
		return true;
	return false;
}

bool CreatureManager::IsFriendAllDead()
{
	if(m_VFriendList.empty())
		return true;
	return false;
}

bool CreatureManager::ResetAllCreature()
{
	for (VCharacter::iterator it=m_VFriendList.begin();it!=m_VFriendList.end();it++)
	{
		if((*it)->GetFinish() == false && (*it)->GetHide()==false)	//���ѷ���δ�ж�
			return false;
	}
	//�ѷ��ж��꣬����з��غ�
	if (m_eCampTurn == eCampTurn_Friend)
	{
		m_eCampTurn = eCampTurn_Enemy;
		m_nSelectNum = -1;
		//MapManager::sInstance().GetCurrentMap()->GoIntoTurn(m_eCampTurn);
	}

	for (VCharacter::iterator it=m_VEnemyList.begin();it!=m_VEnemyList.end();it++)
	{
		if((*it)->GetFinish() == false && (*it)->GetHide()==false)	//�ез���δ�ж�
			return false;
	}
	//�з��ж��꣬������һ�غ�
	if (m_eCampTurn == eCampTurn_Enemy)
	{
		m_eCampTurn = eCampTurn_Friend;
		MapManager::sInstance().GetCurrentMap()->AddTurn();
		MapManager::sInstance().GetCurrentMap()->GoIntoTurn(m_eCampTurn);
	}

	for (VCharacter::iterator it=m_VEnemyList.begin();it!=m_VEnemyList.end();it++)
	{
			(*it)->SetFinish(false);
			(*it)->SetActionStage(eActionStage_WaitStage);
			(*it)->ChangeColor(0xFFFFFFFF);

			//����buff
			(*it)->RemoveBuff();
	}
	for (VCharacter::iterator it=m_VFriendList.begin();it!=m_VFriendList.end();it++)
	{
		(*it)->SetFinish(false);
		(*it)->SetActionStage(eActionStage_WaitStage);
		(*it)->ChangeColor(0xFFFFFFFF);

		//����buff
		(*it)->RemoveBuff();
	}
	return true;
}
/*
int CreatureManager::Notify(int src,int tar,int messageID,int param)
{
	int result = -1;
	Character* target = GetCreature(tar);
	if (!target)
		result = eNotify_NoTarget;

	switch(messageID)
	{
	case eNotify_TowardToAttacker:
		result = target->TowardToAttacker(src);
		break;
	case eNotify_ReadyToBeAttacked:
		{
			//�����Ƿ񱩻�
			bool bCrit = false;
			if (g_RandomInt(0,9) < (int)(target->GetCrit()*10))
				bCrit = true;

			Character* _self = GetCreature(src);
			if(_self)
				CalculateHurt(target,_self,bCrit);

			if(!bCrit)
				target->Attack();
			else
				target->Crit();

			result = eNotify_Success;
			break;
		}
	case eNotify_FinishAttack:
		{
			target->SetFinish(true);
		}
		break;
	default:
		break;
	}

	return result;
}*/

void CreatureManager::CalculateHurt(Character* cast,Character* target,bool bCrit)
{
	if(!cast || !target)
		target->GetPreHurt() = 0;

	int hurt = cast->GetAttack() - target->GetDefend();
	if(bCrit)
		hurt += cast->GetAttack();
	if(hurt > 0)
		target->GetPreHurt() = hurt;
}

// void CreatureManager::CalculateResult(int src,int tar)
// {
// 	Character* cast = GetCreature(src);
// 	Character* target = GetCreature(tar);
// 	//���ȼ����������
// 	bool bhit = false;
// 	if(g_RandomInt(0,9) >= (int)(target->GetDodge()*10))
// 		bhit = true;
// 
// 
// 	target->GetCounter() = true;
// 	if(bhit)
// 	{
// 		target->GetHP() -= cast->GetPreHurt();
// 		cast->GetPreHurt() = 0;
// 
// 		target->Attacked();
// 	}
// 	else
// 		target->Defend();
// }

void CreatureManager::PreAttackAndPushAction(Character* cast,Character* target,DWORD data)
{
	if(!cast || !target)
		return;
	cast->GetPreHurt() = 0;
	target->GetPreHurt() = 0;
	//���ȼ����Ƿ񱩻�
	//�����Ƿ񱩻�
	bool bCrit = false;
	if (g_RandomInt(0,9) < (int)(cast->GetCrit()*10))
		bCrit = true;

	//���й����˺�Ԥ��
	CalculateHurt(cast,target,bCrit);

	//�����Ƿ�����
	bool bhit = false;
	if(g_RandomInt(0,9) >= (int)(target->GetDodge()*10))
		bhit = true;
	else 
		target->GetPreHurt() = 0;

	//Ŀ���Ƿ�����
	bool bDead = false;
	if(target->GetHP() <= target->GetPreHurt())
		bDead = true;

	bool bCounter = false;
	bool bCrit2 = false;
	bool bhit2 = false;
	bool bDead2 = false;
	if(!bDead)
	{
		//�����Ƿ񷴻�
		if(target->GetCounter())
		{
			//�Ƿ��ڷ�����Χ
			if(data!=0)
			{
				//ʹ�ø������ݣ���ʾ����������δ��ĳ����ʩ�ӹ�������
				int x = data & 0x00FF;
				int y = data >> 8;
				if(target->CanHitPoint(x,y))
					bCounter = true;
			}
			else if(target->CanHitTarget(cast))
			{
				bCounter = true;
			}
		}

		if(bCounter)
		{
			//�����Ƿ񱩻�
			if (g_RandomInt(0,9) < (int)(target->GetCrit()*10))
				bCrit2 = true;

			//���й����˺�Ԥ��
			CalculateHurt(target,cast,bCrit2);

			//�����Ƿ�����
			if(g_RandomInt(0,9) >= (int)(cast->GetDodge()*10))
				bhit2 = true;
			else
				cast->GetPreHurt() = 0;

			//cast�Ƿ�����
			if(cast->GetPreHurt() >= cast->GetHP())
				bDead2 = true;
		}
	}

	//����״̬
	cast->SetActionStage(eActionStage_AttackCastStage);

	//���Ͷ���
	ActionProcess* process = ActionProcess::sInstancePtr();
	process->PushAction(eNotify_TowardToAttacker,cast,target,0);
	process->PushAction(eNotify_TowardToAttacker,target,cast,200);
	if(bCrit)
		process->PushAction(eNotify_Crit,cast,target,1000);
	if(bhit)
		process->PushAction(eNotify_Attack,cast,target,500);
	else
		process->PushAction(eNotify_AttackDefend,cast,target,500);
	if(bDead)
		process->PushAction(eNotify_Dead,cast,target,300);
	else
	{
		if(bCounter)
		{
			if(bCrit2)
				process->PushAction(eNotify_Crit,target,cast,1000);
			if(bhit2)
				process->PushAction(eNotify_Attack,target,cast,500);
			else
				process->PushAction(eNotify_AttackDefend,target,cast,500);
			if(bDead2)
				process->PushAction(eNotify_Dead,target,cast,300);
		}
	}
	//�����ж�
	if(!bDead2)
		process->PushAction(eNotify_FinishAttack,cast,target,0);
}

void CreatureManager::PreSkillAndPushAction(Character* cast,Character* target,DWORD data)
{
	if(!cast || !target)
		return;

	//�ҵ����ܷ�Χ�ڵ����е���
	m_vSkillTargets.clear();
	SkillInfo skill = ConfigManager::sInstance().GetSkillInfo().find(cast->GetCastSkill())->second;
	VCharacter vTar;
	int tarX = 0,tarY = 0;
	for (MSkillRange::iterator mit=m_mSkillRange.begin();mit!=m_mSkillRange.end();mit++)
	{
		if(mit->first == skill.m_nCastRange)
		{
			if(skill.m_nCastRange != eSkillRange_ShortLine || skill.m_nCastRange!= eSkillRange_Line)
			{
				for (vector<int>::iterator it=mit->second.begin();it!=mit->second.end();it++)
				{
					tarX = m_vPair[*it].x + target->GetBlock().xpos;
					tarY = m_vPair[*it].y + target->GetBlock().ypos;
					Character* otherChar = NULL;
					if(skill.m_nSkillType == eSkillType_Hurt)
						otherChar = GetEnemy(tarX,tarY);
					else if(skill.m_nSkillType == eSkillType_Heal || skill.m_nSkillType == eSkillType_Buff)
						otherChar = GetFriend(tarX,tarY);
					if(otherChar!=NULL)
						vTar.push_back(otherChar);
				}
			}
			else
			{
				//ȷ��˫��λ��
				eDirection dir = eDirection_None;
				Block& block = target->GetBlock();
				if (block.xpos < cast->GetBlock().xpos)
					dir = eDirection_Left;
				else if(block.xpos > cast->GetBlock().xpos)
					dir = eDirection_Right;
				else if(block.ypos < cast->GetBlock().ypos)
					dir = eDirection_Up;
				else
					dir = eDirection_Down;

				int num = ((skill.m_nCastRange==eSkillRange_ShortLine)?12:24);
				for(int i=num/4*(dir-1);i<num/4*(dir-1)+4;i++)
				{
					tarX = m_vPair[mit->second[i]].x + target->GetBlock().xpos;
					tarY = m_vPair[mit->second[i]].y + target->GetBlock().ypos;
					Character* otherChar = NULL;
					if(skill.m_nSkillType == eSkillType_Hurt)
						otherChar = GetEnemy(tarX,tarY);
					else if(skill.m_nSkillType == eSkillType_Heal || skill.m_nSkillType == eSkillType_Buff)
						otherChar = GetFriend(tarX,tarY);
					if(otherChar!=NULL)
						vTar.push_back(otherChar);
				}
			}
		}
	}
	m_vSkillTargets = vTar;

	//����״̬
	cast->SetActionStage(eActionStage_SkillCastStage);

	ActionProcess* process = ActionProcess::sInstancePtr();
	process->PushAction(eNotify_TowardToAttacker,cast,target,0);
	process->PushAction(eNotify_CastAction,cast,target,500);
	process->PushAction(eNotify_CastSkill,cast,target,0);

	cast->GetPreHurt() = 0;
	if(skill.m_nSkillType == eSkillType_Hurt)
	{
		for(VCharacter::iterator it=vTar.begin();it!=vTar.end();it++)
		{
			(*it)->GetPreHurt() = skill.m_nAttack;
			bool bDead = false;
			if((*it)->GetHP() <= (*it)->GetPreHurt())
				bDead = true;

			process->PushAction(eNotify_Attacked,cast,(*it),500);
			if(bDead)
				process->PushAction(eNotify_Dead,cast,(*it),300);
		}
	}
	else if(skill.m_nSkillType == eSkillType_Heal || skill.m_nSkillType == eSkillType_Buff)
	{
		for(VCharacter::iterator it=vTar.begin();it!=vTar.end();it++)
		{
			(*it)->GetPreHurt() = skill.m_nAttack;
			process->PushAction(eNotify_Healed,cast,(*it),500);
		}
	}

	process->PushAction(eNotify_FinishAttack,cast,target,0);
}

void CreatureManager::PreItemAndPushAction(Character* cast,Character* target,DWORD data)
{
	if(!cast || !target)
		return;

	//����״̬
	cast->SetActionStage(eActionStage_ItemCastStage);

	ActionProcess* process = ActionProcess::sInstancePtr();
	process->PushAction(eNotify_TowardToAttacker,cast,target,0);
	process->PushAction(eNotify_UseItem,cast,target,0);
	process->PushAction(eNotify_ItemEffect,cast,target,500);
	process->PushAction(eNotify_FinishAttack,cast,target,0);
}

void CreatureManager::SelectCreature()
{
	if (!m_bPause)
	{
		//�ѷ��غϲſ��Խ���ѡ������������鿴�з��ж���Χ
		if (m_eCampTurn == eCampTurn_Friend)
		{
			Block mouseBlock = App::sInstance().GetMouseBlock();
			Character* selectChar = NULL;
			if (mouseBlock.xpos!=-1 && mouseBlock.ypos!=-1)
			{
				selectChar = GetCreature(mouseBlock.xpos,mouseBlock.ypos);
				//ѡ�е�λ
				if(selectChar!=NULL)
				{
					//�ر��ܽᴰ��
					UIWindow* summaryWindow = UISystem::sInstance().GetWindow(eWindowID_Summary);
					if(summaryWindow && summaryWindow->IsShow())
						UISystem::sInstance().CloseWindow(eWindowID_Summary);

					int nLastSelect = m_nSelectNum;
					if (selectChar->GetCamp() == eCamp_Friend )
					{
						//�ϴ�û��ѡ��λ
						if(nLastSelect == -1)
						{
							//ѡ�е�δ�ж��ѷ������ƶ��׶�
							if(!selectChar->GetFinish())
							{
								m_nSelectNum = selectChar->GetNum();
								selectChar->SetActionStage(eActionStage_MoveStage);
							}
						}
						//�������ε��ͬһ�ѷ���λ
						else if(nLastSelect == selectChar->GetNum())
						{
							if(!selectChar->GetFinish())
							{
								if(selectChar->GetActionStage()== eActionStage_MoveStage)
								{
									//�����ƶ��׶Σ���������׶�
									selectChar->SetActionStage(eActionStage_HandleStage);
									//�򿪲�������
									// 								UIWindow* commandWindow = UISystem::sInstance().GetWindow(eWindowID_Command);
									// 								if(commandWindow)
									// 								{
									// 									commandWindow->SetShow(true);
									// 									commandWindow->SetBindChar(selectChar);
									// 								}
									UIWindow* commandWindow = UISystem::sInstance().PopUpWindow(eWindowID_Command);
									if(commandWindow)
										commandWindow->SetBindChar(selectChar);
								}
								else if (selectChar->GetActionStage()== eActionStage_SkillTargetStage && selectChar->GetCastSkill()!=-1)
								{
									//���Լ��ͷż���
									if(selectChar->CanSkillHitTarget(selectChar))
									{
										PreSkillAndPushAction(selectChar,selectChar);
										return;
									}
								}
								else if (selectChar->GetActionStage() == eActionStage_ItemTargetStage && selectChar->GetUseItem()!=-1)
								{
									//���Լ�ʹ����Ʒ
									if (selectChar->CanUseItem(selectChar))
									{
										PreItemAndPushAction(selectChar,selectChar);
										return;
									}
								}
							}
						}
						//�������
						else
						{
							Character* lastChar = GetCreature(nLastSelect);
							if(lastChar != NULL)
							{
								//�ϴε���Ǳ���ѷ�
								if(lastChar->GetCamp() == eCamp_Friend)
								{
									if(lastChar->GetActionStage() == eActionStage_SkillTargetStage && lastChar->GetCastSkill()!=-1)
									{
										//���ѷ��ͷż���
										if(lastChar->CanSkillHitTarget(selectChar))
										{
											PreSkillAndPushAction(lastChar,selectChar);
											return;
										}
									}
									else if (lastChar->GetActionStage() == eActionStage_ItemTargetStage && lastChar->GetUseItem()!= -1)
									{
										if (lastChar->CanUseItem(selectChar))
										{
											PreItemAndPushAction(lastChar,selectChar);
											return;
										}
									}
									else if(lastChar->GetFinish() && !selectChar->GetFinish())
									{
										m_nSelectNum = selectChar->GetNum();
										selectChar->SetActionStage(eActionStage_MoveStage);
									}
									// 									//�ϴε��ѷ������������׶�
									// 									lastChar->SetActionStage(eActionStage_WaitStage);
									// 									selectChar->SetActionStage(eActionStage_MoveStage);
								}
								else if(lastChar->GetCamp() == eCamp_Enemy)
								{
									if(!selectChar->GetFinish())
									{
										m_nSelectNum = selectChar->GetNum();
										selectChar->SetActionStage(eActionStage_MoveStage);
									}
								}
							}
							else 
								//�����Ǵ����֧�����������е�����
								g_debugString(__FILE__,__FUNCTION__,__LINE__);
							return;
						}
					}
					else if (selectChar->GetCamp() == eCamp_Enemy)
					{
						Character* lastChar = GetCreature(nLastSelect);
						if(lastChar!=NULL) 
						{
							if(lastChar->GetCamp()==eCamp_Friend)
							{
								if(lastChar->GetActionStage() == eActionStage_AttackStage)
								{
									//�ж��Ƿ���Թ�����ѡ�е�λ
									if(lastChar->CanHitTarget(selectChar))
									{
										//										lastChar->SetTarget(selectChar->GetNum());
										//���ص�λ��Ϣ����
										// 									UIWindow* charInfoWindow = UISystem::sInstance().GetWindow(eWindowID_CharInfo);
										// 									if(charInfoWindow && charInfoWindow->IsShow())
										// 									{
										// 										charInfoWindow->SetShow(false);
										// 										UIWindow* commandWnd = UISystem::sInstance().GetWindow(eWindowID_Command);
										// 										if(commandWnd)
										// 											commandWnd->SetBindChar(NULL);
										// 									}	
										UISystem::sInstance().CloseWindow(eWindowID_CharInfo);

										//lastChar->GeginHit();
										//����Ԥ����
										PreAttackAndPushAction(lastChar,selectChar);
										return;
									}
								}
								else if (lastChar->GetActionStage() == eActionStage_SkillTargetStage && lastChar->GetCastSkill()!=-1)
								{
									if(lastChar->CanSkillHitTarget(selectChar))
									{
										PreSkillAndPushAction(lastChar,selectChar);
										return;
									}
								}
								// 							else if (lastChar->GetActionStage() == eActionStage_ItemStage && lastChar->GetUseItem()!=-1)
								// 							{
								// 								if (lastChar->CanUseItem(selectChar))
								// 								{
								// 									m_nSelectNum = -1;
								// 									PreItemAndPushAction(lastChar,selectChar);
								// 									return;
								// 								}
								// 							}
							}
							else if (lastChar->GetCamp() == eCamp_Enemy)
							{
								m_nSelectNum = selectChar->GetNum();
							}
						}
						else
						{
							m_nSelectNum = selectChar->GetNum();
						}
					}
				}
				//���е���
				else
				{
					if (m_nSelectNum>=0)
					{
						selectChar = GetCreature(m_nSelectNum);
						if (selectChar!= NULL)
						{
							if(selectChar->GetCamp() == eCamp_Friend && selectChar->GetActionStage() == eActionStage_MoveStage)
							{
								//�ж��Ƿ�����ƶ���ȥ
								int length = abs(mouseBlock.xpos - selectChar->GetBlock().xpos) + abs(mouseBlock.ypos - selectChar->GetBlock().ypos);
								//�����ƶ���Χ
								if(length > selectChar->GetMoveAbility())
									return;

								//�ƶ���ȥ����¼ԭʼλ��
								selectChar->GetOrigBlock().xpos = selectChar->GetBlock().xpos;
								selectChar->GetOrigBlock().ypos = selectChar->GetBlock().ypos;
								selectChar->GetOrigDirection() = selectChar->GetCurDirection();
								selectChar->Move(mouseBlock.xpos,mouseBlock.ypos);
							}
						}
					}
				}
			}
		}
	}
}

void CreatureManager::UnSelectCreature()
{
	if (m_nSelectNum >= 0)
	{
		int nLastSelect = m_nSelectNum;
		Character* lastChar = GetCreature(nLastSelect);
		if(lastChar!=NULL && lastChar->GetCamp()==eCamp_Friend && !(lastChar->GetFinish()))
		{
			//������ԭλ��
			eActionStage stage = lastChar->GetActionStage();
			if(stage == eActionStage_HandleStage)
			{
				lastChar->SetActionStage(eActionStage_MoveStage);
				lastChar->CancelMove();
				//�رղ�������
// 				UIWindow* commandWindow = UISystem::sInstance().GetWindow(eWindowID_Command);
// 				if(commandWindow)
// 				{
// 					commandWindow->SetShow(false);
// 				}
				UISystem::sInstance().CloseWindow(eWindowID_Command);
			}
			//ѡ�񹥻��׶Ρ�ѡ���ܽ׶Ρ�ѡ����Ŀ�ꡢѡ����Ʒ��ѡ����ƷĿ��׶ο��Է����������׶�
			else if(stage == eActionStage_AttackStage || stage == eActionStage_ItemTargetStage || stage == eActionStage_SkillTargetStage
				|| stage == eActionStage_ItemStage || stage == eActionStage_SkillStage)
			{
				lastChar->GetCastSkill() = -1;
				lastChar->SetActionStage(eActionStage_HandleStage);
				//�򿪲�������
// 				UIWindow* commandWindow = UISystem::sInstance().GetWindow(eWindowID_Command);
// 				if(commandWindow)
// 				{
// 					commandWindow->SetShow(true);
// 					commandWindow->SetBindChar(lastChar);
// 				}
				UIWindow* commandWindow = UISystem::sInstance().PopUpWindow(eWindowID_Command);
				if(commandWindow)
					commandWindow->SetBindChar(lastChar);
				//�رռ��ܽ���
// 				UIWindow* selectWndow = UISystem::sInstance().GetWindow(eWindowID_Select);
// 				if(selectWndow)
// 				{
// 					selectWndow->SetShow(false);
// 				}
				UISystem::sInstance().CloseWindow(eWindowID_Select);
			}
			//�Ҽ�ȡ��ѡ�е��ѷ�����Ҫ�����ж��׶Σ������ƶ��еĵ�λ�����Ե�ʱȡ��
			else if(stage == eActionStage_MoveStage && lastChar->GetCharacterState()==eCharacterState_Stand)	
			{
				lastChar->SetActionStage(eActionStage_WaitStage);
				m_nSelectNum = -1;
			}
		}
		else
			m_nSelectNum = -1;
	}
	else
	{
		Block mouseBlock = App::sInstance().GetMouseBlock();
		Character* selectChar = NULL;
		if (mouseBlock.xpos!=-1 && mouseBlock.ypos!=-1)
		{
			UIWindow* summaryWindow = UISystem::sInstance().GetWindow(eWindowID_Summary);
			if(summaryWindow && summaryWindow->IsShow())
				UISystem::sInstance().CloseWindow(eWindowID_Summary);
			else
			{
				if (m_eCampTurn == eCampTurn_Friend)
				{
					selectChar = GetCreature(mouseBlock.xpos,mouseBlock.ypos);
					if (selectChar)
					{
						summaryWindow = UISystem::sInstance().PopUpWindow(eWindowID_Summary);
						if(summaryWindow)
							summaryWindow->SetBindChar(selectChar);
					}
				}
			}
		}
	}
}

void CreatureManager::ProcessSelectCreature()
{
	if(m_nSelectNum >= 0)
	{
		Character* selectChar = GetCreature(m_nSelectNum);
		if((selectChar->GetCamp() == eCamp_Enemy) || (selectChar->GetCamp()==eCamp_Friend && selectChar->GetCharacterState()==eCharacterState_Stand && selectChar->GetActionStage() ==eActionStage_MoveStage))
			ShowMoveRange(selectChar);
		if(selectChar->GetCamp() == eCamp_Enemy || (selectChar->GetCamp()==eCamp_Friend && (selectChar->GetActionStage()==eActionStage_AttackStage)))
			ShowAttackRange(selectChar);
		if(selectChar->GetCamp()==eCamp_Friend && selectChar->GetActionStage()==eActionStage_SkillTargetStage && selectChar->GetCastSkill()!=-1)
			ShowSkillCastRange(selectChar);
		if(selectChar->GetCamp() == eCamp_Friend && selectChar->GetActionStage() == eActionStage_ItemTargetStage && selectChar->GetUseItem() != -1)
			ShowItemCastRange(selectChar);

		if(selectChar->GetCastSkill() != -1 && selectChar->GetActionStage() == eActionStage_SkillTargetStage)
		{
			ShowSkillRange(selectChar->GetCastSkill());
		}
	}
}

void CreatureManager::AddItem(int id,int num)
{
	if(num <= 0)
		return;
	//�����Ƿ���ڸ�����Ʒ
	LItems::iterator it = m_lItems.begin();
	for (;it!=m_lItems.end();it++)
	{
		if(it->m_iItem.m_nID == id)
		{
			it->m_nNum += num;
			return;
		}
	}
	//�µ���Ʒ����
	Items newItem;
	newItem.m_iItem = ConfigManager::sInstance().GetItemInfo().find(id)->second;
	newItem.m_nNum = num;
	m_lItems.push_back(newItem);
}

void CreatureManager::RemoveItem(int id)
{
	//�����Ƿ���ڸ�����Ʒ
	LItems::iterator it = m_lItems.begin();
	for (;it!=m_lItems.end();it++)
	{
		if(it->m_iItem.m_nID == id)
		{
			if(it->m_nNum > 0)
				it->m_nNum--;
			break;
		}
	}
	if (it->m_nNum == 0)
		m_lItems.erase(it);
}

void CreatureManager::AddAction(eAction action,int num,DWORD time,eDirection dir,DWORD data,const char*name,const char* word)
{
	NewAction newAction;
	newAction.m_eAction = action;
	newAction.m_nNum = num;
	newAction.m_dwTime = time;
	newAction.m_eDir = dir;
	newAction.m_dwData = data;
	newAction.m_strName = name;
	newAction.m_strWords = word;
	m_lActions.push_back(newAction);
}

void CreatureManager::ProcessAction()
{
	//����������������ͣ���ܽ��о�������
	if(!m_bPause)
		return;

	if (m_eState == eActionState_PickAction)
	{
		if(!m_lActions.empty())
		{
			NewAction aAction = m_lActions.front();
			//�ǵ�λս����������
			if (aAction.m_eAction > eAction_FightActionStart && aAction.m_eAction < eAction_FightActionEnd)
			{
				Character* target = GetCreature(aAction.m_nNum);
				if (target)
				{
					target->PushAction(aAction);
					m_eState = eActionState_Process;
				}
			}
			//�Ի�����
			else if (aAction.m_eAction == eAction_Talk)
			{
				WndDialog* dialog = (WndDialog*)UISystem::sInstance().GetWindow(eWindowID_Dialog);
				if (dialog)
				{
					dialog->SetShow(true);
					dialog->PushWords(aAction.m_dwData,aAction.m_strName.c_str(),aAction.m_strWords.c_str());
					m_eState = eActionState_Process;
				}
			}
		}
		else
		{
			//��ͣ���������̣����Ǿ����Ѿ�û�ж������ʻָ���������
			m_bPause = false;
		}
	}
	else if (m_eState == eActionState_Process)
	{
		bool bActionOver = true;
		NewAction aAction = m_lActions.front();
		Character* target = GetCreature(aAction.m_nNum);
		if (target && target->IsInAction())
		{
			bActionOver = false;
			if (aAction.m_eAction == eAction_Crit)
			{
				int nTempColor = (int)(255*target->GetAction().m_dwTime/1000);
				DWORD dwCritColor = (nTempColor + (nTempColor << 8)) | 0xFFFF0000;
				target->ChangeColor(dwCritColor);
			}
			else if (aAction.m_eAction == eAction_Disappear)
			{
				int nTempColor = (int)(255*target->GetAction().m_dwTime/1000);
				DWORD color = (nTempColor << 24) | 0x00FFFFFF;
				target->ChangeColor(color);
			}
			else if (aAction.m_eAction == eAction_Appears)
			{
				int nTempColor = (int)(255*(1000-target->GetAction().m_dwTime)/1000);
				DWORD color = (nTempColor << 24) | 0x00FFFFFF;
				target->ChangeColor(color);
			}
		}
		if (bActionOver)
		{
			WndDialog* dialog = (WndDialog*)UISystem::sInstance().GetWindow(eWindowID_Dialog);
			if(dialog && !dialog->IsFinishWords())
				bActionOver = false;
		}
		if (bActionOver)
		{
			m_eState = eActionState_End;
		}
	}
	else if (m_eState == eActionState_End)
	{
		NewAction aAction = m_lActions.front();
		Character* target = GetCreature(aAction.m_nNum);
		if (aAction.m_eAction == eAction_Crit)
		{
			if (target)
			{
				target->ChangeColor(0xFFFFFFFF);
			}
		}
		else if (aAction.m_eAction == eAction_Disappear)
		{
			Map* theMap = MapManager::sInstance().GetCurrentMap();
			Block* oldBlock = theMap->GetBlock(target->GetBlock().xpos,target->GetBlock().ypos);
			if(oldBlock!=NULL)
				setOccupied((oldBlock->attri),0);
			target->GetBlock().xpos = target->GetBlock().ypos = -1;
			target->GetOrigBlock().xpos = target->GetOrigBlock().ypos = -1;
			target->SetHide(true);
		}
		else if (aAction.m_eAction == eAction_Appears)
		{
		}
		m_lActions.pop_front();
		//�����ʱû�ж����ˣ���ֵ�հ׶���
		if (m_lActions.empty())
		{
			aAction.m_eAction = eAction_None;
			target->PushAction(aAction);
		}
		m_eState = eActionState_PickAction;
	}
}

VAttackTarget CreatureManager::GetAttackTarget(Character* attacker)
{
	VAttackTarget targets;
	targets.clear();

	if(!attacker)
		return targets;

// 	//��ʼ��һ�ŵ�ͼ�����ڹ��˵�̽����ĵ�ͼ��
// 	int width = 0;
// 	int height = 0;
// 	MapManager::sInstance().GetCurrentMap()->GetWidthLength(width,height);
// 	bool surveyMap[MAX_MAP_WIDTH_NUM][MAX_MAP_LENGTH_NUM];
// 	for(int i=0;i<MAX_MAP_WIDTH_NUM;i++)
// 	{
// 		for(int j=0;j<MAX_MAP_LENGTH_NUM;j++)
// 			surveyMap[i][j] = false;
// 	}

	//�����ߵ��ƶ���Χ
	std::vector<Block*> range = attacker->GetMoveRange();
	//����ÿ�����ƶ��㣬���ҹ�����Χ�ڵĵз�
	for (std::vector<Block*>::iterator it=range.begin();it!=range.end();it++)
	{
		//������Χ����
		for (MAttackRange::iterator mit=m_mAttackRange.begin();mit!=m_mAttackRange.end();mit++)
		{
			if(mit->first == attacker->GetAttackRange())
			{
				for (vector<int>::iterator it2=mit->second.begin();it2!=mit->second.end();it2++)
				{
					int x = m_vPair[*it2].x + (*it)->xpos;
					int y = m_vPair[*it2].y + (*it)->ypos;
					Character* target = GetCreature(x,y);
					//�ҵ�Ŀ�꣬�Ҳ����Լ�
					if(target && target->GetNum() != attacker->GetNum())				
					{
						//���Ŀ����ӹ��ˣ����ж��Ƿ���Ҫ���¹�����
						VAttackTarget::iterator vtit=targets.begin();
						for(;vtit!=targets.end();vtit++)
						{
							if(vtit->m_iTarget == target)
							{
								//�ж�����ҵ��ĵ��Ƿ���ϴεĹ��������
								//��Ҫ��׼���Ƿ�ᱻ������
								if(target->CanHitPoint((*it)->xpos,(*it)->ypos) == false)
								{
									vtit->m_iAttakPoint.x = (*it)->xpos;
									vtit->m_iAttakPoint.y = (*it)->ypos;
								}
								break;
							}
						}

						//Ŀ��û����ӹ�
						if(vtit == targets.end() &&((target->GetCamp()==eCamp_Friend && attacker->GetCamp()==eCamp_Enemy) ||
							(target->GetCamp() == eCamp_Enemy && attacker->GetCamp()==eCamp_Friend)))
						{
							AttackTarget attackTarget;
							attackTarget.m_iTarget = target;
							attackTarget.m_iAttakPoint.x = (*it)->xpos;
							attackTarget.m_iAttakPoint.y = (*it)->ypos;
							targets.push_back(attackTarget);
						}
					}
				}
				break;
			}
		}
	}

	return targets;
}

std::vector<Block*> CreatureManager::GetLiveBlock(Character* target,bool& allSafe)
{
	std::vector<Block*> vLiveBlocks;
	vLiveBlocks.clear();
	if(!target)
		return vLiveBlocks;

	//�Լ����ƶ���Χ
	std::vector<Block*> range = target->GetMoveRange();

	//���������Լ��ĵ�λ
	VCharacter vDangerousTar;
	//�������е��ˣ����˳���Щ���ܹ��������Լ���
	VCharacter& vMyEnemy = (target->GetCamp()==eCamp_Enemy)?GetFriend():GetEnemy();
	for (VCharacter::iterator it=vMyEnemy.begin();it!=vMyEnemy.end();it++)
	{
		int attackType = -1;
		POINT point;
		point.x = point.y = 0;	//����Ҫ��
		if(g_AIMgr.CanKillTarget(*it,target,attackType,point,true) == true)
			vDangerousTar.push_back(*it);
	}
	//û��������λ����Ͳ���Ҫ����Ѱ���ƶ��㣬���е㶼�ǰ�ȫ��
	if(vDangerousTar.empty())
		return vLiveBlocks;

	//�����Լ������ƶ��ĵ㣬�ж���Щ���Ա���Щ������λ������
	//g_AIMgr.CanHitPointsOrTarget(vDangerousTar,NULL,range);

	//�ҵ����е��˵��ƶ�������Χ�����Լ����ƶ���Χ����ֵ����
	bool surveyMap[MAX_MAP_WIDTH_NUM][MAX_MAP_LENGTH_NUM];
	memset(surveyMap,false,sizeof(bool)*MAX_MAP_WIDTH_NUM*MAX_MAP_LENGTH_NUM);
	for (VCharacter::iterator it=vDangerousTar.begin();it!=vDangerousTar.end();it++)
	{
		std::vector<Block*> range2 = (*it)->GetMoveRange();
		for (std::vector<Block*>::iterator bit=range2.begin();bit!=range2.end();bit++)
		{
			int tarX = 0,tarY = 0;
			VPair vPairPoint = CreatureManager::sInstance().GetRangePoint();
			for (MAttackRange::iterator mit=CreatureManager::sInstance().GetAttackRange().begin();mit!=CreatureManager::sInstance().GetAttackRange().end();mit++)
			{
				if(mit->first == (*it)->GetAttackRange())
				{
					for (vector<int>::iterator it2=mit->second.begin();it2!=mit->second.end();it2++)
					{
						tarX = vPairPoint[*it2].x + (*bit)->xpos;
						tarY = vPairPoint[*it2].y + (*bit)->ypos;
						if(surveyMap[tarX][tarY])
							continue;
						surveyMap[tarX][tarY] = true;
					}
				}
			}
		}
	}
	bool bAllBlocksSafe = true;
	for(std::vector<Block*>::iterator it=range.begin();it!=range.end();)
	{
		if(surveyMap[(*it)->xpos][(*it)->ypos])
		{
			it = range.erase(it);
			bAllBlocksSafe = false;
		}
		else
			it++;
	}
	//��Ȼ��������λ���ڣ��������Ǵ򲻵��Լ����ƶ���Χ�������Լ��ǰ�ȫ��
	if(bAllBlocksSafe)
	{
		allSafe = true;
		return vLiveBlocks;
	}
	return range;
}

VCharacter CreatureManager::GetAttackersToTarget(Character* target)
{
	VCharacter attackers;
	if(!target)
		return attackers;

	//ȡ�����п��Թ�����Ŀ��ĵ�λ
	VCharacter& chaList = (target->GetCamp()==eCamp_Friend)?GetEnemy():GetFriend();
	for (VCharacter::iterator vit=chaList.begin();vit!=chaList.end();vit++)
	{
		POINT pt;
		if(g_AIMgr.CanHitPointOrTarget((*vit),target,NULL,pt))
			attackers.push_back((*vit));
	}
	return attackers;
}