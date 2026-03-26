#include "Game.h"

void LoadGame(void)
{
	LoadLeaderboard();
}

void PollEventGame(sfRenderWindow* _renderWindow)
{
	sfEvent event;

	while (sfRenderWindow_pollEvent(_renderWindow, &event))
	{
		switch (event.type)
		{
		case sfEvtClosed:
			sfRenderWindow_close(_renderWindow);
			break;
		case sfEvtKeyPressed:
			KeyPressedGame(_renderWindow, event.key);
			break;
		default:
			break;
		}
	}
}

void KeyPressedGame(sfRenderWindow* _renderWindow, sfKeyEvent _keyEvent)
{
	switch (_keyEvent.code)
	{
	case sfKeyEscape:
		sfRenderWindow_close(_renderWindow);
		break;
	case sfKeySpace:
		CreateScore();
		break;
	default:
		break;
	}
}

void UpdateGame(sfRenderWindow* _renderWindow, float _dt)
{
}

void DrawGame(sfRenderWindow* _renderWindow)
{
	DrawLeaderboard(_renderWindow);
}

void CleanupGame(void)
{
	CleanupLeaderboard();
}
