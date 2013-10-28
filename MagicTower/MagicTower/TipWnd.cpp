#include "TipWnd.h"
#include "App.h"
#include "FontManager.h"
#include "GfxFont.h"

TipWnd::TipWnd()
{
	Clear();
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
	m_Width = 0;	//��С���ƣ����ⲻ��ʾtip����
	m_Height = 0;
	m_LastX = 0;
	m_LastY = 0;
	m_OffLeft = m_OffRight = 5;
	m_OffTop = m_OffBottom = 5;
	m_Show = false;
	m_XPos = m_YPos = -1;
}

void TipWnd::AddText(const char* str,DWORD color/* =0xFFFFFFFF */,float x/* =-1 */,float y/* =-1 */,eFontType type/* =DefaultType */,eFontSize size,bool autoEnter/* =true */,int maxWidth)
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
	else if (maxWidth == 0)
	{
		GfxFont* font = FontManager::sInstance().GetFont(FontAttr(line.fontType,line.size));
		SIZE size = font->GetTextSize(out);
		if(size.cx > m_Width)
			m_Width = size.cx;
	}

	m_vStringLine.push_back(line);
}

void TipWnd::AddEmptyLine()
{
	StringLine line((wchar_t*)"",0xFFFFFFFF,eFontType_DefaultType,eFontSize_FontSmall,true);
	m_vStringLine.push_back(line);
}

