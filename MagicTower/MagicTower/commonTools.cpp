#include "commonTools.h"
#include "GfxFont.h"

const unsigned char GfxFont::g_byAlphaLevel[65] = 
{
	0,  4,  8, 12, 16, 20, 24, 28, 32, 36, 40, 44, 48,
	52, 56, 60, 64, 68, 72, 76, 80, 84, 88, 92, 96,100,
	104,108,112,116,120,124,128,132,136,140,144,148,152,
	156,160,164,168,172,176,180,184,188,192,196,200,204,
	208,212,216,220,224,228,232,236,240,244,248,252,255
};

LBUTTON_STATE g_getLButtonState(HGE* hge)
{
	static bool lastPressed = false;
	static bool currentPressed = false;
	LBUTTON_STATE state = LBUTTON_NULL;
	currentPressed = hge->Input_GetKeyState(HGEK_LBUTTON);

	if (currentPressed==false && lastPressed==false)
		state =  LBUTTON_NULL;
	else if(currentPressed==true && lastPressed==false)
		state =  LBUTTON_DOWN;
	else if(currentPressed==true && lastPressed==true)
		state =  LBUTTON_HOLD;
	else if(currentPressed==false && lastPressed==true)
		state =  LBUTTON_UP;

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
KEY_STATE g_getKeyState(HGE* hge,int Key)
{
	static int lastKey[KEY_NUM] = {-1};
	static bool lastPressed[KEY_NUM] = {false};
	static bool currentPressed[KEY_NUM] = {false};
	KEY_STATE state = KEY_NULL;
	int num = g_getKeyNum(Key);
	if (lastKey[num] != Key)
	{
		lastPressed[num] = false;
		currentPressed[num] = false;
		lastKey[num] = Key;
	}
	currentPressed[num] = hge->Input_GetKeyState(Key);

	if (currentPressed[num]==false && lastPressed[num]==false)
		state =  KEY_NULL;
	else if(currentPressed[num]==true && lastPressed[num]==false)
		state =  KEY_DOWN;
	else if(currentPressed[num]==true && lastPressed[num]==true)
		state =  KEY_HOLD;
	else if(currentPressed[num]==false && lastPressed[num]==true)
		state =  KEY_UP;

	lastPressed[num] = currentPressed[num];
	return state;
}

void g_getFiles( std::string path, std::map<std::string,std::string>& files,char* type,int maxFileNum,bool useDefaultName,bool isCharacter)
{
	std::string* mapNames = NULL;
	std::string* charFightNames = NULL;
	std::string* charDeadNames = NULL;
	std::string* charDefendNames = NULL;
	if (useDefaultName)
	{
		mapNames = new std::string[maxFileNum];
		if (isCharacter)
		{
			charFightNames = new std::string[maxFileNum];
			charDeadNames = new std::string[maxFileNum];
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
				charDeadNames[i].assign(filename);
				sprintf(filename,"%d-3%s",i,type);
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
							(isCharacter && (strcmp(fileinfo.name,charFightNames[i].c_str()) == 0 || strcmp(fileinfo.name,charDeadNames[i].c_str()) == 0|| strcmp(fileinfo.name,charDefendNames[i].c_str()) == 0)))
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
			delete[] charDefendNames;
			delete[] charDeadNames;
			delete[] charFightNames;
		}
		delete[] mapNames;
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
	case Road:
	case CityRoad:
	case City:
		cost = 1;
		break;
	case Forest:
	case Desert:
		cost = 2;
		break;
	case Mountain:
		cost = 3;
		break;
	case River:
		cost = 4;
		break;
	case HillTop:
	case CityWall:
		cost = 200;
		break;
	default:
		break;
	}

	return cost;
}