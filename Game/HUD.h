#ifndef HUD_H
#define HUD_H

#include "Common.h"
#include "EntityManager.h"

typedef struct HUD
{
	sfSprite* score[6];
	sfSprite* powerUps[3];
	sfSprite* playerLife;
	sfSprite* time[5];

}HUD;

void LoadHUD(void);
void UpdateHUD(float _dt, float _timer);

#endif // !HUD_H
#pragma once