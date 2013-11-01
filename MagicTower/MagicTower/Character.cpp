#include "Character.h"
#include "App.h"
#include "MapManager.h"
#include "TexManager.h"
#include "CreatureManager.h"
#include "UI.h"
#include "ActionProcess.h"


Character::Character(void)
{
	m_pAnimation = NULL;
	m_mCharTex.clear();
	m_nMapLevel = 0;
	m_nID = 0;
	m_nNum = 0;
	m_iBlock.xpos = 0;
	m_iBlock.ypos = 0;
	m_iBlock.attri = 0;
	m_bCanMove = true;
	m_nMoveAbility = 0;
	m_bFinishAct = false;
	m_nCamp = eCamp_Neutral;
	m_eCharState = eCharacterState_Stand;
	m_eActionStage = eActionStage_WaitStage;
	m_nActionTime = 0;
	m_eAttackRange = (eAttackRange)(g_RandomInt(0,eAttackRange_Arrow));
	m_vDefaultSkillList.clear();
	m_vNewSkillList.clear();
}

Character::~Character(void)
{
	gSafeDelete(m_pAnimation);
	gSafeDelete(m_pItemSpr);
}

void Character::Init(int _Level,int _ID,int _Num,int _Action,Block _block)
{
	if(m_pAnimation)
		gSafeDelete(m_pAnimation);

	m_mCharTex = TexManager::sInstance().GetTex(_ID);
	//��ʼ��ʧ��
	if(m_mCharTex.empty())
		return;

	//����ͼƬ������ĶԳ�ͼ
	m_pAnimation = new hgeAnimation(m_mCharTex[eActionTex_Walk],1,8,0,(GetTexDir((eDirection)(_Action%4+1))-1)*FLOAT_PIC_SQUARE_WIDTH+6*FLOAT_PIC_SQUARE_WIDTH,FLOAT_PIC_SQUARE_WIDTH,FLOAT_PIC_SQUARE_WIDTH);
	m_nMapLevel = _Level;
	m_nID = _ID;
	m_nNum = _Num;
	m_iBlock = _block;
	m_iOrigBlock = _block;
	m_fStartX = m_fXPos = (MAP_RECT-FLOAT_PIC_SQUARE_WIDTH)/2+MAP_OFF_X +MAP_RECT*m_iBlock.xpos;
	m_fStartY = m_fYPos = MAP_OFF_Y+MAP_RECT*m_iBlock.ypos;
	m_nLeftDistance = 0;
	m_eCurDir = (eDirection)(_Action%4+1);
	m_eOrigDir = m_eCurDir;
	m_bHide = false;
	m_eCharState = eCharacterState_Stand;
	//Ϊ��ʼ�����������ڵ�ͼ����������
	Map* theMap = MapManager::sInstance().GetMap(m_nMapLevel);
	theMap->SetBlockOccupied(_block.xpos,_block.ypos);

	//��ʼ����λ����(������+����)
	//���ԣ�ֱ�Ӹ�ֵ
	m_strIntro = "    ���ɣ�ԭ���������´󽫣�����¥֮��Ͷ���ܲ٣���Ϊ�ܲٵ������֣�����𽻺á�����ң��һ�۴�ܶ��⣬����Զ�";
	m_nForce = 50;
	m_nIntelligence = 48;
	m_nCharm = 20;
	m_nHead = 12;
	char name[50];
	sprintf(name,"%d��С��",m_nNum);
	m_strName = name;
	m_strKind = ConfigManager::sInstance().GetCreatureInfo().find(_ID)->second.m_strKind;
	m_eAttackType = eAttackType_Normal;
	m_nAttack = 5;
	m_nDefend = 2;
	m_nSkillDamage = 3;
	m_nSkillDefend = 2;
	m_fCrit = 0.8f;
	m_fDodge = 0.2f;
	m_nHP = 10;
	m_nHPMax = 10;
	m_nMP = 10;
	m_nMPMax = 10;
	m_nLevel = 1;
	m_nExp = 0;
	m_nExpTotal = 100;
	m_nPreHurt = 0;
	m_bDead = false;
	m_bCounter = true;
	m_vBuffData.clear();

	m_vDefaultSkillList = ConfigManager::sInstance().GetCreatureSkill(m_strKind);
	m_nCastSkill = -1;

	m_nUseItem = -1;
	m_pItemSpr = new hgeSprite(0,0,0,32,32);
	m_fItemRiseHeight = 0.0f;
	m_nDrawItem = 0;
	m_fDrawItemX = 0.0f;
	m_fDrawItemY = 0.0;
	m_pItemTarget = NULL;

	m_iAction.m_eAction = eAction_None;

	m_pAnimation->SetMode(HGEANIM_LOOP|HGEANIM_FWD);
	m_pAnimation->Play();
}

eDirection Character::GetTexDir(eDirection dir)
{
	eDirection tempDir = dir;
	if(tempDir == eDirection_Right)
		tempDir = eDirection_Left;

	return tempDir;
}

void Character::Render()
{
	int offx = MapManager::sInstance().GetCurrentMap()->GetOffX()*MAP_RECT;
	int offy = MapManager::sInstance().GetCurrentMap()->GetOffY()*MAP_RECT;
	if(m_pAnimation)
	{
		if(/*m_eCharState == eCharacterState_Defense && m_eAttackState == eAttackState_Defending && */m_eCurDir == eDirection_Right)	//����ԴͼƬȱ�����ҵĶ���������Ҫy��Գƻ���
			m_pAnimation->RenderSymmetry(m_fXPos - offx,m_fYPos - offy,1);
		else
			m_pAnimation->Render(m_fXPos - offx,m_fYPos - offy);
	}

	if (m_nDrawItem && m_pItemSpr)
	{
		m_pItemSpr->Render(m_fDrawItemX + 10 - offx,m_fDrawItemY + m_fItemRiseHeight - 20 - offy);
	}
}

float Character::GetShowX()
{
	Map* theMap = MapManager::sInstance().GetCurrentMap();
	int offx = 0;
	if (theMap)
	{
		offx = theMap->GetOffX();
	}
	return m_fXPos - offx*MAP_RECT;
}

float Character::GetShowY()
{
	Map* theMap = MapManager::sInstance().GetCurrentMap();
	int offy = 0;
	if (theMap)
	{
		offy = theMap->GetOffY();
	}
	return m_fYPos - offy*MAP_RECT;
}

