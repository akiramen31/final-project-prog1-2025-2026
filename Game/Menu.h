#ifndef MENU_H
#define MENU_H

#include "Common.h"

#define NB_INFO_BUTTONS 6
#define CREDIT_COUNT 5
#define KEY_COUNT 8
#define MAX_INFO 6
#define MUSIC_COUNT 3

typedef enum MenuState
{
	STARTING_MENU,
	PLAY,
	SETTINGS,
	CREDITS,
	CONTROLS
}MenuState;

typedef struct MainMenu
{
	sfTexture* textBox[2];
	sfSprite* infoBox;
	sfSprite* logo[2];
	sfText* topButtons[4];
	sfText* infoDisplay[6];
	sfText* keyType[KEY_COUNT];
	sfText* key[KEY_COUNT];
	char* name[5];
}MainMenu;

typedef struct Menu
{
	sfSprite* background;
	sfSprite* overlay;
	sfColor highlightTextColor;
	sfColor textColor;
	sfMusic* musics[MUSIC_COUNT];
	MenuState state;
	MainMenu mainMenu;
}Menu;

void LoadMenu(void);
void PollEventMenu(sfEvent* _event);
void UpdateMenu(float _dt);

#endif // !MENU_H