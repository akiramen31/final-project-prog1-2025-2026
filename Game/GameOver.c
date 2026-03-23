#include "GameOver.h"
void KeyPressedGameOver(sfKeyEvent* _keyEvent);

GameOver gameOver;

void LoadGameOver(void)
{
	gameOver = (GameOver){ 0 };
	LoadBackground(GetAsset("Assets/Sprites/Background.png"), 4.f);
}

void PollEventGameOver(sfEvent* _event)
{
	switch (_event->type)
	{
	case sfEvtKeyPressed:
		KeyPressedGameOver(&_event->key);
		break;
	default:
		break;
	}
}

void UpdateGameOver(float _dt)
{

}

void KeyPressedGameOver(sfKeyEvent* _keyEvent)
{
	switch (_keyEvent->code)
	{
	case sfKeyEscape:
		SetGameState(MENU);
		break;
	case sfKeyR:
		SetGameState(GAME);
		break;
	default:
		break;
	}
}