void Character::Update(float delta)
{
	if(m_eCharState == eCharacterState_Stand/* || (m_eCharState==eCharacterState_Fight && m_eAttackState==eAttackState_Waiting)*/)
		return;

	//���鶯��ʱ�����
	if (m_iAction.m_dwTime > 0 && m_iAction.m_eAction!=eAction_Move && m_iAction.m_eAction!=eAction_None)
	{
		int pastTime = (int)(delta*1000);
		if(m_iAction.m_dwTime > pastTime)
			m_iAction.m_dwTime -= pastTime;
		else
		{
			m_iAction.m_dwTime = 0;
			if(m_iAction.m_eAction != eAction_Crit)
				ResetFrame();
		}
	}

	if (m_nDrawItem!=0)
	{
		//��Ʒ�����׶�
		if(m_nDrawItem == 1)
		{
			m_fItemRiseHeight -= delta*80;
			if(m_fItemRiseHeight <=  -40.0f)
			{
				m_fDrawItemX = m_pItemTarget->GetRealX();
				m_fDrawItemY = m_pItemTarget->GetRealY();
				m_nDrawItem = 2;
			}
		}
		//��Ʒ�½��׶�
		else if (m_nDrawItem == 2)
		{
			m_fItemRiseHeight += delta*80;
			if (m_fItemRiseHeight >= 0.0f)
			{
				m_nDrawItem = 0;
				ActionProcess::sInstance().TimeUp(0);
			}
		}
	}

	if(m_nActionTime > 0)
	{
		m_nActionTime -= (int)(delta*1000);
		ActionProcess::sInstance().TimeUp(m_nActionTime);
		if(m_nActionTime <= 0)
		{
			m_nActionTime = 0;
			ActionProcess::sInstance().TimeUp(0);
		}
	}

	if (m_eCharState == eCharacterState_Walk)
	{
		//��ֹ������ͼ�߽�
		if ((m_iBlock.xpos<=0 && m_eCurDir==eDirection_Left)
			|| (m_iBlock.xpos>=g_nMapWidthNum-1 && m_eCurDir==eDirection_Right)
			|| (m_iBlock.ypos<=0 && m_eCurDir==eDirection_Up)
			|| (m_iBlock.ypos>=g_nMapHeightNum-1 && m_eCurDir==eDirection_Down))
		{
			m_eCharState = eCharacterState_Stand;
//			m_bFinishAct = true;
			m_nLeftDistance = 0;
			m_lPathDir.clear();
			return;
		}

		//ǰ������޷�ͨ����ֹͣ
		Map* theMap = MapManager::sInstance().GetCurrentMap();
		if (m_eCurDir==eDirection_Up && !IsCanCross((theMap->GetBlock(m_iBlock.xpos,m_iBlock.ypos-1)->attri))
			|| m_eCurDir==eDirection_Down && !IsCanCross((theMap->GetBlock(m_iBlock.xpos,m_iBlock.ypos+1)->attri))
			||m_eCurDir==eDirection_Left && !IsCanCross((theMap->GetBlock(m_iBlock.xpos-1,m_iBlock.ypos)->attri))
			||m_eCurDir==eDirection_Right && !IsCanCross((theMap->GetBlock(m_iBlock.xpos+1,m_iBlock.ypos)->attri)))
		{
			m_eCharState = eCharacterState_Stand;
//			m_bFinishAct = true;
			m_nLeftDistance = 0;
			m_lPathDir.clear();
			return;
		}

		if(abs(m_fXPos-m_fStartX) >= MAP_RECT || abs(m_fYPos-m_fStartY) >= MAP_RECT)
		{
			m_nLeftDistance -= 1;

			//������ǰ��ͼ������ԣ��Լ��ﵽ���µĵ�ͼ�������
			Block* oldBlock = theMap->GetBlock(m_iBlock.xpos,m_iBlock.ypos);
			if(oldBlock!=NULL)
				setOccupied((oldBlock->attri),0);
			Block* newBlock = NULL;
			//���µ�ͼλ�ã����ҽ���ƫ��
			if (m_eCurDir == eDirection_Right)
			{
				newBlock = theMap->GetBlock(m_iBlock.xpos+1,m_iBlock.ypos);
				m_iBlock = (newBlock==NULL)?m_iBlock:(*newBlock);
				m_fXPos = (MAP_RECT-FLOAT_PIC_SQUARE_WIDTH)/2+MAP_OFF_X +MAP_RECT*m_iBlock.xpos;
			}
			else if(m_eCurDir == eDirection_Left)
			{
				newBlock = theMap->GetBlock(m_iBlock.xpos-1,m_iBlock.ypos);
				m_iBlock = (newBlock==NULL)?m_iBlock:(*newBlock);
				m_fXPos = (MAP_RECT-FLOAT_PIC_SQUARE_WIDTH)/2+MAP_OFF_X +MAP_RECT*m_iBlock.xpos;
			}
			else if(m_eCurDir == eDirection_Up)
			{
				newBlock = theMap->GetBlock(m_iBlock.xpos,m_iBlock.ypos-1);
				m_iBlock = (newBlock==NULL)?m_iBlock:(*newBlock);
				m_fYPos = MAP_OFF_Y+MAP_RECT*m_iBlock.ypos;
			}
			else if(m_eCurDir == eDirection_Down)
			{
				newBlock = theMap->GetBlock(m_iBlock.xpos,m_iBlock.ypos+1);
				m_iBlock = (newBlock==NULL)?m_iBlock:(*newBlock);
				m_fYPos = MAP_OFF_Y+MAP_RECT*m_iBlock.ypos;
			}
			theMap->SetBlockOccupied(m_iBlock.xpos,m_iBlock.ypos);
			m_fStartX = m_fXPos;
			m_fStartY = m_fYPos;

			//������һ������
			if(!m_lPathDir.empty())
			{	
				m_eCurDir = m_lPathDir.front();
				m_lPathDir.pop_front();
				m_pAnimation->ResetFrames(0,2*(GetTexDir(m_eCurDir)-1)*FLOAT_PIC_SQUARE_WIDTH,
					FLOAT_PIC_SQUARE_WIDTH,FLOAT_PIC_SQUARE_WIDTH,2,8,true);
			}

			if(m_nLeftDistance == 0)
			{
				m_eCharState = eCharacterState_Stand;
				//m_bFinishAct = true;
				m_eActionStage = eActionStage_HandleStage;
				m_lPathDir.clear();

				//�ѷ���λ�򿪲�������
				//���ھ��鶯���ƶ�ʱ���ܴ򿪲������
				if (m_nCamp == eCamp_Friend && m_iAction.m_eAction==eAction_None)
				{
					UIWindow* commandWindow = UISystem::sInstance().PopUpWindow(eWindowID_Command);
					if(commandWindow)
						commandWindow->SetBindChar(this);
				}
			}
		}

		if(m_nLeftDistance > 0)
		{
			if(m_eCurDir == eDirection_Up)
				m_fYPos -= delta*120;
			else if(m_eCurDir == eDirection_Down)
				m_fYPos += delta*120;
			else if(m_eCurDir == eDirection_Left)
				m_fXPos -= delta*120;
			else if(m_eCurDir == eDirection_Right)
				m_fXPos += delta*120;
		}
	}
/*	else if (m_eCharState == eCharacterState_Fight)
	{
		if(m_eAttackState == eAttackState_Attacking)
		{
			if (m_pAnimation->GetFrame() == 3)
			{
				m_pAnimation->SetTexture(m_mCharTex[eCharacterState_Walk]);
				m_pAnimation->ResetFrames(0,(m_eCurDir-1)*FLOAT_PIC_SQUARE_HEIGHT,
					FLOAT_PIC_SQUARE_WIDTH,FLOAT_PIC_SQUARE_HEIGHT,4,8,false);
				m_pAnimation->SetMode(HGEANIM_LOOP|HGEANIM_FWD);
			}
		}
		else if (m_eAttackState == eAttackState_Criting)
		{
			if(m_dwRecordTime == 0)
				m_dwRecordTime = GetTickCount();
			DWORD currentTime = GetTickCount();
			//Ϊ������Ӿ�Ч������λ��ɫ�������
			int nTempColor = 255-(int)(255*(currentTime-m_dwRecordTime)/1000);
			DWORD dwCritColor = (nTempColor + (nTempColor << 8)) | 0xFFFF0000;
			m_pAnimation->SetColor(dwCritColor);
			//1�����֪ͨ������
			if(currentTime >= m_dwRecordTime + 1000)
			{
				if (m_pAnimation->GetFrame() == 0)
				{
					m_pAnimation->SetColor(0xFFFFFFFF);
					Attack();
				}
				m_dwRecordTime = 0;
			}
		}
	}
	else if (m_eCharState == eCharacterState_Defense)
	{
		if (m_eAttackState == eAttackState_Ready)
		{
			DWORD currentTime = GetTickCount();
			if(m_dwRecordTime == 0)
				m_dwRecordTime = GetTickCount();
			//0.2�����֪ͨ������
			if(currentTime >= m_dwRecordTime + 200)
			{
				if (m_pAnimation->GetFrame() == 0)
				{
					m_eCharState = eCharacterState_Stand;
					CreatureManager::sInstance().Notify(m_nNum,m_nSrc,eNotify_ReadyToBeAttacked,0);
				}
				m_dwRecordTime = 0;
			}
		}
		else if (m_eAttackState == eAttackState_Defending)
		{
			DWORD currentTime = GetTickCount();
			if(m_dwRecordTime == 0)
				m_dwRecordTime = GetTickCount();
			//0.5�����֪ͨ������
			if(currentTime >= m_dwRecordTime + 500)
			{
				if (m_pAnimation->GetFrame() == 0)
				{
					Character* cast = CreatureManager::sInstance().GetCreature(m_nSrc);
					//�Ƿ񷢶������ҿ��Թ�����Դ
					if(m_bCounter && CanHitTarget(cast))
					{
						Counter();			
					}
					else
					{
						m_pAnimation->SetTexture(m_mCharTex[eCharacterState_Walk]);
						m_pAnimation->ResetFrames(0,(m_eCurDir-1)*FLOAT_PIC_SQUARE_HEIGHT,
							FLOAT_PIC_SQUARE_WIDTH,FLOAT_PIC_SQUARE_HEIGHT,4,8,false);
						m_pAnimation->SetMode(HGEANIM_LOOP|HGEANIM_FWD);
						m_eCharState = eCharacterState_Stand;
						//֪ͨ�����߽����ж�������֪ͨ��Ϊ���ñ������߶������Բ���
						CreatureManager::sInstance().Notify(m_nNum,m_nSrc,eNotify_FinishAttack,0);
					}
				}
				m_dwRecordTime = 0;
			}
		}
		else if (m_eAttackState == eAttackState_Attackeding)
		{
			DWORD currentTime = GetTickCount();
			if(m_dwRecordTime == 0)
				m_dwRecordTime = GetTickCount();
			//0.5�����֪ͨ������
			if(currentTime >= m_dwRecordTime + 500)
			{
				if (m_pAnimation->GetFrame() == 0)
				{
					Character* cast = CreatureManager::sInstance().GetCreature(m_nSrc);
					if(GetHP() <= 0)
					{
						//����
						Dead();
					}
					//�Ƿ񷢶������ҿ��Թ�����Դ
					else if(m_bCounter && CanHitTarget(cast))
					{
						Counter();			
					}
					else
					{
						m_pAnimation->SetTexture(m_mCharTex[eCharacterState_Walk]);
						m_pAnimation->ResetFrames(0,(m_eCurDir-1)*FLOAT_PIC_SQUARE_HEIGHT,
							FLOAT_PIC_SQUARE_WIDTH,FLOAT_PIC_SQUARE_HEIGHT,4,8,false);
						m_pAnimation->SetMode(HGEANIM_LOOP|HGEANIM_FWD);
						m_eCharState = eCharacterState_Stand;
						//֪ͨ�����߽����ж�������֪ͨ��Ϊ���ñ������߶������Բ���
						CreatureManager::sInstance().Notify(m_nNum,m_nSrc,eNotify_FinishAttack,0);
					}
				}
				m_dwRecordTime = 0;
			}
		}
	}
	else if (m_eCharState == eCharacterState_Dead)
	{
		if(m_eAttackState == eAttackState_Dead)
		{
			if (m_pAnimation->GetFrame() == 3)
			{
				//��ɫ������׼���Ƴ�
				m_bDead = true;

				//֪ͨ�����߽����ж�������֪ͨ��Ϊ���ñ������߶������Բ���
				CreatureManager::sInstance().Notify(m_nNum,m_nSrc,eNotify_FinishAttack,0);
			}
		}
	}
*/
	if(m_pAnimation)
	{
// 		char test[50];
// 		if(lastFrame != m_ani->GetFrame())
// 		{
// 			sprintf(test,"%d",m_ani->GetFrame());
// 			OutputDebugString(test);
// 			lastFrame = m_ani->GetFrame();
// 		}
// 		if ((m_ani->GetFrame() >= 15 && m_MoveDir==UP))
// 			m_ani->SetFrame((m_MoveDir-1)*4);
// 		else if(m_ani->GetFrame() >= (m_MoveDir*4))
// 			m_ani->SetFrame((m_MoveDir-1)*4);
		
		m_pAnimation->Update(App::sInstance().GetHGE()->Timer_GetDelta());
	}
}

