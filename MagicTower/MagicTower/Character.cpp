#include "Character.h"
#include "App.h"
#include "MapManager.h"


Character::Character(void)
{
	m_pAnimation = NULL;
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
}

Character::~Character(void)
{
	gSafeDelete(m_pAnimation);
}

void Character::Init(HTEXTURE tex,int _Level,int _ID,int _Num,int _Action,Block _block)
{
	if(m_pAnimation)
		gSafeDelete(m_pAnimation);
	m_pAnimation = new hgeAnimation(tex,4,8,0,FLOAT_PIC_SQUARE_HEIGHT*(_Action-1),FLOAT_PIC_SQUARE_WIDTH,FLOAT_PIC_SQUARE_HEIGHT);
	m_nLevel = _Level;
	m_nID = _ID;
	m_nNum = _Num;
	m_iBlock = _block;
	m_fStartX = m_fXPos = (MAP_RECT-FLOAT_PIC_SQUARE_WIDTH)/2+MAP_OFF_X +MAP_RECT*m_iBlock.xpos;
	m_fStartY = m_fYPos = MAP_OFF_Y+MAP_RECT*m_iBlock.ypos;
	m_nLeftDistance = 0;
	m_eMoveDir = None;
	//为初始化的人物所在地图块设置属性
	Map* theMap = MapManager::sInstance().GetMap(m_nLevel);
	theMap->SetBlockOccupied(_block.xpos,_block.ypos);

	m_pAnimation->SetMode(HGEANIM_LOOP|HGEANIM_FWD);
	m_pAnimation->Play();
}

void Character::Render()
{
	if(m_pAnimation)
		m_pAnimation->Render(m_fXPos,m_fYPos);
}

void Character::Update(float delta)
{
	if(m_bCanMove)
	{
		if(m_eMoveDir == None)
			return;

		//禁止超过地图边界
		if ((m_iBlock.xpos<=0 && m_eMoveDir==LEFT)
			|| (m_iBlock.xpos>=MAP_WIDTH_NUM-1 && m_eMoveDir==RIGHT)
			|| (m_iBlock.ypos<=0 && m_eMoveDir==UP)
			|| (m_iBlock.ypos>=MAP_LENGTH_NUM-1 && m_eMoveDir==DOWN))
		{
			m_eMoveDir = None;
			m_bFinishAct = true;
			m_nLeftDistance = 0;
			return;
		}

		//前方如果无法通过则停止
		Map* theMap = MapManager::sInstance().GetCurrentMap();
		if (m_eMoveDir==UP && !IsCanCross((theMap->GetBlock(m_iBlock.xpos,m_iBlock.ypos-1)->attri))
			|| m_eMoveDir==DOWN && !IsCanCross((theMap->GetBlock(m_iBlock.xpos,m_iBlock.ypos+1)->attri))
			||m_eMoveDir==LEFT && !IsCanCross((theMap->GetBlock(m_iBlock.xpos-1,m_iBlock.ypos)->attri))
			||m_eMoveDir==RIGHT && !IsCanCross((theMap->GetBlock(m_iBlock.xpos+1,m_iBlock.ypos)->attri)))
		{
			m_eMoveDir = None;
			m_bFinishAct = true;
			m_nLeftDistance = 0;
			return;
		}
		
		if(abs(m_fXPos-m_fStartX) >= MAP_RECT || abs(m_fYPos-m_fStartY) >= MAP_RECT)
		{
			m_nLeftDistance -= 1;
			
			//更新以前地图块的属性，以及达到的新的地图块的属性
			Block* oldBlock = theMap->GetBlock(m_iBlock.xpos,m_iBlock.ypos);
			if(oldBlock!=NULL)
				setOccupied((oldBlock->attri),0);
			Block* newBlock = NULL;
			//更新地图位置，并且矫正偏移
			if (m_eMoveDir == RIGHT)
			{
				newBlock = theMap->GetBlock(m_iBlock.xpos+1,m_iBlock.ypos);
				m_iBlock = (newBlock==NULL)?m_iBlock:(*newBlock);
				m_fXPos = (MAP_RECT-FLOAT_PIC_SQUARE_WIDTH)/2+MAP_OFF_X +MAP_RECT*m_iBlock.xpos;
			}
			else if(m_eMoveDir == LEFT)
			{
				newBlock = theMap->GetBlock(m_iBlock.xpos-1,m_iBlock.ypos);
				m_iBlock = (newBlock==NULL)?m_iBlock:(*newBlock);
				m_fXPos = (MAP_RECT-FLOAT_PIC_SQUARE_WIDTH)/2+MAP_OFF_X +MAP_RECT*m_iBlock.xpos;
			}
			else if(m_eMoveDir == UP)
			{
				newBlock = theMap->GetBlock(m_iBlock.xpos,m_iBlock.ypos-1);
				m_iBlock = (newBlock==NULL)?m_iBlock:(*newBlock);
				m_fYPos = MAP_OFF_Y+MAP_RECT*m_iBlock.ypos;
			}
			else if(m_eMoveDir == DOWN)
			{
				newBlock = theMap->GetBlock(m_iBlock.xpos,m_iBlock.ypos+1);
				m_iBlock = (newBlock==NULL)?m_iBlock:(*newBlock);
				m_fYPos = MAP_OFF_Y+MAP_RECT*m_iBlock.ypos;
			}
			theMap->SetBlockOccupied(m_iBlock.xpos,m_iBlock.ypos);
			m_fStartX = m_fXPos;
			m_fStartY = m_fYPos;
			if(m_nLeftDistance == 0)
			{
				m_eMoveDir = None;
				m_bFinishAct = true;
			}
		}

		if(m_nLeftDistance > 0)
		{
			if(m_eMoveDir == UP)
				m_fYPos -= delta*60;
			else if(m_eMoveDir == DOWN)
				m_fYPos += delta*60;
			else if(m_eMoveDir == LEFT)
				m_fXPos -= delta*60;
			else if(m_eMoveDir == RIGHT)
				m_fXPos += delta*60;
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

//每调用一次move，将会朝着该方向移动一格子
void Character::Move(Direction dir)
{
	if(dir == None)
	{
		m_bFinishAct = true;
		return;
	}
	//改变移动方向
	if(dir != m_eMoveDir && m_eMoveDir==None)	
	{
		m_eMoveDir = dir;
		m_nLeftDistance = 1;
		
//		m_ani->Stop();
//		m_ani->SetFrame((m_MoveDir-1)*4);
//		m_ani->SetTextureRect((m_MoveDir-1)*FLOAT_PIC_SQUARE_WIDTH,(m_MoveDir-1)*FLOAT_PIC_SQUARE_HEIGHT,FLOAT_PIC_SQUARE_WIDTH,FLOAT_PIC_SQUARE_HEIGHT);
//		m_ani->Play();
//(m_MoveDir-1)*FLOAT_PIC_SQUARE_WIDTH
		m_pAnimation->ResetFrames(0,(m_eMoveDir-1)*FLOAT_PIC_SQUARE_HEIGHT,
			FLOAT_PIC_SQUARE_WIDTH,FLOAT_PIC_SQUARE_HEIGHT,4,8,false);
		return;
	}
	else if(dir == m_eMoveDir)
		m_nLeftDistance++;
}