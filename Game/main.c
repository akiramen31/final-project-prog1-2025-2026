#include "Common.h"
#include "GameState.h"

typedef struct MainData
{
	sfRenderWindow* renderWindow;
	sfClock* clock;
}MainData;

void Load(MainData* _mainData);
void PollEvent(sfRenderWindow* _renderWindow);
void Update(MainData _mainData);
void Cleanup(MainData* _mainData);

void LoadMainData(MainData* _mainData);
void CleanupMainData(MainData* _mainData);

int main(void)
{
	srand(_getpid());
	MainData mainData = { 0 };

	

	Load(&mainData);

	while (sfRenderWindow_isOpen(mainData.renderWindow))
	{
		PollEvent(mainData.renderWindow);
		Update(mainData);
		Draw(mainData.renderWindow);
	}
	Cleanup(&mainData);
	return EXIT_SUCCESS;
}

void Load(MainData* _mainData)
{
	LoadBackup();
	LoadEntityManager();
	LoadMainData(_mainData);
	SetGameState(MENU);
}

void PollEvent(sfRenderWindow* _renderWindow)
{
	sfEvent event;

	while (sfRenderWindow_pollEvent(_renderWindow, &event))
	{
		if (event.type == sfEvtClosed)
		{
			sfRenderWindow_close(_renderWindow);
		}
		else
		{
			switch (GetGameState())
			{
			case MENU:
				PollEventMenu(_renderWindow, &event);
				break;
			case GAME:
				PollEventGame(&event);
				break;
			case GAME_OVER:
				PollEventGameOver(&event);
				break;
			default:
				break;
			}
		}
	}
}

void Update(MainData _mainData)
{
	float dt = sfTime_asSeconds(sfClock_restart(_mainData.clock));

	switch (GetGameState())
	{
	case MENU:
		UpdateMenu(dt);
		break;
	case GAME:
		UpdateGame(dt);
		break;
	case GAME_OVER:
		UpdateGameOver(dt);
		break;
	default:
		break;
	}
}

void Cleanup(MainData* _mainData)
{
	SaveBackup();
	CleanupMainData(_mainData);
	CleanupGlobal();
}

void LoadMainData(MainData* _mainData)
{
	sfVideoMode videoMode = { SCREEN_WIDTH, SCREEN_HEIGHT, BPP };
	_mainData->renderWindow = sfRenderWindow_create(videoMode, "Bomberman", sfClose, NULL);

	

	sfRenderWindow_setFramerateLimit(_mainData->renderWindow, (unsigned int) { 60 });

	_mainData->clock = sfClock_create();
}

void CleanupMainData(MainData* _mainData)
{
	sfRenderWindow_destroy(_mainData->renderWindow);
	_mainData->renderWindow = NULL;

	sfClock_destroy(_mainData->clock);
	_mainData->clock = NULL;
}