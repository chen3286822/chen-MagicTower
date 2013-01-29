#include "Character.h"
#include "App.h"


Character::Character(void)
{
	m_ani = NULL;
	m_ID = 0;
	m_Num = 0;
	m_block.xpos = 0;
	m_block.ypos = 0;
	m_bCanMove = true;
	m_MoveAbility = 0;
}

Character::~Character(void)
{
	gSafeDelete(m_ani);
}

void Character::init(HTEXTURE tex,int _ID,int _Num,Block _block)
{
	if(m_ani)
		gSafeDelete(m_ani);
	m_ani = new hgeAnimation(tex,4,8,0,0,FLOAT_PIC_SQUARE_WIDTH,FLOAT_PIC_SQUARE_HEIGHT);
	m_ID = _ID;
	m_Num = _Num;
	m_block.xpos = _block.xpos;
	m_block.ypos = _block.ypos;
	m_xpos = (MAP_RECT-FLOAT_PIC_SQUARE_WIDTH)/2+MAP_OFF_X +MAP_RECT*m_block.xpos;
	m_ypos = MAP_OFF_Y+MAP_RECT*m_block.ypos;
	m_LeftDistance = 0;
	m_MoveDir = None;
	m_ani->SetMode(HGEANIM_LOOP|HGEANIM_FWD);
	m_ani->Play();
}

void Character::render()
{
	if(m_ani)
		m_ani->Render(m_xpos,m_ypos);
}

void Character::update(float delta)
{
	static float startX = m_xpos;
	static float startY = m_ypos;
	static int lastFrame = m_ani->GetFrame();

	if(m_bCanMove)
	{
		if(m_MoveDir == None)
			return;
		
		if(abs(m_xpos-startX) >= MAP_RECT || abs(m_ypos-startY) >= MAP_RECT)
		{
			m_LeftDistance -= 1;
			startX = m_xpos;
			startY = m_ypos;
			if(m_LeftDistance == 0)
				m_MoveDir = None;
		}

		if(m_LeftDistance > 0)
		{
			if(m_MoveDir == UP)
				m_ypos -= delta*30;
			else if(m_MoveDir == DOWN)
				m_ypos += delta*30;
			else if(m_MoveDir == LEFT)
				m_xpos -= delta*30;
			else if(m_MoveDir == RIGHT)
				m_xpos += delta*30;
		}
	}

	if(m_ani)
	{
		char test[50];
		if(lastFrame != m_ani->GetFrame())
		{
			sprintf(test,"%d",m_ani->GetFrame());
			OutputDebugString(test);
			lastFrame = m_ani->GetFrame();
		}
// 		if ((m_ani->GetFrame() >= 15 && m_MoveDir==UP))
// 			m_ani->SetFrame((m_MoveDir-1)*4);
// 		else if(m_ani->GetFrame() >= (m_MoveDir*4))
// 			m_ani->SetFrame((m_MoveDir-1)*4);
		
		m_ani->Update(App::sInstance().getHGE()->Timer_GetDelta());
	}
}

//每调用一次move，将会朝着该方向移动一格子
void Character::move(Direction dir)
{
	if(dir == None)
		return;

	if(dir != m_MoveDir)	//改变移动方向
	{
		m_MoveDir = dir;
		m_LeftDistance = 1;
		
//		m_ani->Stop();
//		m_ani->SetFrame((m_MoveDir-1)*4);
//		m_ani->SetTextureRect((m_MoveDir-1)*FLOAT_PIC_SQUARE_WIDTH,(m_MoveDir-1)*FLOAT_PIC_SQUARE_HEIGHT,FLOAT_PIC_SQUARE_WIDTH,FLOAT_PIC_SQUARE_HEIGHT);
//		m_ani->Play();
		m_ani->ResetFrames((m_MoveDir-1)*FLOAT_PIC_SQUARE_WIDTH,(m_MoveDir-1)*FLOAT_PIC_SQUARE_HEIGHT,
			FLOAT_PIC_SQUARE_WIDTH,FLOAT_PIC_SQUARE_HEIGHT,4,8,false);
		return;
	}

	m_LeftDistance++;
}