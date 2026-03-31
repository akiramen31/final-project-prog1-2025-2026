#include "Cjson.h"

#define FREE(_x) if (_x) free(_x)
#define REALLOC(_exitBlock, _block, _size) _exitBlock = realloc(_block, _size); if (!temp) return
#define ALLOC(_exitBlock, _count, _size) _exitBlock = calloc(_count, _size); if(!_exitBlock) return

void GetObjectByNameCjson(char* _bufferFile, char* _name, void* _saveValue, TypeValue _typeValue);
Bool StringCompareCjson(char* _string1, char* _string2);
int GetObjectStructLayerCjson(char* _buffer, Layers** _layers);
int GetObjectStructObjectCjson(char* _buffer, Object** _object);
int GetObjectStructTilesetCjson(char* _buffer, Tilesets** _tilesets);
int GetObjectStructWangsetsCjson(char* _buffer, Wangsets** _wangsets);
int GetObjectStructColorsCjson(char* _buffer, Colors** _colors);
int GetObjectStructWangtilesCjson(char* _buffer, Wangtiles** _wangtiles);
int GetIndexValueCjson(char* _buffer, char* _id1);
int GetIndexPoxiCjson(int _index1, int _index2);


Cjson* LoadCjson(char* _file)
{
	FILE* file = fopen(_file, "r");
	if (!file)
	{
		return NULL;
	}
	Cjson* cjson = calloc(1, sizeof(Cjson));
	if (!cjson)
	{
		return NULL;
	}

	fseek(file, 0, SEEK_END);
	long fileSize = ftell(file);
	fseek(file, 0, 0);

	char* buffer = calloc((size_t)(fileSize + 1), sizeof(char));
	if (!buffer)
	{
		fclose(file);
		return NULL;
	}

	fread(buffer, sizeof(char), (size_t)(fileSize + 1), file);

	GetObjectByNameCjson(buffer, "compressionlevel", &cjson->compressionlevel, INT);
	GetObjectByNameCjson(buffer, "height", &cjson->height, INT);
	GetObjectByNameCjson(buffer, "infinite", &cjson->infinite, BOOL);

	cjson->layersCount = GetObjectStructLayerCjson(buffer, &cjson->layers);

	GetObjectByNameCjson(buffer, "nextlayerid", &cjson->nextLayerId, INT);
	GetObjectByNameCjson(buffer, "nextobjectid", &cjson->nextObjectId, INT);
	GetObjectByNameCjson(buffer, "orientation", &cjson->orientation, CHAR_PTR);
	GetObjectByNameCjson(buffer, "renderorder", &cjson->renderOrder, CHAR_PTR);
	GetObjectByNameCjson(buffer, "tiledversion", &cjson->tiledVersion, CHAR_PTR);
	GetObjectByNameCjson(buffer, "tileheight", &cjson->tileHeight, INT);

	cjson->tilesetsCount = GetObjectStructTilesetCjson(buffer, &cjson->tilesets);

	GetObjectByNameCjson(buffer, "tilewidth", &cjson->tileWidth, INT);
	GetObjectByNameCjson(buffer, "type", &cjson->type, CHAR_PTR);
	GetObjectByNameCjson(buffer, "version", &cjson->version, CHAR_PTR);
	GetObjectByNameCjson(buffer, "width", &cjson->width, INT);

	free(buffer);
	return cjson;
}

