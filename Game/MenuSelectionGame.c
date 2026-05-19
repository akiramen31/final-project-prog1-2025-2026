#include "MenuSelectionGame.h"

void KeyPressedMenuSelectionGame(sfEvent* _event);
void MouseButtonPressedMenuSelectionGame(sfMouseButtonEvent* _mouseButtonEvent);
void MouseMovedMenuSelectionGame(sfMouseMoveEvent* _mouseMovedEvent);
void SetMenuSelectionGame(MenuSelectionGameState _state);

#define RECT_BOTTON(buttonRect) (sfIntRect){ ((buttonRect) % 3) * 56, ((buttonRect) / 3) * 18, 56, 18 }
#define COLOR_HIGHLIGHT sfWhite
#define COLOR_BASE COLOR_ORANGE
MenuSelectionGame selectionMenu;

void LoadMenuSelectionGame(void)
{
	selectionMenu = (MenuSelectionGame){ 0 };
	LoadBackground(GetAsset("Assets/Sprites/menu_background.png"), 8.f);
	CreateSprite(GetAsset("Assets/Sprites/selection_menu_overlay.png"), (sfVector2f) { 0 }, 8.f, 1000.f);
	for (int i = 0; i < 3; i++)
	{
		selectionMenu.categoryButton[i] = CreateSprite(GetAsset("Assets/Sprites/selection_menu_buttons.png"), (sfVector2f) { 72.f, 376.f + 256.f * i }, 8.f, 60.f);
		selectionMenu.categoryIcon[i] = CreateSprite(GetAsset("Assets/Sprites/selection_menu_icons.png"), (sfVector2f) { 72.f, 376.f + 256.f * i }, 8.f, 50.f);
		sfSprite_setTextureRect(selectionMenu.categoryButton[i], (sfIntRect) { 0, 0, 18, 18 });
		sfSprite_setTextureRect(selectionMenu.categoryIcon[i], (sfIntRect) { 16 * i, 16, 16, 16 });
		SetSpriteOriginMiddle(selectionMenu.categoryButton[i]);
		SetSpriteOriginMiddle(selectionMenu.categoryIcon[i]);
	}
	for (int i = 0; i < WEAPON_COUNT; i++)
	{
		selectionMenu.generalButton[i] = CreateSprite(GetAsset("Assets/Sprites/selection_menu_sprites.png"), (sfVector2f) { 0 }, 8.f, 60.f);
		selectionMenu.generalIcon[i] = CreateSprite(GetAsset("Assets/Sprites/selection_menu_sprites.png"), (sfVector2f) { 0 }, 8.f, 50.f);
		sfSprite_setTextureRect(selectionMenu.generalButton[i], RECT_BOTTON(UNSELECT));
		sfSprite_setTextureRect(selectionMenu.generalIcon[i], RECT_BOTTON(WEAPON_1));
		SetSpriteOriginMiddle(selectionMenu.generalButton[i]);
		SetSpriteOriginMiddle(selectionMenu.generalIcon[i]);
	}

	//Musics
	int curentMusic = GetIntFromSave(CURRENT_MUSIC);
	if (curentMusic == 0)
	{
		CreateMusic("Assets/Musics/luis_humanoide-clockwork-adventure-steampunk-music-no-melody-288522.wav", 0.5f, sfTrue);
	}
	else if (curentMusic == 1)
	{
		CreateMusic("Assets/Musics/luis_humanoide-against-the-clock-steampunk-victorian-music-292800_1.wav", 0.5f, sfTrue);
	}
	else if (curentMusic == 2)
	{
		CreateMusic("Assets/Musics/luis_humanoide-steampunk-victorian-orchestra-183196_1.wav", 0.5f, sfTrue);
	}

	selectionMenu.description[0][0] = "The new cog on the block\n\n\nWalk 'round the\nfactories and find yer\nway to the docks.";
	selectionMenu.description[0][1] = "Savery in the sky with\nballons\n\n\nNow on a flagship, ye\nmust reach the top to\nensure ye can leave\nwhen it docks.";
	selectionMenu.description[0][2] = "Unused level\n";
	selectionMenu.description[1][0] = "Railgun\n\nDamage=\n\nThe gun of the common\nman, shoots fast bullets\nthat can take out\nmost foes.";
	selectionMenu.description[1][1] = "Steam powered axe\n\nDamage=\n\nThanks to steam\npropulsion axes are back\non the menu! With now\nboosted hits it can shred\nall yer foes.";
	selectionMenu.description[1][2] = "Mi-steel launcher\n\nDamage=\n\nShoots a semi-steel beam\nthat sticks in walls,\nshould it hit one.";
	selectionMenu.description[2][0] = "Kamikaze drone\n\nWhile pretty costly in\nsteam, this drone can\nreach niches and blow\nthem up for ye!";
	selectionMenu.description[2][1] = "Coldbreath\n\nHelp your ennemies\n\"cool\" down with this\ncold projectile.";
	selectionMenu.description[2][2] = "";
	selectionMenu.descriptionText = CreateText(GetAsset(FONT), (sfVector2f) { 936, 96 }, 50, 5.f);

	selectionMenu.bottomText[0] = CreateText(GetAsset(FONT), (sfVector2f) { 830, 840 }, 70, 5.f);
	selectionMenu.bottomText[1] = CreateText(GetAsset(FONT), (sfVector2f) { 1410, 840 }, 70, 5.f);
	sfText_setLetterSpacing(selectionMenu.bottomText[0], 11.5f);
	sfText_setLetterSpacing(selectionMenu.bottomText[1], 11.5f);
	sfText_setColor(selectionMenu.bottomText[0], COLOR_BASE);
	sfText_setColor(selectionMenu.bottomText[1], COLOR_BASE);
	sfText_setString(selectionMenu.bottomText[0], "next");
	sfText_setString(selectionMenu.bottomText[1], "menu");

	SetCurrentMap(0);
	SetWeapon(0);
	SetSecondaryType(0);

	SetMenuSelectionGame(MAP);
}

