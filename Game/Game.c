#include "Game.h"

void KeyPressedGame(sfKeyEvent* _keyEvent);
void UpdateCollider(void);

Game game;
float timer;

void LoadGame(void)
{
	game = (Game){ 0 };
  
	LoadBackground(GetAsset("Assets/Maps/Reduite1.png"), 64.f);
	SetViewCentre((sfVector2f) { SCREEN_WIDTH / 6 * 7, 32.f * 108.f });
	LoadScene();

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
}

void UpdateCollider(void)
{

}