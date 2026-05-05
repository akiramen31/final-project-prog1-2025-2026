#ifndef MENU_H
#define MENU_H

#include "Common.h"

#define NB_INFO_BUTTONS 6
#define KEY_COUNT 8
#define MAX_INFO 6
#define MUSIC_COUNT 5
#define MAP_COUNT 3
#define WEAPON_COUNT 4
#define SECONDARY_COUNT 2

typedef enum MenuState
{
	STARTING_MENU,
	PLAY,
	SETTINGS,
	CREDITS,
	CONTROLS,
	SELECTION_MAP,
	SELECTION_WEAPON,
	SELECTION_BONUS
}MenuState;

typedef enum ButtonRect
{
	MAP_1,
	WEAPON_1,
	UNSELECT,
	MAP_2,
	WEAPON_2,
	HIGHLIGHT,
	MAP_3,
	WEAPON_3,
	SELECT,
	SECONDARY_1,
	SECONDARY_2,
	LOCKED,
	RECT_COUNT
}ButtonRect;


typedef struct MainMenu
{
	sfTexture* textBox[2];
	sfSprite* infoBox;
	sfSprite* logo[2];
	sfText* topButtons[5];
	sfText* infoDisplay[6];
	sfText* keyType[KEY_COUNT];
	sfText* key[KEY_COUNT];
	char* name[5];
}MainMenu;

typedef struct SelectionMenu
{
	sfSprite* categoryButton[3];
	sfSprite* categoryIcon[3];
	sfSprite* generalButton[WEAPON_COUNT];
	sfSprite* generalIcon[WEAPON_COUNT];
	sfIntRect textureRect[RECT_COUNT];
	int selectedOption;
	sfText* bottomText[2];
}SelectionMenu;

typedef struct Menu
{
	sfSprite* background;
	sfSprite* overlay;
	sfColor highlightTextColor;
	sfColor textColor;
	sfMusic* musics[MUSIC_COUNT];
	MenuState state;
	MainMenu mainMenu;
	SelectionMenu selectionMenu;
}Menu;

void LoadMenu(void);
void PollEventMenu(sfEvent* _event);
void UpdateMenu(float _dt);

#endif // !MENU_H