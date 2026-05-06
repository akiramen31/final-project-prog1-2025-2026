#include "Menu.h"
#include "Map.h"
#include "Weapons.h"

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
	for (int i = 0; i < 3; i++)
	{
		menu.selectionMenu.categoryButton[i] = CreateSprite(tempButtons, (sfVector2f) { 72.f, 376.f + 256.f * i }, 0.f, 60.f);
		menu.selectionMenu.categoryIcon[i] = CreateSprite(tempIcons, (sfVector2f) { 72.f, 376.f + 256.f * i }, 0.f, 50.f);
		sfSprite_setTextureRect(menu.selectionMenu.categoryButton[i], (sfIntRect) { 0, 0, 18, 18 });
		sfSprite_setTextureRect(menu.selectionMenu.categoryIcon[i], (sfIntRect) { 16 * i, 16, 16, 16 });
		SetSpriteOriginMiddle(menu.selectionMenu.categoryButton[i]);
		SetSpriteOriginMiddle(menu.selectionMenu.categoryIcon[i]);
	}
	for (int line = 0; line < 4; line++)
	{
		for (int column = 0; column < 3; column++)
		{
			menu.selectionMenu.textureRect[(line * 3) + column] = (sfIntRect){ column * 56, line * 18, 56, 18 };
			printf("Texture n°%d:   { %d, %d, %d, %d}\n", (line * 3)+column, column * 56, line * 18, 56, 18);
		}
	}

	for (int i = 0; i < WEAPON_COUNT; i++)
	{
		menu.selectionMenu.generalButton[i] = CreateSprite(GetAsset("Assets/Sprites/selection_menu_sprites.png"), (sfVector2f) { 0 }, 0.f, 60.f);
		menu.selectionMenu.generalIcon[i] = CreateSprite(GetAsset("Assets/Sprites/selection_menu_sprites.png"), (sfVector2f) { 0 }, 0.f, 50.f);
		sfSprite_setTextureRect(menu.selectionMenu.generalButton[i], menu.selectionMenu.textureRect[UNSELECT]);
		sfSprite_setTextureRect(menu.selectionMenu.generalIcon[i], menu.selectionMenu.textureRect[WEAPON_1]);
		SetSpriteOriginMiddle(menu.selectionMenu.generalButton[i]);
		SetSpriteOriginMiddle(menu.selectionMenu.generalIcon[i]);
	}

	//Musics
	float volume = 0.5f;
	menu.musics[0] = CreateMusic("Assets/Musics/1914_Its_A_Long_Way_To_Tipperary.ogg", volume, sfFalse);
	menu.musics[1] = CreateMusic("Assets/Musics/1914_United_Forces_March.ogg", volume, sfFalse);
	menu.musics[2] = CreateMusic("Assets/Musics/1915_Dont_Bite_The_Hand_Thats_Feeding_You.ogg", volume, sfFalse);
	menu.musics[3] = CreateMusic("Assets/Musics/1917_Oh_Johnny,_Oh_Johnny,_Oh.ogg", volume, sfFalse);
	menu.musics[4] = CreateMusic("Assets/Musics/1917_Over_There.ogg", volume, sfFalse);
	sfMusic_play(menu.musics[GetIntFromSave(CURRENT_MUSIC)]);

	//Texts
	//menu.highlightTextColor = (sfColor){ 255, 165, 0 , 255 };
	menu.textColor = (sfColor){ 255, 165, 0 , 255 };
	menu.highlightTextColor = sfWhite;
	sfFont* font = GetAsset("Assets/Fonts/Daydream.otf");
	if (GetFloatFromSave(LIGHT_LEVEL) < 0.25f)
	{
		SetFloatToSave(LIGHT_LEVEL, 1.f);
	}
	for (int i = 0; i < 2; i++)
	{
		SetSpriteOriginMiddle(menu.mainMenu.logo[i]);
	}
	sfVector2f positionTopButton[5] = { { 32, 23 }, { 332, 23 }, { 850, 23 },{ 1308, 23 }, { 1600, 23 } };
	for (int i = 0; i < 5; i++)
	{
		menu.mainMenu.topButtons[i] = CreateText(font, positionTopButton[i], 50, 5.f);
	}
	char* bufferKeyType[KEY_COUNT] = { "Jump" ,"Down","Right", "Left", "Shoot", "Dash", "Melee", "Second" };
	for (int i = 0; i < KEY_COUNT; i++)
	{
		menu.mainMenu.keyType[i] = CreateText(font, (sfVector2f) { 35, 342 + 55 * (float)i }, 30, 5.f);
		menu.mainMenu.key[i] = CreateText(font, (sfVector2f) { 450, 342 + 55 * (float)i }, 30, 5.f);
		sfText_setString(menu.mainMenu.keyType[i], bufferKeyType[i]);
	}
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
		menu.mainMenu.infoDisplay[i] = CreateText(font, (sfVector2f) { 50, 342 + 72 * (float)i }, 50, 5.f);
	}
	sfVector2f temp[2] = { {830, 840}, {1410, 840} };
	for (int i = 0; i < 2; i++)
	{
		menu.selectionMenu.bottomText[i] = CreateText(font, temp[i], 60, 5.f);
		sfText_setLetterSpacing(menu.selectionMenu.bottomText[i], 11.5f);
		sfText_setScale(menu.selectionMenu.bottomText[i], (sfVector2f) { 0 });
	}
	sfText_setString(menu.selectionMenu.bottomText[0], "next");
	sfText_setString(menu.selectionMenu.bottomText[1], "back");

	if (DEV_MODE)
	{
		SetIntToSave(UNLOCKED_LEVEL, 0);
		SetIntToSave(UNLOCKED_WEAPON, 0);
		SetIntToSave(UNLOCKED_SECONDARY, 0);
		for (int i = 0; i < 3; i++)
		{
			AddIntToSave(UNLOCKED_LEVEL, pow(10, i));
		}
		for (int i = 0; i < WEAPON_COUNT; i++)
		{
			AddIntToSave(UNLOCKED_WEAPON, pow(10, i));
		}
		for (int i = 0; i < SECONDARY_COUNT; i++)
		{
			AddIntToSave(UNLOCKED_SECONDARY, pow(10, i));
		}
	}
	SetCurrentMap(-1);
	SetWeapon(-1);
	SetSecondaryType(-1);

	SetViewZoom(1.f);
	SetViewCenter((sfVector2f) { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 });
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
}

