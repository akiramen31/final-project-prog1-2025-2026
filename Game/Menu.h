#ifndef MENU_H
#define MENU_H

#include "Common.h"

#define NB_INFO_BUTTONS 6
#define KEY_COUNT 8
#define MAX_INFO 6
#define MUSIC_COUNT 5
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
	sfSprite* mapButton[3];
	sfSprite* mapOverlay[3];
	sfSprite* generalButton[WEAPON_COUNT];
	sfSprite* generalIcon[WEAPON_COUNT];
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