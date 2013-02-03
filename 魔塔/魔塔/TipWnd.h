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

	//��autoEnterΪfalseʱ��x��yֵ����Ϊ��Ч;maxWidthΪ0��ʾ������tip���ڵĿ��
	void AddText(const wchar_t* str,DWORD color=0xFFFFFFFF,float x=-1,float y=-1,FontType type=DefaultType,bool autoEnter=true,int maxWidth=0);
	void AddEmptyLine();

private:
	VStringLine m_vStringLine;
	float m_OffX,m_OffY;	//���Ƶ�tip����ڸ����ڵ�ƫ��
	int m_Width,m_Height;	//tip���ڵĿ�Ⱥͳ���
	GfxFont* m_GfxFont;	//���ڻ�������
};


#endif