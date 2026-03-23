#include "Common.h"

typedef struct MainData
{
	sfRenderWindow* renderWindow;
	sfClock* clock;
	sfView* view;
}MainData;

int main(void)
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
