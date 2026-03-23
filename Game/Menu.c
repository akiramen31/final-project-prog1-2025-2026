#include "Menu.h"

Menu menu;

void LoadMenu(void)
{
	menu = (Menu){ 0 };
	LoadBackground(GetAsset("Assets/Sprites/Background.png"), 4.f);
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
	default:
		break;
	}
}

void UpdateMenu(float _dt)
{

}