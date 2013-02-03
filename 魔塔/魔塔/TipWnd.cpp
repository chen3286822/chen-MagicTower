#include "TipWnd.h"
#include "App.h"
#include "FontManager.h"
#include "GfxFont.h"

TipWnd::TipWnd()
{
	Clear();
	char pBuf[MAX_PATH];
	char pathFont[MAX_PATH];
	GetCurrentDirectory(MAX_PATH,pBuf);
	sprintf(pathFont,"%s\\res\\Font\\msyh.ttf",pBuf);
}

TipWnd::~TipWnd()
{
	Clear();
}

void TipWnd::Clear()
{
	m_vStringLine.clear();
	m_OffX = 0;
	m_OffY = 0;
	m_Width = 50;	//最小限制，以免不显示tip窗口
	m_Height = 50;
}

void TipWnd::AddText(const wchar_t* str,DWORD color/* =0xFFFFFFFF */,float x/* =-1 */,float y/* =-1 */,FontType type/* =DefaultType */,bool autoEnter/* =true */,int maxWidth)
{
	StringLine line(str,color,type);
	if(!autoEnter)
	{
		line.x = x;
		line.y = y;
	}

	m_vStringLine.push_back(line);
}

void TipWnd::AddEmptyLine()
{
	StringLine line((wchar_t*)"",0xFFFFFFFF,DefaultType);
	m_vStringLine.push_back(line);
}

void TipWnd::Render()
{
	//先绘制tip窗口
	hgeQuad quad;
	quad.v[0].x = m_OffX;
	quad.v[0].y = m_OffY;
	quad.v[0].tx = 0;		quad.v[0].ty = 0;
	quad.v[1].x = m_OffX+m_Width;
	quad.v[1].y = m_OffY;
	quad.v[1].tx = 1;		quad.v[1].ty = 0;
	quad.v[2].x = m_OffX+m_Width;
	quad.v[2].y = m_OffY+m_Height;
	quad.v[2].tx = 1;		quad.v[2].ty = 1;
	quad.v[3].x = m_OffX;
	quad.v[3].y = m_OffY+m_Height;
	quad.v[3].tx = 0;		quad.v[3].ty = 1;

	for (int i=0;i<4;i++)
	{
		quad.v[i].col = 0xAF484848;
		quad.v[i].z = 0.5;
	}
	quad.blend = BLEND_DEFAULT_Z;
	quad.tex = 0;
	App::sInstance().GetHGE()->Gfx_RenderQuad(&quad);

	//绘制文字
	int ypos = m_OffY;
	for (VStringLine::iterator it=m_vStringLine.begin();it!=m_vStringLine.end();it++)
	{
		GfxFont* font = FontManager::sInstance().GetFont(FontAttr(it->fontType,FontSmall));
		font->Render(m_OffX,ypos,(*it).str.c_str());
		ypos += FontSmall;
	}
}

void TipWnd::Update(float delta)
{
	float xpos,ypos;
	App::sInstance().GetHGE()->Input_GetMousePos(&xpos,&ypos);
	m_OffX = xpos  + 5;
	m_OffY = ypos + 5;
}