void TipWnd::CalTipSizeAndRender(bool bRend)
{
	if (m_Show && !m_vStringLine.empty())
	{
		m_LastX = m_OffX;
		m_LastY = m_OffY;
		//m_Height = 0;
		for (VStringLine::iterator it=m_vStringLine.begin();it!=m_vStringLine.end();it++)
		{
			GfxFont* font = FontManager::sInstance().GetFont(FontAttr(it->fontType,it->size));
			DWORD color = font->GetColor();
			font->SetColor(it->color);
			//��ָ��λ�û�������
			if(it->x!=-1 && it->y!=-1)
			{
				m_LastX = m_OffX + it->x;
				m_LastY = m_OffY + it->y;
				int width = it->x;
				int height = it->y;
				//����һ��һ���ַ������������ı���Ȳ�����tip���������
				SIZE size = font->GetTextSize((*it).str.c_str());
				if(size.cx + it->x <= m_Width)
				{
					if(height + size.cy >= m_Height)
						m_Height += size.cy;
					if(bRend)
						font->Render(m_LastX,m_LastY,(*it).str.c_str());
					if(it->autoEnter)
					{
						m_LastY += it->size;
						m_LastX = m_OffX;
						height += size.cy;
						width = 0;
						if(height+size.cy >= m_Height)
							m_Height += size.cy;
					}
					else
					{
						m_LastX += size.cx;
						width += size.cx;
						height += size.cy;
					}
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
						if(width + charSize.cx <= m_Width)
						{
							if (height + charSize.cy >= m_Height)
							{
								m_Height += charSize.cy;
							}
							if(bRend)
								font->Render(m_LastX,m_LastY,temp);
							m_LastX += charSize.cx;
							width += charSize.cx;
						}
						else	//����
						{
							width = 0;
							height += charSize.cy;
							if(height + charSize.cy  >= m_Height)
							{
								m_Height += charSize.cy;
							}
							m_LastY += charSize.cy;
							m_LastX = m_OffX;
							if(bRend)
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
			//û��ָ��λ�ã������ϴλ���λ���������ı�
			else if (it->x == -1 && it->y == -1)
			{
				int width = m_LastX - m_OffX;
				int height = m_LastY - m_OffY;
				SIZE size = font->GetTextSize((*it).str.c_str());
				if (size.cx + width <= m_Width)
				{
					if(size.cy + height >= m_Height)
						m_Height += size.cy;
					if(bRend)
						font->Render(m_LastX,m_LastY,(*it).str.c_str());
					if (it->autoEnter)
					{
						m_LastY += size.cy;
						m_LastX = m_OffX;
						width = 0;
						height += size.cy;
						if(height + size.cy >= m_Height)
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
						if(width + charSize.cx <= m_Width)
						{
							if (height + charSize.cy >= m_Height)
							{
								m_Height += charSize.cy;
							}
							if(bRend)
								font->Render(m_LastX,m_LastY,temp);
							m_LastX += charSize.cx;
							width += charSize.cx;
						}
						else	//����
						{
							width = 0;
							height += charSize.cy;
							if(height + charSize.cy  >= m_Height)
							{
								m_Height += charSize.cy;
							}
							m_LastY += charSize.cy;
							m_LastX = m_OffX;
							if(bRend)
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
			font->SetColor(color);
		}
	}
}

void TipWnd::Render()
{
	if (m_Show && !m_vStringLine.empty())
	{
		CalTipSizeAndRender();

		//�Ȼ���tip����
		hgeQuad quad;
		quad.v[0].x = m_OffX - m_OffLeft;
		quad.v[0].y = m_OffY - m_OffTop;
		quad.v[0].tx = 0;		quad.v[0].ty = 0;
		quad.v[1].x = m_OffX+m_Width + m_OffRight;
		quad.v[1].y = m_OffY - m_OffTop;
		quad.v[1].tx = 1;		quad.v[1].ty = 0;
		quad.v[2].x = m_OffX+m_Width + m_OffRight;
		quad.v[2].y = m_OffY+m_Height + m_OffBottom;
		quad.v[2].tx = 1;		quad.v[2].ty = 1;
		quad.v[3].x = m_OffX - m_OffLeft;
		quad.v[3].y = m_OffY+m_Height + m_OffBottom;
		quad.v[3].tx = 0;		quad.v[3].ty = 1;

		for (int i=0;i<4;i++)
		{
			quad.v[i].col = 0xCF484848;
			quad.v[i].z = 0.5;
		}
		quad.blend = BLEND_DEFAULT_Z;
		quad.tex = 0;
		App::sInstance().GetHGE()->Gfx_RenderQuad(&quad);

		CalTipSizeAndRender(true);
		//��������
		/*m_LastX = m_OffX;
		m_LastY = m_OffY;
		for (VStringLine::iterator it=m_vStringLine.begin();it!=m_vStringLine.end();it++)
		{
			GfxFont* font = FontManager::sInstance().GetFont(FontAttr(it->fontType,it->size));
			DWORD color = font->GetColor();
			font->SetColor(it->color);
			//��ָ��λ�û�������
			if(it->x!=-1 && it->y!=-1)
			{
				m_LastX = m_OffX + it->x;
				m_LastY = m_OffY + it->y;
				int width = it->x;
				int height = it->y;
				//����һ��һ���ַ������������ı���Ȳ�����tip���������
				SIZE size = font->GetTextSize((*it).str.c_str());
				if(size.cx + it->x <= m_Width)
				{
					if(height + size.cy >= m_Height)
						m_Height += size.cy;
					font->Render(m_LastX,m_LastY,(*it).str.c_str());
					if(it->autoEnter)
					{
						m_LastY += it->size;
						m_LastX = m_OffX;
						height += size.cy;
						width = 0;
						if(height+size.cy >= m_Height)
							m_Height += size.cy;
					}
					else
					{
						m_LastX += size.cx;
						width += size.cx;
						height += size.cy;
					}
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
						if(width + charSize.cx <= m_Width)
						{
							if (height + charSize.cy >= m_Height)
							{
								m_Height += charSize.cy;
							}
							font->Render(m_LastX,m_LastY,temp);
							m_LastX += charSize.cx;
							width += charSize.cx;
						}
						else	//����
						{
							width = 0;
							height += charSize.cy;
							if(height + charSize.cy  >= m_Height)
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
			//û��ָ��λ�ã������ϴλ���λ���������ı�
			else if (it->x == -1 && it->y == -1)
			{
				int width = m_LastX - m_OffX;
				int height = m_LastY - m_OffY;
				SIZE size = font->GetTextSize((*it).str.c_str());
				if (size.cx + width <= m_Width)
				{
					if(size.cy + height >= m_Height)
						m_Height += size.cy;
					font->Render(m_LastX,m_LastY,(*it).str.c_str());
					if (it->autoEnter)
					{
						m_LastY += size.cy;
						m_LastX = m_OffX;
						width = 0;
						height += size.cy;
						if(height + size.cy >= m_Height)
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
						if(width + charSize.cx <= m_Width)
						{
							if (height + charSize.cy >= m_Height)
							{
								m_Height += charSize.cy;
							}
							font->Render(m_LastX,m_LastY,temp);
							m_LastX += charSize.cx;
							width += charSize.cx;
						}
						else	//����
						{
							width = 0;
							height += charSize.cy;
							if(height + charSize.cy  >= m_Height)
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
			font->SetColor(color);
		}
		*/
	}
}

void TipWnd::Update(float delta)
{
	if (m_Show && !m_vStringLine.empty())
	{
		int xpos,ypos;
// 		App::sInstance().GetHGE()->Input_GetMousePos(&xpos,&ypos);
// 		m_OffX = xpos  + 5 + m_OffLeft;
// 		m_OffY = ypos + 5 + m_OffTop;

		//�ж�tip����Ӧ����ʾ�ڸ��ӵ��ı�
		if(m_XPos != -1 && m_YPos != -1)
		{
			xpos = m_XPos*MAP_RECT;
			ypos = m_YPos*MAP_RECT;
			if (xpos+MAP_RECT+m_Width+m_OffLeft >= MAP_RECT*g_nMapWidthNum)
			{
				m_OffX = xpos-m_Width-m_OffRight + MAP_OFF_X;
			}
			else
				m_OffX = xpos + MAP_RECT + m_OffLeft + MAP_OFF_X;
			if (ypos + m_Height+m_OffTop >= MAP_RECT*g_nMapHeightNum)
			{
				m_OffY = ypos - (g_nMapHeightNum*MAP_RECT-ypos-m_Height-m_OffTop) + MAP_OFF_Y;
			}
			else
				m_OffY = ypos + m_OffTop + MAP_OFF_Y;
		}
		else if (m_XPos == -1 && m_YPos == -1)
		{
			if(m_OffX + m_Width >= APP_WIDTH)
				m_OffX = APP_WIDTH - m_Width;
			else if (m_OffX < 0)
				m_OffX = 0;
		}
	}
}

void TipWnd::ParseItem(Item item)
{
	if(item.m_nID == -1)
		return;
	char temp[256] = {0};
	Clear();
	sprintf(temp,"%s",item.m_strName.c_str());
	AddText(temp,0xFFFFFFFF,-1,-1,eFontType_MSYaHei,eFontSize_FontMiddle,true);
	if (item.m_nType == 2)
		sprintf(temp,"����");
	else if(item.m_nType == 3)
		sprintf(temp,"����");
	else if(item.m_nType == 4)
		sprintf(temp,"װ��");
	else
		sprintf(temp,"����Ʒ");
	AddText(temp,0xFF00FF00,-1,-1,eFontType_MSYaHei,eFontSize_FontSmall,true);
	AddEmptyLine();
	for (std::map<int,int>::iterator it=item.m_mEffect.begin();it!=item.m_mEffect.end();it++)
	{
		switch(it->first)
		{
		case 1:
			sprintf(temp,"�����ָ� +%d��",it->second);
			break;
		case 2:
			sprintf(temp,"ħ���ָ� +%d��",it->second);
			break;
		case 3:
			sprintf(temp,"������ +%d",it->second);
			break;
		case 4:
			sprintf(temp,"������� +%d",it->second);
			break;
		case 5:
			sprintf(temp,"�������� +%d%%",it->second);
			break;
		case 6:
			sprintf(temp,"�������� +%d%%",it->second);
			break;
		case 7:
			sprintf(temp,"������Χ���ӣ���������δ��չ�Ĺ�����Χ����");
			break;
		case 8:
			sprintf(temp,"�ƶ��� +%d��",it->second);
			break;
		default:
			sprintf(temp,"�����δ֪����");
			break;
		}
		AddText(temp,0xFF0000FF,-1,-1,eFontType_MSYaHei,eFontSize_FontMiddle,false);
		AddEmptyLine();
	}

	AddEmptyLine();
	sprintf(temp,"    %s",item.m_strInfo.c_str());
	AddText(temp,0xFF00FF00,-1,-1,eFontType_MSYaHei,eFontSize_FontMiddle,true,100);
	SetShow(true);
}

void TipWnd::ParseSkill(SkillInfo skill)
{
	if(skill.m_nID == -1)
		return;

	char temp[256] = {0};
	Clear();
	sprintf(temp,"%s",skill.m_strName.c_str());
	AddText(temp,0xFFFFFFFF,-1,-1,eFontType_MSYaHei,eFontSize_FontMiddle,true);
	sprintf(temp,"����%dħ��",skill.m_nCost);
	AddText(temp,0xFF00FF00,-1,-1,eFontType_MSYaHei,eFontSize_FontSmall,true);
	AddEmptyLine();

	switch(skill.m_nSkillType)
	{
	case 1:
		sprintf(temp,"�������%d�㼼���˺�",skill.m_nAttack);
		break;
	case 2:
		sprintf(temp,"���Իָ�%d������",skill.m_nAttack);
		break;
	case 3:
		{
			int buffType = skill.m_nAttack/1000;
			int buffValue = skill.m_nAttack%1000;
			switch(buffType)
			{
			case 1:
				sprintf(temp,"��ʱ����%d���������޺͵�ǰ������%d�غϺ�ָ�",buffValue,skill.m_nLastTurn);
				break;
			case 2:
				sprintf(temp,"��ʱ����%d��ħ�����޺͵�ǰħ����%d�غϺ�ָ�",buffValue,skill.m_nLastTurn);
				break;
			case 3:
				sprintf(temp,"��ʱ����%d����������%d�غϺ�ָ�",buffValue,skill.m_nLastTurn);
				break;
			case 4:
				sprintf(temp,"��ʱ����%d�����������%d�غϺ�ָ�",buffValue,skill.m_nLastTurn);
				break;
			case 5:
				sprintf(temp,"��ʱ����%d%% �����ʣ�%d�غϺ�ָ�",buffValue,skill.m_nLastTurn);
				break;
			case 6:
				sprintf(temp,"��ʱ����%d%% ���ܣ�%d�غϺ�ָ�",buffValue,skill.m_nLastTurn);
				break;
			default:
				sprintf(temp,"δ֪�����漼��Ч��");
			}
		}
		break;
	default:
		sprintf(temp,"δ֪�����漼��Ч��");
	}

	AddText(temp,0xFF0000FF,-1,-1,eFontType_MSYaHei,eFontSize_FontMiddle,true);
	AddEmptyLine();

	sprintf(temp,"    %s",skill.m_strInfo.c_str());
	AddText(temp,0xFF00FF00,-1,-1,eFontType_MSYaHei,eFontSize_FontMiddle,true,100);
	SetShow(true);
}