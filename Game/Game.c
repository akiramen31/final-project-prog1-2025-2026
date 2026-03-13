#include "Game.h"

void LoadGame(void)
{
	LoadBackground(GetAsset("Assets/Sprites/Map/Background.png"), 4.f);
	CreateSprite(GetAsset("Assets/Sprites/Map/Foreground.png"),(sfVector2f) {0}, 4.f, 2.f);
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
		SetGameState(MENU);
		break;
	case sfKeySpace:
		SetGameState(GAME_OVER);
		break;
	default:
		break;
	}
}

void UpdateGame(float _dt)
{

}