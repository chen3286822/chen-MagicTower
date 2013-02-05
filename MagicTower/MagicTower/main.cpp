#include "App.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	App::sCreate();

	if(App::sInstance().SystemInit() && App::sInstance().LoadResource())
	{
		App::sInstance().Run();
		App::sInstance().FreeResource();
	}
	App::sInstance().CleanUp();

	App::sDestroy();
	return 0;
}