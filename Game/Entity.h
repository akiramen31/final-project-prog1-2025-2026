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

typedef struct ConveyorEntity
{
	sfSprite* sprite;
	Animation animation;
}ConveyorEntity;

typedef struct ConveyorData
{
	ConveyorEntity* entity;
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

typedef struct Entity
{
	JetSteam jetSteam;
	BluePrint bluePrint;
	BoxData boxData;
	ConveyorData conveyorData;
}Entity;

void LoadEntity(void);
void UpdateEntity(float _dt);

void AddBox(sfVector2f _position);
void AddConveyor(sfVector2f _position);
void AddBluePrint(InfoZone* _infoZone);
void AddJetSteam(InfoZone* _infoZone);
sfVector2f ColisionBox(sfFloatRect _hitbox, sfBool _destroy, int _axis);
void ReloadEntity(void);
#endif //ENTITY_H