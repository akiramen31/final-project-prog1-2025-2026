#include "Map.h"

Map map;

MapData LoadMapData(Cjson* _cjson);
sfFloatRect* LoadRectMap(int* _floatRectCount, Object* _object, int _objectCount);
void LoadMapTexture(MapData* _data);
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
		cjson = LoadCjson("Assets/Maps/LevelTest.json");
		map.data = LoadMapData(cjson);
		CleanupCjson(cjson);
		break;
	default:
		break;
	}
}

MapData LoadMapData(Cjson* _cjson)
{
	MapData data = { 0 };

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
	}

	data.image = sfImage_createFromFile("Assets/Maps/MapTesteRéduite.png");

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
	*_floatRectCount = _objectCount;
	for (int i = 0; i < _objectCount; i++)
	{
		hitbox[i] = (sfFloatRect){ (float)_object[i].x,(float)_object[i].y,(float)_object[i].width, (float)_object[i].height };
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
			sfImage_copyImage(image, NULL, column * _data->caseSize.x, row * _data->caseSize.y, (sfIntRect){0}, sfTrue);
		}
	}
}

sfVector2f Colision(sfFloatRect _hitbox)
{
	sfVector2f vectorMove = { 0 };

	for (int i = 0; i < map.data.coliderCount; i++)
	{
		if (map.data.colider[i].left < _hitbox.left + _hitbox.width)
		{
			vectorMove.x += map.data.colider[i].left - _hitbox.left - _hitbox.width;
		}
		else if (_hitbox.left < map.data.colider[i].left + map.data.colider[i].width)
		{
			vectorMove.x += map.data.colider[i].left + map.data.colider[i].width - _hitbox.left;
		}

		if (map.data.colider[i].top < _hitbox.top + _hitbox.height)
		{
			vectorMove.y += map.data.colider[i].top - _hitbox.top - _hitbox.height;
		}
		else if (_hitbox.top < map.data.colider[i].top + map.data.colider[i].height)
		{
			vectorMove.x += map.data.colider[i].top + map.data.colider[i].height - _hitbox.top;
		}
	}
	return vectorMove;
}