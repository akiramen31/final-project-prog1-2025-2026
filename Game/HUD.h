#ifndef HUD_H
#define HUD_H

#include "Common.h"
#include "Player.h"

typedef struct BossBar
{
	sfSprite* bossLifeContainer;
	sfSprite* bossLifeBar;
	float* bossLife;
	float maxBossLife;
}BossBar;

typedef struct HUD
{
	sfSprite* life[PLAYER_MAX_HEALTH];
	sfSprite* gauge;
	BossBar bossBar;
	char isHudVisible;
}HUD;

void LoadHUD(void);
void LoadBossBar(float* _bossHpAdr, float _maxHealth);

void UpdateHUD(float _dt);

void ToggleVisibilityHUD(sfBool _visible);

#endif // !HUD_H