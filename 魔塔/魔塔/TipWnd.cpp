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
	m_LastX = 0;
	m_LastY = 0;
}

void TipWnd::AddText(const char* str,DWORD color/* =0xFFFFFFFF */,float x/* =-1 */,float y/* =-1 */,FontType type/* =DefaultType */,FontSize size,bool autoEnter/* =true */,int maxWidth)
{
	wchar_t out[256];
	g_CTW(str,out);
	StringLine line(out,color,type,size,autoEnter);
	if (x!=-1 && y!=-1)
	{
		line.x = x;
		line.y = y;
	}
	if(maxWidth > 0)
		m_Width = maxWidth;

	m_vStringLine.push_back(line);
}

void TipWnd::AddEmptyLine()
{
	StringLine line((wchar_t*)"",0xFFFFFFFF,DefaultType,FontSmall,true);
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
	m_LastX = m_OffX;
	m_LastY = m_OffY;
	for (VStringLine::iterator it=m_vStringLine.begin();it!=m_vStringLine.end();it++)
	{
		GfxFont* font = FontManager::sInstance().GetFont(FontAttr(it->fontType,it->size));
		DWORD color = font->GetColor();
		font->SetColor(it->color);
		//从指定位置绘制文字
		if(it->x!=-1 && it->y!=-1)
		{
			m_LastX = m_OffX + it->x;
			m_LastY = m_OffY + it->y;
			int width = it->x;
			int height = it->y;
			//采用一个一个字符绘制来控制文本宽度不超过tip窗口最大宽度
			SIZE size = font->GetTextSize((*it).str.c_str());
			if(size.cx + it->x < m_Width)
			{
				if(height + size.cy > m_Height)
					m_Height += size.cy;
				font->Render(m_LastX,m_LastY,(*it).str.c_str());
				if(it->autoEnter)
				{
					m_LastY += it->size;
					m_LastX = m_OffX;
					height += size.cy;
					if(height+size.cy > m_Height)
						m_Height += size.cy;
				}
				else
					m_LastX += size.cx;
			}
			else
			{
				SIZE charSize;
				wchar_t temp[256];
				const wchar_t* oneChar = (*it).str.c_str();
				while (*oneChar)
				{
					memcpy(temp,oneChar,2);
					temp[1] = '\0';
					charSize = font->GetTextSize(temp);
					if(width + charSize.cx < m_Width)
					{
						font->Render(m_LastX,m_LastY,temp);
						m_LastX += charSize.cx;
						width += charSize.cx;
					}
					else	//换行
					{
						width = 0;
						height += charSize.cy;
						if(height + charSize.cy  > m_Height)
						{
							m_Height += charSize.cy;
						}
						m_LastY += charSize.cy;
						m_LastX = m_OffX;
						font->Render(m_LastX,m_LastY,temp);
						m_LastX += charSize.cx;
						width += charSize.cx;
					}
					oneChar++;
				}
				if(it->autoEnter)
				{
					m_LastY += it->size;
					m_LastX = m_OffX;
					height += size.cy;
				}
			}
		}
		//没有指定位置，继续上次绘制位置来绘制文本
		else if (it->x == -1 && it->y == -1)
		{
			int width = m_LastX - m_OffX;
			int height = m_LastY - m_OffY;
			SIZE size = font->GetTextSize((*it).str.c_str());
			if (size.cx + width < m_Width)
			{
				if(size.cy + height > m_Height)
					m_Height += size.cy;
				font->Render(m_LastX,m_LastY,(*it).str.c_str());
				if (it->autoEnter)
				{
					m_LastY += size.cy;
					m_LastX = m_OffX;
					width = 0;
					height += size.cy;
					if(height + size.cy > m_Height)
						m_Height += size.cy;
				}
				else
					m_LastX += size.cx;
			}
			else
			{
				SIZE charSize;
				wchar_t temp[256];
				const wchar_t* oneChar = (*it).str.c_str();
				while (*oneChar)
				{
					memcpy(temp,oneChar,2);
					temp[1] = '\0';
					charSize = font->GetTextSize(temp);
					if(width + charSize.cx < m_Width)
					{
						font->Render(m_LastX,m_LastY,temp);
						m_LastX += charSize.cx;
						width += charSize.cx;
					}
					else	//换行
					{
						width = 0;
						height += charSize.cy;
						if(height + charSize.cy  > m_Height)
						{
							m_Height += charSize.cy;
						}
						m_LastY += charSize.cy;
						m_LastX = m_OffX;
						font->Render(m_LastX,m_LastY,temp);
						m_LastX += charSize.cx;
						width += charSize.cx;
					}
					oneChar++;
				}
				if(it->autoEnter)
				{
					m_LastY += it->size;
					m_LastX = m_OffX;
					height += size.cy;
				}
			}
		}
//		font->Render(xpos,ypos,(*it).str.c_str());
		font->SetColor(color);
//		ypos += it->size;
	}
}

void TipWnd::Update(float delta)
{
	float xpos,ypos;
	App::sInstance().GetHGE()->Input_GetMousePos(&xpos,&ypos);
	m_OffX = xpos  + 5;
	m_OffY = ypos + 5;
}