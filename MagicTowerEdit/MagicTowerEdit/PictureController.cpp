#include "PictureController.h"
#include "MagicTower\MagicTower\commonTools.h"
#include "Application.h"


PictureController::PictureController(void)
{
	id = ID_CTRL_PIC_1;
	bStatic = false;
	bVisible = true;
	 bEnabled = true;
	 m_hge = 0;
	 m_PicNum = 0;
	 m_EachLineNum = 3;
	 m_LineNum = 2;
	 m_mTexNum.clear();
	 rect.Set(0,0,100,100);
}

PictureController::PictureController(HGE* _hge,float _left,float _top,float _right,float _bottom,int _picNum)
{
	id = ID_CTRL_PIC_1;
	bStatic = false;
	bVisible = true;
	bEnabled = true;
	m_hge = _hge;
	m_PicNum = _picNum;
	m_EachLineNum = 3;
	m_LineNum = 2;
	m_mTexNum.clear();
	rect.Set(_left,_top,_right,_bottom);
}

PictureController::~PictureController(void)
{
	int sprNum = m_EachLineNum*m_LineNum;
	for (int i=0;i<sprNum;i++)
	{
		delete m_TexSprList[i];
		m_TexSprList[i] = NULL;
	}
	delete[] m_TexList;
	m_TexList = NULL;
}

void PictureController::init()
{
	m_width = rect.x2 - rect.x1;
	m_height = rect.y2 - rect.y1;
	m_SmallSquareWidth = FLOAT_PIC_SQUARE_WIDTH;
	m_SmallSquareHeight = FLOAT_PIC_SQUARE_HEIGHT;
	m_FullWidth = FLOAT_PIC_WIDTH;
	m_FullHeight = FLOAT_PIC_HEIGHT;
	m_FullTexStartY = FLOAT_PIC_SQUARE_HEIGHT * 3;
	m_TexList = new HTEXTURE[m_PicNum];
	int sprNum = m_EachLineNum*m_LineNum;
	m_TexSprList = new hgeSprite*[sprNum];
	for (int i=0;i<sprNum;i++)
	{
		m_TexSprList[i] = new hgeSprite(0,0,0,0,0);
	}
	m_FullShowTex = new hgeSprite(0,0,0,0,0);
	loadTexList();

	for (int i=0;i<sprNum;i++)
	{
		m_TexSprList[i]->SetTexture(m_TexList[i]);
		m_TexSprList[i]->SetTextureRect(0,
			0,
			m_SmallSquareWidth,
			m_SmallSquareHeight);
	}
	m_WhichTexture = sprNum;
}

