#ifndef ENTITY_H
#define ENTITY_H

#include "Common.h"

typedef struct BoxEntity
{
	sfSprite* sprite;
	sfFloatRect hitbox;
}BoxEntity;

typedef struct BoxData
{
	BoxEntity* entity;
	unsigned count;
	sfSound* breakSound;
}BoxData;

typedef struct ConveyorData
{
	sfSprite** entity;
	Animation animation;
	unsigned count;
}ConveyorData;

typedef struct BluePrintEntity
{
	sfSprite* sprite;
	int type;
}BluePrintEntity;

typedef struct BluePrint
{
	BluePrintEntity* entity;
	int count;
}BluePrint;

typedef struct JetSteamEntity
{
	sfSprite* sprite;
	Animation animation;
	float cooldown;
}JetSteamEntity;

typedef struct JetSteam
{
	JetSteamEntity* entity;
	unsigned count;
}JetSteam;

typedef struct PressEntity
{
	sfSprite* sprite;
	float timer;
}PressEntity;

typedef struct Press
{
	PressEntity* entity;
	int count;
}Press;

typedef struct Entity
{
	Press press;
	JetSteam jetSteam;
	BluePrint bluePrint;
	BoxData boxData;
	ConveyorData conveyorData;
}Entity;

void LoadEntity(void);
void UpdateEntity(float _dt);

void AddBox(sfVector2f _position);
void AddConveyor(sfVector2f _position);
void AddBluePrint(sfVector2f _position, int _type);
void AddJetSteam(InfoZone* _infoZone);
void AddPress(sfVector2f _position);
sfVector2f ColisionBox(sfFloatRect _hitbox, sfBool _destroy, int _axis);
#endif //ENTITY_H