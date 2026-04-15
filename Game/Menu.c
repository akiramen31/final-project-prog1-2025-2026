#include "Menu.h"

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

	//Texts
	menu.highlightTextColor = (sfColor){ 255, 165, 0 , 255 };
	menu.textColor = sfWhite;
	sfFont* font = GetAsset("Assets/Fonts/Daydream.otf");

	if (GetFloatFromSave(LIGHT_LEVEL) < 0.25f)
	{
		SetFloatToSave(LIGHT_LEVEL, 1.f);
	}
	//Sprites
	LoadBackground(GetAsset("Assets/Sprites/menu_background.png"), 8.f);
	menu.overlay = CreateSprite(GetAsset("Assets/Sprites/starting_menu_overlay.png"), (sfVector2f) { 0 }, 8.f, 60.f);
	//Top buttons
	sfVector2f positionTopButton[5] = { { 35, 23 }, { 332, 23 }, { 850, 23 },{ 1308, 23 }, { 1600, 23 } };
	for (int i = 0; i < NB_TOP_BUTTONS; i++)
	{
		menu.topButtons[i] = CreateText(font, positionTopButton[i], 50, 5.f);
	}
	//Keybinds
	char* bufferKeyType[NB_KEY] = { "Jump" ,"Down","Right", "Left", "Shoot", "Dash", "Melee", "Second" };
	for (int i = 0; i < NB_KEY; i++)
	{
		menu.keyType[i] = CreateText(font, (sfVector2f) { 35, 342 + 55 * (float)i }, 30, 5.f);
		menu.key[i] = CreateText(font, (sfVector2f) { 450, 342 + 55 * (float)i }, 30, 5.f);
		sfText_setString(menu.keyType[i], bufferKeyType[i]);
	}
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
	menu.name[5] = "AILLET";
	menu.name[6] = "MEURISSE_PEREZ";
	menu.name[7] = "VOLLAIRE";
	menu.name[8] = "MASSON";
	menu.name[9] = "PAGLIAZZO";

	//infoDisplay
	for (int i = 0; i < MAX_INFO; i++)
	{
		menu.infoDisplay[i] = CreateText(font, (sfVector2f) { 50, 342 + 72 * (float)i }, 50, 5.f);
	}
	//Musics
	menu.musics[0] = CreateMusic("Assets/Musics/1914-Its_A_Long_Way_To_Tipperary.ogg", 10.f, sfFalse);
	menu.musics[1] = CreateMusic("Assets/Musics/1914-United_Forces_March.ogg", 10.f, sfFalse);
	menu.musics[2] = CreateMusic("Assets/Musics/1915-Dont_Bite_The_Hand_Thats_Feeding_You.ogg", 10.f, sfFalse);
	menu.musics[3] = CreateMusic("Assets/Musics/1917-Oh_Johnny,_Oh_Johnny,_Oh.ogg", 10.f, sfFalse);
	menu.musics[4] = CreateMusic("Assets/Musics/1917-Over_There.ogg", 10.f, sfFalse);

	sfMusic_play(menu.musics[rand() % NB_MUSICS]);

	SetViewZoom(1.f);
	SetViewCenter((sfVector2f) { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 });

	//Main
	//Logo
	menu.mainMenu.logo[0] = CreateSprite(GetAsset("Assets/Sprites/vinyl.png"), (sfVector2f) { 1504, 552 }, 8.f, 50.f);
	menu.mainMenu.logo[1] = CreateSprite(GetAsset("Assets/Sprites/cog.png"), (sfVector2f) { 1504, 552 }, 8.f, 50.f);
	menu.mainMenu.logo[2] = CreateSprite(GetAsset("Assets/Sprites/title.png"), (sfVector2f) { 1504, 552 }, 8.f, 50.f);
	sfSprite_setTexture(menu.overlay, GetAsset("Assets/Sprites/starting_menu_overlay.png"), sfFalse);

	for (int i = 0; i < 3; i++)
	{
		SetSpriteOriginMiddel(menu.mainMenu.logo[i]);
	}

	//Top buttons
	sfVector2f positionTopButton[5] = { { 32, 23 }, { 332, 23 }, { 850, 23 },{ 1308, 23 }, { 1600, 23 } };
	for (int i = 0; i < 5; i++)
	{
		menu.mainMenu.topButtons[i] = CreateText(font, positionTopButton[i], 50, 5.f);
	}
	//Keybinds
	char* bufferKeyType[NB_KEY] = { "Jump" ,"Down","Right", "Left", "Shoot", "Dash", "Melee", "Second" };
	for (int i = 0; i < NB_KEY; i++)
	{
		menu.mainMenu.keyType[i] = CreateText(font, (sfVector2f) { 35, 342 + 55 * (float)i }, 30, 5.f);
		menu.mainMenu.key[i] = CreateText(font, (sfVector2f) { 450, 342 + 55 * (float)i }, 30, 5.f);
		sfText_setString(menu.mainMenu.keyType[i], bufferKeyType[i]);
	}

	for (int i = 0; i < NB_KEY; i++)
	{
		UpdateTextKey(i, GetKeyFromSave(i));
	}
	//credits
	menu.mainMenu.name[0] = "Akira";
	menu.mainMenu.name[1] = "Benjamin";
	menu.mainMenu.name[2] = "Alice";
	menu.mainMenu.name[3] = "Ylix";
	menu.mainMenu.name[4] = "Pierre";
	menu.mainMenu.name[5] = "AILLET";
	menu.mainMenu.name[6] = "MEURISSE PEREZ";
	menu.mainMenu.name[7] = "VOLLAIRE";
	menu.mainMenu.name[8] = "MASSON";
	menu.mainMenu.name[9] = "PAGLIAZZO";

	//infoDisplay
	for (int i = 0; i < MAX_INFO; i++)
	{
		menu.mainMenu.infoDisplay[i] = CreateText(font, (sfVector2f) { 50, 342 + 72 * (float)i }, 50, 5.f);
	}

	//Selection
	sfTexture* tempButtons = GetAsset("Assets/Sprites/selection_menu_buttons.png");
	sfTexture* tempIcons = GetAsset("Assets/Sprites/selection_menu_icons.png");
	sfVector2f buttonPos[3] = { {25, 300}, {25, 400 }, {25, 600} };
	sfIntRect buttonRect = { 56, 76, 18, 18 };
	sfIntRect iconRect[3] = { {0,16,16,16},{16,16,16,16},{32,16,16,16} };

	sfVector2f temp[2] = { {830, 840}, {1410, 840} };
	for (int i = 0; i < 2; i++)
	{
		menu.selectionMenu.bottomText[i] = CreateText(font, temp[i], 60, 5.f);
		sfText_setLetterSpacing(menu.selectionMenu.bottomText[i], 11.5f);
		sfText_setScale(menu.selectionMenu.bottomText[i], (sfVector2f) { 0 });
	}
	sfText_setString(menu.selectionMenu.bottomText[0], "next");
	sfText_setString(menu.selectionMenu.bottomText[1], "back");

	for (int i = 0; i < 3; i++)
	{
		menu.selectionMenu.sideButton[i] = CreateSprite(tempButtons, (sfVector2f) { 72, 376 + 256 * i }, 0.f, 50.f);
		menu.selectionMenu.sideIcon[i] = CreateSprite(tempIcons, (sfVector2f) { 72, 376 + 256 * i }, 0.f, 50.f);
		sfSprite_setTextureRect(menu.selectionMenu.sideButton[i], buttonRect);
		sfSprite_setTextureRect(menu.selectionMenu.sideIcon[i], iconRect[i]);
		sfSprite_setOrigin(menu.selectionMenu.sideButton[i], (sfVector2f) { 9, 9 });
		sfSprite_setOrigin(menu.selectionMenu.sideIcon[i], (sfVector2f) { 8, 8 });
	}


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
		sfRenderWindow_close(GetRenderWindow());
		return;
	default:
		break;
	}
	for (int i = 0; i < NB_KEY; i++)
	{

		if (CompareColor(sfText_getColor(menu.mainMenu.key[i]), menu.highlightTextColor))
		{
			UpdateTextKey(i, _event->key.code);
			return;
		}
	}
}

