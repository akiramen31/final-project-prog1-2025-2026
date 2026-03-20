#include "Menu.h"

void EnterInGame(void);
sfSprite* title;

void LoadMenu(void)
{
	LoadBackground(GetAsset("Assets/Sprites/Menu/MenuBackground.png"), 1.f);
	title = CreateSprite(GetAsset("Assets/Sprites/Menu/Title.png"), (sfVector2f) { (SCREEN_WIDTH / 2)-317, 160 }, 1.f, 10);
	sfMusic* menuMusic = CreateMusic("Assets/Musics/Title-Screen.ogg", 10.f, sfFalse);
	sfMusic_setLoop(menuMusic, sfTrue);
	sfMusic_play(menuMusic);
	SetIntToSave(SCORE, 0);
	SetIntToSave(BOMB, 5);
	SetIntToSave(SPEED, 1);
	SetIntToSave(ENNEMY_COUNT, 1);
	SetIntToSave(FIRE, 10);
	SetIntToSave(LIFE, 4);
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
	case sfKeyG:
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