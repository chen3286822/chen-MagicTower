#pragma once
#include "commonTools.h"

class App : public Singleton<App>
{
public:
	App(void);
	~App(void);

	bool SystemInit();
	bool LoadResource();
	void Run();
	void FreeResource();
	void CleanUp();

	bool AppUpdate();
	bool AppRender();

	inline HGE* App::GetHGE(){return hge;}

	Block GetMouseBlock(){return m_Block;}

private:
	HGE* hge;
	Block m_Block;	//鼠标所在block

	//画出鼠标位置的方格
	void DrawMouseRect();
	//画出小方格
	void DrawSmallRect(Block block);
};

