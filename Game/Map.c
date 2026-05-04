#include "Map.h"
#include "Ennemy.h"
#include "CjsonB.h"
#include "Boss.h"
#include "Parallax.h"
Map map;

int rectShapeCount;
sfRectangleShape** rectShape;

sfColor colorBackground;

void LoadMapData(CjsonB* _cjson);
void LoadObjectMap(InfoZone** _infoZoneExit, int* _infoZoneCountExit, ObjectCjsonB* _object, int _objectCount);
void SetPositionEntity(InfoZone* _point, int _count);
void CreateRectVisible(InfoZone* _infoZone, int _count);

void LoadMap()
{
	map = (Map){ 0 };
	map.state = -1;
	map.foreground = CreateSprite(NULL, (sfVector2f) { 0 }, 1.f, 70.f);
	map.background = CreateSprite(NULL, (sfVector2f) { 0 }, 1.f, 100.f);

	int val = 169;
	colorBackground = sfColor_fromRGB(val, val, val);

	sfSprite_setColor(map.background, colorBackground);

	LoadBox();
}

void SetMap(MapState _map)
{
	SetCameraZoom(0.f);

	CjsonB* cjson = NULL;
#if DEV_PIERRE_ENEMY
	if (map.data.image)
	{
		sfImage_destroy(map.data.image);
	}

	if (DEV_ENNEMY && map.state != -1)
	{
		ResetEnemy();
	}
#endif

	switch (_map)
	{
	case LEVEL1:
		cjson = LoadCjsonB("Assets/Maps/Level1.json");
		sfSprite_setTexture(map.foreground, GetAsset("Assets/Maps/Level1Front.png"), sfTrue);
		sfSprite_setTexture(map.background, GetAsset("Assets/Maps/Level1Back.png"), sfTrue);
		//map.data.image = sfImage_createFromFile("Assets/Maps/Level1Reduite.png");
		SetParallaxLayerTexture(1, GetAsset("Assets/Maps/parallax_lv1_la1.png"));
		break;
	case LEVEL2:
		cjson = LoadCjsonB("Assets/Maps/Level2.json");
		sfSprite_setTexture(map.foreground, GetAsset("Assets/Maps/Level2Front.png"), sfTrue);
		sfSprite_setTexture(map.background, GetAsset("Assets/Maps/Level2Back.png"), sfTrue);
		//map.data.image = sfImage_createFromFile("Assets/Maps/Level2Reduite.png");
		SetParallaxLayerTexture(1, GetAsset("Assets/Maps/parallax_lv2_la1.png"));
		break;
	case LEVEL3:
		cjson = LoadCjsonB("Assets/Maps/Level3.json");
		sfSprite_setTexture(map.foreground, GetAsset("Assets/Maps/Level3.png"), sfTrue);
		//map.data.image = sfImage_createFromFile("Assets/Maps/Level3Reduite.png");
		break;
	case LEVEL_TEST:
		cjson = LoadCjsonB("Assets/Maps/LevelTest.json");
		sfSprite_setTexture(map.foreground, GetAsset("Assets/Maps/LevelTest.png"), sfTrue);
		//map.data.image = sfImage_createFromFile("Assets/Maps/MapTesteReduite.png");
		break;
	default:
		break;
	}
	map.state = _map;
	if (cjson)
	{
		LoadMapData(cjson);
		CleanupCjsonB(cjson);
	}

#if DEV_PIERRE_ENEMY && DEV_ENNEMY
	LoadEnemy();
#else
	ResetEnemy();
#endif

	ReloadBox();
	SetPositionEntity(map.data.point, map.data.pointCount);
	map.state = _map;
}

MapState GetActualyMap(void)
{
	return map.state;
}

