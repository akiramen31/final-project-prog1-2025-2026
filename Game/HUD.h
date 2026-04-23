#ifndef HUD_H
#define HUD_H

#include "Common.h"
#include "Player.h"

typedef struct HUD
{
	sfSprite* life[PLAYER_MAX_HEALTH];
	sfSprite* gauge;
}HUD;

void LoadHUD(void);
void UpdateHUD(float _dt);

#endif // !HUD_H
#pragma once