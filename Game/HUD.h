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

	sfBool isBossBarShown;
}HUD;

void LoadHUD(void);
void LoadBossBar(float* _bossHpAdr);

void UpdateHUD(float _dt);

void VisibilityBossBar(sfBool _visible);

void VisibilityHUD(sfBool _visible);

#endif // !HUD_H