void LoadMapData(CjsonB* _cjson)
{
	map.data.size = (sfVector2u){ _cjson->width, _cjson->height };

	for (unsigned i = 0; i < _cjson->layersCount; i++)
	{
		if (StringCompare(_cjson->layers[i].name, "Collider"))
		{
			LoadObjectMap(&map.data.colider, &map.data.coliderCount, _cjson->layers[i].objects, _cjson->layers[i].objectsCount);
		}
		else if (StringCompare(_cjson->layers[i].name, "Trigger"))
		{
			LoadObjectMap(&map.data.triger, &map.data.trigerCount, _cjson->layers[i].objects, _cjson->layers[i].objectsCount);
		}
		else if (StringCompare(_cjson->layers[i].name, "Move"))
		{
			LoadObjectMap(&map.data.move, &map.data.moveCount, _cjson->layers[i].objects, _cjson->layers[i].objectsCount);
		}
		else if (StringCompare(_cjson->layers[i].name, "Point"))
		{
			LoadObjectMap(&map.data.point, &map.data.pointCount, _cjson->layers[i].objects, _cjson->layers[i].objectsCount);
		}
		else if (StringCompare(_cjson->layers[i].name, "PassThrough"))
		{
			LoadObjectMap(&map.data.passThrough, &map.data.PassThroughCount, _cjson->layers[i].objects, _cjson->layers[i].objectsCount);
		}
	}

	CreateRectVisible(map.data.move, map.data.moveCount);

	map.data.caseSize = (sfVector2f){ (float)_cjson->tilewidth, (float)_cjson->tileheight };
}


MapData* GetMapData(void)
{
	return &map.data;
}

void LoadObjectMap(InfoZone** _infoZoneExit, int* _infoZoneCountExit, ObjectCjsonB* _object, int _objectCount)
{
	*_infoZoneCountExit = 0;
	*_infoZoneExit = NULL;
	InfoZone* temp = calloc(_objectCount, sizeof(InfoZone));
	if (temp)
	{
		for (int i = 0; i < _objectCount; i++)
		{
			temp[i].hitbox = (sfFloatRect){ (float)_object[i].x,(float)_object[i].y,(float)_object[i].width, (float)_object[i].height };
			temp[i].name = _object[i].name;
			temp[i].type = _object[i].type;
		}
		*_infoZoneCountExit = _objectCount;
		*_infoZoneExit = temp;
	}
}

void SetPositionEntity(InfoZone* _point, int _count)
{
	for (int i = 0; i < _count; i++)
	{
		if (StringCompare(_point[i].type, "Box"))
		{
			AddBox((sfVector2f) { _point[i].hitbox.left, _point[i].hitbox.top });
		}
		else if (StringCompare(_point[i].type, "Boss"))
		{
			//SwitchBoss(map.state,  (sfVector2f) { _point[i].hitbox.left, _point[i].hitbox.top });
			SwitchBoss(map.state, (sfVector2f) { 8671.f, 863.f });
		}
		else if (StringCompare(_point[i].type, "Enemy"))
		{
			if (DEV_ENNEMY)
			{
				for (int j = 0; j < map.data.trigerCount; j++)
				{
					if (sfFloatRect_contains(&map.data.triger[j].hitbox, map.data.point[i].hitbox.left, map.data.point[i].hitbox.top))
					{
						AddEnemy((sfVector2f) { _point[i].hitbox.left, _point[i].hitbox.top }, ALEATORY, map.data.triger[j].hitbox);
					}
				}
			}
		}
		else if (StringCompare(_point[i].type, "SpawnPlayer"))
		{
			SetSpawnPlayer((sfVector2f) { _point[i].hitbox.left, _point[i].hitbox.top });
		}
		else if (StringCompare(_point[i].type, "TpPlayer"))
		{
			SetTpPlayerBoss((sfVector2f) { _point[i].hitbox.left, _point[i].hitbox.top });
		}
	}
}

