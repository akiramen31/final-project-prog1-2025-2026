#ifndef MAP_H
#define MAP_H

#include "Common.h"
#include "Map.h"
#include "Cjson.h"

typedef enum MapState
{
	LEVEL1,
	LEVEL2,
	LEVEL3
}MapState;

typedef struct MapData
{
	sfFloatRect* colider;
	int coliderCount;
	sfFloatRect* triger;
	int trigerCount;
	sfFloatRect* move;
	int moveCount;
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
	MapState state;
}Map;

void LoadMap(void);
void SetMap(MapState _map);

MapData* GetMapData(void);

sfVector2f Colision(sfFloatRect _hitbox);
void DrawDev(sfRenderWindow* _renderWindow);

#endif // !MAP_H