void MouseButtonPressedMenu(sfMouseButtonEvent* _mouseButtonEvent)
{
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
				sfIntRect temp = sfSprite_getTextureRect(menu.selectionMenu.categoryButton[0]);
				if (temp.left == 18)
				{
					SetMenuState(SELECTION_MAP);
				}
			}
			if (menu.state != SELECTION_WEAPON)
			{
				sfIntRect temp = sfSprite_getTextureRect(menu.selectionMenu.categoryButton[1]);
				if (temp.left == 18)
				{
					SetMenuState(SELECTION_WEAPON);
				}
			}
			if (menu.state != SELECTION_BONUS)
			{
				sfIntRect temp = sfSprite_getTextureRect(menu.selectionMenu.categoryButton[2]);
				if (temp.left == 18)
				{
					SetMenuState(SELECTION_BONUS);
				}
			}
			if (CompareColor(sfText_getColor(menu.selectionMenu.bottomText[0]), menu.highlightTextColor))
			{
				if (menu.state == SELECTION_BONUS)
				{
					if (GetCurrentMap()==-1)
					{
						SetCurrentMap(0);
					}
					if (GetWeapon().weaponType==-1)
					{
						SetWeapon(0);
					}
					if (GetSecondaryType()==-1)
					{
						SetSecondaryType(0);
					}
					SetGameState(GAME);
				}
				else
				{
					SetMenuState(menu.state + 1);
				}
			}
			else if (CompareColor(sfText_getColor(menu.selectionMenu.bottomText[1]), menu.highlightTextColor))
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
		}
		sfIntRect temp;
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
					case 1:
						SetIntToSave(UNLOCKED_LEVEL, 0);
						SetIntToSave(UNLOCKED_WEAPON, 1);
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
						ChangeVolume((float)_mouseButtonEvent->x - hitbox.left);
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
			for (int i = 0; i < 3; i++)
			{
				temp = sfSprite_getTextureRect(menu.selectionMenu.generalButton[i]);
				if (CompareIntRect(temp, menu.selectionMenu.textureRect[HIGHLIGHT]))
				{
					sfSprite_setTextureRect(menu.selectionMenu.generalButton[i], menu.selectionMenu.textureRect[SELECT]);
					SetCurrentMap(i);
					for (int j = 1; j < MAP_COUNT; j++)
					{
						sfSprite_setTextureRect(menu.selectionMenu.generalButton[(i + j) % 4], menu.selectionMenu.textureRect[UNSELECT]);
					}
				}
			}
			break;
		case SELECTION_WEAPON:
			for (int i = 0; i < WEAPON_COUNT; i++)
			{
				temp = sfSprite_getTextureRect(menu.selectionMenu.generalButton[i]);
				if (CompareIntRect(temp, menu.selectionMenu.textureRect[HIGHLIGHT]))
				{
					sfSprite_setTextureRect(menu.selectionMenu.generalButton[i], menu.selectionMenu.textureRect[SELECT]);
					SetWeapon(i);
					for (int j = 1; j < WEAPON_COUNT; j++)
					{
						sfSprite_setTextureRect(menu.selectionMenu.generalButton[(i + j) % 4], menu.selectionMenu.textureRect[UNSELECT]);
					}
				}
			}
			break;
		case SELECTION_BONUS:
		{
			for (int i = 0; i < SECONDARY_COUNT; i++)
			{
				temp = sfSprite_getTextureRect(menu.selectionMenu.generalButton[i]);
				if (CompareIntRect(temp, menu.selectionMenu.textureRect[HIGHLIGHT]))
				{
					sfSprite_setTextureRect(menu.selectionMenu.generalButton[i], menu.selectionMenu.textureRect[SELECT]);
					SetSecondaryType(i);
					for (int j = 1; j < SECONDARY_COUNT; j++)
					{
						sfSprite_setTextureRect(menu.selectionMenu.generalButton[(i + j) % 4], menu.selectionMenu.textureRect[UNSELECT]);
					}
				}
			}
		}
		break;
		}
	}
}

