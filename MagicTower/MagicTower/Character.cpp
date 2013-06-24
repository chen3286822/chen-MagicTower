#include "Character.h"
#include "App.h"
#include "MapManager.h"
#include "TexManager.h"
#include "CreatureManager.h"


Character::Character(void)
{
	m_pAnimation = NULL;
	m_mCharTex.clear();
	m_nLevel = 0;
	m_nID = 0;
	m_nNum = 0;
	m_iBlock.xpos = 0;
	m_iBlock.ypos = 0;
	m_iBlock.attri = 0;
	m_bCanMove = true;
	m_nMoveAbility = 0;
	m_bFinishAct = false;
	m_nCamp = Neutral;
	m_eCharState = Stand;
	m_nTar = 0;
	m_dwRecordTime = 0;
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

	m_pAnimation = new hgeAnimation(m_mCharTex[Walk],4,8,0,FLOAT_PIC_SQUARE_HEIGHT*(_Action-1),FLOAT_PIC_SQUARE_WIDTH,FLOAT_PIC_SQUARE_HEIGHT);
	m_nLevel = _Level;
	m_nID = _ID;
	m_nNum = _Num;
	m_iBlock = _block;
	m_fStartX = m_fXPos = (MAP_RECT-FLOAT_PIC_SQUARE_WIDTH)/2+MAP_OFF_X +MAP_RECT*m_iBlock.xpos;
	m_fStartY = m_fYPos = MAP_OFF_Y+MAP_RECT*m_iBlock.ypos;
	m_nLeftDistance = 0;
	m_eCurDir = (Direction)(_Action%4);
	m_eCharState = Stand;
	//Ϊ��ʼ�����������ڵ�ͼ����������
	Map* theMap = MapManager::sInstance().GetMap(m_nLevel);
	theMap->SetBlockOccupied(_block.xpos,_block.ypos);

	m_pAnimation->SetMode(HGEANIM_LOOP|HGEANIM_FWD);
	m_pAnimation->Play();
}

void Character::Render()
{
	if(m_pAnimation)
	{
		if(m_eCharState == Defend && m_eAttackState == Attackeding && m_eCurDir == RIGHT)	//����ԴͼƬȱ�����ҵĶ���������Ҫy��Գƻ���
			m_pAnimation->RenderSymmetry(m_fXPos,m_fYPos,1);
		else
			m_pAnimation->Render(m_fXPos,m_fYPos);
	}
}

