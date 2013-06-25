#ifndef TIP_WND_H
#define TIP_WND_H

#include "commonTools.h"

struct StringLine
{
	std::wstring str;
	DWORD color;
	eFontType fontType;
	eFontSize size;
	bool autoEnter;
	float x,y;
	StringLine()
	{
		clear();
	}
	StringLine(const wchar_t* _str,DWORD _color,eFontType _type,eFontSize _size,bool _autoEnter)
	{
		str = _str;
		color = _color;
		fontType = _type;
		size = _size;
		autoEnter = _autoEnter;
		x = y = -1;
	}
	void clear()
	{
		str = (wchar_t*)"";
		color = 0xFFFFFFFF;
		fontType = eFontType_DefaultType;
		size = eFontSize_FontSmall;
		autoEnter = true;
		x = y = -1;
	}
};
typedef std::vector<StringLine> VStringLine;

class TipWnd : public Singleton<TipWnd>
{
public:
	TipWnd();
	~TipWnd();

	void Render();
	void Update(float delta);
	void Clear();
	void SetOffset(float offx,float offy){m_OffX = offx;m_OffY = offy;}

	//当autoEnter为false时，x，y值被视为有效;maxWidth为0表示不限制tip窗口的宽度，则以最长的那一行文本为最大宽度
	void AddText(const char* str,DWORD color=0xFFFFFFFF,float x=-1,float y=-1,eFontType type=eFontType_DefaultType,eFontSize size=eFontSize_FontSmall,bool autoEnter=true,int maxWidth=0);
	void AddEmptyLine();

	void SetBoardOffSet(int _offLeft,int _offRight,int _offTop,int _offBottom){m_OffLeft=_offLeft;m_OffRight=_offRight;m_OffTop=_offTop;m_OffBottom=_offBottom;}
	void SetShow(bool _show){m_Show=_show;}
	bool IsShow(){return m_Show;}
	void SetPos(int xpos,int ypos){m_XPos=xpos;m_YPos=ypos;}

private:
	VStringLine m_vStringLine;
	float m_OffX,m_OffY;	//绘制的tip相对于父窗口的偏移
	int m_Width,m_Height;	//tip窗口的宽度和长度
	int m_LastX,m_LastY;		//下一行绘制的起始位置
	int m_OffLeft,m_OffRight,m_OffTop,m_OffBottom;	//tip窗口上下左右预留的空白位置
	bool m_Show;	//是否显示tip窗口
	int m_XPos,m_YPos;	//显示tip窗口时所在的格子，用于固定tip窗口位置
};


#endif