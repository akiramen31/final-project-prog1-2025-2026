#ifndef MENU_H
#define MENU_H

#include "Common.h"

typedef struct Menu
{
	sfSprite* temp;
}Menu;

void LoadMenu(void);
void PollEventMenu(sfEvent* _event);
void UpdateMenu(float _dt);

void KeyPressedMenu(sfKeyEvent* _keyEvent);

#endif // !MENU_H