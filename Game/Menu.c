#include "Menu.h"

void KeyPressedMenu(sfEvent* _event);
void MouseButtonPressedMenu(sfMouseButtonEvent* _mouseButtonEvent);
void MouseMovedMenu(sfMouseMoveEvent* _mouseMovedEvent);
void SetMenuState(MenuState _state);
void UpdateTextKey(int _index, int _key);

Menu menu;

void LoadMenu(void)
{
	menu = (Menu){ 0 };

	if (GetFloatFromSave(LIGHT_LEVEL) < 0.25f)
	{
		SetFloatToSave(LIGHT_LEVEL, 1.f);
	}
	//Logo
	LoadBackground(GetAsset("Assets/Sprites/main_menu.png"), 8.f);
	menu.logo[0] = CreateSprite(GetAsset("Assets/Sprites/vinyl.png"), (sfVector2f) { 0 }, 8.f, 50.f);
	menu.logo[1] = CreateSprite(GetAsset("Assets/Sprites/cog.png"), (sfVector2f) { 0 }, 8.f, 50.f);
	menu.logo[2] = CreateSprite(GetAsset("Assets/Sprites/title.png"), (sfVector2f) { 0 }, 8.f, 50.f);
	for (int i = 0; i < 3; i++) {
		sfSprite_setOrigin(menu.logo[i], (sfVector2f) { 64, 64 });
		sfSprite_setPosition(menu.logo[i], (sfVector2f) { 1504, 552 });
	}

	sfFont* font = GetAsset("Assets/Fonts/Daydream.otf");
	//Top buttons
	for (int i = 0; i < NB_BUTTONS; i++)
	{
		menu.topButtons[i] = CreateText(font, (sfVector2f) { 0.f, 0.f }, 50, 5.f);
		switch (i)
		{
		case 0:
			sfText_setPosition(menu.topButtons[i], (sfVector2f) { 35, 23 });
			break;
		case 1:
			sfText_setPosition(menu.topButtons[i], (sfVector2f) { 332, 23 });
			break;
		case 2:
			sfText_setPosition(menu.topButtons[i], (sfVector2f) { 850, 23 });
			break;
		case 3:
			sfText_setPosition(menu.topButtons[i], (sfVector2f) { 1308, 23 });
			break;
		case 4:
			sfText_setPosition(menu.topButtons[i], (sfVector2f) { 1600, 23 });
			break;
		}
	}
	//Keybinds
	for (int i = 0; i < NB_KEY; i++)
	{
			menu.keyType[i] = CreateText(font, (sfVector2f) { 35, 342 + 55 * i }, 30, 5.f);
			menu.key[i] = CreateText(font, (sfVector2f) { 450, 342 + 55 * i }, 30, 5.f);
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
	//credits
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

	//infoDisplay
	for (int i = 0; i < MAX_INFO; i++)
	{
		menu.infoDisplay[i] = CreateText(font, (sfVector2f) { 50, 342 + 72 * i }, 50, 5.f);
	}

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
	for (int i = 0; i < NB_KEY; i++)
	{

		if (CompareColor(sfText_getColor(menu.key[i]), sfRed))
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
		if (CompareColor(sfText_getColor(menu.topButtons[3]), sfRed))
		{
			sfRenderWindow_close(GetRenderWindow());
		}
		switch (menu.state)
		{
		case MENU_BASE:
			for (int i = 0; i < NB_BUTTONS; i++)
			{
				if (CompareColor(sfText_getColor(menu.topButtons[i]), sfRed))
				{
					switch (i)
					{
					case 0:
						SetMenuState(PLAY);
						break;
					case 1:
						SetMenuState(SETTINGS);
						break;
					case 2:
						SetMenuState(CREDITS);
						break;
					}
				}
			}
			break;
		case PLAY:
			for (int i = 0; i < 3; i++)
			{
				if (CompareColor(sfText_getColor(menu.infoDisplay[i]), sfRed))
				{
					switch (i)
					{
					case 0:
						SetGameState(GAME);
						break;
					}
				}
			}
			for (int i = 0; i < NB_BUTTONS; i++)
			{
				if (CompareColor(sfText_getColor(menu.topButtons[i]), sfRed))
				{
					switch (i)
					{
					case 0:
						SetMenuState(MENU_BASE);
						break;
					case 1:
						SetMenuState(SETTINGS);
						break;
					case 2:
						SetMenuState(CREDITS);
						break;
					}
				}
			}
			break;
		case SETTINGS:
			for (int i = 0; i < 4; i++)
			{
				sfFloatRect hitbox = sfText_getGlobalBounds(menu.infoDisplay[i]);
				if (sfFloatRect_contains(&hitbox, _mouseButtonEvent->x, _mouseButtonEvent->y))
				{
					switch (i)
					{
					case 0:
						SetFloatToSave(LIGHT_LEVEL, ((_mouseButtonEvent->x - hitbox.left) / hitbox.width) * 0.75f + 0.25f);
						break;
					case 1:
						SetFloatToSave(SOUND_VOLUME, _mouseButtonEvent->x - hitbox.left);
						break;
					case 2:
						ChangeFullSceen();
						break;
					case 3:
						SetMenuState(CONTROLS);
						break;
					}
					return;
				}
			}
			for (int i = 0; i < NB_BUTTONS; i++)
			{
				if (CompareColor(sfText_getColor(menu.topButtons[i]), sfRed))
				{
					switch (i)
					{
					case 0:
						SetMenuState(PLAY);
						break;
					case 1:
						SetMenuState(MENU_BASE);
						break;
					case 2:
						SetMenuState(CREDITS);
						break;
					}
				}
			}
			break;
		case CONTROLS:
			for (int i = 0; i < NB_KEY; i++)
			{
				if (CompareColor(sfText_getColor(menu.key[i]), sfRed))
				{
					UpdateTextKey(i, _mouseButtonEvent->button + sfKeyCount);
					return;
				}
			}
			for (int i = 0; i < NB_BUTTONS; i++)
			{
				if (CompareColor(sfText_getColor(menu.topButtons[i]), sfRed))
				{
					switch (i)
					{
					case 0:
						SetMenuState(PLAY);
						break;
					case 1:
						SetMenuState(MENU_BASE);
						break;
					case 2:
						SetMenuState(CREDITS);
						break;
					}
				}
			}
			break;
		case CREDITS:
			for (int i = 0; i < NB_BUTTONS; i++)
			{
				if (CompareColor(sfText_getColor(menu.topButtons[i]), sfRed))
				{
					switch (i)
					{
					case 0:
						SetMenuState(PLAY);
						break;
					case 1:
						SetMenuState(SETTINGS);
						break;
					case 2:
						SetMenuState(MENU_BASE);
						break;
					}
				}
			}
			break;
		}
	}
	else if (_mouseButtonEvent->button == sfMouseRight)
	{
		for (int i = 0; i < NB_KEY; i++)
		{
			if (CompareColor(sfText_getColor(menu.key[i]), sfRed))
			{
				UpdateTextKey(i, _mouseButtonEvent->button + sfKeyCount);
				return;
			}
		}
	}
	else if (_mouseButtonEvent->button == sfMouseMiddle)
	{
		for (int i = 0; i < NB_KEY; i++)
		{
			if (CompareColor(sfText_getColor(menu.key[i]), sfRed))
			{
				UpdateTextKey(i, _mouseButtonEvent->button + sfKeyCount);
				return;
			}
		}
	}
}

void MouseMovedMenu(sfMouseMoveEvent* _mouseMovedEvent)
{
	sfFloatRect hitbox = { 0 };
	sfColor highlightColor = sfRed;
	sfColor baseColor = sfWhite;

	for (int i = 0; i < NB_BUTTONS; i++)
	{
		hitbox = sfText_getGlobalBounds(menu.topButtons[i]);
		if (sfFloatRect_contains(&hitbox, (float)_mouseMovedEvent->x, (float)_mouseMovedEvent->y))
		{
			sfText_setColor(menu.topButtons[i], highlightColor);
		}
		else
		{
			sfText_setColor(menu.topButtons[i], baseColor);
		}
	}
	switch (menu.state)
	{
	case PLAY:
		for (int i = 0; i < 4; i++)
		{
			hitbox = sfText_getGlobalBounds(menu.infoDisplay[i]);
			if (sfFloatRect_contains(&hitbox, (float)_mouseMovedEvent->x, (float)_mouseMovedEvent->y))
			{
				sfText_setColor(menu.infoDisplay[i], highlightColor);
			}
			else
			{
				sfText_setColor(menu.infoDisplay[i], baseColor);
			}
		}
		break;
	case SETTINGS:
		for (int i = 0; i < 4; i++)
		{
			hitbox = sfText_getGlobalBounds(menu.infoDisplay[i]);
			if (sfFloatRect_contains(&hitbox, (float)_mouseMovedEvent->x, (float)_mouseMovedEvent->y))
			{
				sfText_setColor(menu.infoDisplay[i], highlightColor);
			}
			else
			{
				sfText_setColor(menu.infoDisplay[i], baseColor);
			}
		}
		break;
	case CONTROLS:
	{
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
			sfText_setColor(menu.infoDisplay[0], highlightColor);
		}
		if (GetFloatFromSave(SOUND_VOLUME) <= 0.0f)
		{
			sfText_setColor(menu.infoDisplay[1], highlightColor);
		}
		break;
	}
	case CREDITS:
		for (int i = 0; i < NB_BUTTONS; i++)
		{
			hitbox = sfText_getGlobalBounds(menu.infoDisplay[i]);
			if (sfFloatRect_contains(&hitbox, (float)_mouseMovedEvent->x, (float)_mouseMovedEvent->y))
			{
				sfText_setColor(menu.infoDisplay[i], highlightColor);
				if (i < 5)
				{
					sfText_setString(menu.infoDisplay[i], menu.name[i + 5]);
				}
			}
			else
			{
				sfText_setColor(menu.infoDisplay[i], baseColor);
				if (i < 5)
				{
					sfText_setString(menu.infoDisplay[i], menu.name[i]);
				}
			}
		}
		break;
	}
}

