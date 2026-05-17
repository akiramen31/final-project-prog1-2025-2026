#include "Map.h"
#include "Enemy.h"
#include "CjsonB.h"
#include "Boss.h"
#include "Parallax.h"
#include "Entity.h"
#include "Elevator.h"
#include "Camera.h"

Map map;
void LoadMapData(CjsonB* _cjson);
void LoadObjectMap(InfoZone** _infoZoneExit, int* _infoZoneCountExit, ObjectCjsonB* _object, int _objectCount);
void SetPositionEntityPoint(InfoZone* _point, int _count);
void SetPositionEntityTrigger(InfoZone* _trigger, int _count);

void LoadMap()
{
	int temp = map.state;
	map = (Map){ 0 };
	map.state = temp;

	map.foreground = CreateSprite(NULL, (sfVector2f) { 0 }, 1.f, 70.f);
	map.background = CreateSprite(NULL, (sfVector2f) { 0 }, 1.f, 100.f);

	int val = 169;
	sfSprite_setColor(map.background, (sfColor) { val, val, val, 255 });

	LoadEntity();
	SetCameraZoom(CAMERA_DEFAULT_ZOOM);

	CjsonB* cjson = NULL;

	switch (map.state)
	{
	case LEVEL1:
		cjson = LoadCjsonB("Assets/Maps/Level1.json");
		sfSprite_setTexture(map.foreground, GetAsset("Assets/Maps/Level1Front.png"), sfTrue);
		sfSprite_setTexture(map.background, GetAsset("Assets/Maps/Level1Back.png"), sfTrue);
		SetParallaxLayerTexture(1, GetAsset("Assets/Maps/parallax_lv1_la1.png"));
		break;
	case LEVEL2:
		cjson = LoadCjsonB("Assets/Maps/Level2.json");
		sfSprite_setTexture(map.foreground, GetAsset("Assets/Maps/Level2Front.png"), sfTrue);
		sfSprite_setTexture(map.background, GetAsset("Assets/Maps/Level2Back.png"), sfTrue);
		SetParallaxLayerTexture(1, GetAsset("Assets/Maps/parallax_lv2_la1.png"));
		break;
	case LEVEL3:
		cjson = LoadCjsonB("Assets/Maps/Level3.json");
		sfSprite_setTexture(map.foreground, GetAsset("Assets/Maps/Level3Front.png"), sfTrue);
		sfSprite_setTexture(map.background, GetAsset("Assets/Maps/Level3Back.png"), sfTrue);

		break;
	case LEVEL_TEST:
		cjson = LoadCjsonB("Assets/Maps/LevelTest.json");
		sfSprite_setTexture(map.foreground, GetAsset("Assets/Maps/LevelTest.png"), sfTrue);
		break;
	default:
		break;
	}
	if (cjson)
	{
		LoadMapData(cjson);
		CleanupCjsonB(cjson);
	}
	LoadEnemy();

	LoadElevator();
	SetPositionEntityPoint(map.data.point, map.data.pointCount);
	SetPositionEntityTrigger(map.data.triger, map.data.trigerCount);
}

void SetCurrentMap(MapState _map)
{
	map.state = _map;
}

MapState GetCurrentMap(void)
{
	return map.state;
}

void LoadMapData(CjsonB* _cjson)
{
	map.data.size = (sfVector2u){ 0 };
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
			LoadObjectMap(&map.data.velocity, &map.data.moveCount, _cjson->layers[i].objects, _cjson->layers[i].objectsCount);
		}
		else if (StringCompare(_cjson->layers[i].name, "Point"))
		{
			LoadObjectMap(&map.data.point, &map.data.pointCount, _cjson->layers[i].objects, _cjson->layers[i].objectsCount);
		}
		else if (StringCompare(_cjson->layers[i].name, "PassThrough"))
		{
			LoadObjectMap(&map.data.passThrough, &map.data.PassThroughCount, _cjson->layers[i].objects, _cjson->layers[i].objectsCount);
		}
		else if (_cjson->layers[i].data && !(map.data.size.x + map.data.size.y))
		{
			map.data.size = (sfVector2u){ _cjson->layers[i].width, _cjson->layers[i].height };
		}
	}
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
	InfoZone* temp = Calloc(_objectCount, sizeof(InfoZone));
	if (temp)
	{
		for (int i = 0; i < _objectCount; i++)
		{
			temp[i].hitbox = (sfFloatRect){ (float)_object[i].x,(float)_object[i].y,(float)_object[i].width, (float)_object[i].height };
			temp[i].name = StringCopy(_object[i].name);
			temp[i].type = StringCopy(_object[i].type);
		}
		*_infoZoneCountExit = _objectCount;
		*_infoZoneExit = temp;
	}
}

void SetPositionEntityPoint(InfoZone* _point, int _count)
{
	for (int i = 0; i < _count; i++)
	{
		if (StringCompare(_point[i].type, "Box"))
		{
			AddBox((sfVector2f) { _point[i].hitbox.left, _point[i].hitbox.top });
		}
		else if (StringCompare(_point[i].type, "Conveyor"))
		{
			AddConveyor((sfVector2f) { _point[i].hitbox.left, _point[i].hitbox.top });
		}
		else if (StringCompare(_point[i].type, "Boss"))
		{
			LoadBoss(map.state, (sfVector2f) { _point[i].hitbox.left, _point[i].hitbox.top });
		}
		else if (StringCompare(_point[i].type, "Enemy"))
		{
			printf("\n\n%d", i);
			for (int j = 0; j < map.data.trigerCount; j++)
			{
				if (map.data.triger[j].hitbox.left < map.data.point[i].hitbox.left && map.data.triger[j].hitbox.top < map.data.point[i].hitbox.top && map.data.triger[j].hitbox.left + map.data.triger[j].hitbox.width > map.data.point[i].hitbox.left && map.data.triger[j].hitbox.top + map.data.triger[j].hitbox.height > map.data.point[i].hitbox.top)
				{
					AddEnemy((sfVector2f) { _point[i].hitbox.left, _point[i].hitbox.top }, _point[i].name[0] - '0', map.data.triger[j].hitbox);
					j = map.data.trigerCount;
					printf("%d", i);
				}
			}
		}
		else if (StringCompare(_point[i].type, "Bonus"))
		{
			AddBluePrint(&_point[i]);
		}
		else if (StringCompare(_point[i].type, "Press"))
		{
			AddPress((sfVector2f) { _point[i].hitbox.left, _point[i].hitbox.top });
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

void SetPositionEntityTrigger(InfoZone* _trigger, int _count)
{
	for (int i = 0; i < _count; i++)
	{
		if (StringCompare(_trigger[i].type, "Jet"))
		{
			AddJetSteam(&_trigger[i]);
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
			float hitboxCenterY = _hitbox.top + (_hitbox.height / 2.f);
			float coliderCenterY = map.data.passThrough[i].hitbox.top + (map.data.passThrough[i].hitbox.height / 4.f);
			float push = (hitboxCenterY < coliderCenterY) ? -reaction.height : 0;

			vectorMove.y += push;
			_hitbox.top += push;
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
	return map.data.velocity;
}

int GetMoveCount(void)
{
	return map.data.moveCount;
}
