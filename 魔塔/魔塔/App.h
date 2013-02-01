#pragma once
#include "commonTools.h"

class App : public Singleton<App>
{
public:
	App(void);
	~App(void);

	bool SystemInit();
	void LoadResource();
	void Run();
	void FreeResource();
	void CleanUp();

	bool AppUpdate();
	bool AppRender();

	inline HGE* App::GetHGE(){return hge;}

private:
	HGE* hge;

	//画出鼠标位置的方格
	void DrawMouseRect();
	//画出小方格
	void DrawSmallRect(float x,float y);
};

