#ifndef MAP_H
#define MAP_H

#include "Common.h"

typedef enum MapState
{
	LEVEL1,
	LEVEL2,
	LEVEL3,
	LEVEL_TEST
}MapState;

typedef struct InfoZone
{
	sfFloatRect hitbox;
	char* type;
	char* name;
}InfoZone;

typedef struct MapData
{
	InfoZone* colider;
	int coliderCount;

	InfoZone* passThrough;
	int PassThroughCount;

	InfoZone* triger;
	int trigerCount;

	InfoZone* move;
	int moveCount;

	InfoZone* point;
	int pointCount;

	sfVector2u size;
	sfVector2f caseSize;
	unsigned** caseImage;
	sfImage* image;
}MapData;

typedef struct HitboxMap
{
	sfImage* image;
	sfVector2u size;
	int ratio;
}HitboxMap;

typedef struct Map
{
	MapData data;
	sfSprite* background;
	MapState state;
}Map;

void LoadMap(void);
void SetMap(MapState _map);
MapState GetActualyMap(void);

MapData* GetMapData(void);
InfoZone* GetInfoZoneTriger(sfFloatRect _hitbox);
int GetTrigerCount(void);
InfoZone* GetInfoZoneMove(void);
int GetMoveCount(void);


sfVector2f Colision(sfFloatRect _hitbox, int _axis);
sfVector2f CollisionPassThrough(sfFloatRect _hitbox);

void DrawDev(sfRenderWindow* _renderWindow);
#endif // !MAP_H