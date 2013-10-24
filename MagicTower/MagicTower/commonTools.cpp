#include "commonTools.h"
#include "GfxFont.h"
#include "FontManager.h"
#include <fstream>

#pragma comment(lib,"version.lib")

char g_strVersion[64] = {0};

const unsigned char GfxFont::g_byAlphaLevel[65] = 
{
	0,  4,  8, 12, 16, 20, 24, 28, 32, 36, 40, 44, 48,
	52, 56, 60, 64, 68, 72, 76, 80, 84, 88, 92, 96,100,
	104,108,112,116,120,124,128,132,136,140,144,148,152,
	156,160,164,168,172,176,180,184,188,192,196,200,204,
	208,212,216,220,224,228,232,236,240,244,248,252,255
};

eLButtonState g_getLButtonState(HGE* hge)
{
	static bool lastPressed = false;
	static bool currentPressed = false;
	eLButtonState state = eLButtonState_Null;
	currentPressed = hge->Input_GetKeyState(HGEK_LBUTTON);

	if (currentPressed==false && lastPressed==false)
		state =  eLButtonState_Null;
	else if(currentPressed==true && lastPressed==false)
		state =  eLButtonState_Down;
	else if(currentPressed==true && lastPressed==true)
		state =  eLButtonState_Hold;
	else if(currentPressed==false && lastPressed==true)
		state =  eLButtonState_Up;

	lastPressed = currentPressed;
	return state;
}

eRButtonState g_getRButtonState(HGE* hge)
{
	static bool lastPressed = false;
	static bool currentPressed = false;
	eRButtonState state = eRButtonState_Null;
	currentPressed = hge->Input_GetKeyState(HGEK_RBUTTON);

	if (currentPressed==false && lastPressed==false)
		state =  eRButtonState_Null;
	else if(currentPressed==true && lastPressed==false)
		state =  eRButtonState_Down;
	else if(currentPressed==true && lastPressed==true)
		state =  eRButtonState_Hold;
	else if(currentPressed==false && lastPressed==true)
		state =  eRButtonState_Up;

	lastPressed = currentPressed;
	return state;
}

int g_getKeyNum(int Key)
{
	if(Key >= HGEK_0 && Key <= HGEK_9)
		return Key-HGEK_0;
	if(Key >= HGEK_A && Key <= HGEK_Z)
		return Key-HGEK_A;
	return 0;
}

//对于不同的key，需要重置state
//目前支持A-Z 0-9  ，扩展其他按键需要扩展静态数组KEY_NUM
eKeyState g_getKeyState(HGE* hge,int Key)
{
	static int lastKey[KEY_NUM] = {-1};
	static bool lastPressed[KEY_NUM] = {false};
	static bool currentPressed[KEY_NUM] = {false};
	eKeyState state = eKeyState_Null;
	int num = g_getKeyNum(Key);
	if (lastKey[num] != Key)
	{
		lastPressed[num] = false;
		currentPressed[num] = false;
		lastKey[num] = Key;
	}
	currentPressed[num] = hge->Input_GetKeyState(Key);

	if (currentPressed[num]==false && lastPressed[num]==false)
		state =  eKeyState_Null;
	else if(currentPressed[num]==true && lastPressed[num]==false)
		state =  eKeyState_Down;
	else if(currentPressed[num]==true && lastPressed[num]==true)
		state =  eKeyState_Hold;
	else if(currentPressed[num]==false && lastPressed[num]==true)
		state =  eKeyState_Up;

	lastPressed[num] = currentPressed[num];
	return state;
}

