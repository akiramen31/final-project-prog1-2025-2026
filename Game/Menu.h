#ifndef MENU_H
#define MENU_H

#include "Common.h"

#define NB_TOP_BUTTONS 5
#define NB_INFO_BUTTONS 6
#define NB_KEY 8
#define MAX_INFO 6
#define NB_MUSICS 5

typedef enum MenuState
{
	MENU_BASE,
	PLAY,
	SETTINGS,
	CREDITS,
	CONTROLS
}MenuState;


typedef struct Menu
{
	char* name[10];
	sfText* topButtons[NB_TOP_BUTTONS];
	sfText* infoDisplay[6];
	sfText* keyType[NB_KEY];
	sfText* key[NB_KEY];
	sfSprite* logo[3];
	sfMusic* musics[NB_MUSICS];
	MenuState state;
	sfColor highlightTextColor;
	sfColor textColor;
}Menu;

void LoadMenu(void);
void PollEventMenu(sfEvent* _event);
void UpdateMenu(float _dt);


#endif // !MENU_H