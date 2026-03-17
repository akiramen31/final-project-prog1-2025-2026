#include "GameOver.h"

void KeyPressedGameOver(sfKeyEvent* _keyEvent);

void LoadGameOver(void)
{
	LoadBackground(GetAsset("Assets/Sprites/Menu/MenuBackground.png"), 1.f);
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