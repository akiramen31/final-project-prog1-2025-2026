#include "Menu.h"
#include "Map.h"
#include "Weapons.h"
#include "LoadGameSave.h"

void KeyPressedMenu(sfEvent* _event);
void MouseButtonPressedMenu(sfMouseButtonEvent* _mouseButtonEvent);
void MouseMovedMenu(sfMouseMoveEvent* _mouseMovedEvent);
void SetMenuState(MenuState _state);
void UpdateTextKey(int _index, int _key);
int WhichMusicPlaying();
void NextMusic(void);

Menu menu;

void LoadMenu(void)
{
	//General
	menu = (Menu){ 0 };
	sfRenderWindow_setMouseCursorVisible(GetRenderWindow(), sfTrue);

	//Sprites
	menu.background = LoadBackground(GetAsset("Assets/Sprites/menu_background.png"), 8.f);
	sfSprite_setColor(menu.background, sfColor_fromRGB(200, 200, 200));
	menu.overlay = CreateSprite(GetAsset("Assets/Sprites/starting_menu_overlay.png"), (sfVector2f) { 0 }, 8.f, 70.f);
	//mainMenu
	menu.mainMenu.textBox[0] = GetAsset("Assets/Sprites/starting_menu_text1.png");
	menu.mainMenu.textBox[1] = GetAsset("Assets/Sprites/starting_menu_text2.png");
	menu.mainMenu.infoBox = CreateSprite(menu.mainMenu.textBox[0], (sfVector2f) { 0 }, 8.f, 40.f);
	menu.mainMenu.logo[0] = CreateSprite(GetAsset("Assets/Sprites/vinyl.png"), (sfVector2f) { 1504, 552 }, 8.f, 50.f);
	menu.mainMenu.logo[1] = CreateSprite(GetAsset("Assets/Sprites/title.png"), (sfVector2f) { 1504, 552 }, 8.f, 50.f);
	//selectionMenu
	sfTexture* tempButtons = GetAsset("Assets/Sprites/selection_menu_buttons.png");
	sfTexture* tempIcons = GetAsset("Assets/Sprites/selection_menu_icons.png");

	//Musics
	float volume = 0.5f;
	menu.musics[0] = CreateMusic("Assets/Musics/luis_humanoide-clockwork-adventure-steampunk-music-no-melody-288522.wav", volume, sfFalse);
	menu.musics[1] = CreateMusic("Assets/Musics/luis_humanoide-against-the-clock-steampunk-victorian-music-292800_1.wav", volume, sfFalse);
	menu.musics[2] = CreateMusic("Assets/Musics/luis_humanoide-steampunk-victorian-orchestra-183196_1.wav", volume, sfFalse);
	//menu.musics[2] = CreateMusic("Assets/Musics/luis_humanoide-steampunk-adventure-scherzo-with-sounds-316269_1.wav", volume, sfFalse);
	sfMusic_play(menu.musics[GetIntFromSave(CURRENT_MUSIC)]);

	//Texts
	menu.textColor = COLOR_ORANGE;
	menu.highlightTextColor = sfWhite;
	int textSizeLarge = 70;
	int textSizeMedium = 50;
	int textSizeSmall = 45;
	sfFont* font = GetAsset(FONT);
	if (GetFloatFromSave(LIGHT_LEVEL) < 0.25f)
	{
		SetFloatToSave(LIGHT_LEVEL, 1.f);
	}
	for (int i = 0; i < 2; i++)
	{
		SetSpriteOriginMiddle(menu.mainMenu.logo[i]);
	}
	sfVector2f positionTopButton[4] = { { 24, 8 }, { 336, 8 }, { 848, 8 },{ 1312, 8 } };
	for (int i = 0; i < 4; i++)
	{
		menu.mainMenu.topButtons[i] = CreateText(font, positionTopButton[i], textSizeLarge, 5.f);
		sfText_setColor(menu.mainMenu.topButtons[i], menu.textColor);
	}
	char* bufferKeyType[KEY_COUNT] = { "Jump" ,"Down","Right", "Left", "Shoot", "Dash", "Melee", "Second" };
	for (int i = 0; i < KEY_COUNT; i++)
	{
		menu.mainMenu.key[i] = CreateText(font, (sfVector2f) { 450, 342 + 55 * (float)i }, textSizeSmall, 5.f);
		menu.mainMenu.keyType[i] = CreateText(font, (sfVector2f) { 35, 342 + 55 * (float)i }, textSizeSmall, 5.f);
		sfText_setString(menu.mainMenu.keyType[i], bufferKeyType[i]);
		sfText_setColor(menu.mainMenu.keyType[i], menu.textColor);
		sfText_setColor(menu.mainMenu.key[i], menu.textColor);
	}
	menu.state = CONTROLS;
	for (int i = 0; i < KEY_COUNT; i++)
	{
		UpdateTextKey(i, GetKeyFromSave(i));
	}

	menu.mainMenu.name[0] = "AILLET A.";
	menu.mainMenu.name[1] = "MASSON Y.";
	menu.mainMenu.name[2] = "MEURISSE PEREZ B.";
	menu.mainMenu.name[3] = "PAGLIAZZO P.";
	menu.mainMenu.name[4] = "VOLLAIRE A.";

	for (int i = 0; i < MAX_INFO; i++)
	{
		menu.mainMenu.infoDisplay[i] = CreateText(font, (sfVector2f) { 50, 342 + 96 * (float)i }, textSizeLarge, 5.f);
		sfText_setColor(menu.mainMenu.infoDisplay[i], menu.textColor);
	}
	SetCurrentMap(0);
	SetWeapon(NO_WEAPON);
	SetSecondaryType(NO_SECONDARY);

	SetViewZoom(1.f);
	SetViewCenter((sfVector2f) { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 });

	LoadLoadGameSave(font);
	SetMenuState(STARTING_MENU);
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
		if (menu.state < 5)
		{
			SetMenuState(STARTING_MENU);
		}
		else
		{
			if (menu.state == 5)
			{
				SetMenuState(STARTING_MENU);
			}
			else
			{
				SetMenuState(menu.state - 1);
			}

		}
		return;
	default:
		break;
	}
	for (int i = 0; i < KEY_COUNT; i++)
	{
		if (CompareColor(sfText_getColor(menu.mainMenu.key[i]), menu.highlightTextColor))
		{
			UpdateTextKey(i, _event->key.code);
			return;
		}
	}
	KeyPressedLoadGameSave(_event);
}