void PollEventMenuSelectionGame(sfEvent* _event)
{
	switch (_event->type)
	{
	case sfEvtKeyPressed:
		KeyPressedMenuSelectionGame(_event);
		break;
	case sfEvtMouseButtonPressed:
		MouseButtonPressedMenuSelectionGame(&_event->mouseButton);
		break;
	case sfEvtMouseMoved:
		MouseMovedMenuSelectionGame(&_event->mouseMove);
		break;
	default:
		break;
	}
}

void KeyPressedMenuSelectionGame(sfEvent* _event)
{
	if (_event->key.code == sfKeyEscape)
	{
		SetGameState(MENU);
	}
}

void MouseButtonPressedMenuSelectionGame(sfMouseButtonEvent* _mouseButtonEvent)
{
	if (_mouseButtonEvent->button == sfMouseLeft)
	{
		sfVector2f posMouse = { _mouseButtonEvent->x, _mouseButtonEvent->y };
		sfFloatRect hitbox = { 0 };
		sfIntRect temp = { 0 };

		if (CompareColor(sfText_getColor(selectionMenu.bottomText[0]), COLOR_HIGHLIGHT))
		{
			if (selectionMenu.state == SECONRARY)
			{
				SetGameState(GAME);
			}
			else
			{
				SetMenuSelectionGame(selectionMenu.state + 1);
			}
			return;
		}
		else if (CompareColor(sfText_getColor(selectionMenu.bottomText[1]), COLOR_HIGHLIGHT))
		{
			if (selectionMenu.state == MAP)
			{
				SetGameState(MENU);
			}
			else
			{
				SetMenuSelectionGame(selectionMenu.state - 1);
			}
			return;
		}

		for (int i = 0; i < 3; i++)
		{
			hitbox = sfSprite_getGlobalBounds(selectionMenu.categoryButton[i]);
			if (sfFloatRect_contains(&hitbox, _mouseButtonEvent->x, _mouseButtonEvent->y))
			{
				SetMenuSelectionGame(i);
				return;
			}
		}

		for (int i = 0; i < MAP_COUNT; i++)
		{
			if (CompareIntRect(sfSprite_getTextureRect(selectionMenu.generalButton[i]), RECT_BOTTON(HIGHLIGHT)))
			{
				sfSprite_setTextureRect(selectionMenu.generalButton[i], RECT_BOTTON(SELECT));
				if (selectionMenu.state == MAP)
				{
					SetCurrentMap(i);
				}
				else if (selectionMenu.state == WEAPON)
				{
					SetWeapon(i);
				}
				else if (selectionMenu.state == SECONRARY)
				{
					SetSecondaryType(i);
				}
				for (int j = 0; j < MAP_COUNT; j++)
				{
					if (j != i && CompareIntRect(sfSprite_getTextureRect(selectionMenu.generalButton[i]), RECT_BOTTON(SELECT)))
					{
						sfSprite_setTextureRect(selectionMenu.generalButton[i], RECT_BOTTON(UNSELECT));
					}
				}
				return;
			}
			else
			{
				hitbox = sfSprite_getGlobalBounds(selectionMenu.categoryButton[i]);
				if (sfFloatRect_contains(&hitbox, _mouseButtonEvent->x, _mouseButtonEvent->y) && CompareIntRect(sfSprite_getTextureRect(selectionMenu.generalButton[i]), RECT_BOTTON(SELECT)))
				{
					sfSprite_setTextureRect(selectionMenu.generalButton[i], RECT_BOTTON(UNSELECT));
					if (selectionMenu.state == MAP)
					{
						SetCurrentMap(MAP_1);
					}
					else if (selectionMenu.state == WEAPON)
					{
						SetWeapon(NO_WEAPON);
					}
					else if (selectionMenu.state == SECONRARY)
					{
						SetSecondaryType(NO_SECONDARY);
					}
				}
			}
		}
	}
}