void Character::Update(float delta)
{
	if(m_eCharState == Stand || (m_eCharState==Fight && m_eAttackState==Waiting))
		return;

	if (m_eCharState == Walk)
	{
		//��ֹ������ͼ�߽�
		if ((m_iBlock.xpos<=0 && m_eCurDir==LEFT)
			|| (m_iBlock.xpos>=MAP_WIDTH_NUM-1 && m_eCurDir==RIGHT)
			|| (m_iBlock.ypos<=0 && m_eCurDir==UP)
			|| (m_iBlock.ypos>=MAP_LENGTH_NUM-1 && m_eCurDir==DOWN))
		{
			m_eCharState = Stand;
			m_bFinishAct = true;
			m_nLeftDistance = 0;
			m_lPathDir.clear();
			return;
		}

		//ǰ������޷�ͨ����ֹͣ
		Map* theMap = MapManager::sInstance().GetCurrentMap();
		if (m_eCurDir==UP && !IsCanCross((theMap->GetBlock(m_iBlock.xpos,m_iBlock.ypos-1)->attri))
			|| m_eCurDir==DOWN && !IsCanCross((theMap->GetBlock(m_iBlock.xpos,m_iBlock.ypos+1)->attri))
			||m_eCurDir==LEFT && !IsCanCross((theMap->GetBlock(m_iBlock.xpos-1,m_iBlock.ypos)->attri))
			||m_eCurDir==RIGHT && !IsCanCross((theMap->GetBlock(m_iBlock.xpos+1,m_iBlock.ypos)->attri)))
		{
			m_eCharState = Stand;
			m_bFinishAct = true;
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
			if (m_eCurDir == RIGHT)
			{
				newBlock = theMap->GetBlock(m_iBlock.xpos+1,m_iBlock.ypos);
				m_iBlock = (newBlock==NULL)?m_iBlock:(*newBlock);
				m_fXPos = (MAP_RECT-FLOAT_PIC_SQUARE_WIDTH)/2+MAP_OFF_X +MAP_RECT*m_iBlock.xpos;
			}
			else if(m_eCurDir == LEFT)
			{
				newBlock = theMap->GetBlock(m_iBlock.xpos-1,m_iBlock.ypos);
				m_iBlock = (newBlock==NULL)?m_iBlock:(*newBlock);
				m_fXPos = (MAP_RECT-FLOAT_PIC_SQUARE_WIDTH)/2+MAP_OFF_X +MAP_RECT*m_iBlock.xpos;
			}
			else if(m_eCurDir == UP)
			{
				newBlock = theMap->GetBlock(m_iBlock.xpos,m_iBlock.ypos-1);
				m_iBlock = (newBlock==NULL)?m_iBlock:(*newBlock);
				m_fYPos = MAP_OFF_Y+MAP_RECT*m_iBlock.ypos;
			}
			else if(m_eCurDir == DOWN)
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
				m_pAnimation->ResetFrames(0,(m_eCurDir-1)*FLOAT_PIC_SQUARE_HEIGHT,
					FLOAT_PIC_SQUARE_WIDTH,FLOAT_PIC_SQUARE_HEIGHT,4,8,false);
			}

			if(m_nLeftDistance == 0)
			{
				m_eCharState = Stand;
				m_bFinishAct = true;
				m_lPathDir.clear();
			}
		}

		if(m_nLeftDistance > 0)
		{
			if(m_eCurDir == UP)
				m_fYPos -= delta*60;
			else if(m_eCurDir == DOWN)
				m_fYPos += delta*60;
			else if(m_eCurDir == LEFT)
				m_fXPos -= delta*60;
			else if(m_eCurDir == RIGHT)
				m_fXPos += delta*60;
		}
	}
	else if (m_eCharState == Fight)
	{
		if(m_eAttackState == Attacking)
		{
			if (m_pAnimation->GetFrame() == 3)
			{
				m_pAnimation->SetTexture(m_mCharTex[Walk]);
				m_pAnimation->ResetFrames(0,(m_eCurDir-1)*FLOAT_PIC_SQUARE_HEIGHT,
					FLOAT_PIC_SQUARE_WIDTH,FLOAT_PIC_SQUARE_HEIGHT,4,8,false);
				m_pAnimation->SetMode(HGEANIM_LOOP|HGEANIM_FWD);
				m_eCharState = Stand;
			}
		}
	}
	else if (m_eCharState == Defend)
	{
		if (m_eAttackState == Ready)
		{
			DWORD currentTime = GetTickCount();
			if(m_dwRecordTime == 0)
				m_dwRecordTime = GetTickCount();
			//0.1�����֪ͨ������
			if(currentTime >= m_dwRecordTime + 100)
			{
				if (m_pAnimation->GetFrame() == 0)
				{
					m_eCharState = Stand;
					CreatureManager::sInstance().Notify(m_nNum,m_nSrc,Notify_ReadyToBeAttacked,0);
				}
				m_dwRecordTime = 0;
			}
		}
		else if (m_eAttackState == Attackeding)
		{
			DWORD currentTime = GetTickCount();
			if(m_dwRecordTime == 0)
				m_dwRecordTime = GetTickCount();
			//0.1�����֪ͨ������
			if(currentTime >= m_dwRecordTime + 1000)
			{
				if (m_pAnimation->GetFrame() == 0)
				{
					m_pAnimation->SetTexture(m_mCharTex[Walk]);
					m_pAnimation->ResetFrames(0,(m_eCurDir-1)*FLOAT_PIC_SQUARE_HEIGHT,
						FLOAT_PIC_SQUARE_WIDTH,FLOAT_PIC_SQUARE_HEIGHT,4,8,false);
					m_pAnimation->SetMode(HGEANIM_LOOP|HGEANIM_FWD);
					m_eCharState = Stand;
				}
				m_dwRecordTime = 0;
			}
		}
	}

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

void Character::Move(int tarX,int tarY)
{
	//��λ�����ƶ�
	if (!m_bCanMove) 
	{
		return;
	}

	//��ǰ�ƶ�δ����������Ѱ·
	if(m_eCharState != Stand)
	{
		return; 
	}

	vector<Block*> path = MapManager::sInstance().GetCurrentMap()->FindPath(m_iBlock.xpos,m_iBlock.ypos,tarX,tarY);
	if(!path.empty())
	{
		//���ƶ���������
		Direction dir = None;
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
						Move(DOWN);
					else if(next->ypos == current->ypos-1)
						Move(UP);
				}
				else if(next->ypos == current->ypos)
				{
					if(next->xpos == current->xpos+1)
						Move(RIGHT);
					else if(next->xpos == current->xpos-1)
						Move(LEFT);
				}
			}
			current = next;
		}
	}
	//û���ҵ�·������Ҫ�����ж�
	else
	{
		m_bFinishAct = true;
	}
}