void Character::ResetFrame()
{
	if(m_nHP < 0.3*(float)m_nHPMax)
	{
		m_pAnimation->SetTexture(m_mCharTex[eActionTex_Dead]);
		m_pAnimation->ResetFrames(0,9*FLOAT_PIC_SQUARE_WIDTH,
			FLOAT_PIC_SQUARE_WIDTH,FLOAT_PIC_SQUARE_WIDTH,2,8,true);
	}
	else
	{
		m_pAnimation->SetTexture(m_mCharTex[eActionTex_Stand]);
		m_pAnimation->ResetFrames(0,(GetTexDir(m_eCurDir)-1)*FLOAT_PIC_SQUARE_WIDTH + 6*FLOAT_PIC_SQUARE_WIDTH,
			FLOAT_PIC_SQUARE_WIDTH,FLOAT_PIC_SQUARE_WIDTH,1,8,true);
	}
	m_pAnimation->SetMode(HGEANIM_LOOP|HGEANIM_FWD);
	m_pAnimation->Resume();
	m_eCharState = eCharacterState_Stand;
}

void Character::SetFinish(bool _finish)
{
	m_bFinishAct = _finish;
	if (m_bFinishAct)
	{
		m_pAnimation->SetColor(0xFF59636C);
		m_iOrigBlock.xpos = m_iBlock.xpos;
		m_iOrigBlock.ypos = m_iBlock.ypos;
		m_eOrigDir = m_eCurDir;
		ResetFrame();

		//�ж�������Ƿ񴥷�����
		//����Ƿ�Ӵ�����
		MapManager::sInstance().GetCurrentMap()->IsTriggerTouch(m_nNum);
		//����Ƿ��ƶ�����
		MapManager::sInstance().GetCurrentMap()->IsTriggerLocation(m_nNum);

		//����Ƿ�ʤ��
		if(MapManager::sInstance().GetCurrentMap()->CheckVictory(eVictoryCondition_GetToPosition,m_nNum))
			MapManager::sInstance().GetCurrentMap()->SetVictory(true);
		if(MapManager::sInstance().GetCurrentMap()->CheckVictory(eVictoryCondition_KillAllEnemies,-1))
			MapManager::sInstance().GetCurrentMap()->SetVictory(true);
	}
}