void MouseMovedMenuSelectionGame(sfMouseMoveEvent* _mouseMovedEvent)
{
	sfVector2f posMouse = { _mouseMovedEvent->x, _mouseMovedEvent->y };
	sfFloatRect hitbox = { 0 };
	sfIntRect temp = { 0 };

	for (int i = 0; i < 2; i++)
	{
		UpdateTextHighlightTextColor(selectionMenu.bottomText[i], COLOR_BASE, COLOR_HIGHLIGHT, posMouse);
	}
	for (int i = 0; i < 3; i++)
	{
		if (selectionMenu.state != i)
		{
			hitbox = sfSprite_getGlobalBounds(selectionMenu.categoryButton[i]);
			sfSprite_setTextureRect(selectionMenu.categoryButton[i], (sfIntRect) { 18 * sfFloatRect_contains(&hitbox, _mouseMovedEvent->x, _mouseMovedEvent->y), 0, 18, 18 });
		}
	}

	for (int i = 0; i < MAP_COUNT; i++)
	{
		temp = sfSprite_getTextureRect(selectionMenu.generalButton[i]);
		if (CompareIntRect(temp, RECT_BOTTON(UNSELECT)) || CompareIntRect(temp, RECT_BOTTON(HIGHLIGHT)))
		{
			hitbox = sfSprite_getGlobalBounds(selectionMenu.generalButton[i]);
			if (sfFloatRect_contains(&hitbox, _mouseMovedEvent->x, _mouseMovedEvent->y))
			{
				sfSprite_setTextureRect(selectionMenu.generalButton[i], RECT_BOTTON(HIGHLIGHT));
			}
			else
			{
				sfSprite_setTextureRect(selectionMenu.generalButton[i], RECT_BOTTON(UNSELECT));
			}
		}
	}
}