void MouseButtonPressedMenu(sfMouseButtonEvent* _mouseButtonEvent)
{
	sfVector2f invisible = { 0 };
	sfVector2f visibleText = { 1,1 };
	if (menu.state == CONTROLS)
	{
		for (int i = 0; i < KEY_COUNT; i++)
		{
			if (CompareColor(sfText_getColor(menu.mainMenu.key[i]), menu.highlightTextColor))
			{
				UpdateTextKey(i, _mouseButtonEvent->button + sfKeyCount);
				return;
			}
		}
	}
	if (_mouseButtonEvent->button == sfMouseLeft)
	{
		if (CompareColor(sfText_getColor(menu.mainMenu.topButtons[3]), menu.highlightTextColor))
		{
			sfRenderWindow_close(GetRenderWindow());
		}
		else if (POW2((_mouseButtonEvent->x - 1504)) + POW2((_mouseButtonEvent->y - 552)) < POW2(400))
		{
			NextMusic();
		}
		else if (MouseButtonPressedLoadGameSave(_mouseButtonEvent))
		{
			SetGameState(MENU_SELECTION_GAME);
		}
		else
		{
			for (int i = 0; i < 4; i++)
			{
				if (CompareColor(sfText_getColor(menu.mainMenu.topButtons[i]), menu.highlightTextColor))
				{
					if (i + 1 == menu.state)
					{
						SetMenuState(STARTING_MENU);
						return;
					}
					else
					{
						switch (i)
						{
						case 0:
							SetMenuState(PLAY);
							return;
						case 1:
							SetMenuState(SETTINGS);
							return;
						case 2:
							SetMenuState(CREDITS);
							return;
						default:
							break;
						}
					}
				}
			}
		}
	}
}

