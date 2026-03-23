#ifndef HUD_H
#define HUD_H

#include "Common.h"

typedef struct HUD
{
	sfSprite* life;
}HUD;

void LoadHUD(void);
void UpdateHUD(float _dt);

#endif // !HUD_H
#pragma once