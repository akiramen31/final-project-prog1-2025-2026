#include "Map.h"

Map map;

void LoadRectMap(sfFloatRect* _floatRect, int _floatRectCount, Object* _object, int _objectCount);
Bool StringCompareMap(char* _string1, char* _string2);

void LoadMap(void)
{
    SetMap(LEVEL1);

    Cjson* cjson = LoadCjson("Assets/Map/Map.json");
    map.map = LoadMapData(cjson);
    CleanupCjson(cjson);
}

void SetMap(MapState _map)
{
    ;
}


MapData* GetMapData(void)
{
    return &map.map;
}

MapData LoadMapData(Cjson* _cjson)
{
	MapData data = { 0 };

	for (int i = 0; i < _cjson->layersCount; i++)
	{
		if (StringCompareMap(_cjson->layers[i].name, "Collider"))
		{
			LoadRectMap(data.colider, data.coliderCount, _cjson->layers[i].objects, _cjson->layers[i].objectsCount);
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
			LoadRectMap(data.colider, data.coliderCount, _cjson->layers[i].objects, _cjson->layers[i].objectsCount);
		}
	}

	data.caseSize = (sfVector2f){ (float)_cjson->tileWidth, (float)_cjson->tileHeight };



	return data;
}

void LoadRectMap(sfFloatRect* _floatRect, int _floatRectCount, Object* _object, int _objectCount)
{
	_floatRect = calloc(_objectCount, sizeof(sfFloatRect));

	for (int i = 0; i < _objectCount; i++)
	{
		_floatRect[i] = (sfFloatRect){ (float)_object[i].x,(float)_object[i].y,(float)_object[i].width, (float)_object[i].height };
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
	const MapData* const mapData = GetMapData();

	for (int i = 0; i < mapData->coliderCount; i++)
	{
		if (mapData->colider[i].left < _hitbox.left + _hitbox.width)
		{
			vectorMove.x += mapData->colider[i].left - _hitbox.left - _hitbox.width;
		}
		else if (_hitbox.left < mapData->colider[i].left + mapData->colider[i].width)
		{
			vectorMove.x += mapData->colider[i].left + mapData->colider[i].width - _hitbox.left;
		}

		if (mapData->colider[i].top < _hitbox.top + _hitbox.height)
		{
			vectorMove.y += mapData->colider[i].top - _hitbox.top - _hitbox.height;
		}
		else if (_hitbox.top < mapData->colider[i].top + mapData->colider[i].height)
		{
			vectorMove.x += mapData->colider[i].top + mapData->colider[i].height - _hitbox.top;
		}
	}
	return vectorMove;
}