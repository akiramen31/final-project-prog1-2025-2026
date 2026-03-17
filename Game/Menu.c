#include "Menu.h"

void EnterInGame(void);

void LoadMenu(void)
{
	LoadBackground(GetAsset("Assets/Sprites/Menu/MenuBackground.png"), 1.f);
}

void PollEventMenu(sfRenderWindow* _renderWindow, sfEvent* _event)
{
	switch (_event->type)
	{
	case sfEvtKeyPressed:
		KeyPressedMenu(_renderWindow, &_event->key);
		break;
	default:
		break;
	}
}

void KeyPressedMenu(sfRenderWindow* _renderWindow, sfKeyEvent* _keyEvent)
{
	switch (_keyEvent->code)
	{
	case sfKeyEscape:
		sfRenderWindow_close(_renderWindow);
		break;
	case sfKeySpace:
		EnterInGame();
		break;
	default:
		break;
	}
}

void UpdateMenu(float _dt)
{

}

void EnterInGame(void)
{
	SetIntToSave(SCORE, 0);

	SetGameState(GAME);
}