#ifndef MENU_H
#define MENU_H

#include "Common.h"

#define NB_INFO_BUTTONS 6
#define NB_KEY 8
#define MAX_INFO 6
#define NB_MUSICS 5

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
	char* name[10];
	sfText* topButtons[5];
	sfText* infoDisplay[6];
	sfText* keyType[NB_KEY];
	sfText* key[NB_KEY];
	sfSprite* logo[3];
}MainMenu;

typedef struct SelectionMenu
{
	sfSprite* sideButton[3];
	sfSprite* sideIcon[3];
	sfSprite* mapIcon[3];
	sfSprite* mapOverlay[3];
	int selectedOption;
	sfText* bottomText[2];

}SelectionMenu;

typedef struct Menu
{
	sfColor highlightTextColor;
	sfColor textColor;
	sfSprite* overlay;
	sfMusic* musics[NB_MUSICS];
	MenuState state;
	MainMenu mainMenu;
	SelectionMenu selectionMenu;
}Menu;



void LoadMenu(void);
void PollEventMenu(sfEvent* _event);
void UpdateMenu(float _dt);


#endif // !MENU_H