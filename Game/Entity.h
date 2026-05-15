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
	sfTexture* texture;
}ConveyorData;

typedef struct BluePrintEntity
{
	sfSprite* sprite;
	int type;
}BluePrintEntity;

typedef struct BluePrint
{
	BluePrintEntity* entity;
	sfTexture* texture;
	int count;
}BluePrint;

typedef struct Entity
{
	BluePrint bluePrint;
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