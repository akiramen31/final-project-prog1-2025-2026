#include "Game.h"

void KeyPressedGame(sfKeyEvent* _keyEvent);
void UpdateCollider(void);

Game game;
float timer;

void LoadGame(void)
{
	game = (Game){ 0 };
	LoadScene();
	SetViewCentre((sfVector2f) { SCREEN_WIDTH, 3500.f });
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
	UpdateEnnemy(_dt,0);
	UpdateHUD(_dt);
	UpdateCollider();
	float x = 80.f;
	float y = 80.f;
	sfVector2f position = { x,y };
	printf("cordoné x%f  y%f   devien x%d  y%d\n", x,y,RealPositionConvertTableauPosition(position).x, RealPositionConvertTableauPosition(position).y);
}

void UpdateCollider(void)
{

}