std::map<std::string,std::string> g_parseConfigFile(const std::string file)
{
	std::ifstream ifs(file);
	std::vector<std::string> vSrc;
	while(!ifs.fail())
	{
		char line[256];
		ifs.getline(line,256);
		vSrc.push_back(line);
	}

	std::map<std::string,std::string> result;
	std::string simbol;
	//解析分类 [XXX]
	for (std::vector<std::string>::iterator it=vSrc.begin();it!=vSrc.end();it++)
	{
		int comment = -1;
		comment = it->find(';');
		//过滤注释
		if(comment != -1)
			continue;

		int first = -1,last=-1;
		first = it->find('[');
		last = it->find(']');
		if (first!=-1 && last!=-1)
		{
			//找到分类符
			simbol = it->substr(first+1,last-first-1);
			simbol += " ";
		}
		else
		{
			int equ = -1;
			equ = it->find('=');
			if(equ!=-1)
			{
				//对正文进行处理
				std::string name = it->substr(0,equ);
				std::string explan = it->substr(equ+1,it->length()-equ);
				if(!name.empty() && !explan.empty())
				{
					name.insert(0,simbol);
					result[name] = explan;
				}
			}
		}
	}
	return result;
}

void g_getFiles( std::string path, std::map<std::string,std::string>& files,char* type,int maxFileNum,bool useDefaultName,bool isCharacter)
{
	std::string* mapNames = NULL;
	std::string* charFightNames = NULL;
	std::string* charDefendNames = NULL;
	if (useDefaultName)
	{
		mapNames = new std::string[maxFileNum];
		if (isCharacter)
		{
			charFightNames = new std::string[maxFileNum];
			charDefendNames = new std::string[maxFileNum];
		}
	}

	if (useDefaultName)
	{
		for (int i=0;i<maxFileNum;i++)
		{
			char filename[50];
			sprintf(filename,"%d%s",i,type);
			mapNames[i].assign(filename);
			if (isCharacter)
			{
				sprintf(filename,"%d-1%s",i,type);
				charFightNames[i].assign(filename);
				sprintf(filename,"%d-2%s",i,type);
				charDefendNames[i].assign(filename);
			}
		}
	}


	//文件句柄
	long   hFile   =   0;
	//文件信息
	_finddata_t fileinfo;
	std::string p;
	if((hFile = _findfirst(p.assign(path).append("\\*").c_str(),&fileinfo)) !=  -1)
	{
		do
		{
			//如果是目录,迭代之
			//如果不是,加入列表
			if((fileinfo.attrib &  _A_SUBDIR))
			{
				if(strcmp(fileinfo.name,".") != 0  &&  strcmp(fileinfo.name,"..") != 0)
					g_getFiles( p.assign(path).append("\\").append(fileinfo.name), files,type,maxFileNum,useDefaultName,isCharacter );
			}
			else
			{
				if(useDefaultName)
				{
					for(int i=0;i<maxFileNum;i++)
					{
						if(strcmp(fileinfo.name,mapNames[i].c_str()) == 0 || 
							(isCharacter && (strcmp(fileinfo.name,charFightNames[i].c_str()) == 0 || strcmp(fileinfo.name,charDefendNames[i].c_str()) == 0)))
						{
							files[p.assign(path).append("\\").append(fileinfo.name)] = fileinfo.name;
							break;
						}
					}
				}
				else
				{
					files[p.assign(path).append("\\").append(fileinfo.name)] = fileinfo.name;
				}
			}
		}while(_findnext(hFile, &fileinfo)  == 0);
		_findclose(hFile);
	}

	if(useDefaultName)
	{
		if (isCharacter)
		{
			gSafeDeleteArray(charDefendNames);
			gSafeDeleteArray(charFightNames);
		}
		gSafeDeleteArray(mapNames);
	}
}

void g_CTW(const char* text,wchar_t* out)
{
	const char *ptext = text;
	size_t nLen = strlen(ptext) + 1;
	size_t nwLen = MultiByteToWideChar(CP_ACP, 0, (const char*)ptext, (int)nLen, NULL, 0);

	MultiByteToWideChar(CP_ACP, 0, ptext, nLen, out, nwLen);
}

int g_getPointToPointValueByIndex(int** array,int n,int i,int j)
{
	return *((int*)array + n*i + j); 
}

