#include "Map.h"

Map map;

int rectShapeCount;
sfRectangleShape** rectShape;

MapData LoadMapData(Cjson* _cjson);
void LoadObjectMap(InfoZone** _infoZoneExit, int* _infoZoneCountExit, Object* _object, int _objectCount);
void CreateRectVisible(InfoZone* _infoZone, int _count);
Bool StringCompareMap(char* _string1, char* _string2);

void LoadMap(void)
{
	map.background = LoadBackground(GetAsset("Assets/Maps/Level1.png"), 1.f);
	SetMap(LEVEL1);
}

void SetMap(MapState _map)
{
	Cjson* cjson = NULL;
	switch (_map)
	{
	case LEVEL1:
		cjson = LoadCjson("Assets/Maps/Level1.json");
		sfSprite_setTexture(map.background, GetAsset("Assets/Maps/Level1.png"), sfTrue);
		break;
	case LEVEL2:
		cjson = LoadCjson("Assets/Maps/Level2.json");
		sfSprite_setTexture(map.background, GetAsset("Assets/Maps/Level2.png"), sfTrue);
		break;
	case LEVEL3:
		cjson = LoadCjson("Assets/Maps/Level3.json");
		sfSprite_setTexture(map.background, GetAsset("Assets/Maps/Level3.png"), sfTrue);
		break;
	case LEVEL_TEST:
		cjson = LoadCjson("Assets/Maps/LevelTest.json");
		sfSprite_setTexture(map.background, GetAsset("Assets/Maps/LevelTest.png"), sfTrue);
		break;
	default:
		break;
	}
	if (cjson)
	{
		map.data = LoadMapData(cjson);
		CleanupCjson(cjson);
	}
}

MapData LoadMapData(Cjson* _cjson)
{
	MapData data = { 0 };

	data.size = (sfVector2u){ _cjson->width, _cjson->height };

	for (int i = 0; i < _cjson->layersCount; i++)
	{
		if (StringCompareMap(_cjson->layers[i].name, "Collider"))
		{
			LoadObjectMap(&data.colider, &data.coliderCount, _cjson->layers[i].objects, _cjson->layers[i].objectsCount);
		}
		else if (StringCompareMap(_cjson->layers[i].name, "Triger"))
		{
			LoadObjectMap(&data.triger, &data.trigerCount, _cjson->layers[i].objects, _cjson->layers[i].objectsCount);
		}
		else if (StringCompareMap(_cjson->layers[i].name, "Move"))
		{
			LoadObjectMap(&data.move, &data.moveCount, _cjson->layers[i].objects, _cjson->layers[i].objectsCount);
		}
		else if (StringCompareMap(_cjson->layers[i].name, "Piont"))
		{
			//SetPlayerPosition((sfVector2f) {_cjson->layers[i].objects.x,_cjson->layers[i].objects.y });
		}
	}



	CreateRectVisible(data.move, data.moveCount);
	data.image = sfImage_createFromFile("Assets/Maps/Level1Reduite.png");

	data.caseSize = (sfVector2f){ (float)_cjson->tileWidth, (float)_cjson->tileHeight };

	return data;
}


MapData* GetMapData(void)
{
	return &map.data;
}

void LoadObjectMap(InfoZone** _infoZoneExit,int* _infoZoneCountExit, Object* _object, int _objectCount)
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

void CreateRectVisible(InfoZone* _infoZone, int _count)
{
	rectShapeCount = _count;
	rectShape = calloc(_count, sizeof(sfRectangleShape*));
	if (!rectShape)
	{
		return NULL;
	}
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

Bool StringCompareMap(char* _string1, char* _string2)
{
	int i = 0;
	while (_string1[i] == _string2[i])
	{
		i++;
		if (_string1[i] == 0 && _string2[i] == 0)
		{
			return TRUE;
		}
	}
	return FALSE;
}

sfVector2f Colision(sfFloatRect _hitbox)
{
	sfVector2f vectorMove = { 0 };
	sfFloatRect reaction = { 0 };


	for (int i = 0; i < map.data.coliderCount; i++)
	{
		if (sfFloatRect_intersects(&_hitbox, &map.data.colider[i].hitbox, &reaction))
		{
			if (reaction.width < reaction.height)
			{
				if (_hitbox.left + vectorMove.x - map.data.colider[i].hitbox.left < (map.data.colider[i].hitbox.width - _hitbox.width) / 2.0f)
				{
					vectorMove.x -= reaction.width;
				}
				else
				{
					vectorMove.x += reaction.width;
				}
			}
			else
			{
				if (_hitbox.top + vectorMove.y - map.data.colider[i].hitbox.top < (map.data.colider[i].hitbox.height - _hitbox.height) / 2.0f)
				{
					vectorMove.y -= reaction.height;
				}
				else
				{
					vectorMove.y += reaction.height;
				}
			}
		}
	}
	return vectorMove;
}

InfoZone* GetInfoZoneTriger(sfFloatRect _hitbox)
{
	sfFloatRect reaction = { 0 };
	for (int i = 0; i < map.data.trigerCount; i++)
	{
		if (sfFloatRect_intersects(&_hitbox, &map.data.triger[i].hitbox, &reaction))
		{
			return &map.data.triger[i];
		}
	}
	return NULL;
}

InfoZone* GetInfoZoneMove(sfFloatRect _hitbox)
{
	sfFloatRect reaction = { 0 };
	for (int i = 0; i < map.data.trigerCount; i++)
	{
		if (sfFloatRect_intersects(&_hitbox, &map.data.triger[i].hitbox, &reaction))
		{
			return &map.data.triger[i];
		}
	}
	return NULL;
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