void MouseButtonPressedMenu(sfMouseButtonEvent* _mouseButtonEvent)
{
	if (menu.state == CONTROLS)
	{
		for (int i = 0; i < NB_KEY; i++)
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
		if (menu.state < 5)
		{
			if (CompareColor(sfText_getColor(menu.mainMenu.topButtons[3]), menu.highlightTextColor))
			{
				sfRenderWindow_close(GetRenderWindow());
			}
			else if (POW2((_mouseButtonEvent->x - 1504)) + POW2((_mouseButtonEvent->y - 552)) < POW2(400))
			{
				NextMusic();
			}
			for (int i = 0; i < 5; i++)
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
							return;
						}
					}
				}
			}
		}
		else
		{
			if (menu.state != SELECTION_MAP)
			{
				sfIntRect temp = sfSprite_getTextureRect(menu.selectionMenu.sideIcon[0]);
				if (temp.top == 16)
				{
					SetMenuState(SELECTION_MAP);
				}
			}
			if (menu.state != SELECTION_WEAPON)
			{
				sfIntRect temp = sfSprite_getTextureRect(menu.selectionMenu.sideIcon[1]);
				if (temp.top == 16)
				{
					SetMenuState(SELECTION_WEAPON);
				}
			}
			if (menu.state != SELECTION_BONUS)
			{
				sfIntRect temp = sfSprite_getTextureRect(menu.selectionMenu.sideIcon[2]);
				if (temp.top == 16)
				{
					SetMenuState(SELECTION_BONUS);
				}
			}
			if (CompareColor(sfText_getColor(menu.selectionMenu.bottomText[0]), menu.highlightTextColor))
			{
				if (menu.state == SELECTION_BONUS)
				{
					SetGameState(GAME);
				}
				else
				{
					SetMenuState(menu.state + 1);
				}
			}
			if (CompareColor(sfText_getColor(menu.selectionMenu.bottomText[1]), menu.highlightTextColor))
			{
				if (menu.state = 5)
				{
					SetMenuState(STARTING_MENU);
				}
				else
				{
					SetMenuState(menu.state - 1);
				}
			}
		}
		switch (menu.state)
		{
		case PLAY:
			for (int i = 0; i < 3; i++)
			{
				if (CompareColor(sfText_getColor(menu.mainMenu.infoDisplay[i]), menu.highlightTextColor))
				{
					switch (i)
					{
					case 0:
						SetMenuState(SELECTION_MAP);
						break;
					default:
						break;
					}
					return;
				}
			}
			break;
		case SETTINGS:
			for (int i = 0; i < 4; i++)
			{
				sfFloatRect hitbox = sfText_getGlobalBounds(menu.mainMenu.infoDisplay[i]);
				if (sfFloatRect_contains(&hitbox, (float)_mouseButtonEvent->x, (float)_mouseButtonEvent->y))
				{
					switch (i)
					{
					case 0:
						SetFloatToSave(LIGHT_LEVEL, (((float)_mouseButtonEvent->x - hitbox.left) / hitbox.width) * 0.75f + 0.25f);
						break;
					case 1:
						SetFloatToSave(SOUND_VOLUME, (float)_mouseButtonEvent->x - hitbox.left);
						break;
					case 2:
						ChangeFullSceen();
						break;
					case 3:
						SetMenuState(CONTROLS);
						break;
					default:
						break;
					}
					return;
				}
			}
			break;
		case CREDITS:
			break;
		case SELECTION_MAP:

			break;
		case SELECTION_WEAPON:

			break;
		case SELECTION_BONUS:

			break;
		}
	}
}

