#ifndef MENU_H
#define MENU_H

#include "Common.h"

#define NB_INFO_BUTTONS 6
#define NB_KEY 8
#define MAX_INFO 6
#define NB_MUSICS 5
#define WEAPON_COUNT 4
#define SECONDAY_COUNT 2

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
	sfText* keyType[NB_KEY];
	sfText* key[NB_KEY];
	char* name[5];
}MainMenu;

typedef struct SelectionMenu
{
	sfSprite* categoryButton[3];
	sfSprite* categoryIcon[3];
	sfSprite* mapButton[3];
	sfSprite* mapOverlay[3];
	sfSprite* generalButton[5];
	sfSprite* generalIcon[5];
	int selectedOption;

	sfText* bottomText[2];

}SelectionMenu;

typedef struct Menu
{
	sfSprite* background;
	sfSprite* overlay;
	sfColor highlightTextColor;
	sfColor textColor;
	sfMusic* musics[NB_MUSICS];
	MenuState state;
	MainMenu mainMenu;
	SelectionMenu selectionMenu;
}Menu;

void LoadMenu(void);
void PollEventMenu(sfEvent* _event);
void UpdateMenu(float _dt);

#endif // !MENU_H