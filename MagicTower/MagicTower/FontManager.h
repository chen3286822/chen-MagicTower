#ifndef FONT_MANAGER_H
#define FONT_MANAGER_H

#include "commonTools.h"

class GfxFont;
typedef struct tagFontAttr
{
	FontType type;
	int size;
	tagFontAttr()
	{
		type = DefaultType;
		size = 10;
	}
	tagFontAttr(FontType _type,FontSize _size)
	{
		type = _type;
		size = _size;
	}
	bool operator <(const tagFontAttr& fa) const
	{
		if(type < fa.type)
			return true;
		else if(type == fa.type)
			return size<fa.size;
		else
			return false;
	}
}FontAttr;

typedef std::map<FontAttr,GfxFont*> MFont;
typedef std::map<std::string,FontType> MFontTable;
typedef std::map<FontType,std::string> MSystemFontTable;


class FontManager : public Singleton<FontManager>
{
public:
	FontManager();
	~FontManager();

	//�е����⣬createFont������Ҫ��font family��Ѱ�����壬Ŀǰû��ʵ�ּ����û�����
	//�ݲ�ʹ�����������
	bool LoadFonts(std::string path);
	GfxFont* GetFont(FontAttr _attr);

	//����LoadFonts����ϵͳ�Դ�����
	void InitFont();
private:
	MFont m_mFonts;

	static MFontTable sCreateFileTable();
	static MFontTable sm_mFileTable;

	static MSystemFontTable sCreateSystemFontTable();
	static MSystemFontTable sm_mSystemFontTable;
};

#endif