void SetMenuSelectionGame(MenuSelectionGameState _state)
{
	int temp = 0;
	sfVector2f tempPos = { 512, 56 };
	GameData* gameData = GetGameData();
	selectionMenu.state = _state;

	for (int i = 0; i < 3; i++)
	{
		if (selectionMenu.state == i)
		{
			sfSprite_setTextureRect(selectionMenu.categoryButton[i], (sfIntRect) { 36, 0, 18, 18 });
		}
		else
		{
			sfSprite_setTextureRect(selectionMenu.categoryButton[i], (sfIntRect) { 0, 0, 18, 18 });
		}
	}

	if (selectionMenu.state == MAP)
	{
		temp = GetCurrentMap();
		sfText_setString(selectionMenu.descriptionText, selectionMenu.description[selectionMenu.state][temp]);
		sfSprite_setTextureRect(selectionMenu.generalButton[temp - 1], RECT_BOTTON(SELECT));
		for (int i = 0; i < MAP_COUNT; i++)
		{
			tempPos.y = 56 + (768 / (MAP_COUNT + 1) * (i + 1));
			sfSprite_setPosition(selectionMenu.generalButton[i], tempPos);
			sfSprite_setPosition(selectionMenu.generalIcon[i], tempPos);
			if (gameData->levelUnlock > i)
			{
				sfSprite_setTextureRect(selectionMenu.generalButton[i], RECT_BOTTON(UNSELECT));
				sfSprite_setTextureRect(selectionMenu.generalIcon[i], RECT_BOTTON(i * 3));
			}
			else
			{
				sfSprite_setTextureRect(selectionMenu.generalButton[i], RECT_BOTTON(LOCKED));
				sfSprite_setTextureRect(selectionMenu.generalIcon[i], (sfIntRect) { 0 });
			}
		}
		sfText_setString(selectionMenu.descriptionText, selectionMenu.description[selectionMenu.state][temp]);
		sfSprite_setTextureRect(selectionMenu.generalButton[temp], RECT_BOTTON(SELECT));
	}
	else if (selectionMenu.state == WEAPON)
	{
		temp = GetWeapon().weaponType;
		if (temp)
		{
			sfText_setString(selectionMenu.descriptionText, selectionMenu.description[selectionMenu.state][temp - 1]);
			sfSprite_setTextureRect(selectionMenu.generalButton[temp - 1], RECT_BOTTON(SELECT));
		}
		else
		{
			sfText_setString(selectionMenu.descriptionText, selectionMenu.description[2][2]);
		}
		for (int i = 0; i < WEAPON_COUNT; i++)
		{
			tempPos.y = 56 + (768 / (WEAPON_COUNT + 1) * (i + 1));
			sfSprite_setPosition(selectionMenu.generalButton[i], tempPos);
			sfSprite_setPosition(selectionMenu.generalIcon[i], tempPos);
			if ((gameData->weaponUnlock >> i) & 1)
			{
				sfSprite_setTextureRect(selectionMenu.generalButton[i], RECT_BOTTON(UNSELECT));
				sfIntRect rect = RECT_BOTTON(i * 3 + 1);
				sfSprite_setTextureRect(selectionMenu.generalIcon[i], RECT_BOTTON(i * 3 + 1));
			}
			else
			{
				sfSprite_setTextureRect(selectionMenu.generalButton[i], RECT_BOTTON(LOCKED));
				sfSprite_setTextureRect(selectionMenu.generalIcon[i], (sfIntRect) { 0 });
			}
		}
		sfText_setString(selectionMenu.descriptionText, selectionMenu.description[selectionMenu.state][temp]);
		sfSprite_setTextureRect(selectionMenu.generalButton[temp], RECT_BOTTON(SELECT));
	}
	else if (selectionMenu.state == SECONRARY)
	{
		temp = GetSecondaryType();
		if (temp)
		{
			sfText_setString(selectionMenu.descriptionText, selectionMenu.description[selectionMenu.state][temp - 1]);
			sfSprite_setTextureRect(selectionMenu.generalButton[temp - 1], RECT_BOTTON(SELECT));
		}
		else
		{
			sfText_setString(selectionMenu.descriptionText, selectionMenu.description[2][2]);
		}
		sfSprite_setTextureRect(selectionMenu.generalButton[2], (sfIntRect) { 0 });
		sfSprite_setTextureRect(selectionMenu.generalIcon[2], (sfIntRect) { 0 });
		for (int i = 0; i < SECONDARY_COUNT; i++)
		{
			tempPos.y = 56 + (768 / (SECONDARY_COUNT + 1) * (i + 1));
			sfSprite_setPosition(selectionMenu.generalButton[i], tempPos);
			sfSprite_setPosition(selectionMenu.generalIcon[i], tempPos);
			if ((gameData->secondaryUnlock >> i) & 1)
			{
				sfSprite_setTextureRect(selectionMenu.generalButton[i], RECT_BOTTON(UNSELECT));
				sfSprite_setTextureRect(selectionMenu.generalIcon[i], RECT_BOTTON(i + 9));
			}
			else
			{
				sfSprite_setTextureRect(selectionMenu.generalIcon[i], (sfIntRect) { 0 });
				sfSprite_setTextureRect(selectionMenu.generalButton[i], RECT_BOTTON(LOCKED));
			}
		}
		sfText_setString(selectionMenu.descriptionText, selectionMenu.description[selectionMenu.state][temp]);
		sfSprite_setTextureRect(selectionMenu.generalButton[temp], RECT_BOTTON(SELECT));
	}
}