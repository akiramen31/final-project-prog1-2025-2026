#ifndef MENU_H
#define MENU_H

#include "Common.h"

#define NB_BUTTONS 5
#define NB_KEY 8
#define MAX_INFO 6

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
	sfText* topButtons[NB_BUTTONS];
	sfText* infoDisplay[6];
	sfText* keyType[NB_KEY];
	sfText* key[NB_KEY];
	sfSprite* logo[3];
	MenuState state;
	sfColor colorTriger;
	sfColor colorNotTriger;
	
}Menu;

void LoadMenu(void);
void PollEventMenu(sfEvent* _event);
void UpdateMenu(float _dt);


#endif // !MENU_H