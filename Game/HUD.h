#ifndef HUD_H
#define HUD_H

#include "Common.h"
#include "Player.h"

typedef struct BossBar
{
	sfSprite* bossLifeContainer;
	sfSprite* bossLifeBar;
	sfBool isActive;
	float* bossLife;
	float maxBossLife;
}BossBar;

typedef struct HUD
{
	sfSprite* life[PLAYER_MAX_HEALTH];
	sfSprite* gauge;
	BossBar bossBar;
}HUD;

void LoadHUD(void);
void LoadBossBar(void);

void UpdateHUD(float _dt);

void SetHpFocus(float* _bossHpAdr);

void VisibilityBossBar(sfBool _visibility);

#endif // !HUD_H