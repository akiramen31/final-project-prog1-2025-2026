#ifndef GUI_H
#define GUI_H

#include "Common.h"

typedef struct PauseMenu
{
	sfSprite* background;
	sfTexture* buttonBase;
	sfSprite* button[4];
	sfText* buttonText[4];
}PauseMenu;

void LoadGUI(void);
void UpdateGUI(float _dt);

#endif // !GUI_H
#pragma once