void Character::CancelMove()
{
	Map* currentMap = MapManager::sInstance().GetCurrentMap();
	//ȡ�����ڸ��ӵ�ռ��״̬
	Block* cannelBlock = currentMap->GetBlock(m_iBlock.xpos,m_iBlock.ypos);
	if(cannelBlock)
		setOccupied(cannelBlock->attri,0);
	m_iBlock.xpos = m_iOrigBlock.xpos;
	m_iBlock.ypos = m_iOrigBlock.ypos;
	m_eCurDir = m_eOrigDir;
	if(m_nHP < 0.3*(float)m_nHPMax)
		m_pAnimation->ResetFrames(0,9*FLOAT_PIC_SQUARE_WIDTH,FLOAT_PIC_SQUARE_WIDTH,FLOAT_PIC_SQUARE_WIDTH,2,8,true);
	else
		m_pAnimation->ResetFrames(0,(GetTexDir(m_eCurDir)-1)*FLOAT_PIC_SQUARE_WIDTH + 6*FLOAT_PIC_SQUARE_WIDTH,
		FLOAT_PIC_SQUARE_WIDTH,FLOAT_PIC_SQUARE_WIDTH,1,8,true);

	//ԭ����ռ��״̬
	cannelBlock = currentMap->GetBlock(m_iBlock.xpos,m_iBlock.ypos);
	if(cannelBlock)
		setOccupied(cannelBlock->attri,1);

	m_fXPos = (MAP_RECT-FLOAT_PIC_SQUARE_WIDTH)/2+MAP_OFF_X +MAP_RECT*m_iBlock.xpos;
	m_fYPos = MAP_OFF_Y+MAP_RECT*m_iBlock.ypos;
	m_fStartX = m_fXPos;
	m_fStartY = m_fYPos;
}

void	Character::SetMoveAbility(int _ability)
{
		m_nMoveAbility = _ability;
// 		if (map)
// 			CreateMoveRange(map);
}

std::vector<Block*> Character::CreateRange(Map* map,int _range,bool bAllBlockInclude,bool bNoMoveAbilityLimit)
{
	std::vector<Block*> range;
	if (map)
	{
		int mapWidth = 0,mapLength = 0;
		map->GetWidthLength(mapWidth,mapLength);
		int offX = 0,offY = 0;
		if(bNoMoveAbilityLimit)
		{
			for (int i=0;i<mapWidth;i++)
			{
				for (int j=0;j<mapLength;j++)
				{
					if(!bAllBlockInclude && (i==m_iBlock.xpos && j==m_iBlock.ypos))
						continue;
					if (!map->GetBlockOccupied(i,j) || bAllBlockInclude)
					{
						range.push_back(map->GetBlock(i,j));
					}
				}
			}
		}
		else
		{
			for (int i=m_iBlock.xpos-_range;i<=m_iBlock.xpos+_range;i++)
			{
				if(i >= 0 && i< mapWidth)
				{
					for (int j=m_iBlock.ypos-_range;j<=m_iBlock.ypos+_range;j++)
					{
						if (j >= 0 && j < mapLength)
						{
							offX = abs(i - m_iBlock.xpos);
							offY = abs(j - m_iBlock.ypos);
							if(offX + offY > _range)
								continue;
							if(!bAllBlockInclude && (i==m_iBlock.xpos && j==m_iBlock.ypos))
								continue;
							if (!map->GetBlockOccupied(i,j) || bAllBlockInclude)
							{
								range.push_back(map->GetBlock(i,j));
							}
						}
					}
				}
			}
		}
	}
	return range;
}

eErrorCode Character::Move(int tarX,int tarY,bool bAllBlockInclude,bool bNoMoveAbilityLimit)
{
	//��λ�����ƶ�
	if (!m_bCanMove) 
	{
		return eErrorCode_CannotMove;
	}

	//��ǰ�ƶ�δ����������Ѱ·
	if(m_eCharState != eCharacterState_Stand)
	{
		return eErrorCode_NotStandState; 
	}

	MapManager::sInstance().GetCurrentMap()->SetSpecificRange(CreateRange(MapManager::sInstance().GetCurrentMap(),m_nMoveAbility,bAllBlockInclude,bNoMoveAbilityLimit));
	vector<Block*> path = MapManager::sInstance().GetCurrentMap()->FindPath(m_iBlock.xpos,m_iBlock.ypos,tarX,tarY);
	if(!path.empty())
	{
		//���ƶ���������
		eDirection dir = eDirection_None;
		Block* current = path.front();
		Block* next = NULL;
		for (vector<Block*>::iterator it=(++path.begin());it!=path.end();it++)
		{
			next = *it;
			if(next != NULL)
			{
				if(next->xpos == current->xpos)
				{
					if(next->ypos == current->ypos+1)
						Move(eDirection_Down);
					else if(next->ypos == current->ypos-1)
						Move(eDirection_Up);
				}
				else if(next->ypos == current->ypos)
				{
					if(next->xpos == current->xpos+1)
						Move(eDirection_Right);
					else if(next->xpos == current->xpos-1)
						Move(eDirection_Left);
				}
			}
			current = next;
		}
	}
	//û���ҵ�·������Ҫ�����ж�
	else
	{
// 		//�Ҳ���·����Ӧ�ý����ж�
// 		//�з�����AI�㷨δ��ȫ�ʿ����Ҳ���·����Ϊ�˲��Է�����ֱ�ӽ����ж�
// 		if(m_nCamp == eCamp_Enemy)
// 			m_bFinishAct = true;
		return eErrorCode_NoPath;
	}
	//���������ƶ��׶�
	m_eActionStage = eActionStage_MovingStage;
	return eErrorCode_Success;
}

//ÿ����һ��move�����ᳯ�Ÿ÷����ƶ�һ����
void Character::Move(eDirection dir)
{
	//��λ�����ƶ�
	if (!m_bCanMove)
	{
		//m_bFinishAct = true;
		return;
	}

	//ֻ����stand ���� walk ʱ�����ƶ�
	if (m_eCharState != eCharacterState_Stand && m_eCharState != eCharacterState_Walk)
	{
		//m_bFinishAct = true;
		return;
	}

	//��Ч����
	if(dir <= eDirection_None || dir > eDirection_End)
	{
		//m_bFinishAct = true;
		return;
	}

	m_lPathDir.push_back(dir);
	m_nLeftDistance++;

	if(m_eCharState == eCharacterState_Stand)
	{
		m_eCurDir = m_lPathDir.front();
		m_pAnimation->SetTexture(m_mCharTex[eCharacterState_Walk]);
		m_pAnimation->ResetFrames(0,2*(GetTexDir(m_eCurDir)-1)*FLOAT_PIC_SQUARE_WIDTH,
			FLOAT_PIC_SQUARE_WIDTH,FLOAT_PIC_SQUARE_WIDTH,2,8,true);
		m_pAnimation->SetMode(HGEANIM_LOOP|HGEANIM_FWD);
		m_lPathDir.pop_front();
		m_eCharState = eCharacterState_Walk;
	}
}

