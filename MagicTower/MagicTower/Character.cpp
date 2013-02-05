#include "Character.h"
#include "App.h"
#include "MapManager.h"


Character::Character(void)
{
	m_ani = NULL;
	m_Level = 0;
	m_ID = 0;
	m_Num = 0;
	m_block.xpos = 0;
	m_block.ypos = 0;
	m_block.attri = 0;
	m_bCanMove = true;
	m_MoveAbility = 0;
	m_bFinishAct = false;
}

Character::~Character(void)
{
	gSafeDelete(m_ani);
}

void Character::Init(HTEXTURE tex,int _Level,int _ID,int _Num,int _Action,Block _block)
{
	if(m_ani)
		gSafeDelete(m_ani);
	m_ani = new hgeAnimation(tex,4,8,0,FLOAT_PIC_SQUARE_HEIGHT*(_Action-1),FLOAT_PIC_SQUARE_WIDTH,FLOAT_PIC_SQUARE_HEIGHT);
	m_Level = _Level;
	m_ID = _ID;
	m_Num = _Num;
	m_block = _block;
	m_StartX = m_xpos = (MAP_RECT-FLOAT_PIC_SQUARE_WIDTH)/2+MAP_OFF_X +MAP_RECT*m_block.xpos;
	m_StartY = m_ypos = MAP_OFF_Y+MAP_RECT*m_block.ypos;
	m_LeftDistance = 0;
	m_MoveDir = None;
	//为初始化的人物所在地图块设置属性
	Map* theMap = MapManager::sInstance().GetMap(m_Level);
	theMap->SetBlockOccupied(_block.xpos,_block.ypos);

	m_ani->SetMode(HGEANIM_LOOP|HGEANIM_FWD);
	m_ani->Play();
}

void Character::Render()
{
	if(m_ani)
		m_ani->Render(m_xpos,m_ypos);
}

void Character::Update(float delta)
{
	if(m_bCanMove)
	{
		if(m_MoveDir == None)
			return;

		//禁止超过地图边界
		if ((m_block.xpos<=0 && m_MoveDir==LEFT)
			|| (m_block.xpos>=MAP_WIDTH_NUM-1 && m_MoveDir==RIGHT)
			|| (m_block.ypos<=0 && m_MoveDir==UP)
			|| (m_block.ypos>=MAP_LENGTH_NUM-1 && m_MoveDir==DOWN))
		{
			m_MoveDir = None;
			m_bFinishAct = true;
			m_LeftDistance = 0;
			return;
		}

		//前方如果无法通过则停止
		Map* theMap = MapManager::sInstance().GetCurrentMap();
		if (m_MoveDir==UP && !IsCanCross((theMap->GetBlock(m_block.xpos,m_block.ypos-1)->attri))
			|| m_MoveDir==DOWN && !IsCanCross((theMap->GetBlock(m_block.xpos,m_block.ypos+1)->attri))
			||m_MoveDir==LEFT && !IsCanCross((theMap->GetBlock(m_block.xpos-1,m_block.ypos)->attri))
			||m_MoveDir==RIGHT && !IsCanCross((theMap->GetBlock(m_block.xpos+1,m_block.ypos)->attri)))
		{
			m_MoveDir = None;
			m_bFinishAct = true;
			m_LeftDistance = 0;
			return;
		}
		
		if(abs(m_xpos-m_StartX) >= MAP_RECT || abs(m_ypos-m_StartY) >= MAP_RECT)
		{
			m_LeftDistance -= 1;
			
			//更新以前地图块的属性，以及达到的新的地图块的属性
			Block* oldBlock = theMap->GetBlock(m_block.xpos,m_block.ypos);
			if(oldBlock!=NULL)
				setOccupied((oldBlock->attri),0);
			Block* newBlock = NULL;
			//更新地图位置，并且矫正偏移
			if (m_MoveDir == RIGHT)
			{
				newBlock = theMap->GetBlock(m_block.xpos+1,m_block.ypos);
				m_block = (newBlock==NULL)?m_block:(*newBlock);
				m_xpos = (MAP_RECT-FLOAT_PIC_SQUARE_WIDTH)/2+MAP_OFF_X +MAP_RECT*m_block.xpos;
			}
			else if(m_MoveDir == LEFT)
			{
				newBlock = theMap->GetBlock(m_block.xpos-1,m_block.ypos);
				m_block = (newBlock==NULL)?m_block:(*newBlock);
				m_xpos = (MAP_RECT-FLOAT_PIC_SQUARE_WIDTH)/2+MAP_OFF_X +MAP_RECT*m_block.xpos;
			}
			else if(m_MoveDir == UP)
			{
				newBlock = theMap->GetBlock(m_block.xpos,m_block.ypos-1);
				m_block = (newBlock==NULL)?m_block:(*newBlock);
				m_ypos = MAP_OFF_Y+MAP_RECT*m_block.ypos;
			}
			else if(m_MoveDir == DOWN)
			{
				newBlock = theMap->GetBlock(m_block.xpos,m_block.ypos+1);
				m_block = (newBlock==NULL)?m_block:(*newBlock);
				m_ypos = MAP_OFF_Y+MAP_RECT*m_block.ypos;
			}
			theMap->SetBlockOccupied(m_block.xpos,m_block.ypos);
			m_StartX = m_xpos;
			m_StartY = m_ypos;
			if(m_LeftDistance == 0)
			{
				m_MoveDir = None;
				m_bFinishAct = true;
			}
		}

		if(m_LeftDistance > 0)
		{
			if(m_MoveDir == UP)
				m_ypos -= delta*60;
			else if(m_MoveDir == DOWN)
				m_ypos += delta*60;
			else if(m_MoveDir == LEFT)
				m_xpos -= delta*60;
			else if(m_MoveDir == RIGHT)
				m_xpos += delta*60;
		}
	}

	if(m_ani)
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
		
		m_ani->Update(App::sInstance().GetHGE()->Timer_GetDelta());
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
	if(dir != m_MoveDir && m_MoveDir==None)	
	{
		m_MoveDir = dir;
		m_LeftDistance = 1;
		
//		m_ani->Stop();
//		m_ani->SetFrame((m_MoveDir-1)*4);
//		m_ani->SetTextureRect((m_MoveDir-1)*FLOAT_PIC_SQUARE_WIDTH,(m_MoveDir-1)*FLOAT_PIC_SQUARE_HEIGHT,FLOAT_PIC_SQUARE_WIDTH,FLOAT_PIC_SQUARE_HEIGHT);
//		m_ani->Play();
//(m_MoveDir-1)*FLOAT_PIC_SQUARE_WIDTH
		m_ani->ResetFrames(0,(m_MoveDir-1)*FLOAT_PIC_SQUARE_HEIGHT,
			FLOAT_PIC_SQUARE_WIDTH,FLOAT_PIC_SQUARE_HEIGHT,4,8,false);
		return;
	}
	else if(dir == m_MoveDir)
		m_LeftDistance++;
}