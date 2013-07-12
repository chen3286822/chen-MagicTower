#include "Character.h"
#include "App.h"
#include "MapManager.h"
#include "TexManager.h"
#include "CreatureManager.h"
#include "UI.h"
#include "ActionProcess.h"
#include "ConfigManager.h"


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
	m_eCharState = eCharacterState_Stand;
	//Ϊ��ʼ�����������ڵ�ͼ����������
	Map* theMap = MapManager::sInstance().GetMap(m_nMapLevel);
	theMap->SetBlockOccupied(_block.xpos,_block.ypos);

	//��ʼ����λ����(������+����)
	//���ԣ�ֱ�Ӹ�ֵ
	char name[50];
	sprintf(name,"%d��С��",m_nNum);
	m_strName = name;
	m_strKind = ConfigManager::sInstance().GetCreatureInfo().find(_ID)->second.m_strKind;
	m_eAttackType = eAttackType_Normal;
	m_nAttack = 5;
	m_nDefend = 2;
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

	m_vDefaultSkillList = ConfigManager::sInstance().GetCreatureSkill(m_strKind);
	m_nCastSkill = -1;

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
	if(m_pAnimation)
	{
		DWORD color = 0xFFFFFFFF;
		if(m_bFinishAct)
			color = 0xFF59636C;
		m_pAnimation->SetColor(color);
		if(/*m_eCharState == eCharacterState_Defense && m_eAttackState == eAttackState_Defending && */m_eCurDir == eDirection_Right)	//����ԴͼƬȱ�����ҵĶ���������Ҫy��Գƻ���
			m_pAnimation->RenderSymmetry(m_fXPos,m_fYPos,1);
		else
			m_pAnimation->Render(m_fXPos,m_fYPos);
	}
}

void Character::Update(float delta)
{
	if(m_eCharState == eCharacterState_Stand/* || (m_eCharState==eCharacterState_Fight && m_eAttackState==eAttackState_Waiting)*/)
		return;

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
			|| (m_iBlock.xpos>=MAP_WIDTH_NUM-1 && m_eCurDir==eDirection_Right)
			|| (m_iBlock.ypos<=0 && m_eCurDir==eDirection_Up)
			|| (m_iBlock.ypos>=MAP_LENGTH_NUM-1 && m_eCurDir==eDirection_Down))
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
				if (m_nCamp == eCamp_Friend)
				{
					UIWindow* commandWindow = UISystem::sInstance().GetWindow(eWindowID_Command);
					if(commandWindow)
					{
						commandWindow->SetShow(true);
						commandWindow->SetBindChar(this);
					}
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
		m_iOrigBlock.xpos = m_iBlock.xpos;
		m_iOrigBlock.ypos = m_iBlock.ypos;
		m_eOrigDir = m_eCurDir;
		ResetFrame();
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

void	Character::SetMoveAbility(int _ability,Map* map)
{
		m_nMoveAbility = _ability;
// 		if (map)
// 			CreateMoveRange(map);
}

std::vector<Block*> Character::CreateRange(Map* map,int _range,bool bAllBlockInclude)
{
	std::vector<Block*> range;
	if (map)
	{
		int mapWidth = 0,mapLength = 0;
		map->GetWidthLength(mapWidth,mapLength);
		int offX = 0,offY = 0;
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
						if((offX + offY > _range) || (i==m_iBlock.xpos && j==m_iBlock.ypos))
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
	return range;
}

eErrorCode Character::Move(int tarX,int tarY)
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

	MapManager::sInstance().GetCurrentMap()->SetSpecificRange(CreateRange(MapManager::sInstance().GetCurrentMap(),m_nMoveAbility));
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