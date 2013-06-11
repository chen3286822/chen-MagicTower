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

	inline HGE* App::GetHGE(){return m_pHge;}

	Block GetMouseBlock(){return m_iBlock;}

private:
	HGE* m_pHge;
	Block m_iBlock;	//鼠标所在block

	//画出鼠标位置的方格
	void DrawMouseRect();
	//画出小方格
	void DrawSmallRect(Block block);
};