void GetObjectByNameCjson(char* _bufferFile, char* _name, void* _saveValue, TypeValue _typeValue)
{
	int tempCalcule = 0;

	if (!*(int*)_saveValue)
	{
		int i = 0;
		int j = 0;
		while (_bufferFile[i])
		{
			j = 0;
			tempCalcule = i + j;
			while (_bufferFile[tempCalcule] == _name[j])
			{
				j++;
				tempCalcule = i + j;
				if (_name[j] == 0 && _bufferFile[i - 1] == '\"' && _bufferFile[tempCalcule] == '\"')
				{
					if (_typeValue == BOOL)
					{
						if (_bufferFile[i + j + 2] == 't')
						{
							*(Bool*)_saveValue = TRUE;
						}
						else if (_bufferFile[i + j + 2] == 'f')
						{
							*(Bool*)_saveValue = FALSE;
						}
					}
					else if (_typeValue == CHAR)
					{
						sscanf_s(&_bufferFile[i + j], "%*[^0-9-]%d", (int*)_saveValue);
					}
					else if (_typeValue == INT)
					{
						sscanf_s(&_bufferFile[i + j], "%*[^0-9-]%d", (int*)_saveValue);
					}
					else if (_typeValue == FLOAT)
					{
						sscanf_s(&_bufferFile[i + j], "%*[^0-9-]%f", (float*)_saveValue);
					}
					else if (_typeValue == CHAR_PTR)
					{
						char* buffer = calloc(30, sizeof(char));
						if (!buffer)
						{
							return;
						}

						int h = 3;
						do {
							buffer[h - 3] = _bufferFile[i + j + h];
							h++;
						} while (_bufferFile[i + j + h] != '\"');

						*(char**)_saveValue = realloc(buffer, (size_t)(h - 2));
					}
					else if (_typeValue == INT_PTR)
					{
						int count = 0;
						int nbColumn = 1;
						int nbRow = 1;

						while (_bufferFile[i + j + count] != ']')
						{
							if (_bufferFile[i + j + count] == '\n')
							{
								nbRow++;
							}
							if (_bufferFile[i + j + count] == ',')
							{
								nbColumn++;
							}
							count++;
						}
						nbColumn /= nbRow;

						int** buffer = *(int***)_saveValue = calloc(nbRow, sizeof(int*));
						if (!buffer)
						{
							return;
						}

						for (int g = 0; g < nbRow; g++)
						{
							buffer[g] = calloc(nbColumn, sizeof(int*));
							if (!buffer[g])
							{
								return;
							}
						}

						int indexChar = 0;
						for (int row = 0; row < nbRow; row++)
						{
							for (int column = 0; column < nbColumn; column++)
							{
								if (_bufferFile[i + j + indexChar - 1] == ',' || _bufferFile[i + j + indexChar - 1] == '[')
								{
									if (_bufferFile[i + j + indexChar] == '\n')
									{
										indexChar += 6;
									}
									sscanf_s(&_bufferFile[i + j + indexChar], "%d", &buffer[row][column]);

								}
								else
								{
									column--;
								}
								indexChar++;
							}
						}
					}

					{
						int j = 0;
						while (_bufferFile[tempCalcule] != '\0' && _bufferFile[tempCalcule] != '\t' && _bufferFile[tempCalcule] != ',')
						{
							_bufferFile[tempCalcule] = 'N';
							j++;
							tempCalcule = i + j;
						}
					}

					return ;
				}
			}
			i += j + 1;
		}
	}
	return;
}

Bool StringCompareCjson(char* _string1, char* _string2)
{
	int i = 0;
	while (_string1[i] == _string2[i])
	{
		i++;
		if (_string2[i] == 0)
		{
			return TRUE;
		}
	}
	return FALSE;
}