void SetMenuState(MenuState _state)
{
	switch (menu.state)
	{
	case PLAY:
		sfText_setString(menu.topButtons[0], "Play");
		break;
	case SETTINGS:
		if (_state != CONTROLS)
		{
			sfText_setString(menu.topButtons[1], "Setting");
		}
		break;
	case CONTROLS:
		sfText_setString(menu.topButtons[1], "Setting");
		for (int i = 0; i < NB_KEY; i++)
		{
			sfText_setScale(menu.key[i], (sfVector2f) { 0 });
			sfText_setScale(menu.keyType[i], (sfVector2f) { 0 });
		}
		break;
	case  CREDITS:
		sfText_setString(menu.topButtons[2], "Credits");
		break;
	}
	menu.state = _state;
	switch (menu.state)
	{
	case MENU_BASE:
		sfText_setString(menu.topButtons[0], "Play");
		sfText_setString(menu.topButtons[1], "Setting");
		sfText_setString(menu.topButtons[2], "Credits");
		sfText_setString(menu.topButtons[3], "Quit");
		sfText_setString(menu.topButtons[4], "???");
		for (int i = 0; i < NB_KEY; i++)
		{
			sfText_setScale(menu.key[i], (sfVector2f) { 0 });
			sfText_setScale(menu.keyType[i], (sfVector2f) { 0 });
		}
		for (int i = 0; i < MAX_INFO; i++)
		{
			sfText_setScale(menu.infoDisplay[i], (sfVector2f) { 0 });
		}
		break;
	case PLAY:
		sfText_setString(menu.topButtons[0], "Back");
		for (int i = 0; i < 3; i++)
		{
			sfText_setScale(menu.infoDisplay[i], (sfVector2f) { 1, 1 });
		}
		sfText_setString(menu.infoDisplay[0], "New save");
		sfText_setString(menu.infoDisplay[1], "Load save");
		sfText_setString(menu.infoDisplay[2], "Delete save");
		break;
	case SETTINGS:
		sfText_setString(menu.topButtons[1], "Back");
		for (int i = 0; i < 4; i++)
		{
			sfText_setScale(menu.infoDisplay[i], (sfVector2f) { 1, 1 });
		}
		sfText_setString(menu.infoDisplay[0], "Light Level");
		sfText_setString(menu.infoDisplay[1], "Sound Volume");
		sfText_setString(menu.infoDisplay[2], "Full Sceen");
		sfText_setString(menu.infoDisplay[3], "Controls");
		if (GetFloatFromSave(LIGHT_LEVEL) <= 0.25f)
		{
			sfText_setColor(menu.infoDisplay[0], sfRed);
		}
		if (GetFloatFromSave(SOUND_VOLUME) <= 0.0f)
		{
			sfText_setColor(menu.infoDisplay[1], sfRed);
		}
		break;
	case CONTROLS:
		for (int i = 0; i < NB_KEY; i++)
		{
			sfText_setScale(menu.key[i], (sfVector2f) { 1, 1 });
			sfText_setScale(menu.keyType[i], (sfVector2f) { 1, 1 });
		}
		for (int i = 0; i < MAX_INFO; i++)
		{
			sfText_setScale(menu.infoDisplay[i], (sfVector2f) { 0 });
		}
		break;
	case CREDITS:
		sfText_setString(menu.topButtons[2], "Back");
		for (int i = 0; i < 5; i++)
		{
			sfText_setScale(menu.infoDisplay[i], (sfVector2f) { 1, 1 });
			sfText_setString(menu.infoDisplay[i], menu.name[i]);
		}
		break;
	}
}

void UpdateMenu(float _dt)
{
	float rotation = sfSprite_getRotation(menu.logo[1]);
	if (rotation > 360)
	{
		rotation -= 360;
	}
	rotation += 5 * _dt;
	sfSprite_setRotation(menu.logo[1], rotation);
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