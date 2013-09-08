#ifndef FONT_MANAGER_H
#define FONT_MANAGER_H

#include "commonTools.h"

class GfxFont;
typedef struct tagFontAttr
{
	eFontType m_eType;
	int m_nSize;
	tagFontAttr()
	{
		m_eType = eFontType_DefaultType;
		m_nSize = 10;
	}
	tagFontAttr(eFontType _type,eFontSize _size)
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
typedef std::map<std::string,eFontType> MFontTable;
typedef std::map<eFontType,std::string> MSystemFontTable;


class FontManager : public Singleton<FontManager>
{
public:
	FontManager();
	virtual ~FontManager();

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