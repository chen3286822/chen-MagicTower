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


class FontManager : public Singleton<FontManager>
{
public:
	FontManager();
	~FontManager();

	bool LoadFonts(std::string path);
	GfxFont* GetFont(FontAttr _attr);
private:
	MFont m_mFonts;

	static MFontTable sCreateFileTable();
	static MFontTable sm_mFileTable;
};

#endif