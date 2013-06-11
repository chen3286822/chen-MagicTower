#include "GfxFont.h"
#include "FontManager.h"


//文件加载
MFontTable FontManager::sCreateFileTable()
{
	MFontTable table;
	table["msyh"] = MSYaHei;
	return table;
}

MFontTable FontManager::sm_mFileTable = sCreateFileTable();


//系统自带
MSystemFontTable FontManager::sCreateSystemFontTable()
{
	MSystemFontTable table;
	table[Calibri] = "Calibri";
	table[SongTi] = "宋体";
	table[MSYaHei] = "微软雅黑";
	return table;
}

MSystemFontTable FontManager::sm_mSystemFontTable = sCreateSystemFontTable();



FontManager::FontManager()
{
	m_mFonts.clear();
}

FontManager::~FontManager()
{
	for (MFont::iterator mit=m_mFonts.begin();mit!=m_mFonts.end();mit++)
	{
		gSafeDelete((mit->second));
	}
	m_mFonts.clear();
}

void FontManager::InitFont()
{
	FontAttr attr;
	for (int j=0;j<SystemFontNum;j++)
	{
		attr.m_eType = (FontType)j;
		for (int i=0;i<FontSizeNum;i++)
		{
			attr.m_nSize = FontSmall + 2*i;
			GfxFont* font = new GfxFont(sm_mSystemFontTable[(FontType)j].c_str(),attr.m_nSize);
			m_mFonts[attr] = font;
		}
	}

}

bool FontManager::LoadFonts(std::string path)
{
	std::map<std::string,std::string> files;
	g_getFiles(path,files,".ttf",0,false);

	size_t found = 0;
 	for (std::map<std::string,std::string>::iterator mit=files.begin();mit!=files.end();mit++)
 	{
 		found = mit->second.find('.');
 		if(found != 0)
 		{
 			FontAttr attr;
 			char strID[10];
 			strncpy(strID,mit->second.c_str(),found);
 			strID[found] = '\0';		
 			for (MFontTable::iterator it=sm_mFileTable.begin();it!=sm_mFileTable.end();it++)
 			{
 				if(strcmp(it->first.c_str(),strID)==0)
 				{
 					attr.m_eType = it->second;
 					//载入字体，包含数种大小
 					for (int i=0;i<FontSizeNum;i++)
 					{
 						attr.m_nSize = FontSmall + 2*i;
 						GfxFont* font = new GfxFont(mit->first.c_str(),attr.m_nSize);
 						m_mFonts[attr] = font;
 					}
 				}
 			}
 		}
 	}
	if(m_mFonts.size() == 0)
		return false;
	return true;
}

GfxFont* FontManager::GetFont(FontAttr _attr)
{
	for (MFont::iterator mit=m_mFonts.begin();mit!=m_mFonts.end();mit++)
	{
		if(mit->first.m_nSize == _attr.m_nSize && mit->first.m_eType== _attr.m_eType)
			return mit->second;
	}
	return NULL;
}