void PictureController::Update(float dt)
{
	int sprNum = m_EachLineNum*m_LineNum;
	if(Application::sInstance().getNext())
	{
		if((m_WhichTexture+sprNum) < m_PicNum)
		{		
			m_WhichTexture += sprNum;
			for (int i=0;i<sprNum;i++)
			{
				m_TexSprList[i]->SetTexture(m_TexList[i+m_WhichTexture]);
			}			
		}
		else
		{
			for (int i=0;i<(m_PicNum-m_WhichTexture);i++)
			{
				m_TexSprList[i]->SetTexture(m_TexList[i+m_WhichTexture]);
			}		
		}
		Application::sInstance().setNext(false);
	}
	if(Application::sInstance().getLast())
	{	
		if((m_WhichTexture-sprNum) >= 0)
			m_WhichTexture -= sprNum;
		for (int i=0;i<sprNum;i++)
		{
			m_TexSprList[i]->SetTexture(m_TexList[i+m_WhichTexture]);
		}			
		Application::sInstance().setLast(false);
	}
	float xpos,ypos;
	float xCtrl,yCtrl;
	m_hge->Input_GetMousePos(&xpos,&ypos);
	xCtrl = xpos - rect.x1;
	yCtrl = ypos - rect.y1;
	for (int i=0;i<m_LineNum;i++)
	{
		for (int j=0;j<m_EachLineNum;j++)
		{
			if(xCtrl >= (float)j/(float)m_EachLineNum*m_width &&
				xCtrl < (float)(j+1)/(float)m_EachLineNum*m_width &&
				yCtrl >= (float)i/(float)m_LineNum*m_SmallSquareHeight*m_LineNum &&
				yCtrl < (float)(i+1)/(float)m_LineNum*m_SmallSquareHeight*m_LineNum)
			{
				m_Lines.topX = (float)j/(float)m_EachLineNum*m_width+rect.x1;
				m_Lines.topY = (float)i/(float)m_LineNum*m_SmallSquareHeight*m_LineNum+rect.y1;
				m_Lines.rightX = (float)(j+1)/(float)m_EachLineNum*m_width+rect.x1;
				m_Lines.rightY = m_Lines.topY;
				m_Lines.leftX = m_Lines.topX;
				m_Lines.leftY = (float)(i+1)/(float)m_LineNum*m_SmallSquareHeight*m_LineNum+rect.y1;
				m_Lines.bottomX = m_Lines.rightX;
				m_Lines.bottomY = m_Lines.leftY;
				return;
			}
		}
	}
	static int xLineNum = (int)(m_FullWidth/m_SmallSquareWidth);
	static int yLineNum = (int)(m_FullHeight/m_SmallSquareHeight);
	for (int p=0;p<yLineNum;p++)
	{
		for (int q=0;q<xLineNum;q++)
		{
			if (xCtrl >= (float)q/(float)xLineNum*m_FullWidth &&
				xCtrl < (float)(q+1)/(float)xLineNum*m_FullWidth &&
				yCtrl >= (float)p/(float)yLineNum*m_FullHeight+m_FullTexStartY &&
				yCtrl < (float)(p+1)/(float)yLineNum*m_FullHeight+m_FullTexStartY)
			{
				m_Lines.topX = (float)q/(float)xLineNum*m_FullWidth+rect.x1;
				m_Lines.topY = (float)p/(float)xLineNum*m_FullHeight+m_FullTexStartY+rect.y1;
				m_Lines.leftX = m_Lines.topX;
				m_Lines.leftY = (float)(p+1)/(float)xLineNum*m_FullHeight+m_FullTexStartY+rect.y1;
				m_Lines.rightX = (float)(q+1)/(float)xLineNum*m_FullWidth+rect.x1;
				m_Lines.rightY = m_Lines.topY;
				m_Lines.bottomX = m_Lines.rightX;
				m_Lines.bottomY = m_Lines.leftY;
				return;
			}
		}
	}
}

void PictureController::Render()
{
	for (int i=0;i<m_LineNum;i++)
	{
		for (int j=0;j<m_EachLineNum;j++)
		{
			m_TexSprList[i*m_EachLineNum+j]->Render(rect.x1+(float)j/(float)m_EachLineNum*m_width,
				rect.y1+(float)i/(float)m_LineNum*m_SmallSquareHeight*m_LineNum);
		}
	}
	m_FullShowTex->Render(rect.x1,rect.y1+m_FullTexStartY);
	m_hge->Gfx_RenderLine(m_Lines.topX,m_Lines.topY,
		m_Lines.rightX,m_Lines.rightY,0xFFFF0000);
	m_hge->Gfx_RenderLine(m_Lines.rightX,m_Lines.rightY,
		m_Lines.bottomX,m_Lines.bottomY,0xFFFF0000);
	m_hge->Gfx_RenderLine(m_Lines.bottomX,m_Lines.bottomY,
		m_Lines.leftX,m_Lines.leftY,0xFFFF0000);
	m_hge->Gfx_RenderLine(m_Lines.leftX,m_Lines.leftY,
		m_Lines.topX,m_Lines.topY,0xFFFF0000);
}

