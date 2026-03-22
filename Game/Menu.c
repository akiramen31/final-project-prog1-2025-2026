#include "Menu.h"

sfSprite* title;

void LoadMenu(void)
{
	SetIntToSave(SCORE, 0);
	SetIntToSave(BOMB, 1);
	SetIntToSave(SPEED, 1);
	SetIntToSave(ENNEMY_COUNT, 1);
	SetIntToSave(FIRE, 1);
	SetIntToSave(LIFE, 4);
	SetIntToSave(ROUND, 1);

	LoadBackground(GetAsset("Assets/Sprites/Menu/MenuBackground.png"), 1.f);
	sfFont* font = GetAsset("Assets/Fonts/Bomberman.ttf");
	sfText* text = CreateText(font, (sfVector2f) { 0, SCREEN_HEIGHT / 1.5f }, 1.f, 5.f);
	sfText_setString(text, "Press <Espace> to start game \n press <Echap> to quit");
	title = CreateSprite(GetAsset("Assets/Sprites/Menu/Title.png"), (sfVector2f) { SCREEN_WIDTH / 2, 160 }, 1.f, 5.f);
	sfSprite_setOrigin(title, (sfVector2f) { sfSprite_getLocalBounds(title).width / 2, 0 });
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