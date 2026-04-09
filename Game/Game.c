#include "Game.h"

void KeyPressedGame(sfKeyEvent* _keyEvent);
void UpdateCollider(void);

Game game;
sfSprite* background;
float timer;

void LoadGame(void)
{
	game = (Game){ 0 };

	background = LoadBackground(GetAsset("Assets/Maps/Level1.png"), 1.f);
	LoadMap();

	if (DEV_ENNEMY)
	{
		LoadEnnemy();
		AddEnnemy((sfVector2f) { 200, 500 }, ALEATORY);
	}

	LoadHUD();
	LoadPlayer();
	LoadBullet();
	LoadMissile();
	LoadAim();
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
	if (DEV_ENNEMY)
	{
		UpdateEnnemy(_dt, 0);
	}
	UpdateHUD(_dt);
	UpdateCollider();
	UpdateAim(_dt);
	UpdateBullet(_dt);
	UpdateMissile(_dt);
}

void UpdateCollider(void)
{

}