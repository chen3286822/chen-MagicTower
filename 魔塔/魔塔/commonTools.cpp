#include "commonTools.h"

LBUTTON_STATE getLButtonState(HGE* hge)
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

//对于不同的key，需要重置state
KEY_STATE getKeyState(HGE* hge,int Key)
{
	static int lastKey = -1;
	static bool lastPressed = false;
	static bool currentPressed = false;
	KEY_STATE state = KEY_NULL;
	if (lastKey != Key)
	{
		lastPressed = false;
		currentPressed = false;
		lastKey = Key;
	}
	currentPressed = hge->Input_GetKeyState(Key);

	if (currentPressed==false && lastPressed==false)
		state =  KEY_NULL;
	else if(currentPressed==true && lastPressed==false)
		state =  KEY_DOWN;
	else if(currentPressed==true && lastPressed==true)
		state =  KEY_HOLD;
	else if(currentPressed==false && lastPressed==true)
		state =  KEY_UP;

	lastPressed = currentPressed;
	return state;
}

void getFiles( std::string path, std::map<std::string,std::string>& files,char* type,int maxFileNum)
{
	std::string* mapNames = new std::string[maxFileNum];
	for (int i=0;i<maxFileNum;i++)
	{
		char filename[50];
		sprintf(filename,"%d%s",i,type);
		mapNames[i].assign(filename);
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
					getFiles( p.assign(path).append("\\").append(fileinfo.name), files,type,maxFileNum );
			}
			else
			{
				for(int i=0;i<maxFileNum;i++)
				{
					if(strcmp(fileinfo.name,mapNames[i].c_str()) == 0)
					{
						files[p.assign(path).append("\\").append(fileinfo.name)] = fileinfo.name;
						break;
					}
				}
			}
		}while(_findnext(hFile, &fileinfo)  == 0);
		_findclose(hFile);
	}

	delete[] mapNames;
}