int GetObjectStructLayerCjson(char* _buffer, Layers** _layers)
{
	Layers* ALLOC(temp, 1, sizeof(Layers)) 0;

	int layersCount = 0;
	int layersType = GetIndexPoxiCjson(GetIndexValueCjson(_buffer, "data"), GetIndexValueCjson(_buffer, "draworder"));

	while (layersType)
	{
		REALLOC(temp, *_layers, (size_t)(layersCount + 1) * sizeof(Layers)) layersCount;
		temp[layersCount] = (Layers){ 0 };
		if (layersType == 1)
		{
			GetObjectByNameCjson(_buffer, "data", &temp[layersCount].data, INT_PTR);
			GetObjectByNameCjson(_buffer, "height", &temp[layersCount].height, INT);
			GetObjectByNameCjson(_buffer, "id", &temp[layersCount].id, INT);
			GetObjectByNameCjson(_buffer, "name", &temp[layersCount].name, CHAR_PTR);
			GetObjectByNameCjson(_buffer, "opacity", &temp[layersCount].opacity, FLOAT);
			GetObjectByNameCjson(_buffer, "type", &temp[layersCount].type, CHAR_PTR);
			GetObjectByNameCjson(_buffer, "visible", &temp[layersCount].visible, BOOL);
			GetObjectByNameCjson(_buffer, "width", &temp[layersCount].width, INT);
			GetObjectByNameCjson(_buffer, "x", &temp[layersCount].x, INT);
			GetObjectByNameCjson(_buffer, "y", &temp[layersCount].y, INT);
		}
		else if (layersType == 2)
		{
			GetObjectByNameCjson(_buffer, "draworder", &temp[layersCount].draworder, CHAR_PTR);
			GetObjectByNameCjson(_buffer, "id", &temp[layersCount].id, INT);
			GetObjectByNameCjson(_buffer, "name", &temp[layersCount].name, CHAR_PTR);

			temp[layersCount].objectsCount = GetObjectStructObjectCjson(_buffer, &temp[layersCount].objects);

			GetObjectByNameCjson(_buffer, "opacity", &temp[layersCount].opacity, FLOAT);
			GetObjectByNameCjson(_buffer, "type", &temp[layersCount].type, CHAR_PTR);
			GetObjectByNameCjson(_buffer, "visible", &temp[layersCount].visible, BOOL);
			GetObjectByNameCjson(_buffer, "x", &temp[layersCount].x, INT);
			GetObjectByNameCjson(_buffer, "y", &temp[layersCount].y, INT);


		}
		*_layers = temp;
		layersCount++;
		layersType = GetIndexPoxiCjson(GetIndexValueCjson(_buffer, "data"), GetIndexValueCjson(_buffer, "draworder"));
	}
	return layersCount;
}

int GetObjectStructObjectCjson(char* _buffer, Object** _object)
{
	Object* ALLOC(temp, 1, sizeof(Object)) 0;

	int objectCount = 0;
	while (GetIndexPoxiCjson(GetIndexValueCjson(_buffer, "height"), GetIndexValueCjson(_buffer, "opacity")) == 1)
	{
		REALLOC(temp, *_object, (size_t)(objectCount + 1) * sizeof(Object)) objectCount;
		temp[objectCount] = (Object){ 0 };

		GetObjectByNameCjson(_buffer, "height", &temp[objectCount].height, INT);
		GetObjectByNameCjson(_buffer, "id", &temp[objectCount].id, INT);
		GetObjectByNameCjson(_buffer, "name", &temp[objectCount].name, CHAR_PTR);
		GetObjectByNameCjson(_buffer, "rotation", &temp[objectCount].rotation, INT);
		GetObjectByNameCjson(_buffer, "type", &temp[objectCount].type, CHAR_PTR);
		GetObjectByNameCjson(_buffer, "visible", &temp[objectCount].visible, BOOL);
		GetObjectByNameCjson(_buffer, "width", &temp[objectCount].width, INT);
		GetObjectByNameCjson(_buffer, "x", &temp[objectCount].x, INT);
		GetObjectByNameCjson(_buffer, "y", &temp[objectCount].y, INT);

		*_object = temp;
		objectCount++;
	}
	return objectCount;
}

