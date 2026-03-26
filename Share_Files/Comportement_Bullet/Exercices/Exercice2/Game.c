#include "Game.h"

void LoadGame(void)
{
	LoadPlayer();
	LoadBullet();
	LoadEnemy();
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
	default:
		break;
	}
}

void UpdateGame(sfRenderWindow* _renderWindow, float _dt)
{
	UpdatePlayer(_dt, GetPlayerPosition(), GetNearestEnemyPosition(GetPlayerPosition()));
	UpdateBullet(_renderWindow, _dt);
	UpdateEnemy(_renderWindow, _dt);
	GetAngleBetweenVertices(GetPlayerPosition(), GetNearestEnemyPosition(GetPlayerPosition()));
	UpdatePlayerLineOfSight(GetNearestEnemyPosition(GetPlayerPosition()));
}

void DrawGame(sfRenderWindow* _renderWindow)
{
	DrawBullet(_renderWindow);
	DrawPlayer(_renderWindow);
	DrawEnemy(_renderWindow);
}

void CleanupGame(void)
{
	CleanupPlayer();
	CleanupBullet();
	CleanupEnemy();
}