// int Character::TowardToAttacker(int src)
// {
// 	if (m_eCharState == eCharacterState_Stand)
// 	{
// 		m_nSrc = src;
// 		m_eCharState = eCharacterState_Defense;
// 		m_eAttackState = eAttackState_Ready;
// 		Block& block = CreatureManager::sInstance().GetCreature(src)->GetBlock();
// 		if (block.xpos < m_iBlock.xpos)
// 			m_eCurDir = eDirection_Left;
// 		else if(block.xpos > m_iBlock.xpos)
// 			m_eCurDir = eDirection_Right;
// 		else if(block.ypos < m_iBlock.ypos)
// 			m_eCurDir = eDirection_Up;
// 		else
// 			m_eCurDir = eDirection_Down;
// 		m_pAnimation->ResetFrames(0,(m_eCurDir-1)*FLOAT_PIC_SQUARE_HEIGHT,
// 			FLOAT_PIC_SQUARE_WIDTH,FLOAT_PIC_SQUARE_HEIGHT,1,8,false);
// 		m_pAnimation->SetMode(HGEANIM_LOOP|HGEANIM_FWD);
// 
// 		return eNotify_Success;
// 	}
// 
// 	return eNotify_CannotBeAttacked;
// }

void Character::TowardToAttacker(eNotification notify,Character* target,int time)
{
	if (m_eCharState == eCharacterState_Stand)
	{
		m_eNotify = notify;
		m_nActionTime = time;
//		m_dwRecordTime = time;
//		if(time != 0)
//			m_nSrc = target->GetNum();

		m_eCharState = eCharacterState_Fight;
//		m_eAttackState = eAttackState_Ready;
		Block& block = target->GetBlock();
		if (block.xpos < m_iBlock.xpos)
			m_eCurDir = eDirection_Left;
		else if(block.xpos > m_iBlock.xpos)
			m_eCurDir = eDirection_Right;
		else if(block.ypos < m_iBlock.ypos)
			m_eCurDir = eDirection_Up;
		else
			m_eCurDir = eDirection_Down;
		m_pAnimation->ResetFrames(0,(GetTexDir(m_eCurDir)-1)*FLOAT_PIC_SQUARE_WIDTH + 6*FLOAT_PIC_SQUARE_WIDTH,
			FLOAT_PIC_SQUARE_WIDTH,FLOAT_PIC_SQUARE_WIDTH,1,8,true);
		m_pAnimation->SetMode(HGEANIM_LOOP|HGEANIM_FWD);
	}
}

// void Character::Attack()
// {
// 	//�����ߺͱ������ߵ������ֵ���ݸ����������㹥�����
// 	CreatureManager::sInstance().CalculateResult(m_nNum,m_nTar);
// 
// 	//���Ź�������
// 	m_pAnimation->SetTexture(m_mCharTex[eCharacterState_Fight]);
// 	m_pAnimation->ResetFrames(0,(m_eCurDir-1)*FLOAT_PIC_SQUARE_HEIGHT,
// 		FLOAT_PIC_SQUARE_HEIGHT,FLOAT_PIC_SQUARE_HEIGHT,4,8,false);
// 	m_pAnimation->SetMode(HGEANIM_FWD | HGEANIM_NOLOOP);
// 
// 	//���ù�����״̬
// 	m_eAttackState = eAttackState_Attacking;
// }

void Character::Attack(eNotification notify,Character* target,int time)
{
	//���Ź�������
	m_pAnimation->SetTexture(m_mCharTex[eActionTex_Attack]);
	m_pAnimation->ResetFrames(0,4*(GetTexDir(m_eCurDir)-1)*FLOAT_PIC_SQUARE_HEIGHT,
		FLOAT_PIC_SQUARE_HEIGHT,FLOAT_PIC_SQUARE_HEIGHT,4,8,true);
	m_pAnimation->SetMode(HGEANIM_FWD | HGEANIM_NOLOOP);

	//���ù�����״̬
//	m_eAttackState = eAttackState_Attacking;
	m_eCharState = eCharacterState_Fight;

	m_nActionTime = time;
	m_eNotify = notify;
}

// void Character::Attacked()
// {
// 	//���ű���������
// 	m_pAnimation->SetTexture(m_mCharTex[eCharacterState_Defense]);
// 	m_pAnimation->ResetFrames(0,3*FLOAT_PIC_SQUARE_WIDTH,
// 		FLOAT_PIC_SQUARE_WIDTH,FLOAT_PIC_SQUARE_WIDTH,1,8,false);
// 	m_pAnimation->SetMode(HGEANIM_FWD | HGEANIM_LOOP);
// 
// 
// 	//���ñ�������״̬
// 	m_eCharState = eCharacterState_Defense;
// 	m_eAttackState = eAttackState_Attackeding;
// }

void Character::Attacked(eNotification notify,Character* target,int time)
{
	//���ű���������
	m_pAnimation->SetTexture(m_mCharTex[eActionTex_Attacked]);
	m_pAnimation->ResetFrames(0,3*FLOAT_PIC_SQUARE_WIDTH,
		FLOAT_PIC_SQUARE_WIDTH,FLOAT_PIC_SQUARE_WIDTH,1,8,true);
	m_pAnimation->SetMode(HGEANIM_FWD | HGEANIM_LOOP);


	//���ñ�������״̬
	m_eCharState = eCharacterState_Attacked;
//	m_eAttackState = eAttackState_Attackeding;

	m_nActionTime = time;
	m_eNotify = notify;
}

// void Character::Defend()
// {
// 	//���ű���������
// 	int offset = 0;
// 	if(m_eCurDir == eDirection_Down)
// 		offset = 0;
// 	else if (m_eCurDir == eDirection_Up)
// 		offset = 1;
// 	else if(m_eCurDir == eDirection_Left || m_eCurDir == eDirection_Right)
// 		offset = 2;
// 	m_pAnimation->SetTexture(m_mCharTex[eCharacterState_Defense]);
// 	m_pAnimation->ResetFrames(0,offset*FLOAT_PIC_SQUARE_WIDTH,
// 		FLOAT_PIC_SQUARE_WIDTH,FLOAT_PIC_SQUARE_WIDTH,1,8,false);
// 	m_pAnimation->SetMode(HGEANIM_FWD | HGEANIM_LOOP);
// 
// 
// 	//���ñ�������״̬
// 	m_eCharState = eCharacterState_Defense;
// 	m_eAttackState = eAttackState_Defending;
// }

void Character::Defend(eNotification notify,Character* target,int time)
{
	//���ű���������
	int offset = 0;
	m_pAnimation->SetTexture(m_mCharTex[eActionTex_Defend]);
	m_pAnimation->ResetFrames(0,(GetTexDir(m_eCurDir)-1)*FLOAT_PIC_SQUARE_WIDTH,
		FLOAT_PIC_SQUARE_WIDTH,FLOAT_PIC_SQUARE_WIDTH,1,8,true);
	m_pAnimation->SetMode(HGEANIM_FWD | HGEANIM_LOOP);


	//���ñ�������״̬
	m_eCharState = eCharacterState_Defense;
//	m_eAttackState = eAttackState_Defending;

	m_nActionTime = time;
	m_eNotify = notify;
}

// void Character::Counter()
// {
// 
// }

