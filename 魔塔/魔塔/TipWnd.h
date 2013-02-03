#ifndef TIP_WND_H
#define TIP_WND_H

#include "commonTools.h"

struct StringLine
{
	std::wstring str;
	DWORD color;
	FontType fontType;
	float x,y;
	StringLine()
	{
		clear();
	}
	StringLine(const wchar_t* _str,DWORD _color,FontType _type)
	{
		str = _str;
		color = _color;
		fontType = _type;
		x = y = -1;
	}
	void clear()
	{
		str = (wchar_t*)"";
		color = 0xFFFFFFFF;
		fontType = DefaultType;
		x = y = -1;
	}
};
typedef std::vector<StringLine> VStringLine;

class GfxFont;
class TipWnd : public Singleton<TipWnd>
{
public:
	TipWnd();
	~TipWnd();

	void Render();
	void Update(float delta);
	void Clear();
	void SetOffset(float offx,float offy){m_OffX = offx;m_OffY = offy;}

	//当autoEnter为false时，x，y值被视为有效;maxWidth为0表示不限制tip窗口的宽度
	void AddText(const wchar_t* str,DWORD color=0xFFFFFFFF,float x=-1,float y=-1,FontType type=DefaultType,bool autoEnter=true,int maxWidth=0);
	void AddEmptyLine();

private:
	VStringLine m_vStringLine;
	float m_OffX,m_OffY;	//绘制的tip相对于父窗口的偏移
	int m_Width,m_Height;	//tip窗口的宽度和长度
	GfxFont* m_GfxFont;	//用于绘制字体
};


#endif