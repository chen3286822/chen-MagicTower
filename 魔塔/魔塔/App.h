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

	//�������λ�õķ���
	void DrawMouseRect();
	//����С����
	void DrawSmallRect(float x,float y);
};

