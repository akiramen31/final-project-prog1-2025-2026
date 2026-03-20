#include "Menu.h"

sfSprite* title;

void LoadMenu(void)
{
	LoadBackground(GetAsset("Assets/Sprites/Menu/MenuBackground.png"), 1.f);
	title = CreateSprite(GetAsset("Assets/Sprites/Menu/Title.png"), (sfVector2f) { (SCREEN_WIDTH / 2)-317, 160 }, 1.f, 10.f);
	sfMusic* menuMusic = CreateMusic("Assets/Musics/Title-Screen.ogg", GAME_VOLUME, sfFalse);
	sfMusic_setLoop(menuMusic, sfTrue);
	sfMusic_play(menuMusic);
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
		SetGameState(GAME);
		break;
	default:
		break;
	}
}

void UpdateMenu(float _dt)
{

}