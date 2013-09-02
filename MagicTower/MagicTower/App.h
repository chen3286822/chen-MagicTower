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

	//画出小方格
	void DrawSmallRect(Block block,DWORD color);
	//画方框,startX、startY表示起始点，width表示线条宽度，boxLength、boxWidth表示方框长宽
	void DrawBox(int startX,int startY,DWORD color,int width,int boxWidth,int boxLength);
	//画矩形
	void DrawRect(float left,float top,float right,float bottom,DWORD color);
private:
	HGE* m_pHge;
	Block m_iBlock;	//鼠标所在block

	//画出鼠标位置的方格
	void DrawMouseRect();
	void LuaInit();
};