int GetObjectStructTilesetCjson(char* _buffer, Tilesets** _tilesets)
{
	Tilesets* ALLOC(temp, 1, sizeof(Tilesets)) 0;

	int tilesetsCount = 0;

	while (GetIndexValueCjson(_buffer, "tilewidth") > GetIndexValueCjson(_buffer, "columns"))
	{
		REALLOC(temp, *_tilesets, (size_t)(tilesetsCount + 1) * sizeof(Tilesets)) tilesetsCount;
		temp[tilesetsCount] = (Tilesets){ 0 };

		GetObjectByNameCjson(_buffer, "columns", &temp[tilesetsCount].columns, INT);
		GetObjectByNameCjson(_buffer, "firstgid", &temp[tilesetsCount].firstgid, INT);
		GetObjectByNameCjson(_buffer, "image", &temp[tilesetsCount].image, CHAR_PTR);
		GetObjectByNameCjson(_buffer, "imageheight", &temp[tilesetsCount].imageheight, INT);
		GetObjectByNameCjson(_buffer, "imagewidth", &temp[tilesetsCount].imagewidth, INT);
		GetObjectByNameCjson(_buffer, "margin", &temp[tilesetsCount].margin, INT);
		GetObjectByNameCjson(_buffer, "name", &temp[tilesetsCount].name, CHAR_PTR);
		GetObjectByNameCjson(_buffer, "spacing", &temp[tilesetsCount].spacing, INT);
		GetObjectByNameCjson(_buffer, "tilecount", &temp[tilesetsCount].tilecount, INT);
		GetObjectByNameCjson(_buffer, "tileheight", &temp[tilesetsCount].tileheight, INT);
		GetObjectByNameCjson(_buffer, "tilewidth", &temp[tilesetsCount].tileheight, INT);

		temp[tilesetsCount].wangsetsCount = GetObjectStructWangsetsCjson(_buffer, &temp[tilesetsCount].wangsets);

		*_tilesets = temp;
		tilesetsCount++;
	}
	return tilesetsCount;
}

int GetObjectStructWangsetsCjson(char* _buffer, Wangsets** _wangsets)
{
	Wangsets* ALLOC(temp, 1, sizeof(Wangsets)) 0;
	int wangsetsCount = 0;

	while (GetIndexValueCjson(_buffer, "type") > GetIndexValueCjson(_buffer, "colors"))
	{
		REALLOC(temp, *_wangsets, (size_t)(wangsetsCount + 1) * sizeof(Wangsets)) wangsetsCount;
		temp[wangsetsCount] = (Wangsets){ 0 };

		GetObjectStructColorsCjson(_buffer, &temp[wangsetsCount].colors);

		int tmepVoid = 0;
		GetObjectByNameCjson(_buffer, "colors", &tmepVoid, INT);

		GetObjectByNameCjson(_buffer, "name", &temp[wangsetsCount].name, CHAR_PTR);
		GetObjectByNameCjson(_buffer, "tile", &temp[wangsetsCount].tile, INT);
		GetObjectByNameCjson(_buffer, "type", &temp[wangsetsCount].type, CHAR_PTR);

		temp[wangsetsCount].wangtilesCount = GetObjectStructWangtilesCjson(_buffer, &temp[wangsetsCount].wangtiles);


		*_wangsets = temp;
		wangsetsCount++;
	}
	return wangsetsCount;
}

int GetObjectStructColorsCjson(char* _buffer, Colors** _colors)
{
	Colors* ALLOC(temp, 1, sizeof(Colors)) 0;
	int colorsCount = 0;

	while (GetIndexValueCjson(_buffer, "probability") > GetIndexValueCjson(_buffer, "name"))
	{
		REALLOC(temp, *_colors, (size_t)(colorsCount + 1) * sizeof(Colors)) colorsCount;
		temp[colorsCount] = (Colors){ 0 };

		GetObjectByNameCjson(_buffer, "color", &temp[colorsCount].color, CHAR_PTR);
		GetObjectByNameCjson(_buffer, "name", &temp[colorsCount].name, CHAR_PTR);
		GetObjectByNameCjson(_buffer, "tile", &temp[colorsCount].tile, INT);
		GetObjectByNameCjson(_buffer, "probability", &temp[colorsCount].probability, FLOAT);

		*_colors = temp;
		colorsCount++;
	}
	return colorsCount;
}

