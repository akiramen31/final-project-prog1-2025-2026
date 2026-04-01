#include "Game.h"

void KeyPressedGame(sfKeyEvent* _keyEvent);
void UpdateCollider(void);

Game game;
sfSprite* background;
float timer;

void LoadGame(void)
{
	game = (Game){ 0 };
  
	background = LoadBackground(GetAsset("Assets/Maps/LevelTest.png"), 1.f);
	sfSprite_setOrigin(background, (sfVector2f) { 860, 430 + 50 - 39 });
	SetViewCenter((sfVector2f) { SCREEN_WIDTH / 6 * 7, TILE_SIZE * 108.f });
	LoadMap();

	LoadEnnemy();
	LoadHUD();
	LoadPlayer();
	AddEnnemy((sfVector2f) { 200, 200 }, ALEATORY);
}

void PollEventGame(sfEvent* _event)
{
	switch (_event->type)
	{
	case sfEvtKeyPressed:
		KeyPressedGame(&_event->key);
		break;
	case sfEvtMouseMoved:
		break;
	default:
		break;
	}
}

void KeyPressedGame(sfKeyEvent* _keyEvent)
{
	switch (_keyEvent->code)
	{
	case sfKeyEscape:
		SetGameState(MENU);
		break;
	default:
		break;
	}
}

void UpdateGame(float _dt)
{
	UpdatePlayer(_dt);
	UpdateEnnemy(_dt, 0);
	UpdateHUD(_dt);
	UpdateCollider();

	SetViewCenter(GetPlayerPosition());
}

void UpdateCollider(void)
{

}