void MouseMovedMenu(sfMouseMoveEvent* _mouseMovedEvent)
{
	sfFloatRect hitbox = { 0 };
	sfColor highlightColor = menu.highlightTextColor;
	sfColor baseColor = menu.textColor;
	if (menu.state < 5)
	{
		for (int i = 0; i < 5; i++)
		{
			hitbox = sfText_getGlobalBounds(menu.mainMenu.topButtons[i]);
			if (sfFloatRect_contains(&hitbox, (float)_mouseMovedEvent->x, (float)_mouseMovedEvent->y))
			{
				sfText_setColor(menu.mainMenu.topButtons[i], highlightColor);
			}
			else
			{
				sfText_setColor(menu.mainMenu.topButtons[i], baseColor);
			}
		}
		for (int i = 0; i < NB_INFO_BUTTONS; i++)
		{
			hitbox = sfText_getGlobalBounds(menu.mainMenu.infoDisplay[i]);
			if (sfFloatRect_contains(&hitbox, (float)_mouseMovedEvent->x, (float)_mouseMovedEvent->y))
			{
				sfText_setColor(menu.mainMenu.infoDisplay[i], highlightColor);
			}
			else
			{
				sfText_setColor(menu.mainMenu.infoDisplay[i], baseColor);
			}
		}
	}
	else
	{
		for (int i = 0; i < 2; i++)
		{
			hitbox = sfText_getGlobalBounds(menu.selectionMenu.bottomText[i]);
			if (sfFloatRect_contains(&hitbox, (float)_mouseMovedEvent->x, (float)_mouseMovedEvent->y))
			{
				sfText_setColor(menu.selectionMenu.bottomText[i], highlightColor);
			}
			else
			{
				sfText_setColor(menu.selectionMenu.bottomText[i], baseColor);
			}
		}
		for (int i = 0; i < 3; i++)
		{
			if (menu.state - 5 != i)
			{
				hitbox = sfSprite_getGlobalBounds(menu.selectionMenu.sideButton[i]);
				if (sfFloatRect_contains(&hitbox, (float)_mouseMovedEvent->x, (float)_mouseMovedEvent->y))
				{
					sfSprite_setTextureRect(menu.selectionMenu.sideIcon[i], (sfIntRect) { 16 * i, 16, 16, 16 });
				}
				else
				{
					sfSprite_setTextureRect(menu.selectionMenu.sideIcon[i], (sfIntRect) { 16 * i, 0, 16, 16 });
				}
			}
		}

	}

	switch (menu.state)
	{
	case PLAY:

		break;
	case SETTINGS:
		if (GetFloatFromSave(LIGHT_LEVEL) <= 0.25f)
		{
			sfText_setColor(menu.mainMenu.infoDisplay[0], highlightColor);
		}
		if (GetFloatFromSave(SOUND_VOLUME) <= 0.0f)
		{
			sfText_setColor(menu.mainMenu.infoDisplay[1], highlightColor);
		}
		break;
	case CONTROLS:
	{
		sfFloatRect hitbox2 = { 0 };
		for (int i = 0; i < NB_KEY; i++)
		{
			hitbox = sfText_getGlobalBounds(menu.mainMenu.key[i]);
			hitbox2 = sfText_getGlobalBounds(menu.mainMenu.keyType[i]);
			if (sfFloatRect_contains(&hitbox, (float)_mouseMovedEvent->x, (float)_mouseMovedEvent->y) || sfFloatRect_contains(&hitbox2, (float)_mouseMovedEvent->x, (float)_mouseMovedEvent->y))
			{
				sfText_setColor(menu.mainMenu.key[i], highlightColor);
				sfText_setColor(menu.mainMenu.keyType[i], highlightColor);
			}
			else
			{
				sfText_setColor(menu.mainMenu.key[i], baseColor);
				sfText_setColor(menu.mainMenu.keyType[i], baseColor);
			}
		}
		break;
	}
	case CREDITS:
		for (int i = 0; i < 5; i++)
		{
			hitbox = sfText_getGlobalBounds(menu.mainMenu.infoDisplay[i]);
			if (sfFloatRect_contains(&hitbox, (float)_mouseMovedEvent->x, (float)_mouseMovedEvent->y))
			{
				sfText_setColor(menu.mainMenu.infoDisplay[i], highlightColor);
				if (i < 5)
				{
					sfText_setString(menu.mainMenu.infoDisplay[i], menu.mainMenu.name[i + 5]);
				}
			}
			else
			{
				sfText_setColor(menu.mainMenu.infoDisplay[i], baseColor);
				if (i < 5)
				{
					sfText_setString(menu.mainMenu.infoDisplay[i], menu.mainMenu.name[i]);
				}
			}
		}
		break;
	}
}

