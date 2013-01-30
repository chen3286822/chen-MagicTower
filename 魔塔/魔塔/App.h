#pragma once
#include "commonTools.h"

class App : public Singleton<App>
{
public:
	App(void);
	~App(void);

	bool systemInit();
	void loadResource();
	void run();
	void freeResource();
	void cleanUp();

	bool appUpdate();
	bool appRender();

	void drawSmallRect(float x,float y);

	inline HGE* App::getHGE(){return hge;}

private:
	HGE* hge;
};