void PictureController::Enter()
{
	m_Lines.topX = rect.x1;
	m_Lines.topY = rect.y1;
	m_Lines.leftX = rect.x1;
	m_Lines.leftY = rect.y2;
	m_Lines.rightX = rect.x2;
	m_Lines.rightY = rect.y1;
	m_Lines.bottomX = rect.x2;
	m_Lines.bottomY = rect.y2;

	m_FullShowTex->SetTexture(m_TexSprList[0]->GetTexture());
	m_FullShowTex->SetTextureRect(0,
		0,
		m_FullWidth,
		m_FullHeight);
}

void PictureController::Leave()
{

}

// void PictureController::MouseOver(bool bOver)
// {
// 	if (bOver)
// 	{
// 		int sprNum = m_EachLineNum*m_LineNum;
// 		for (int i=0;i<sprNum;i++)
// 		{
// 			m_TexSprList[i]->SetTextureRect(0,0,
// 				m_SmallSquareWidth,
// 				m_SmallSquareHeight);
// 		}
// 	}
// 	else 
// 	{
// 		int sprNum = m_EachLineNum*m_LineNum;
// 		for (int i=0;i<sprNum;i++)
// 		{
// 			m_TexSprList[i]->SetTextureRect(m_SmallSquareWidth,
// 				m_SmallSquareHeight,
// 				m_SmallSquareWidth,
// 				m_SmallSquareHeight);
// 		}
// 	}
// }

bool PictureController::MouseLButton(bool bDown)
{
	if (bDown)
	{
		float xpos,ypos;
		float xCtrl,yCtrl;
		m_hge->Input_GetMousePos(&xpos,&ypos);
		xCtrl = xpos - rect.x1;
		yCtrl = ypos - rect.y1;
		for (int i=0;i<m_LineNum;i++)
		{
			for (int j=0;j<m_EachLineNum;j++)
			{
				if(xCtrl >= (float)j/(float)m_EachLineNum*m_width &&
					xCtrl < (float)(j+1)/(float)m_EachLineNum*m_width &&
					yCtrl >= (float)i/(float)m_LineNum*m_SmallSquareHeight*m_LineNum &&
					yCtrl < (float)(i+1)/(float)m_LineNum*m_SmallSquareHeight*m_LineNum)
				{
					m_FullShowTex->SetTexture(m_TexSprList[i*3+j]->GetTexture());
					return false;
				}
			}
		}

		static int xLineNum = (int)(m_FullWidth/m_SmallSquareWidth);
		static int yLineNum = (int)(m_FullHeight/m_SmallSquareHeight);
		for (int p=0;p<yLineNum;p++)
		{
			for (int q=0;q<xLineNum;q++)
			{
				if (xCtrl >= (float)q/(float)xLineNum*m_FullWidth &&
					xCtrl < (float)(q+1)/(float)xLineNum*m_FullWidth &&
					yCtrl >= (float)p/(float)yLineNum*m_FullHeight+m_FullTexStartY &&
					yCtrl < (float)(p+1)/(float)yLineNum*m_FullHeight+m_FullTexStartY)
				{
					hgeSprite* spr = new hgeSprite(m_FullShowTex->GetTexture(),q*m_SmallSquareWidth,p*m_SmallSquareHeight,
						m_SmallSquareWidth,m_SmallSquareHeight);
					spr->SetZ(0);
					spr->SetBlendMode(BLEND_DEFAULT_Z);
					Application::sInstance().setMouseTex(spr);
					Application::sInstance().setObjectTag(m_mTexNum[m_FullShowTex->GetTexture()],p*xLineNum+q);
					return false;
				}
			}
		}
		
	}
	return true;
}

void PictureController::loadTexList()
{
	int picId = 0;
	char buffer[MAX_PATH];
	char path[MAX_PATH];
	GetCurrentDirectory(MAX_PATH,buffer);
	for(int i=1;i<=m_PicNum;i++)
	{
		sprintf(path,"%s\\res\\tex\\%d.png",buffer,i);
// 		picId = i;
// 		itoa(i,buffer,10);
// 		strcat(path,buffer);
// 		strcat(path,".png");
		m_TexList[i-1] = m_hge->Texture_Load(path);
		m_mTexNum[m_TexList[i-1]] = i-1;
		memset(path,0,20);
	}
}