void SetMenuState(MenuState _state)
{
	sfVector2f invisible = { 0 };
	sfVector2f visibleText = { 1,1 };
	sfVector2f visibleSprite = { 8,8 };
	if (_state < 5)
	{
		if (menu.state > 4)
		{
			//Unloading the selection menu
			for (int i = 0; i < 3; i++)
			{
				sfSprite_setScale(menu.selectionMenu.sideButton[i], invisible);
				sfSprite_setScale(menu.selectionMenu.sideIcon[i], invisible);
			}
			for (int i = 0; i < 2; i++)
			{
				sfText_setScale(menu.selectionMenu.bottomText[i], invisible);
			}
			//Loading the main menu
			sfSprite_setTexture(menu.overlay, GetAsset("Assets/Sprites/starting_menu_overlay.png"), NULL);
			for (int i = 0; i < 3; i++)
			{
				sfSprite_setScale(menu.mainMenu.logo[i], visibleSprite);
			}
			for (int i = 0; i < 5; i++)
			{
				sfText_setScale(menu.mainMenu.topButtons[i], visibleText);
			}
		}
		for (int i = 0; i < NB_KEY; i++)
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
		sfText_setString(menu.mainMenu.topButtons[4], "");

		menu.state = _state;
		switch (menu.state)
		{
		case PLAY:
			sfText_setString(menu.mainMenu.topButtons[0], "Back");
			for (int i = 0; i < 3; i++)
			{
				sfText_setScale(menu.mainMenu.infoDisplay[i], visibleText);
			}
			sfText_setString(menu.mainMenu.infoDisplay[0], "New Game");
			sfText_setString(menu.mainMenu.infoDisplay[1], "Load Save");
			sfText_setString(menu.mainMenu.infoDisplay[2], "Delete Save");
			break;
		case SETTINGS:
			sfText_setString(menu.mainMenu.topButtons[1], "Back");
			for (int i = 0; i < 4; i++)
			{
				sfText_setScale(menu.mainMenu.infoDisplay[i], visibleText);
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
			for (int i = 0; i < 5; i++)
			{
				sfText_setScale(menu.mainMenu.infoDisplay[i], visibleText);
				sfText_setString(menu.mainMenu.infoDisplay[i], menu.mainMenu.name[i]);
			}
			break;
		case CONTROLS:
			sfText_setString(menu.mainMenu.topButtons[1], "Back");
			for (int i = 0; i < NB_KEY; i++)
			{
				sfText_setScale(menu.mainMenu.key[i], visibleText);
				sfText_setScale(menu.mainMenu.keyType[i], visibleText);
			}
			break;
		default:
			break;
		}
	}
	else
	{
		if (menu.state < 5)
		{
			//Unloading the main menu
			for (int i = 0; i < 3; i++)
			{
				sfSprite_setScale(menu.mainMenu.logo[i], invisible);
			}
			for (int i = 0; i < NB_KEY; i++)
			{
				sfText_setScale(menu.mainMenu.key[i], invisible);
				sfText_setScale(menu.mainMenu.keyType[i], invisible);
			}
			for (int i = 0; i < MAX_INFO; i++)
			{
				sfText_setScale(menu.mainMenu.infoDisplay[i], invisible);
			}
			for (int i = 0; i < 5; i++)
			{
				sfText_setScale(menu.mainMenu.topButtons[i], invisible);
			}
			//Loading the selection menu
			sfSprite_setTexture(menu.overlay, GetAsset("Assets/Sprites/selection_menu_overlay.png"), NULL);
			for (int i = 0; i < 3; i++)
			{
				sfSprite_setScale(menu.selectionMenu.sideButton[i], visibleSprite);
				sfSprite_setScale(menu.selectionMenu.sideIcon[i], visibleSprite);
			}
			for (int i = 0; i < 2; i++)
			{
				sfText_setScale(menu.selectionMenu.bottomText[i], visibleText);
			}
		}
		sfIntRect unselected = { 56, 76, 18, 18 };
		sfIntRect selected = { 74, 76, 18, 18 };
		switch (menu.state)
		{
		case SELECTION_MAP:
			sfSprite_setTextureRect(menu.selectionMenu.sideButton[0], unselected);
			sfSprite_setTextureRect(menu.selectionMenu.sideIcon[0], (sfIntRect) { 0, 0, 16, 16 });
			break;
		case SELECTION_WEAPON:
			sfSprite_setTextureRect(menu.selectionMenu.sideButton[1], unselected);
			sfSprite_setTextureRect(menu.selectionMenu.sideIcon[1], (sfIntRect) { 16, 0, 16, 16 });
			break;
		case SELECTION_BONUS:
			sfSprite_setTextureRect(menu.selectionMenu.sideButton[2], unselected);
			sfSprite_setTextureRect(menu.selectionMenu.sideIcon[2], (sfIntRect) { 32, 0, 16, 16 });
			sfText_setString(menu.selectionMenu.bottomText[0], "next");
			sfText_setLetterSpacing(menu.selectionMenu.bottomText[0], 11.5f);
			break;
		}
		menu.state = _state;
		switch (menu.state)
		{
		case SELECTION_MAP:
			sfSprite_setTextureRect(menu.selectionMenu.sideButton[0], selected);
			sfSprite_setTextureRect(menu.selectionMenu.sideIcon[0], (sfIntRect) { 0, 16, 16, 16 });
			break;
		case SELECTION_WEAPON:
			sfSprite_setTextureRect(menu.selectionMenu.sideButton[1], selected);
			sfSprite_setTextureRect(menu.selectionMenu.sideIcon[1], (sfIntRect) { 16, 16, 16, 16 });
			break;
		case SELECTION_BONUS:
			sfSprite_setTextureRect(menu.selectionMenu.sideButton[2], selected);
			sfSprite_setTextureRect(menu.selectionMenu.sideIcon[2], (sfIntRect) { 32, 16, 16, 16 });
			sfText_setString(menu.selectionMenu.bottomText[0], "Play");
			sfText_setLetterSpacing(menu.selectionMenu.bottomText[0], 13.5f);
			break;
		}
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
	sfText_setString(menu.mainMenu.key[_index], buffer);
}

int WhichMusicPlaying()
{
	for (int i = 0; i < NB_MUSICS; i++)
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
	int musicPlaying = WhichMusicPlaying();
	sfMusic_stop(menu.musics[musicPlaying]);
	if (musicPlaying == NB_MUSICS - 1)
	{
		sfMusic_play(menu.musics[0]);
	}
	else
	{
		sfMusic_play(menu.musics[musicPlaying + 1]);
	}
	if (menu.state < 5)
	{
		sfSprite_setRotation(menu.mainMenu.logo[0], 0);
	}
}