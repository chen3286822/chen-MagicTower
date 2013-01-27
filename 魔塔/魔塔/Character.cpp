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
	m_ani->SetMode(HGEANIM_LOOP|HGEANIM_FWD);
	m_ani->Play();
}

void Character::render()
{
	if(m_ani)
		m_ani->Render((MAP_RECT-FLOAT_PIC_SQUARE_WIDTH)/2+MAP_OFF_X +MAP_RECT*m_block.xpos,MAP_OFF_Y+MAP_RECT*m_block.ypos);
}

void Character::update()
{
	if(m_ani)
		m_ani->Update(App::sInstance().getHGE()->Timer_GetDelta());

//	if(m_bCanMove)

}