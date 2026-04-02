#include "Map.h"

Map map;

sfRectangleShape** colision;

MapData LoadMapData(Cjson* _cjson);
sfFloatRect* LoadRectMap(int* _floatRectCount, Object* _object, int _objectCount);
Bool StringCompareMap(char* _string1, char* _string2);

void LoadMap(void)
{
	SetMap(LEVEL1);
}

void SetMap(MapState _map)
{
	Cjson* cjson = NULL;
	switch (_map)
	{
	case LEVEL1:
		cjson = LoadCjson("Assets/Maps/Level1.json");
		if (cjson)
		{
			map.data = LoadMapData(cjson);
			CleanupCjson(cjson);
		}
		break;
	default:
		break;
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
			data.colider = LoadRectMap(&data.coliderCount, _cjson->layers[i].objects, _cjson->layers[i].objectsCount);
			if (data.size.x < _cjson->layers[i].width)
			{
				data.size.x = _cjson->layers[i].width;
			}
			if (data.size.y < _cjson->layers[i].height)
			{
				data.size.y = _cjson->layers[i].height;
			}
		}
		else if (StringCompareMap(_cjson->layers[i].name, "Triger"))
		{
			data.triger = LoadRectMap(&data.trigerCount, _cjson->layers[i].objects, _cjson->layers[i].objectsCount);
		}
		else if (StringCompareMap(_cjson->layers[i].name, "Move"))
		{
			data.move = LoadRectMap(&data.moveCount, _cjson->layers[i].objects, _cjson->layers[i].objectsCount);
		}
	}
	data.image = sfImage_createFromFile("Assets/Maps/Level1Reduite.png");

	data.caseSize = (sfVector2f){ (float)_cjson->tileWidth, (float)_cjson->tileHeight };

	return data;
}


MapData* GetMapData(void)
{
	return &map.data;
}

sfFloatRect* LoadRectMap(int* _floatRectCount, Object* _object, int _objectCount)
{
	sfFloatRect* hitbox = calloc(_objectCount, sizeof(sfFloatRect));
	if (!hitbox)
	{
		return NULL;
	}

	if (DEV_MODE)
	{
		colision = calloc(_objectCount, sizeof(sfRectangleShape*));
		if (!colision)
		{
			return NULL;
		}
	}


	*_floatRectCount = _objectCount;
	for (int i = 0; i < _objectCount; i++)
	{
		hitbox[i] = (sfFloatRect){ (float)_object[i].x,(float)_object[i].y,(float)_object[i].width, (float)_object[i].height };

		if (DEV_MODE)
		{
			colision[i] = sfRectangleShape_create();
			sfRectangleShape_setFillColor(colision[i], sfColor_fromRGBA(0, 0, 255, 125));
			sfRectangleShape_setSize(colision[i], (sfVector2f) { hitbox[i].width, hitbox[i].height });
			sfRectangleShape_setPosition(colision[i], (sfVector2f) { hitbox[i].left, hitbox[i].top });
			sfRectangleShape_setOutlineColor(colision[i], sfColor_fromRGB(rand() % 256, rand() % 256, rand() % 256));
			sfRectangleShape_setOutlineThickness(colision[i], -1.f);
		}
	}
	return hitbox;
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

void LoadMapTexture(MapData* _data)
{
	sfImage* image = sfImage_create((unsigned)_data->caseSize.x * _data->size.x, (unsigned)_data->caseSize.y * _data->size.y);
	for (int row = 0; row < _data->size.y; row++)
	{
		for (int column = 0; column < _data->size.y; column++)
		{
			sfImage_copyImage(image, NULL, column * (unsigned int)_data->caseSize.x, row * (unsigned int)_data->caseSize.y, (sfIntRect) { 0 }, sfTrue);
		}
	}
}

sfVector2f Colision(sfFloatRect _hitbox)
{
	sfVector2f vectorMove = { 0 };
	sfFloatRect reaction = { 0 };


	for (int i = 0; i < map.data.coliderCount; i++)
	{
		if (sfFloatRect_intersects(&_hitbox, &map.data.colider[i], &reaction))
		{
			if (reaction.width < reaction.height)
			{
				if (_hitbox.left + vectorMove.x - map.data.colider[i].left < (map.data.colider[i].width - _hitbox.width) / 2.0f)
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
				if (_hitbox.top + vectorMove.y - map.data.colider[i].top < (map.data.colider[i].height - _hitbox.height) / 2.0f)
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

void DrawDev(sfRenderWindow* _renderWindow)
{
	if (DEV_MAP_COLIDER)
	{
		for (int i = 0; i < map.data.coliderCount; i++)
		{
			sfRenderWindow_drawRectangleShape(_renderWindow, colision[i], NULL);
		}

	}
}