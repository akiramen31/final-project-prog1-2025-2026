#include "Common.h"

int WinMain(void)
{
	srand(_getpid());

	Load();
	while (sfRenderWindow_isOpen(GetRenderWindow()))
	{
		PollEvent();
		Update();
		Draw();
	}
	Cleanup();
	return EXIT_SUCCESS;
}