void CreateRectVisible(InfoZone* _infoZone, int _count)
{
	rectShapeCount = _count;
	rectShape = Calloc(_count, sizeof(sfRectangleShape*));
	if (rectShape)
	{
		for (int i = 0; i < _count; i++)
		{
			rectShape[i] = sfRectangleShape_create();
			sfRectangleShape_setFillColor(rectShape[i], sfColor_fromRGBA(0, 0, 255, 125));
			sfRectangleShape_setSize(rectShape[i], (sfVector2f) { _infoZone[i].hitbox.width, _infoZone[i].hitbox.height });
			sfRectangleShape_setPosition(rectShape[i], (sfVector2f) { _infoZone[i].hitbox.left, _infoZone[i].hitbox.top });
			sfRectangleShape_setOutlineColor(rectShape[i], sfColor_fromRGB(rand() % 256, rand() % 256, rand() % 256));
			sfRectangleShape_setOutlineThickness(rectShape[i], -1.f);
		}
	}
}

sfVector2f Colision(sfFloatRect _hitbox, int _axis)
{
	sfVector2f vectorMove = { 0, 0 };
	sfFloatRect reaction = { 0, 0, 0, 0 };

	for (int i = 0; i < map.data.coliderCount; i++)
	{
		if (sfFloatRect_intersects(&_hitbox, &map.data.colider[i].hitbox, &reaction))
		{
			int resolveX = 0;

			// Determine which axis to resolve based on the passed parameter
			if (_axis == AXIS_X)
			{
				resolveX = 1;
			}
			else if (_axis == AXIS_Y)
			{
				resolveX = 0;
			}
			else
			{
				// Fallback for AXIS_BOTH (old behavior)
				resolveX = (reaction.width < reaction.height);
			}

			if (resolveX)
			{
				float hitboxCenterX = _hitbox.left + (_hitbox.width / 2.0f);
				float coliderCenterX = map.data.colider[i].hitbox.left + (map.data.colider[i].hitbox.width / 2.0f);

				float push = (hitboxCenterX < coliderCenterX) ? -reaction.width : reaction.width;
				vectorMove.x += push;
				_hitbox.left += push; // Instantly update hitbox for the next loop iteration
			}
			else
			{
				float hitboxCenterY = _hitbox.top + (_hitbox.height / 2.0f);
				float coliderCenterY = map.data.colider[i].hitbox.top + (map.data.colider[i].hitbox.height / 2.0f);

				float push = (hitboxCenterY < coliderCenterY) ? -reaction.height : reaction.height;
				vectorMove.y += push;
				_hitbox.top += push; // Instantly update hitbox for the next loop iteration
			}
		}
	}
	return vectorMove;
}

sfVector2f CollisionPassThrough(sfFloatRect _hitbox)
{
	sfVector2f vectorMove = { 0 };
	sfFloatRect reaction = { 0 };

	for (int i = 0; i < map.data.PassThroughCount; i++)
	{
		if (sfFloatRect_intersects(&_hitbox, &map.data.passThrough[i].hitbox, &reaction))
		{
			if (_hitbox.top + _hitbox.height / 4.f * 3.f + vectorMove.y - map.data.passThrough[i].hitbox.top < (map.data.passThrough[i].hitbox.height - _hitbox.height / 2))
			{
				vectorMove.y -= reaction.height;
			}
			_hitbox.top += vectorMove.y;
		}
	}
	return vectorMove;
}

InfoZone* GetInfoZoneTriger(sfFloatRect _hitbox)
{
	for (int i = 0; i < map.data.trigerCount; i++)
	{
		if (sfFloatRect_intersects(&_hitbox, &map.data.triger[i].hitbox, NULL))
		{
			return map.data.triger;
		}
	}
	return NULL;
}

int GetTrigerCount(void)
{
	return map.data.trigerCount;
}

InfoZone* GetInfoZoneMove(void)
{
	return map.data.move;
}

int GetMoveCount(void)
{
	return map.data.moveCount;
}

void DrawDev(sfRenderWindow* _renderWindow)
{
	if (DEV_MAP_COLIDER)
	{
		for (int i = 0; i < rectShapeCount; i++)
		{
			sfRenderWindow_drawRectangleShape(_renderWindow, rectShape[i], NULL);
		}

	}
}