// void Character::Crit()
// {
// 	//���Ź�������
// 	m_pAnimation->SetTexture(m_mCharTex[eCharacterState_Fight]);
// 	m_pAnimation->ResetFrames(0,(m_eCurDir-1)*FLOAT_PIC_SQUARE_HEIGHT,
// 		FLOAT_PIC_SQUARE_HEIGHT,FLOAT_PIC_SQUARE_HEIGHT,1,8,false);
// 	m_pAnimation->SetMode(HGEANIM_FWD | HGEANIM_LOOP);
// 
// 	//���ù�����״̬
// 	m_eAttackState = eAttackState_Criting;
// }

void Character::Crit(eNotification notify,Character* target,int time)
{
	//���Ź�������
	m_pAnimation->SetTexture(m_mCharTex[eActionTex_Attack]);
	m_pAnimation->ResetFrames(0,4*(GetTexDir(m_eCurDir)-1)*FLOAT_PIC_SQUARE_HEIGHT,
		FLOAT_PIC_SQUARE_HEIGHT,FLOAT_PIC_SQUARE_HEIGHT,1,8,true);
	m_pAnimation->SetMode(HGEANIM_FWD | HGEANIM_LOOP);

	//���ù�����״̬
//	m_eAttackState = eAttackState_Criting;
	m_eCharState = eCharacterState_Fight;

	m_nActionTime = time;
	m_eNotify = notify;
}

// void Character::Dead()
// {
// 	//���Ź�������
// 	m_pAnimation->SetTexture(m_mCharTex[eCharacterState_Dead]);
// 	m_pAnimation->ResetFrames(0,(m_eCurDir-1)*FLOAT_PIC_SQUARE_WIDTH,
// 		FLOAT_PIC_SQUARE_WIDTH,FLOAT_PIC_SQUARE_WIDTH,4,8,false);
// 	m_pAnimation->SetMode(HGEANIM_FWD | HGEANIM_NOLOOP);
// 
// 	//���ù�����״̬
// 	m_eAttackState = eAttackState_Dead;
// 	m_eCharState = eCharacterState_Dead;
// }

void Character::Dead(eNotification notify,Character* target,int time)
{
	//���Ź�������
	m_pAnimation->SetTexture(m_mCharTex[eActionTex_Dead]);
	m_pAnimation->ResetFrames(0,9*FLOAT_PIC_SQUARE_WIDTH,
		FLOAT_PIC_SQUARE_WIDTH,FLOAT_PIC_SQUARE_WIDTH,2,8,true);
	m_pAnimation->SetMode(HGEANIM_FWD | HGEANIM_LOOP);

	//���ù�����״̬
//	m_eAttackState = eAttackState_Dead;
	m_eCharState = eCharacterState_Dead;

	m_nActionTime = time;
	m_eNotify = notify;
}

void Character::Healed(eNotification notify,Character* target,int time)
{
	//���Ź�������
	m_pAnimation->SetTexture(m_mCharTex[eActionTex_LevelUp]);
	m_pAnimation->ResetFrames(0,4*FLOAT_PIC_SQUARE_WIDTH,
		FLOAT_PIC_SQUARE_WIDTH,FLOAT_PIC_SQUARE_WIDTH,1,8,true);
	m_pAnimation->SetMode(HGEANIM_FWD | HGEANIM_LOOP);

	m_eCharState = eCharacterState_Healed;

	m_nActionTime = time;
	m_eNotify = notify;
}

void Character::UseItem(eNotification notify,Character* target,int time)
{
	//��ƷͼƬ���ͷ�������Ʈ������ʹ��������
	if(m_nUseItem == -1)
		return;
	Item item = ConfigManager::sInstance().GetItemInfo().find(m_nUseItem)->second;
	m_pItemSpr->SetTexture(TexManager::sInstance().GetItemTex(item.m_nID));
	m_nDrawItem = 1;
	m_fDrawItemX = m_fXPos;
	m_fDrawItemY = m_fYPos;
	m_pItemTarget = target;

	m_eCharState = eCharacterState_UseItem;

	m_nActionTime = time;
	m_eNotify = notify;
}

bool Character::CanHitTarget(Character* target)
{
	if(!target)
		return false;

	int tarX = 0,tarY = 0;
	VPair vPairPoint = CreatureManager::sInstance().GetRangePoint();
	for (MAttackRange::iterator mit=CreatureManager::sInstance().GetAttackRange().begin();mit!=CreatureManager::sInstance().GetAttackRange().end();mit++)
	{
		if(mit->first == m_eAttackRange)
		{
			for (vector<int>::iterator it=mit->second.begin();it!=mit->second.end();it++)
			{
				tarX = vPairPoint[*it].x + m_iBlock.xpos;
				tarY = vPairPoint[*it].y + m_iBlock.ypos;
				if(target->GetBlock().xpos == tarX && target->GetBlock().ypos == tarY)
				{
					return true;
				}
			}
		}
	}
	return false;
}

bool Character::CanSkillHitTarget(Character* target)
{
	if(!target)
		return false;

	Block tarBlock = target->GetBlock();
	SkillInfo skill = ConfigManager::sInstance().GetSkillInfo().find(m_nCastSkill)->second;
	if(skill.m_nSkillType == eSkillType_Hurt && target->GetCamp() != eCamp_Enemy)
		return false;
	if((skill.m_nSkillType == eSkillType_Heal || skill.m_nSkillType == eSkillType_Buff) && target->GetCamp() != eCamp_Friend)
		return false;
	if(abs(tarBlock.xpos-m_iBlock.xpos) + abs(tarBlock.ypos-m_iBlock.ypos) <= skill.m_nCastRange)
		return true;
	return false;
}

std::vector<int>	Character::GetSkillList()
{
	std::vector<int> vSkillList;
	vSkillList.insert(vSkillList.begin(),m_vDefaultSkillList.begin(),m_vDefaultSkillList.end());
	vSkillList.insert(vSkillList.end(),m_vNewSkillList.begin(),m_vNewSkillList.end());
	return vSkillList;
}


/*�����������ͣ���������ֵ
type : ����
1 : ����HP�����Լ���ǰHPֵ
2������MP�����Լ���ǰMPֵ
3�����ӹ�����
4�����ӷ�����
5�����ӱ�������
6���������ܸ���
*/
void Character::SetAttributeValue(int type,int value)
{
	switch(type)
	{
	case 1:
		{
			m_nHPMax += value;
			m_nHP += value;
		}
		break;
	case 2:
		{
			m_nMPMax += value;
			m_nMP += value;
		}
		break;
	case 3:
		m_nAttack += value;
		break;
	case 4:
		m_nDefend += value;
		break;
	case 5:
		m_fCrit += ((float)value)/1000;
		break;
	case 6:
		m_fDodge += ((float)value)/1000;
		break;
	}
}

void Character::RemoveBuff()
{
	for (std::vector<BuffData>::iterator it=m_vBuffData.begin();it!=m_vBuffData.end();)
	{
		//����ʱ���1
		it->m_nLastTurns--;
		//�Ƴ�ʱ��Ϊ0��buff
		if (it->m_nLastTurns <= 0)
		{
			switch(it->m_nType)
			{
			case 1:
				m_nHPMax -= it->m_nValue;
				break;
			case 2:
				m_nMPMax -= it->m_nValue;
				break;
			case 3:
				m_nAttack -= it->m_nValue;
				break;
			case 4:
				m_nDefend -= it->m_nValue;
				break;
			case 5:
				m_fCrit -= ((float)it->m_nValue)/1000;
				break;
			case 6:
				m_fDodge -= ((float)it->m_nValue)/1000;
				break;
			default:
				break;
			}
			it = m_vBuffData.erase(it);
		}
		else
		{
			it++;
		}
	}
}

