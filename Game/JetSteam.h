#ifndef JET_STEAM_H
#define JET_STEAM_H

#include "Common.h"
#include "Map.h"


typedef struct JetSteamEntity
{
	sfSprite* sprite;
	sfFloatRect* rect;
	AdvencedAnimation animation;
}JetSteamEntity;

typedef struct JetSteam
{
	JetSteamEntity* entity;
	unsigned count;
}JetSteam;

void LoadJetSteam(void);
void UpdateJetSteam(float _dt);
#endif // !JET_STEAM_H