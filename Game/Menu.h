#ifndef MENU_H
#define MENU_H

#include "Common.h"
#include "GameState.h"

void LoadMenu(void);
void PollEventMenu(sfRenderWindow* _renderWindow, sfEvent* _event);
void UpdateMenu(float _dt);

void KeyPressedMenu(sfRenderWindow* _renderWindow, sfKeyEvent* _keyEvent);

#endif // !MENU_H