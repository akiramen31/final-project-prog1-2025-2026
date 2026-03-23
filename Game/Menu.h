#ifndef MENU_H
#define MENU_H

#include "Common.h"

#define NB_BUTTON 6

typedef enum MenuState
{
	MENU_BASE,
	SETTING,
	CREDITS
}MenuState;


typedef struct Menu
{
	char* name[10];
	sfText* button[NB_BUTTON];
	MenuState state;
	
}Menu;

void LoadMenu(void);
void PollEventMenu(sfEvent* _event);
void UpdateMenu(float _dt);


#endif // !MENU_H