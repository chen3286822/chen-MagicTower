#ifndef FONT_MANAGER_H
#define FONT_MANAGER_H

#include "commonTools.h"

class GfxFont;
typedef struct tagFontAttr
{
	FontType m_eType;
	int m_nSize;
	tagFontAttr()
	{
		m_eType = DefaultType;
		m_nSize = 10;
	}
	tagFontAttr(FontType _type,FontSize _size)
	{
		m_eType = _type;
		m_nSize = _size;
	}
	bool operator <(const tagFontAttr& fa) const
	{
		if(m_eType < fa.m_eType)
			return true;
		else if(m_eType == fa.m_eType)
			return m_nSize<fa.m_nSize;
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