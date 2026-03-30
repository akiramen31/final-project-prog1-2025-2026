#include "Menu.h"

void KeyPressedMenu(sfEvent* _event);
void MouseButtonPressedMenu(sfMouseButtonEvent* _mouseButtonEvent);
void MouseMovedMenu(sfMouseMoveEvent* _mouseMovedEvent);
void SetMenuState(MenuState _state);
void UpdateTextKey(int _index, int _key);

Menu menu;
sfSprite* vinyl;
sfSprite* cog;
sfSprite* title;

void LoadMenu(void)
{
	printf("%d", rand() % -10);

	menu = (Menu){ 0 };

	LoadBackground(GetAsset("Assets/Sprites/main_menu.png"), 8.f);
	vinyl = CreateSprite(GetAsset("Assets/Sprites/vinyl.png"), (sfVector2f) {0}, 8.f, 50.f);
	cog = CreateSprite(GetAsset("Assets/Sprites/cog.png"), (sfVector2f) {0}, 8.f, 50.f);
	title = CreateSprite(GetAsset("Assets/Sprites/title.png"), (sfVector2f) {0}, 8.f, 50.f);
	sfSprite_setOrigin(vinyl, (sfVector2f){64,64});
	sfSprite_setOrigin(cog, (sfVector2f){64,64});
	sfSprite_setOrigin(title, (sfVector2f){64,64});
	sfSprite_setPosition(vinyl, (sfVector2f){1504, 552});
	sfSprite_setPosition(cog, (sfVector2f){1504, 552});
	sfSprite_setPosition(title, (sfVector2f){1504, 552});

	if (GetFloatFromSave(LIGHT_LEVEL) < 0.25f)
	{
		SetFloatToSave(LIGHT_LEVEL, 1.f);
	}

	sfFont* font = GetAsset("Assets/Font/Daydream.otf");

	for (int i = 0; i < NB_BUTTON; i++)
	{
		menu.button[i] = CreateText(font, (sfVector2f) { 0.f, 0.f}, 50, 5.f);
		switch (i)
		{
		case 0:
			sfText_setPosition(menu.button[i], (sfVector2f){35, 23});
			break;
		case 1:
			sfText_setPosition(menu.button[i], (sfVector2f) { 332, 23 });
			break;
		case 2:
			sfText_setPosition(menu.button[i], (sfVector2f) { 850, 23});
			break;
		case 3:
			sfText_setPosition(menu.button[i], (sfVector2f) { 1308, 23});
			break;
		}
	}

	for (int i = 0; i < NB_KEY; i++)
	{
		menu.keyRod[i] = CreateText(font, (sfVector2f) { (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT / 2 + 50 * i }, 50, 5.f);
		menu.keyType[i] = CreateText(font, (sfVector2f) { (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT / 2 + 50 * i }, 50, 5.f);
		menu.key[i] = CreateText(font, (sfVector2f) { (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT / 2 + 50 * i }, 50, 5.f);
	}

	sfText_setString(menu.keyType[0], "Jump");
	sfText_setString(menu.keyType[1], "Down");
	sfText_setString(menu.keyType[2], "Right");
	sfText_setString(menu.keyType[3], "Left");
	sfText_setString(menu.keyType[4], "Shoot");
	sfText_setString(menu.keyType[5], "Dash");
	sfText_setString(menu.keyType[6], "Melee");
	sfText_setString(menu.keyType[7], "Second");

	for (int i = 0; i < NB_KEY; i++)
	{
		UpdateTextKey(i, GetKeyFromSave(i));
	}

	menu.name[0] = "Akira";
	menu.name[1] = "Benjamin";
	menu.name[2] = "Alice";
	menu.name[3] = "Ylix";
	menu.name[4] = "Pierre";
	menu.name[5] = "Akiramen";
	menu.name[6] = "Scorpiontron";
	menu.name[7] = "Zelouve";
	menu.name[8] = "Pyxal";
	menu.name[9] = "Parc9999";

	SetMenuState(MENU_BASE);
}

void PollEventMenu(sfEvent* _event)
{
	switch (_event->type)
	{
	case sfEvtKeyPressed:
		KeyPressedMenu(_event);
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

void KeyPressedMenu(sfEvent* _event)
{
	switch (_event->key.code)
	{
	case sfKeyEscape:
		sfRenderWindow_close(GetRenderWindow());
		return;
	default:
		break;
	}

	sfFloatRect hitbox = { 0 };
	sfFloatRect hitbox2 = { 0 };
	for (int i = 0; i < NB_KEY; i++)
	{
		
		if (CompareColor(sfText_getColor(menu.key[i]), sfYellow))
		{
			UpdateTextKey(i, _event->key.code);
			return;
		}
	}
}

void MouseButtonPressedMenu(sfMouseButtonEvent* _mouseButtonEvent)
{
	sfFloatRect hitbox = { 0 };
	if (_mouseButtonEvent->button == sfMouseLeft)
	{
		switch (menu.state)
		{
		case MENU_BASE:
			for (int i = 0; i < NB_BUTTON; i++)
			{
				if (CompareColor(sfText_getColor(menu.button[i]), sfRed))
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
					return;
				}
			}
			break;
		case SETTING:
			for (int i = 0; i < NB_BUTTON; i++)
			{
				sfFloatRect hitbox = sfText_getGlobalBounds(menu.button[i]);
				if (sfFloatRect_contains(&hitbox, _mouseButtonEvent->x, _mouseButtonEvent->y))
				{
					if (i == 0)
					{
						SetFloatToSave(LIGHT_LEVEL, ((_mouseButtonEvent->x - hitbox.left) / hitbox.width) * 0.75f + 0.25f);
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
					return;
				}
			}
			break;
		case CREDITS:
			if (CompareColor(sfText_getColor(menu.button[5]), sfYellow))
			{
				SetMenuState(MENU_BASE);
				return;
			}
			break;
		default:
			break;
		}
	}
	else if (_mouseButtonEvent->button == sfMouseRight)
	{
		switch (menu.state)
		{
		case SETTING:
			for (int i = 0; i < 2; i++)
			{
				if (CompareColor(sfText_getColor(menu.button[i]), sfYellow))
				{
					if (i == 0)
					{
						SetFloatToSave(LIGHT_LEVEL, 0.25f);
					}
					else if (i == 1)
					{
						SetFloatToSave(SOUND_VOLUME, 0.f);
					}
					return;
				}
			}
			break;
		default:
			break;
		}
	}
	for (int i = 0; i < NB_KEY; i++)
	{
		if (CompareColor(sfText_getColor(menu.key[i]), sfYellow))
		{
			UpdateTextKey(i, _mouseButtonEvent->button + sfKeyCount);
			return;
		}
	}
}

void MouseMovedMenu(sfMouseMoveEvent* _mouseMovedEvent)
{
	sfFloatRect hitbox = { 0 };
	sfColor highlightColor = sfRed;
	sfColor baseColor = sfWhite;
	switch (menu.state)
	{
	case MENU_BASE:
		for (int i = 0; i < NB_BUTTON; i++)
		{
			hitbox = sfText_getGlobalBounds(menu.button[i]);
			if (sfFloatRect_contains(&hitbox, (float)_mouseMovedEvent->x, (float)_mouseMovedEvent->y))
			{
				sfText_setColor(menu.button[i], highlightColor);
			}
			else
			{
				sfText_setColor(menu.button[i], baseColor);
			}
		}
		break;
	case SETTING:
		for (int i = 0; i < NB_BUTTON; i++)
		{
			hitbox = sfText_getGlobalBounds(menu.button[i]);
			if (sfFloatRect_contains(&hitbox, (float)_mouseMovedEvent->x, (float)_mouseMovedEvent->y))
			{
				sfText_setColor(menu.button[i], highlightColor);
			}
			else
			{
				sfText_setColor(menu.button[i], baseColor);
			}
		}
		sfFloatRect hitbox2 = { 0 };
		for (int i = 0; i < NB_KEY; i++)
		{
			hitbox = sfText_getGlobalBounds(menu.key[i]);
			hitbox2 = sfText_getGlobalBounds(menu.keyType[i]);
			if (sfFloatRect_contains(&hitbox, (float)_mouseMovedEvent->x, (float)_mouseMovedEvent->y) || sfFloatRect_contains(&hitbox2, (float)_mouseMovedEvent->x, (float)_mouseMovedEvent->y))
			{
				sfText_setColor(menu.key[i], highlightColor);
				sfText_setColor(menu.keyType[i], highlightColor);
			}
			else
			{
				sfText_setColor(menu.key[i], baseColor);
				sfText_setColor(menu.keyType[i], baseColor);
			}
		}

		if (GetFloatFromSave(LIGHT_LEVEL) <= 0.25f)
		{
			sfText_setColor(menu.button[0], highlightColor);
		}
		if (GetFloatFromSave(SOUND_VOLUME) <= 0.0f)
		{
			sfText_setColor(menu.button[1], highlightColor);
		}
		break;
	case CREDITS:
		for (int i = 0; i < NB_BUTTON; i++)
		{
			hitbox = sfText_getGlobalBounds(menu.button[i]);
			if (sfFloatRect_contains(&hitbox, (float)_mouseMovedEvent->x, (float)_mouseMovedEvent->y))
			{
				sfText_setColor(menu.button[i], highlightColor);
				if (i < 5)
				{
					sfText_setString(menu.button[i], menu.name[i + 5]);
				}
			}
			else
			{
				sfText_setColor(menu.button[i], baseColor);
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
		sfText_setColor(menu.button[0], sfWhite);
		sfText_setColor(menu.button[1], sfWhite);
		for (int i = 0; i < NB_KEY; i++)
		{
			sfText_setPosition(menu.key[i], (sfVector2f) { (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT / 2 + 50 * i });
			sfText_setPosition(menu.keyType[i], (sfVector2f) { (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT / 2 + 50 * i });
		}
		break;
	case SETTING:
		sfText_setString(menu.button[0], "Light Level");
		sfText_setString(menu.button[1], "Sound Volume");
		sfText_setString(menu.button[2], "Full Sceen");
		sfText_setString(menu.button[3], "Back");
		sfText_setString(menu.button[4], "");
		sfText_setString(menu.button[5], "");

		if (GetFloatFromSave(LIGHT_LEVEL) <= 0.25f)
		{
			sfText_setColor(menu.button[0], sfRed);
		}
		if (GetFloatFromSave(SOUND_VOLUME) <= 0.0f)
		{
			sfText_setColor(menu.button[1], sfRed);
		}

		for (int i = 0; i < NB_KEY; i++)
		{
			sfText_setPosition(menu.key[i], (sfVector2f) { (float)SCREEN_WIDTH / 2 + 230, (float)SCREEN_HEIGHT / 2 + 50 * i });
			sfText_setPosition(menu.keyType[i], (sfVector2f) { (float)SCREEN_WIDTH / 2, (float)SCREEN_HEIGHT / 2 + 50 * i });
		}
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
	float rotation = sfSprite_getRotation(vinyl);
	if (rotation > 360)
	{
		rotation -= 360;
	}
	rotation += 5 * _dt;
	sfSprite_setRotation(vinyl, rotation);
}

void UpdateTextKey(int _index, int _key)
{
	char buffer[14] = { 0 };
	if (_key >= 0 && _key < 26)
	{
		buffer[0] = 'A' + _key;
	}
	else if (_key > 25 && _key < 36)
	{
		buffer[0] = '0' + _key - 26;
	}
	else if (_key == sfKeyLControl)
	{
		CopyStingToBuffer(buffer, "LControl");
	}
	else if (_key == sfKeyLShift)
	{
		CopyStingToBuffer(buffer, "LShift");
	}
	else if (_key == sfKeyLAlt)
	{
		CopyStingToBuffer(buffer, "LAlt");
	}
	else if (_key == sfKeyRControl)
	{
		CopyStingToBuffer(buffer, "RControl");
	}
	else if (_key == sfKeyRShift)
	{
		CopyStingToBuffer(buffer, "RShift");
	}
	else if (_key == sfKeyRAlt)
	{
		CopyStingToBuffer(buffer, "RAlt");
	}
	else if (_key == sfKeyLBracket)
	{
		CopyStingToBuffer(buffer, "LBracket");
	}
	else if (_key == sfKeyRBracket)
	{
		CopyStingToBuffer(buffer, "RBracket");
	}
	else if (_key == sfKeySemicolon)
	{
		CopyStingToBuffer(buffer, "Semicolon");
	}
	else if (_key == sfKeyComma)
	{
		CopyStingToBuffer(buffer, "Comma");
	}
	else if (_key == sfKeyPeriod)
	{
		CopyStingToBuffer(buffer, "Period");
	}
	else if (_key == sfKeyQuote)
	{
		CopyStingToBuffer(buffer, "Quote");
	}
	else if (_key == sfKeySlash)
	{
		CopyStingToBuffer(buffer, "Slash");
	}
	else if (_key == sfKeyBackslash)
	{
		CopyStingToBuffer(buffer, "Backslash");
	}
	else if (_key == sfKeyTilde)
	{
		CopyStingToBuffer(buffer, "Tilde");
	}
	else if (_key == sfKeyEqual)
	{
		CopyStingToBuffer(buffer, "Equal");
	}
	else if (_key == sfKeyHyphen)
	{
		CopyStingToBuffer(buffer, "Hyphen");
	}
	else if (_key == sfKeySpace)
	{
		CopyStingToBuffer(buffer, "Space");
	}
	else if (_key == sfKeyEnter)
	{
		CopyStingToBuffer(buffer, "Enter");
	}
	else if (_key == sfKeyBackspace)
	{
		CopyStingToBuffer(buffer, "Backspace");
	}
	else if (_key == sfKeyTab)
	{
		CopyStingToBuffer(buffer, "Tab");
	}
	else if (_key == sfKeyPageUp)
	{
		CopyStingToBuffer(buffer, "PageUp");
	}
	else if (_key == sfKeyPageDown)
	{
		CopyStingToBuffer(buffer, "PageDown");
	}
	else if (_key == sfKeyEnd)
	{
		CopyStingToBuffer(buffer, "End");
	}
	else if (_key == sfKeyHome)
	{
		CopyStingToBuffer(buffer, "Home");
	}
	else if (_key == sfKeyInsert)
	{
		CopyStingToBuffer(buffer, "Insert");
	}
	else if (_key == sfKeyDelete)
	{
		CopyStingToBuffer(buffer, "Delete");
	}
	else if (_key == sfKeyAdd)
	{
		CopyStingToBuffer(buffer, "Add");
	}
	else if (_key == sfKeySubtract)
	{
		CopyStingToBuffer(buffer, "Subtract");
	}
	else if (_key == sfKeyMultiply)
	{
		CopyStingToBuffer(buffer, "Multiply");
	}
	else if (_key == sfKeyDivide)
	{
		CopyStingToBuffer(buffer, "Divide");
	}
	else if (_key == sfKeyLeft)
	{
		CopyStingToBuffer(buffer, "Left");
	}
	else if (_key == sfKeyRight)
	{
		CopyStingToBuffer(buffer, "Right");
	}
	else if (_key == sfKeyUp)
	{
		CopyStingToBuffer(buffer, "Up");
	}
	else if (_key == sfKeyDown)
	{
		CopyStingToBuffer(buffer, "Down");
	}
	else if (_key > 74 && _key < 85)
	{
		CopyStingToBuffer(buffer, "Numpad");
		buffer[6] = '0' + _key - 75;
	}
	else if (_key > 84 && _key < 94)
	{
		buffer[0] = 'F';
		buffer[1] = '1' + _key - 85;
	}
	else if (_key < 100)
	{
		buffer[0] = 'F';
		buffer[1] = '1';
		buffer[2] = '0' + _key - 94;
	}
	else if (_key == sfMouseLeft + sfKeyCount)
	{
		CopyStingToBuffer(buffer, "MouseLeft");
	}
	else if (_key == sfMouseRight + sfKeyCount)
	{
		CopyStingToBuffer(buffer, "MouseRight");
	}
	else if (_key == sfMouseMiddle + sfKeyCount)
	{
		CopyStingToBuffer(buffer, "MouseMiddle");
	}
	else if (_key == sfMouseXButton1 + sfKeyCount)
	{
		CopyStingToBuffer(buffer, "MouseXButton1");
	}
	else if (_key == sfMouseXButton2 + sfKeyCount)
	{
		CopyStingToBuffer(buffer, "MouseXButton2");
	}
	else
	{
		return;
	}

	SetKeyToSave(_index, _key);
	sfText_setString(menu.key[_index], buffer);
}