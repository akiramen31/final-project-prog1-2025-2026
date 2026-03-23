#include "Menu.h"

Menu menu;

void LoadMenu(void)
{
	menu = (Menu){ 0 };
	LoadBackground(GetAsset("Assets/Sprites/Background.png"), 12.f);
}

void PollEventMenu(sfEvent* _event)
{
	switch (_event->type)
	{
	case sfEvtKeyPressed:
		KeyPressedMenu(&_event->key);
		break;
	default:
		break;
	}
}

void KeyPressedMenu(sfKeyEvent* _keyEvent)
{
	switch (_keyEvent->code)
	{
	case sfKeyEscape:
		sfRenderWindow_close(GetRenderWindow());
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