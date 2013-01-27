#include "App.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	App::sCreate();

	if(App::sInstance().systemInit())
	{
		App::sInstance().loadResource();
		App::sInstance().run();
		App::sInstance().freeResource();
	}
	App::sInstance().cleanUp();

	App::sDestroy();
	return 0;
}