void MouseMovedMenu(sfMouseMoveEvent* _mouseMovedEvent)
{
	float mouseX = (float)_mouseMovedEvent->x;
	float mouseY = (float)_mouseMovedEvent->y;
	sfFloatRect hitbox;
	sfColor highlightColor = menu.highlightTextColor;
	sfColor baseColor = menu.textColor;
	if (menu.state < 5)
	{
		for (int i = 0; i < 5; i++)
		{
			hitbox = sfText_getGlobalBounds(menu.mainMenu.topButtons[i]);
			if (sfFloatRect_contains(&hitbox, mouseX, mouseY))
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
			if (sfFloatRect_contains(&hitbox, mouseX, mouseY))
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
			if (sfFloatRect_contains(&hitbox, mouseX, mouseY))
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
				hitbox = sfSprite_getGlobalBounds(menu.selectionMenu.categoryButton[i]);
				if (sfFloatRect_contains(&hitbox, mouseX, mouseY))
				{
					sfSprite_setTextureRect(menu.selectionMenu.categoryButton[i], (sfIntRect) { 18, 0, 18, 18 });
				}
				else
				{
					sfSprite_setTextureRect(menu.selectionMenu.categoryButton[i], (sfIntRect) { 0, 0, 18, 18 });
				}
			}
		}
		sfIntRect temp;
		if (menu.state == SELECTION_MAP)
		{
			for (int i = 0; i < MAP_COUNT; i++)
			{
				hitbox = sfSprite_getGlobalBounds(menu.selectionMenu.generalButton[i]);
				temp = sfSprite_getTextureRect(menu.selectionMenu.generalButton[i]);
				if (CompareIntRect(temp, menu.selectionMenu.textureRect[UNSELECT]) || CompareIntRect(temp, menu.selectionMenu.textureRect[HIGHLIGHT]))
				{
					if (sfFloatRect_contains(&hitbox, mouseX, mouseY))
					{
						sfSprite_setTextureRect(menu.selectionMenu.generalButton[i], menu.selectionMenu.textureRect[HIGHLIGHT]);
					}
					else
					{
						sfSprite_setTextureRect(menu.selectionMenu.generalButton[i], menu.selectionMenu.textureRect[UNSELECT]);
					}
				}
			}
		}
		else if (menu.state == SELECTION_WEAPON)
		{
			for (int i = 0; i < WEAPON_COUNT; i++)
			{
				hitbox = sfSprite_getGlobalBounds(menu.selectionMenu.generalButton[i]);
				temp = sfSprite_getTextureRect(menu.selectionMenu.generalButton[i]);
				if (CompareIntRect(temp, menu.selectionMenu.textureRect[UNSELECT]) || CompareIntRect(temp, menu.selectionMenu.textureRect[HIGHLIGHT]))
				{
					if (sfFloatRect_contains(&hitbox, mouseX, mouseY))
					{
						sfSprite_setTextureRect(menu.selectionMenu.generalButton[i], menu.selectionMenu.textureRect[HIGHLIGHT]);
					}
					else
					{
						sfSprite_setTextureRect(menu.selectionMenu.generalButton[i], menu.selectionMenu.textureRect[UNSELECT]);
					}
				}
			}
		}
		else if (menu.state == SELECTION_BONUS)
		{
			for (int i = 0; i < SECONDARY_COUNT; i++)
			{
				hitbox = sfSprite_getGlobalBounds(menu.selectionMenu.generalButton[i]);
				sfIntRect temp = sfSprite_getTextureRect(menu.selectionMenu.generalButton[i]);
				if (CompareIntRect(temp, menu.selectionMenu.textureRect[UNSELECT]) || CompareIntRect(temp, menu.selectionMenu.textureRect[HIGHLIGHT]))
				{
					if (sfFloatRect_contains(&hitbox, mouseX, mouseY))
					{
						sfSprite_setTextureRect(menu.selectionMenu.generalButton[i], menu.selectionMenu.textureRect[HIGHLIGHT]);
					}
					else
					{
						sfSprite_setTextureRect(menu.selectionMenu.generalButton[i], menu.selectionMenu.textureRect[UNSELECT]);
					}
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
		for (int i = 0; i < KEY_COUNT; i++)
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
			}
			else
			{
				sfText_setColor(menu.mainMenu.infoDisplay[i], baseColor);
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
				sfSprite_setScale(menu.selectionMenu.categoryButton[i], invisible);
				sfSprite_setScale(menu.selectionMenu.categoryIcon[i], invisible);
			}
			for (int i = 0; i < 2; i++)
			{
				sfText_setScale(menu.selectionMenu.bottomText[i], invisible);
			}
			for (int i = 0; i < WEAPON_COUNT; i++)
			{
				sfSprite_setScale(menu.selectionMenu.generalButton[i], invisible);
				sfSprite_setScale(menu.selectionMenu.generalIcon[i], invisible);
			}
			//Loading the main menu
			sfSprite_setTexture(menu.overlay, GetAsset("Assets/Sprites/starting_menu_overlay.png"), sfFalse);
			for (int i = 0; i < 2; i++)
			{
				sfSprite_setScale(menu.mainMenu.logo[i], visibleSprite);
			}
			for (int i = 0; i < 5; i++)
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
		sfText_setString(menu.mainMenu.topButtons[4], "");
		sfSprite_setScale(menu.mainMenu.infoBox, visibleSprite);

		menu.state = _state;
		switch (menu.state)
		{
		case PLAY:
			sfText_setString(menu.mainMenu.topButtons[0], "Back");
			for (int i = 0; i < 2; i++)
			{
				sfText_setScale(menu.mainMenu.infoDisplay[i], visibleText);
			}
			sfText_setString(menu.mainMenu.infoDisplay[0], "Load Save");
			sfText_setString(menu.mainMenu.infoDisplay[1], "New Save");
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
	else
	{
		if (menu.state < 5)
		{
			//Unloading the main menu
			for (int i = 0; i < 2; i++)
			{
				sfSprite_setScale(menu.mainMenu.logo[i], invisible);
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
			for (int i = 0; i < 5; i++)
			{
				sfText_setScale(menu.mainMenu.topButtons[i], invisible);
			}
			sfSprite_setScale(menu.mainMenu.infoBox, invisible);
			//Loading the selection menu
			sfSprite_setTexture(menu.overlay, GetAsset("Assets/Sprites/selection_menu_overlay.png"), sfFalse);
			for (int i = 0; i < 3; i++)
			{
				sfSprite_setScale(menu.selectionMenu.categoryButton[i], visibleSprite);
				sfSprite_setScale(menu.selectionMenu.categoryIcon[i], visibleSprite);
			}
			for (int i = 0; i < 2; i++)
			{
				sfText_setScale(menu.selectionMenu.bottomText[i], visibleText);
			}
			for (int i = 0; i < WEAPON_COUNT; i++)
			{
				sfSprite_setScale(menu.selectionMenu.generalButton[i], visibleSprite);
				sfSprite_setScale(menu.selectionMenu.generalIcon[i], visibleSprite);
			}
		}

		for (int i = 0; i < WEAPON_COUNT; i++)
		{
			sfSprite_setScale(menu.selectionMenu.generalButton[i], invisible);
			sfSprite_setScale(menu.selectionMenu.generalIcon[i], invisible);
		}
		switch (menu.state)
		{
		case SELECTION_MAP:
			sfSprite_setTextureRect(menu.selectionMenu.categoryButton[0], (sfIntRect) { 0, 0, 18, 18 });
			sfSprite_setTextureRect(menu.selectionMenu.categoryIcon[0], (sfIntRect) { 0, 16, 16, 16 });
			break;
		case SELECTION_WEAPON:
			sfSprite_setTextureRect(menu.selectionMenu.categoryButton[1], (sfIntRect) { 0, 0, 18, 18 });
			sfSprite_setTextureRect(menu.selectionMenu.categoryIcon[1], (sfIntRect) { 16, 16, 16, 16 });
			break;
		case SELECTION_BONUS:
			sfSprite_setTextureRect(menu.selectionMenu.categoryButton[2], (sfIntRect) { 0, 0, 18, 18 });
			sfSprite_setTextureRect(menu.selectionMenu.categoryIcon[2], (sfIntRect) { 32, 16, 16, 16 });
			sfText_setString(menu.selectionMenu.bottomText[0], "next");
			sfText_setLetterSpacing(menu.selectionMenu.bottomText[0], 11.5f);
			break;
		}
		menu.state = _state;
		switch (menu.state)
		{
		case SELECTION_MAP:;
			sfSprite_setTextureRect(menu.selectionMenu.categoryButton[0], (sfIntRect) { 36, 0, 18, 18 });
			sfSprite_setTextureRect(menu.selectionMenu.categoryIcon[0], (sfIntRect) { 0, 0, 16, 16 });
			for (int i = 0; i < MAP_COUNT; i++)
			{
				sfSprite_setScale(menu.selectionMenu.generalButton[i], visibleSprite);
				if (GetCurrentMap() == i)
				{
					sfSprite_setTextureRect(menu.selectionMenu.generalButton[i], menu.selectionMenu.textureRect[SELECT]);
				}
				else
				{
					sfSprite_setTextureRect(menu.selectionMenu.generalButton[i], menu.selectionMenu.textureRect[UNSELECT]);
				}
				int unlockedLevel = GetIntFromSave(UNLOCKED_LEVEL);
				if ((unlockedLevel % ((int)pow(10, i + 1)) - unlockedLevel % (int)(pow(10, i))) / (int)pow(10, i))
				{
					sfSprite_setScale(menu.selectionMenu.generalIcon[i], visibleSprite);
					sfSprite_setTextureRect(menu.selectionMenu.generalIcon[i], menu.selectionMenu.textureRect[3*i]);
				}
				else
				{
					sfSprite_setTextureRect(menu.selectionMenu.generalButton[i], menu.selectionMenu.textureRect[LOCKED]);
					sfSprite_setScale(menu.selectionMenu.generalIcon[i], invisible);
				}
				sfVector2f tempPos = { 512, 56 };
				tempPos.y += (768 / (MAP_COUNT + 1) * (i+1));
				sfSprite_setPosition(menu.selectionMenu.generalButton[i], tempPos);
				sfSprite_setPosition(menu.selectionMenu.generalIcon[i], tempPos);
			}
			break;
		case SELECTION_WEAPON:
			sfSprite_setTextureRect(menu.selectionMenu.categoryButton[1], (sfIntRect) { 36, 0, 18, 18 });
			sfSprite_setTextureRect(menu.selectionMenu.categoryIcon[1], (sfIntRect) { 16, 0, 16, 16 });
			for (int i = 0; i < WEAPON_COUNT; i++)
			{
				sfSprite_setScale(menu.selectionMenu.generalButton[i], visibleSprite);
				if (GetWeapon().weaponType == i)
				{
					sfSprite_setTextureRect(menu.selectionMenu.generalButton[i], menu.selectionMenu.textureRect[SELECT]);
				}
				else
				{
					sfSprite_setTextureRect(menu.selectionMenu.generalButton[i], menu.selectionMenu.textureRect[UNSELECT]);
				}
				int unlockedWeapon = GetIntFromSave(UNLOCKED_WEAPON);
				if ((unlockedWeapon % ((int)pow(10, i + 1)) - unlockedWeapon % (int)(pow(10, i))) / (int)pow(10, i))
				{
					sfSprite_setScale(menu.selectionMenu.generalIcon[i], visibleSprite);
					sfSprite_setTextureRect(menu.selectionMenu.generalIcon[i], menu.selectionMenu.textureRect[1+(3*i)]);
				}
				else
				{
					sfSprite_setTextureRect(menu.selectionMenu.generalButton[i], menu.selectionMenu.textureRect[LOCKED]);
					sfSprite_setScale(menu.selectionMenu.generalIcon[i], invisible);
				}
				sfVector2f tempPos = { 512, 56 };
				tempPos.y += (768 / (WEAPON_COUNT + 1) * (i+1));
				sfSprite_setPosition(menu.selectionMenu.generalButton[i], tempPos);
				sfSprite_setPosition(menu.selectionMenu.generalIcon[i], tempPos);
			}
			break;
		case SELECTION_BONUS:
			sfSprite_setTextureRect(menu.selectionMenu.categoryButton[2], (sfIntRect) {36,0,18,18});
			sfSprite_setTextureRect(menu.selectionMenu.categoryIcon[2], (sfIntRect) { 32, 0, 16, 16 });
			sfText_setString(menu.selectionMenu.bottomText[0], "Play");
			sfText_setLetterSpacing(menu.selectionMenu.bottomText[0], 13.5f);
			for (int i = 0; i < SECONDARY_COUNT; i++)
			{
				sfSprite_setScale(menu.selectionMenu.generalButton[i], visibleSprite);
				if (GetSecondaryType() == i)
				{
					sfSprite_setTextureRect(menu.selectionMenu.generalButton[i], menu.selectionMenu.textureRect[SELECT]);
				}
				else
				{
					sfSprite_setTextureRect(menu.selectionMenu.generalButton[i], menu.selectionMenu.textureRect[UNSELECT]);
				}
				int unlockedSecondary = GetIntFromSave(UNLOCKED_SECONDARY);
				if ((unlockedSecondary % ((int)pow(10, i + 1)) - unlockedSecondary % (int)(pow(10, i))) / (int)pow(10, i))
				{
					sfSprite_setScale(menu.selectionMenu.generalIcon[i], visibleSprite);
					sfSprite_setTextureRect(menu.selectionMenu.generalIcon[i], menu.selectionMenu.textureRect[9+(i*1)]);
				}
				else
				{
					sfSprite_setTextureRect(menu.selectionMenu.generalButton[i], menu.selectionMenu.textureRect[LOCKED]);
					sfSprite_setScale(menu.selectionMenu.generalIcon[i], invisible);
				}
				sfVector2f tempPos = { 512, 56 };
				tempPos.y += (768 / (SECONDARY_COUNT + 1) * (i+1));
				sfSprite_setPosition(menu.selectionMenu.generalButton[i], tempPos);
				sfSprite_setPosition(menu.selectionMenu.generalIcon[i], tempPos);
			}
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