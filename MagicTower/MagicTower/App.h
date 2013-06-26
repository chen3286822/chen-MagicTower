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

	//����С����
	void DrawSmallRect(Block block,DWORD color);
	//������
	void DrawBox(Block block,DWORD color);
private:
	HGE* m_pHge;
	Block m_iBlock;	//�������block

	//�������λ�õķ���
	void DrawMouseRect();

	//������
	void DrawRect(float left,float top,float right,float bottom,DWORD color);
};

