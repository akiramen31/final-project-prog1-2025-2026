#include "Menu.h"

void EnterInGame(void);
sfSprite* title;

void LoadMenu(void)
{
	LoadBackground(GetAsset("Assets/Sprites/Menu/MenuBackground.png"), 1.f);
	title = CreateSprite(GetAsset("Assets/Sprites/Menu/Title.png"), (sfVector2f) { (SCREEN_WIDTH / 2)-317, 160 }, 1.f, 10);
	/*menuMusic = CreateSound(GetAsset("Assets/Musics/Main-Menu.ogg"), 1.f, sfFalse);
	sfSound_setLoop(menuMusic, sfTrue);
	sfSound_play(menuMusic);*/
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