void MouseMovedMenu(sfMouseMoveEvent* _mouseMovedEvent)
{
	float mouseX = (float)_mouseMovedEvent->x;
	float mouseY = (float)_mouseMovedEvent->y;
	sfVector2f posMouse = { _mouseMovedEvent->x , _mouseMovedEvent->y };
	for (int i = 0; i < 4; i++)
	{
		UpdateTextHighlightTextColor(menu.mainMenu.topButtons[i], menu.textColor, menu.highlightTextColor, posMouse);
	}
	for (int i = 0; i < NB_INFO_BUTTONS; i++)
	{
		UpdateTextHighlightTextColor(menu.mainMenu.infoDisplay[i], menu.textColor, menu.highlightTextColor, posMouse);
	}

	switch (menu.state)
	{
	case PLAY:
		MouseMovedLoadGameSave(_mouseMovedEvent);
		break;
	case SETTINGS:
		if (GetFloatFromSave(LIGHT_LEVEL) <= 0.25f)
		{
			sfText_setColor(menu.mainMenu.infoDisplay[0], sfRed);
		}
		if (GetFloatFromSave(SOUND_VOLUME) <= 0.0f)
		{
			sfText_setColor(menu.mainMenu.infoDisplay[1], sfRed);
		}
		break;
	case CONTROLS:
	{
		for (int i = 0; i < KEY_COUNT; i++)
		{
			int temp = UpdateTextHighlightTextColor(menu.mainMenu.key[i], menu.textColor, menu.highlightTextColor, posMouse) - UpdateTextHighlightTextColor(menu.mainMenu.keyType[i], menu.textColor, menu.highlightTextColor, posMouse);
			if (temp == -1)
			{
				sfText_setColor(menu.mainMenu.key[i], menu.highlightTextColor);
			}
			else if (temp == 1)
			{
				sfText_setColor(menu.mainMenu.keyType[i], menu.highlightTextColor);
			}
		}
		break;
	}
	case CREDITS:
		for (int i = 0; i < 5; i++)
		{
			UpdateTextHighlightTextColor(menu.mainMenu.infoDisplay[i], menu.textColor, menu.highlightTextColor, posMouse);
		}
		break;
	}
}

void SetMenuState(MenuState _state)
{
	sfVector2f invisible = { 0 };
	sfVector2f visibleText = { 1,1 };
	sfVector2f visibleSprite = { 8,8 };
	SetVisibleLoadGameSave(sfFalse);

	if (menu.state > 4)
	{
		//Loading the main menu
		sfSprite_setTexture(menu.overlay, GetAsset("Assets/Sprites/starting_menu_overlay.png"), sfFalse);
		for (int i = 0; i < 2; i++)
		{
			sfSprite_setScale(menu.mainMenu.logo[i], visibleSprite);
		}
		for (int i = 0; i < 4; i++)
		{
			sfText_setScale(menu.mainMenu.topButtons[i], visibleText);
		}
	}
	for (int i = 0; i < KEY_COUNT; i++)
	{
		sfText_setScale(menu.mainMenu.key[i], invisible);
		sfText_setScale(menu.mainMenu.keyType[i], invisible);
	}
	for (int i = 0; i < MAX_INFO; i++)
	{
		sfText_setScale(menu.mainMenu.infoDisplay[i], invisible);
	}
	sfText_setString(menu.mainMenu.topButtons[0], "Play");
	sfText_setString(menu.mainMenu.topButtons[1], "Setting");
	sfText_setString(menu.mainMenu.topButtons[2], "Credits");
	sfText_setString(menu.mainMenu.topButtons[3], "Quit");
	sfSprite_setScale(menu.mainMenu.infoBox, visibleSprite);

	menu.state = _state;
	sfVector2f tempPos = { 0 };
	switch (menu.state)
	{
	case PLAY:
		SetVisibleLoadGameSave(sfTrue);
		sfText_setString(menu.mainMenu.topButtons[0], "Back");
		break;
	case SETTINGS:
		sfText_setString(menu.mainMenu.topButtons[1], "Back");

		for (int i = 0; i < 4; i++)
		{
			sfText_setScale(menu.mainMenu.infoDisplay[i], visibleText);

			tempPos = (sfVector2f){ 40, 352 };
			tempPos.y += (432 / (4) * (i));
			sfText_setPosition(menu.mainMenu.infoDisplay[i], tempPos);
		}
		sfText_setString(menu.mainMenu.infoDisplay[0], "Light Level");
		sfText_setString(menu.mainMenu.infoDisplay[1], "Sound Volume");
		sfText_setString(menu.mainMenu.infoDisplay[2], "Full Sceen");
		sfText_setString(menu.mainMenu.infoDisplay[3], "Controls");
		if (GetFloatFromSave(LIGHT_LEVEL) <= 0.25f)
		{
			sfText_setColor(menu.mainMenu.infoDisplay[0], sfRed);
		}
		if (GetFloatFromSave(SOUND_VOLUME) <= 0.0f)
		{
			sfText_setColor(menu.mainMenu.infoDisplay[1], sfRed);
		}
		break;
	case CREDITS:
		sfText_setString(menu.mainMenu.topButtons[2], "Back");
		for (int i = 0; i < CREDIT_COUNT; i++)
		{
			sfText_setScale(menu.mainMenu.infoDisplay[i], visibleText);
			sfText_setString(menu.mainMenu.infoDisplay[i], menu.mainMenu.name[i]);

			tempPos = (sfVector2f){ 40, 352 };
			tempPos.y += (432 / (CREDIT_COUNT) * (i));
			sfText_setPosition(menu.mainMenu.infoDisplay[i], tempPos);
		}
		break;
	case CONTROLS:
		sfText_setString(menu.mainMenu.topButtons[1], "Back");
		for (int i = 0; i < KEY_COUNT; i++)
		{
			sfText_setScale(menu.mainMenu.key[i], visibleText);
			sfText_setScale(menu.mainMenu.keyType[i], visibleText);
		}
		break;
	case STARTING_MENU:
		sfSprite_setScale(menu.mainMenu.infoBox, invisible);
		break;
	default:
		break;
	}
}

