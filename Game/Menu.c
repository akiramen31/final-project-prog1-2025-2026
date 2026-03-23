#include "Menu.h"

void KeyPressedMenu(sfKeyEvent* _keyEvent);
void MouseButtonPressedMenu(sfMouseButtonEvent* _mouseButtonEvent);
void MouseMovedMenu(sfMouseMoveEvent* _mouseMovedEvent);
void SetMenuState(MenuState _state);

Menu menu;

void LoadMenu(void)
{
	menu = (Menu){ 0 };
	LoadBackground(GetAsset("Assets/Sprites/Background.png"), 12.f);

	if (GetFloatFromSave(LIGHT_LEVEL) < 0.25f)
	{
		SetFloatToSave(LIGHT_LEVEL, 1.f);
	}

	for (int i = 0; i < NB_BUTTON; i++)
	{
		menu.button[i] = CreateText(GetAsset("Assets/Font/Daydream.otf"), (sfVector2f) { (float)20, (float)SCREEN_HEIGHT / 2 + 50 * i }, 1.f, 5.f);
	}

	menu.name[0] = "Akira";
	menu.name[1] = "Benjamin";
	menu.name[2] = "Alice";
	menu.name[3] = "Ylix";
	menu.name[4] = "Pierre";
	menu.name[5] = "Akiramen";
	menu.name[6] = "Scorpiontron";
	menu.name[7] = "Zelouve";
	menu.name[8] = "Ylix";
	menu.name[9] = "Parc9999";

	SetMenuState(MENU_BASE);
}

void PollEventMenu(sfEvent* _event)
{
	switch (_event->type)
	{
	case sfEvtKeyPressed:
		KeyPressedMenu(&_event->key);
		break;
	case sfEvtMouseButtonPressed:
		MouseButtonPressedMenu(&_event->mouseButton);
		break;
	case sfEvtMouseMoved:
		MouseMovedMenu(&_event->mouseMove);
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

void MouseButtonPressedMenu(sfMouseButtonEvent* _mouseButtonEvent)
{
	sfFloatRect hitbox = { 0 };
	switch (menu.state)
	{
	case MENU_BASE:
		for (int i = 0; i < NB_BUTTON; i++)
		{
			hitbox = sfText_getGlobalBounds(menu.button[i]);
			if (sfFloatRect_contains(&hitbox, _mouseButtonEvent->x, _mouseButtonEvent->y))
			{
				if (i == 0)
				{
					SetGameState(GAME);
				}
				else if (i == 1)
				{
					SetMenuState(SETTING);
				}
				else if (i == 2)
				{
					SetMenuState(CREDITS);
				}
				else if (i == 3)
				{
					sfRenderWindow_close(GetRenderWindow());
				}
			}
		}
		break;
	case SETTING:
		for (int i = 0; i < NB_BUTTON; i++)
		{
			hitbox = sfText_getGlobalBounds(menu.button[i]);
			if (sfFloatRect_contains(&hitbox, _mouseButtonEvent->x, _mouseButtonEvent->y))
			{
				if (i == 0)
				{
					SetFloatToSave(LIGHT_LEVEL, ((_mouseButtonEvent->x - hitbox.left) / hitbox.width) * 0.75 + 0.25);
				}
				else if (i == 1)
				{
					SetFloatToSave(SOUND_VOLUME, _mouseButtonEvent->x - hitbox.left);
				}
				else if (i == 2)
				{
					ChangeFullSceen();
				}
				else if (i == 3)
				{
					SetMenuState(MENU_BASE);
				}
			}
		}
		break;
	case CREDITS:
		hitbox = sfText_getGlobalBounds(menu.button[5]);
		if (sfFloatRect_contains(&hitbox, _mouseButtonEvent->x, _mouseButtonEvent->y))
		{
			SetMenuState(MENU_BASE);
		}
		break;
	default:
		break;
	}
}

void MouseMovedMenu(sfMouseMoveEvent* _mouseMovedEvent)
{
	sfFloatRect hitbox = { 0 };
	switch (menu.state)
	{
	case MENU_BASE:
		for (int i = 0; i < NB_BUTTON; i++)
		{
			hitbox = sfText_getGlobalBounds(menu.button[i]);
			if (sfFloatRect_contains(&hitbox, _mouseMovedEvent->x, _mouseMovedEvent->y))
			{
				sfText_setColor(menu.button[i], sfYellow);
			}
			else
			{
				sfText_setColor(menu.button[i], sfWhite);
			}
		}
		break;
	case SETTING:
		for (int i = 0; i < NB_BUTTON; i++)
		{
			hitbox = sfText_getGlobalBounds(menu.button[i]);
			if (sfFloatRect_contains(&hitbox, _mouseMovedEvent->x, _mouseMovedEvent->y))
			{
				sfText_setColor(menu.button[i], sfYellow);
			}
			else
			{
				sfText_setColor(menu.button[i], sfWhite);
			}
		}
		break;
	case CREDITS:
		for (int i = 0; i < NB_BUTTON; i++)
		{
			hitbox = sfText_getGlobalBounds(menu.button[i]);
			if (sfFloatRect_contains(&hitbox, _mouseMovedEvent->x, _mouseMovedEvent->y))
			{
				sfText_setColor(menu.button[i], sfYellow);
				if (i < 5)
				{
					sfText_setString(menu.button[i], menu.name[i + 5]);
				}
			}
			else
			{
				sfText_setColor(menu.button[i], sfWhite);
				if (i < 5)
				{
					sfText_setString(menu.button[i], menu.name[i]);
				}
			}
		}
		break;
	default:
		break;
	}
}

void SetMenuState(MenuState _state)
{
	menu.state = _state;
	switch (menu.state)
	{
	case MENU_BASE:
		sfText_setString(menu.button[0], "Play");
		sfText_setString(menu.button[1], "Setting");
		sfText_setString(menu.button[2], "Credits");
		sfText_setString(menu.button[3], "Quit");
		sfText_setString(menu.button[4], "");
		sfText_setString(menu.button[5], "");
		break;
	case SETTING:
		sfText_setString(menu.button[0], "Light");
		sfText_setString(menu.button[1], "Sound Volume");
		sfText_setString(menu.button[2], "Full Sceen");
		sfText_setString(menu.button[3], "Back");
		sfText_setString(menu.button[4], "");
		sfText_setString(menu.button[5], "");
		break;
	case CREDITS:
		for (int i = 0; i < 5; i++)
		{
			sfText_setString(menu.button[i], menu.name[i]);
		}
		sfText_setString(menu.button[5], "Back");
		break;
	default:
		break;
	}
}

void UpdateMenu(float _dt)
{

}