//ÿ����һ��move�����ᳯ�Ÿ÷����ƶ�һ����
void Character::Move(Direction dir)
{
	//��λ�����ƶ�
	if (!m_bCanMove)
	{
		m_bFinishAct = true;
		return;
	}

	//ֻ����stand ���� walk ʱ�����ƶ�
	if (m_eCharState != Stand && m_eCharState != Walk)
	{
		m_bFinishAct = true;
		return;
	}

	//��Ч����
	if(dir <= None || dir > UP)
	{
		m_bFinishAct = true;
		return;
	}

	m_lPathDir.push_back(dir);
	m_nLeftDistance++;

	if(m_eCharState == Stand)
	{
		m_eCurDir = m_lPathDir.front();
		m_pAnimation->ResetFrames(0,(m_eCurDir-1)*FLOAT_PIC_SQUARE_HEIGHT,
			FLOAT_PIC_SQUARE_WIDTH,FLOAT_PIC_SQUARE_HEIGHT,4,8,false);
		m_pAnimation->SetMode(HGEANIM_LOOP|HGEANIM_FWD);
		m_lPathDir.pop_front();
		m_eCharState = Walk;
	}
}

int Character::TowardToAttacker(int src,int dir)
{
	if (m_eCharState == Stand)
	{
		m_nSrc = src;
		m_eCharState = Defend;
		m_eAttackState = Ready;
		switch(dir)
		{
		case UP:
			m_eCurDir = DOWN;
			break;
		case DOWN:
			m_eCurDir = UP;
			break;
		case LEFT:
			m_eCurDir = RIGHT;
			break;
		case RIGHT:
			m_eCurDir = LEFT;
			break;
		}
		m_pAnimation->ResetFrames(0,(m_eCurDir-1)*FLOAT_PIC_SQUARE_HEIGHT,
			FLOAT_PIC_SQUARE_WIDTH,FLOAT_PIC_SQUARE_HEIGHT,1,8,false);
		m_pAnimation->SetMode(HGEANIM_LOOP|HGEANIM_FWD);

		return Notify_Success;
	}

	return Notify_CannotBeAttacked;
}

void Character::Attack()
{
	//�����ߺͱ������ߵ������ֵ���ݸ����������㹥�����
	CreatureManager::sInstance().CalculateResult(m_nNum,m_nTar);

	//���Ź�������
	m_pAnimation->SetTexture(m_mCharTex[Fight]);
	m_pAnimation->ResetFrames(0,(m_eCurDir-1)*FLOAT_PIC_SQUARE_HEIGHT,
		FLOAT_PIC_SQUARE_HEIGHT,FLOAT_PIC_SQUARE_HEIGHT,4,8,false);
	m_pAnimation->SetMode(HGEANIM_FWD | HGEANIM_NOLOOP);

	//���ù�����״̬
	m_eAttackState = Attacking;
}

void Character::Attacked()
{
	//���ű���������
	int offset = 0;
	if(m_eCurDir == DOWN)
		offset = 0;
	else if (m_eCurDir == UP)
		offset = 1;
	else if(m_eCurDir == LEFT || m_eCurDir == RIGHT)
		offset = 2;
	m_pAnimation->SetTexture(m_mCharTex[Defend]);
	m_pAnimation->ResetFrames(0,offset*FLOAT_PIC_SQUARE_WIDTH,
		FLOAT_PIC_SQUARE_WIDTH,FLOAT_PIC_SQUARE_WIDTH,1,8,false);
	m_pAnimation->SetMode(HGEANIM_FWD | HGEANIM_NOLOOP);


	//���ñ�������״̬
	m_eCharState = Defend;
	m_eAttackState = Attackeding;
}

void Character::Defending()
{

}

void Character::GeginHit()
{
	if(m_eCharState == Stand)
	{
		//֪ͨĿ������Լ�
		if(CreatureManager::sInstance().Notify(m_nNum,m_nTar,Notify_TowardToAttacker,m_eCurDir) == Notify_Success)
		{
			m_eCharState = Fight;
			m_eAttackState = Waiting;
		}
	}
}