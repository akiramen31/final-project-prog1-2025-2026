#include "Common.h"

//int main(void)
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