bool	Character::CanUseItem(Character* target)
{
	return true;
}

void	Character::ItemEffect(Item item,bool bAdd)
{
	if (item.m_nType == 1)	//��ʹ����Ʒ
	{
		for (std::map<int,int>::iterator it=item.m_mEffect.begin();it!=item.m_mEffect.end();it++)
		{
			switch(it->first)
			{
			case 1:	//HP�ָ�
				{
					m_nHP += it->second;
					if(m_nHP >= m_nHPMax)
						m_nHP = m_nHPMax;
				}
				break;
			case 2:	//MP�ָ�
				{
					m_nMP += it->second;
					if(m_nMP >= m_nMPMax)
						m_nMP = m_nMPMax;
				}
				break;
			case 3:	//�������üӳ�
				{
					m_nAttack += it->second;
				}
				break;
			case 4:	//�������üӳ�
				m_nDefend += it->second;
				break;
			case 5:	//�������üӳ�
				m_fCrit += it->second;
				break;
			case 6:	//�������üӳ�
				m_fDodge += it->second;
				break;
			case 7:	//������Χ���üӳ�
				{
					switch(m_eAttackRange)
					{
					case eAttackRange_Cross:
						m_eAttackRange = eAttackRange_CrossEx;
						break;
					case eAttackRange_Box:
						m_eAttackRange = eAttackRange_BoxEx;
						break;
					case eAttackRange_BigCross:
						m_eAttackRange = eAttackRange_BigCrossEx;
						break;
					case eAttackRange_Arrow:
						m_eAttackRange = eAttackRange_ArrowEx;
						break;
					}
					//������Χ�޷��ӳ�
				}
				break;
			case 8:	//�ƶ������üӳ�
				m_nMoveAbility += it->second;
				break;
			}
		}
	}
	else if (item.m_nType >= 2 && item.m_nType <= 4)		//װ��
	{
		for (std::map<int,int>::iterator it=item.m_mEffect.begin();it!=item.m_mEffect.end();it++)
		{
			int value = bAdd?it->second:(-1*it->second);
			switch(it->first)
			{
			case 3:
				m_nAttack += value;
				if(m_nAttack <= 0)
					m_nAttack = 0;
				break;
			case 4:
				m_nDefend += value;
				if(m_nDefend <= 0)
					m_nDefend = 0;
				break;
			case 5:	//�������üӳ�
				m_fCrit += value;
				if(m_fCrit <= 0)
					m_fCrit = 0;
				break;
			case 6:	//�������üӳ�
				m_fDodge += value;
				if(m_fDodge <= 0)
					m_fDodge = 0;
				break;
			case 7:	//������Χ���üӳ�
				{
					//��bug����������eAttackRange_CrossEx��װ���˼ӳɷ�Χ��װ����û��ʵ�ʼӳ�
					//Ȼ��ж��װ����ȴ�Ὣ�ҵķ�Χ���eAttackRange_Cross,��Ҫ���һ��
					//�ӳ����Ա���������ֱ�ӽ��������ڵ�λ����������
					switch(m_eAttackRange)
					{
					case eAttackRange_Cross:
						if(bAdd)
							m_eAttackRange = eAttackRange_CrossEx;
						break;
					case eAttackRange_Box:
						if(bAdd)
							m_eAttackRange = eAttackRange_BoxEx;
						break;
					case eAttackRange_BigCross:
						if(bAdd)
							m_eAttackRange = eAttackRange_BigCrossEx;
						break;
					case eAttackRange_Arrow:
						if(bAdd)
							m_eAttackRange = eAttackRange_ArrowEx;
						break;
					case eAttackRange_CrossEx:
						if(!bAdd)
							m_eAttackRange = eAttackRange_Cross;
						break;
					case eAttackRange_BoxEx:
						if(!bAdd)
							m_eAttackRange = eAttackRange_Box;
						break;
					case eAttackRange_BigCrossEx:
						if(!bAdd)
							m_eAttackRange = eAttackRange_BigCross;
						break;
					case eAttackRange_ArrowEx:
						if(!bAdd)
							m_eAttackRange = eAttackRange_Arrow;
						break;
					}
					//������Χ�޷��ӳ�
				}
				break;
			case 8:	//�ƶ������üӳ�
				m_nMoveAbility += value;
				if(m_nMoveAbility <= 1)
					m_nMoveAbility = 1;
				break;
			}
		}
	}
}

void Character::Talk(int head,const char* word)
{
	CreatureManager::sInstance().AddAction(eAction_Talk,m_nNum,0,eDirection_None,head,m_strName.c_str(),word);
}

void Character::Wait(DWORD time)
{
	CreatureManager::sInstance().AddAction(eAction_Wait,m_nNum,time);
}

void Character::Turn(int dir,DWORD time)
{
	CreatureManager::sInstance().AddAction(eAction_Turn,m_nNum,time,(eDirection)dir);
}

void Character::Attack()
{
	CreatureManager::sInstance().AddAction(eAction_Attack,m_nNum,500);
}

void Character::Crit()
{
	//Ĭ�ϱ�����������1��
	CreatureManager::sInstance().AddAction(eAction_Crit,m_nNum,1000);
	CreatureManager::sInstance().AddAction(eAction_Attack,m_nNum,500);
}

void Character::Attacked(DWORD time)
{
	CreatureManager::sInstance().AddAction(eAction_Attacked,m_nNum,time);
}

void Character::Defend(DWORD time)
{
	CreatureManager::sInstance().AddAction(eAction_Defend,m_nNum,time);
}

void Character::Hurt(DWORD time)
{
	CreatureManager::sInstance().AddAction(eAction_Hurt,m_nNum,time);
}

void Character::Step(DWORD time)
{
	CreatureManager::sInstance().AddAction(eAction_Step,m_nNum,time);
}

void Character::MoveTo(int x,int y)
{
	DWORD data = x + (y << 8);
	CreatureManager::sInstance().AddAction(eAction_Move,m_nNum,0,eDirection_None,data);
}

void Character::Disappear()
{
	CreatureManager::sInstance().AddAction(eAction_Disappear,m_nNum,1000);
}

void Character::Appear(int dir,int x,int y)
{
	DWORD data = x + (y << 8);
	CreatureManager::sInstance().AddAction(eAction_Appears,m_nNum,1000,(eDirection)dir,data);
}

