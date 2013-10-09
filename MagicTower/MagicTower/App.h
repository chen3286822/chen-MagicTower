#pragma once
#include "commonTools.h"

class App : public Singleton<App>
{
public:
	App(void);
	virtual ~App(void);

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
	//������,startX��startY��ʾ��ʼ�㣬width��ʾ������ȣ�boxLength��boxWidth��ʾ���򳤿�
	void DrawBox(int startX,int startY,DWORD color,int width,int boxWidth,int boxLength);
	//������
	void DrawRect(float left,float top,float right,float bottom,DWORD color);

	//���õ�ǰͼ��
	void SetLayer(eLayer layer);
	void StartNextScene();	//��ʼ��һ������
private:
	HGE* m_pHge;
	Block m_iBlock;	//�������block

	eLayer m_eCurLayer;		//��ǰ������ʾ��ͼ��
	bool m_bCheckNextScene;	//����Ƿ�ʼ��һ������
	bool m_bCheckPreFight;		//�Ƿ���Ҫ����սǰ�ű�

	//�������λ�õķ���
	void DrawMouseRect();
};

