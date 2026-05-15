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
	sfTexture* texture;
}BoxData;

typedef struct ConveyorEntity
{
	sfSprite* sprite;
}ConveyorEntity;

typedef struct ConveyorData
{
	ConveyorEntity* entity;
	Animation animation;
	unsigned count;
	sfTexture* texture;
}ConveyorData;

typedef struct BluePrintEntity
{
	sfSprite* sprite;
	int type;
}BluePrintEntity;

typedef struct BluePrintData
{
	BluePrintEntity* entity;
	sfTexture* texture;
	int count;
}BluePrintData;

typedef struct Entity
{
	BluePrintData bluePrintData;
	BoxData boxData;
	ConveyorData conveyorData;
}Entity;

void LoadEntity(void);
void UpdateEntity(float _dt);

void AddBox(sfVector2f _position);
void AddConveyor(sfVector2f _position);
void AddBluePrint(InfoZone* _infoZone);
sfVector2f ColisionBox(sfFloatRect _hitbox, sfBool _destroy, int _axis);
void ReloadEntity(void);
#endif //ENTITY_H