void UpdateMenu(float _dt)
{
	if (menu.state < 5)
	{
		float rotation = sfSprite_getRotation(menu.mainMenu.logo[0]);
		if (rotation > 360)
		{
			rotation -= 360;
		}
		rotation += 5 * _dt;
		sfSprite_setRotation(menu.mainMenu.logo[0], rotation);
	}
	int currentMusic = WhichMusicPlaying();
	if (currentMusic == -1)
	{
		currentMusic = 0;
	}
	if (sfTime_asSeconds(sfMusic_getPlayingOffset(menu.musics[currentMusic])) > sfTime_asSeconds(sfMusic_getDuration(menu.musics[currentMusic])) - 1)
	{
		NextMusic();
	}
}

void UpdateTextKey(int _index, int _key)
{
	if (menu.state == CONTROLS)
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
			CopyStringToBuffer(buffer, "LControl");
		}
		else if (_key == sfKeyLShift)
		{
			CopyStringToBuffer(buffer, "LShift");
		}
		else if (_key == sfKeyLAlt)
		{
			CopyStringToBuffer(buffer, "LAlt");
		}
		else if (_key == sfKeyRControl)
		{
			CopyStringToBuffer(buffer, "RControl");
		}
		else if (_key == sfKeyRShift)
		{
			CopyStringToBuffer(buffer, "RShift");
		}
		else if (_key == sfKeyRAlt)
		{
			CopyStringToBuffer(buffer, "RAlt");
		}
		else if (_key == sfKeyLBracket)
		{
			CopyStringToBuffer(buffer, "LBracket");
		}
		else if (_key == sfKeyRBracket)
		{
			CopyStringToBuffer(buffer, "RBracket");
		}
		else if (_key == sfKeySemicolon)
		{
			CopyStringToBuffer(buffer, "Semicolon");
		}
		else if (_key == sfKeyComma)
		{
			CopyStringToBuffer(buffer, "Comma");
		}
		else if (_key == sfKeyPeriod)
		{
			CopyStringToBuffer(buffer, "Period");
		}
		else if (_key == sfKeyQuote)
		{
			CopyStringToBuffer(buffer, "Quote");
		}
		else if (_key == sfKeySlash)
		{
			CopyStringToBuffer(buffer, "Slash");
		}
		else if (_key == sfKeyBackslash)
		{
			CopyStringToBuffer(buffer, "Backslash");
		}
		else if (_key == sfKeyTilde)
		{
			CopyStringToBuffer(buffer, "Tilde");
		}
		else if (_key == sfKeyEqual)
		{
			CopyStringToBuffer(buffer, "Equal");
		}
		else if (_key == sfKeyHyphen)
		{
			CopyStringToBuffer(buffer, "Hyphen");
		}
		else if (_key == sfKeySpace)
		{
			CopyStringToBuffer(buffer, "Space");
		}
		else if (_key == sfKeyEnter)
		{
			CopyStringToBuffer(buffer, "Enter");
		}
		else if (_key == sfKeyBackspace)
		{
			CopyStringToBuffer(buffer, "Backspace");
		}
		else if (_key == sfKeyTab)
		{
			CopyStringToBuffer(buffer, "Tab");
		}
		else if (_key == sfKeyPageUp)
		{
			CopyStringToBuffer(buffer, "PageUp");
		}
		else if (_key == sfKeyPageDown)
		{
			CopyStringToBuffer(buffer, "PageDown");
		}
		else if (_key == sfKeyEnd)
		{
			CopyStringToBuffer(buffer, "End");
		}
		else if (_key == sfKeyHome)
		{
			CopyStringToBuffer(buffer, "Home");
		}
		else if (_key == sfKeyInsert)
		{
			CopyStringToBuffer(buffer, "Insert");
		}
		else if (_key == sfKeyDelete)
		{
			CopyStringToBuffer(buffer, "Delete");
		}
		else if (_key == sfKeyAdd)
		{
			CopyStringToBuffer(buffer, "Add");
		}
		else if (_key == sfKeySubtract)
		{
			CopyStringToBuffer(buffer, "Subtract");
		}
		else if (_key == sfKeyMultiply)
		{
			CopyStringToBuffer(buffer, "Multiply");
		}
		else if (_key == sfKeyDivide)
		{
			CopyStringToBuffer(buffer, "Divide");
		}
		else if (_key == sfKeyLeft)
		{
			CopyStringToBuffer(buffer, "Left");
		}
		else if (_key == sfKeyRight)
		{
			CopyStringToBuffer(buffer, "Right");
		}
		else if (_key == sfKeyUp)
		{
			CopyStringToBuffer(buffer, "Up");
		}
		else if (_key == sfKeyDown)
		{
			CopyStringToBuffer(buffer, "Down");
		}
		else if (_key > 74 && _key < 85)
		{
			CopyStringToBuffer(buffer, "Numpad");
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
			CopyStringToBuffer(buffer, "MouseLeft");
		}
		else if (_key == sfMouseRight + sfKeyCount)
		{
			CopyStringToBuffer(buffer, "MouseRight");
		}
		else if (_key == sfMouseMiddle + sfKeyCount)
		{
			CopyStringToBuffer(buffer, "MouseMiddle");
		}
		else if (_key == sfMouseXButton1 + sfKeyCount)
		{
			CopyStringToBuffer(buffer, "MouseXButton1");
		}
		else if (_key == sfMouseXButton2 + sfKeyCount)
		{
			CopyStringToBuffer(buffer, "MouseXButton2");
		}
		else
		{
			return;
		}

		SetKeyToSave(_index, _key);
		sfText_setString(menu.mainMenu.key[_index], buffer);
	}
}

int WhichMusicPlaying()
{
	for (int i = 0; i < MUSIC_COUNT; i++)
	{
		if (sfMusic_getStatus(menu.musics[i]) == sfPlaying)
		{
			return (i);
		}
	}
	return (-1);
}

void NextMusic()
{
	if (menu.state < 5)
	{
		int musicPlaying = WhichMusicPlaying();
		sfMusic_stop(menu.musics[musicPlaying]);
		sfMusic_play(menu.musics[(musicPlaying + 1) % MUSIC_COUNT]);
		SetIntToSave(CURRENT_MUSIC, (musicPlaying + 1) % MUSIC_COUNT);
		sfSprite_setRotation(menu.mainMenu.logo[0], 0);
	}
}