int GetObjectStructWangtilesCjson(char* _buffer, Wangtiles** _wangtiles)
{
	Wangtiles* ALLOC(temp, 1, sizeof(Wangtiles)) 0;
	int wangtilesCount = 0;

	while (GetIndexValueCjson(_buffer, "type") > GetIndexValueCjson(_buffer, "tileid"))
	{
		REALLOC(temp, *_wangtiles, (size_t)(wangtilesCount + 1) * sizeof(Wangtiles)) wangtilesCount;
		temp[wangtilesCount] = (Wangtiles){ 0 };

		GetObjectByNameCjson(_buffer, "tileid", &temp[wangtilesCount].tileid, INT);
		GetObjectByNameCjson(_buffer, "wangid", &temp[wangtilesCount].wangid, INT_PTR);

		*_wangtiles = temp;
		wangtilesCount++;
	}
	return  wangtilesCount;
}

int GetIndexValueCjson(char* _buffer, char* _id1)
{
	int i = 0;
	int j = 0;
	while (_buffer[i])
	{
		j = 0;
		while (_buffer[i + j] == _id1[j])
		{
			j++;
			if (_id1[j] == 0)
			{
				return i;
			}
		}
		i++;
	}
	return i;
}

int GetIndexPoxiCjson(int _index1, int _index2)
{
	if (_index1 < _index2)
	{
		return 1;
	}
	else if (_index1 > _index2)
	{
		return 2;
	}
	return 0;
}

void CleanupCjson(Cjson* _cjson)
{
	if (_cjson->layers)
	{
		if (_cjson->layers->objects)
		{
			for (int i = 0; i < _cjson->layers->objectsCount; i++)
			{
				FREE(_cjson->layers->objects[i].name);
				FREE(_cjson->layers->objects[i].type);
			}
			FREE(_cjson->layers->objects);
		}
		if (_cjson->layers->data)
		{
			for (unsigned row = 0; row < _cjson->layers->height; row++)
			{
				FREE(_cjson->layers->data[row]);
			}
			FREE(_cjson->layers->data);
		}
		for (int i = 0; i < _cjson->layersCount; i++)
		{
			FREE(_cjson->layers[i].draworder);
			FREE(_cjson->layers[i].name);
			FREE(_cjson->layers[i].type);
		}
		FREE(_cjson->layers);
	}
	if (_cjson->tilesets)
	{
		if (_cjson->tilesets->wangsets)
		{
			if (_cjson->tilesets->wangsets->colors)
			{
				for (int i = 0; i < _cjson->tilesets->wangsets->colorsCount; i++)
				{
					FREE(_cjson->tilesets->wangsets->colors[i].name);
					FREE(_cjson->tilesets->wangsets->colors[i].color);
				}
				FREE(_cjson->tilesets->wangsets->colors);
			}
			if (_cjson->tilesets->wangsets->wangtiles)
			{
				for (int i = 0; i < _cjson->tilesets->wangsets->wangtilesCount; i++)
				{
					FREE(_cjson->tilesets->wangsets->wangtiles[i].wangid);
				}
				FREE(_cjson->tilesets->wangsets->wangtiles);
			}
			for (int i = 0; i < _cjson->tilesets->wangsetsCount; i++)
			{
				FREE(_cjson->tilesets->wangsets[i].type);
				FREE(_cjson->tilesets->wangsets[i].name);
			}
			FREE(_cjson->tilesets->wangsets);
		}
		for (int i = 0; i < _cjson->tilesetsCount; i++)
		{
			FREE(_cjson->tilesets[i].image);
			FREE(_cjson->tilesets[i].name);
		}
		FREE(_cjson->tilesets);
	}

	FREE(_cjson->orientation);
	FREE(_cjson->renderOrder);
	FREE(_cjson->tiledVersion);
	FREE(_cjson->type);
	FREE(_cjson->version);
	FREE(_cjson);
}