/*
地形消耗
Road -- 1
River -- 4
Mountain -- 3
HillTop -- 200
CityWall -- 200
Forest -- 2
Desert -- 2
CityRoad -- 1
City -- 1
*/
int g_getTerrainCost(int terrain)
{
	int cost = 1;
	switch(terrain)
	{
	case eTerrain_Road:
	case eTerrain_CityRoad:
	case eTerrain_City:
		cost = 1;
		break;
	case eTerrain_Forest:
	case eTerrain_Desert:
		cost = 2;
		break;
	case eTerrain_Mountain:
		cost = 3;
		break;
	case eTerrain_River:
		cost = 4;
		break;
	case eTerrain_HillTop:
	case eTerrain_CityWall:
		cost = 200;
		break;
	default:
		break;
	}

	return cost;
}

int g_RandomInt(int start,int end)
{
	HGE* hge = hgeCreate(HGE_VERSION);
	int result = hge->Random_Int(start,end);
	hge->Release();
	return result;
}

void g_debugString(char* file,char* func,int line,char* msg)
{
	char temp[1024];
	sprintf(temp,"文件%s第%d行，函数%s 出现错误，请查看!\n错误信息: %s\n",file,line,func,msg);

	OutputDebugString(temp);
	MessageBox(NULL,temp,"错误",MB_OK);
}

void g_getAlignString(char* src,int setWitdh,eAlign alignType,eFontType type,eFontSize size)
{
	GfxFont* font = FontManager::sInstance().GetFont(FontAttr(type,size));
	wchar_t out[256];
	g_CTW(src,out);
	int length = font->GetTextSize(out).cx;
	if (setWitdh <= length)
		return;

	if(alignType == eAlign_Left)
		return;
	else if(alignType == eAlign_Center || alignType == eAlign_Right)
	{
		g_CTW(" ",out);
		int spaceLength = font->GetTextSize(out).cx;
		int preSpace = 0;
		if(alignType == eAlign_Center)
			preSpace = (setWitdh-length)/2;
		else
			preSpace = setWitdh - length;
		preSpace = preSpace/spaceLength;

		std::string strSpace;
		strSpace.assign(preSpace,' ');
		strSpace.append(src);
		sprintf(src,"%s",strSpace.c_str());
		return;
	}
}

DWORD g_getGameVersion()
{
	TCHAR szFullPath[MAX_PATH];
	DWORD dwVerInfoSize = 0;
	DWORD dwVerHnd;
	VS_FIXEDFILEINFO* pFileInfo;

	DWORD dwVersion = 0x0;

	GetModuleFileName(NULL,szFullPath,sizeof(szFullPath));
	dwVerInfoSize = GetFileVersionInfoSize(szFullPath,&dwVerHnd);
	if (dwVerInfoSize)
	{
		HANDLE hMem;
		LPVOID lpvMem;
		unsigned int uInfoSize = 0;

		hMem = GlobalAlloc(GMEM_MOVEABLE,dwVerInfoSize);
		lpvMem = GlobalLock(hMem);
		GetFileVersionInfo(szFullPath,dwVerHnd,dwVerInfoSize,lpvMem);

		::VerQueryValue(lpvMem, (LPTSTR)("\\"),(void**)&pFileInfo,&uInfoSize);

		WORD nProdVer[4];
		nProdVer[0] = HIWORD(pFileInfo->dwProductVersionMS);
		nProdVer[1] = LOWORD(pFileInfo->dwProductVersionMS);
		nProdVer[2] = HIWORD(pFileInfo->dwProductVersionLS);
		nProdVer[3] = LOWORD(pFileInfo->dwProductVersionLS);

		for (int i=0;i<4;i++)
		{
			dwVersion = (dwVersion << 8) + (BYTE)nProdVer[i];
		}

		WORD dwVersionTail = HIWORD(pFileInfo->dwFileVersionMS);
		if (dwVersionTail == 0)
		{
			sprintf(g_strVersion,"%d.%d.%d.%d",nProdVer[0],nProdVer[1],nProdVer[2],nProdVer[3]);
		}
		else
			sprintf(g_strVersion,"%d.%d.%d.%d.%d",nProdVer[0],nProdVer[1],nProdVer[2],nProdVer[3],dwVersionTail);

		GlobalUnlock(hMem);
		GlobalFree(hMem);
	}
	return dwVersion;
}