void Character::PushAction(NewAction action)
{
	//ִ�о��鶯������ɫ��Ϊ����
	if(action.m_eAction != eAction_None)
		ChangeColor(0xFFFFFFFF);
	//��ɫ�ָ�Ϊ�ж���״̬
	else
		ChangeColor(0xFF59636C);

	m_iAction = action;
	switch(m_iAction.m_eAction)
	{
	case eAction_Appears:
		{
			m_bHide = false;
			ChangeColor(0x00FFFFFF);
			Map* theMap = MapManager::sInstance().GetCurrentMap();
			int x = m_iAction.m_dwData & 0x00FF;
			int y = m_iAction.m_dwData >> 8;
			Block* oldBlock = theMap->GetBlock(x,y);
			if(oldBlock!=NULL)
				setOccupied((oldBlock->attri),1);
			m_iBlock = m_iOrigBlock = *oldBlock;
			m_eCurDir = m_eOrigDir = m_iAction.m_eDir;
			m_fXPos = (MAP_RECT-FLOAT_PIC_SQUARE_WIDTH)/2+MAP_OFF_X +MAP_RECT*m_iBlock.xpos;
			m_fYPos = MAP_OFF_Y+MAP_RECT*m_iBlock.ypos;
			m_fStartX = m_fXPos;
			m_fStartY = m_fYPos;
			m_eCharState = eCharacterState_Appear;
		}
		break;
	case eAction_Disappear:
		{
			m_eCharState = eCharacterState_Disappear;
		}
		break;
	case eAction_Wait:
		m_eCharState = eCharacterState_Wait;
		break;
	case eAction_Turn:
		m_eCurDir = m_iAction.m_eDir;
		m_eOrigDir = m_eCurDir;
		m_pAnimation->SetTexture(m_mCharTex[eActionTex_Stand]);
		m_pAnimation->ResetFrames(0,(GetTexDir(m_eCurDir)-1)*FLOAT_PIC_SQUARE_WIDTH + 6*FLOAT_PIC_SQUARE_WIDTH,
			FLOAT_PIC_SQUARE_WIDTH,FLOAT_PIC_SQUARE_WIDTH,1,8,true);
		m_pAnimation->SetMode(HGEANIM_FWD | HGEANIM_LOOP);
		m_eCharState = eCharacterState_Turn;
		break;
	case eAction_Attack:
		m_pAnimation->SetTexture(m_mCharTex[eActionTex_Attack]);
		m_pAnimation->ResetFrames(0,4*(GetTexDir(m_eCurDir)-1)*FLOAT_PIC_SQUARE_HEIGHT,
			FLOAT_PIC_SQUARE_HEIGHT,FLOAT_PIC_SQUARE_HEIGHT,4,8,true);
		m_pAnimation->SetMode(HGEANIM_FWD | HGEANIM_NOLOOP);
		m_eCharState = eCharacterState_Attack;
		break;
	case eAction_Crit:
		m_pAnimation->SetTexture(m_mCharTex[eActionTex_Attack]);
		m_pAnimation->ResetFrames(0,4*(GetTexDir(m_eCurDir)-1)*FLOAT_PIC_SQUARE_HEIGHT,
			FLOAT_PIC_SQUARE_HEIGHT,FLOAT_PIC_SQUARE_HEIGHT,1,8,true);
		m_pAnimation->SetMode(HGEANIM_FWD | HGEANIM_LOOP);
		m_eCharState = eCharacterState_Crit;
		break;
	case eAction_Attacked:
		m_pAnimation->SetTexture(m_mCharTex[eActionTex_Attacked]);
		m_pAnimation->ResetFrames(0,3*FLOAT_PIC_SQUARE_WIDTH,
			FLOAT_PIC_SQUARE_WIDTH,FLOAT_PIC_SQUARE_WIDTH,1,8,true);
		m_pAnimation->SetMode(HGEANIM_FWD | HGEANIM_LOOP);
		m_eCharState = eCharacterState_Attacked;
		break;
	case eAction_Defend:
		m_pAnimation->SetTexture(m_mCharTex[eActionTex_Defend]);
		m_pAnimation->ResetFrames(0,(GetTexDir(m_eCurDir)-1)*FLOAT_PIC_SQUARE_WIDTH,
			FLOAT_PIC_SQUARE_WIDTH,FLOAT_PIC_SQUARE_WIDTH,1,8,true);
		m_pAnimation->SetMode(HGEANIM_FWD | HGEANIM_LOOP);
		m_eCharState = eCharacterState_Defense;
		break;
	case eAction_Hurt:
		m_pAnimation->SetTexture(m_mCharTex[eActionTex_Dead]);
		m_pAnimation->ResetFrames(0,9*FLOAT_PIC_SQUARE_WIDTH,
			FLOAT_PIC_SQUARE_WIDTH,FLOAT_PIC_SQUARE_WIDTH,2,6,true);
		m_pAnimation->SetMode(HGEANIM_FWD | HGEANIM_LOOP);
		m_eCharState = eCharacterState_Hurt;
		break;
	case eAction_Step:
		m_pAnimation->SetTexture(m_mCharTex[eCharacterState_Walk]);
		m_pAnimation->ResetFrames(0,2*(GetTexDir(m_eCurDir)-1)*FLOAT_PIC_SQUARE_WIDTH,
			FLOAT_PIC_SQUARE_WIDTH,FLOAT_PIC_SQUARE_WIDTH,2,8,true);
		m_pAnimation->SetMode(HGEANIM_LOOP|HGEANIM_FWD);
		m_eCharState = eCharacterState_Step;
		break;
	case eAction_Move:
		{
			int x = m_iAction.m_dwData & 0x00FF;
			int y = m_iAction.m_dwData >> 8;
			if(Move(x,y,true,true) == eErrorCode_Success)
			{
				Map* theMap = MapManager::sInstance().GetCurrentMap();
				Block* oldBlock = theMap->GetBlock(x,y);
				m_iOrigBlock = *oldBlock;
			}
		}
		break;
	}
}

bool Character::IsInAction()
{
	if(m_iAction.m_eAction == eAction_None)
		return false;
	//���ƶ��ද���ж�ʣ��ʱ��
	if (m_iAction.m_dwTime == 0 && m_iAction.m_eAction!=eAction_Move)
	{
		return false;
	}
	//�ƶ��ද���ж��Ƿ���Ŀ�ĵ�
	if (m_iAction.m_eAction==eAction_Move)
	{
		int x = m_iAction.m_dwData & 0x00FF;
		int y = m_iAction.m_dwData >> 8;
		if (m_iBlock.xpos == x && m_iBlock.ypos == y)
		{
			return false;
		}
	}
	return true;
}

void Character::AddEquip(int grid,int equipID)
{
	if(grid >= eEquipGrid_TotalEquip)
		return;

	//�Ѿ���װ����
	if(m_iEquip[grid].m_nID != -1)
		return;

	std::map<int,Item>::iterator it = ConfigManager::sInstance().GetItemInfo().find(equipID);
	if(it!=ConfigManager::sInstance().GetItemInfo().end())
	{
		m_iEquip[grid] = it->second;

		//���Լӳ�
		ItemEffect(it->second,true);
	}
}

void Character::RemoveEquip(int grid)
{
	if(grid >= eEquipGrid_TotalEquip)
		return;

	//�Ƴ�����
	if (m_iEquip[grid].m_nID != -1)
	{
		ItemEffect(m_iEquip[grid],false);
	}

	m_iEquip[grid].m_nID = -1;
	m_iEquip[grid].m_mEffect.clear();
	m_iEquip[grid].m_nType = -1;
	m_iEquip[grid].m_strName = "";
}

Item Character::GetEquip(int grid)
{
	Item temp;
	if(grid >= eEquipGrid_TotalEquip)
		return temp;

	return m_iEquip[grid];
}