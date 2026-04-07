#include "Cjson.h"

#define FREE(_x) if (_x) free(_x)
#define REALLOC(_exitBlock, _block, _size) _exitBlock = realloc(_block, _size); if (!temp) return
#define ALLOC(_exitBlock, _count, _size) _exitBlock = calloc(_count, _size); if(!_exitBlock) return

int GetObjectByNameCjson(char* _bufferFile, char* _name, void* _saveValue, TypeValue _typeValue);
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
	int first = GetIndexValueCjson(buffer, "compressionlevel");
	first += GetObjectByNameCjson(&buffer[first], "compressionlevel", &cjson->compressionlevel, INT);
	first += GetObjectByNameCjson(&buffer[first], "height", &cjson->height, INT);
	first += GetObjectByNameCjson(&buffer[first], "infinite", &cjson->infinite, BOOL);

	first += GetIndexValueCjson(&buffer[first], "layer");
	cjson->layersCount = GetObjectStructLayerCjson(&buffer[first], &cjson->layers);

	first += GetIndexValueCjson(&buffer[first], "nextlayerid");
	GetObjectByNameCjson(&buffer[first], "nextlayerid", &cjson->nextLayerId, INT);
	GetObjectByNameCjson(&buffer[first], "nextobjectid", &cjson->nextObjectId, INT);
	GetObjectByNameCjson(&buffer[first], "orientation", &cjson->orientation, CHAR_PTR);
	GetObjectByNameCjson(&buffer[first], "renderorder", &cjson->renderOrder, CHAR_PTR);
	GetObjectByNameCjson(&buffer[first], "tiledversion", &cjson->tiledVersion, CHAR_PTR);
	GetObjectByNameCjson(&buffer[first], "tileheight", &cjson->tileHeight, INT);

	first += GetIndexValueCjson(&buffer[first], "tileset");
	cjson->tilesetsCount = GetObjectStructTilesetCjson(&buffer[first], &cjson->tilesets);

	first += GetIndexValueCjson(&buffer[first], "tilewidth");
	GetObjectByNameCjson(&buffer[first], "tilewidth", &cjson->tileWidth, INT);
	GetObjectByNameCjson(&buffer[first], "type", &cjson->type, CHAR_PTR);
	GetObjectByNameCjson(&buffer[first], "version", &cjson->version, CHAR_PTR);
	GetObjectByNameCjson(&buffer[first], "width", &cjson->width, INT);

	free(buffer);
	return cjson;
}

int GetObjectByNameCjson(char* _bufferFile, char* _name, void* _saveValue, TypeValue _typeValue)
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
							return 0;
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
							return 0;
						}

						for (int g = 0; g < nbRow; g++)
						{
							buffer[g] = calloc(nbColumn, sizeof(int*));
							if (!buffer[g])
							{
								return 0;
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
						while (_bufferFile[i] != '\0' && _bufferFile[i] != '\t' && _bufferFile[i] != ',')
						{
							_bufferFile[i] = 'N';
							i++;
						}
						return indexChar;
					}

					while (_bufferFile[i] != '\0' && _bufferFile[i] != '\t' && _bufferFile[i] != ',')
					{
						_bufferFile[i] = 'N';
						i++;
					}


					return i;
				}
			}
			i += j + 1;
		}
	}
	return 0;
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

	int dataIndex = GetIndexValueCjson(_buffer, "data");
	int draworderIndex = GetIndexValueCjson(_buffer, "draworder");

	while (dataIndex != draworderIndex)
	{
		REALLOC(temp, *_layers, (size_t)(layersCount + 1) * sizeof(Layers)) layersCount;
		temp[layersCount] = (Layers){ 0 };
		if (dataIndex < draworderIndex)
		{
			dataIndex += GetObjectByNameCjson(&_buffer[dataIndex], "data", &temp[layersCount].data, INT_PTR);
			dataIndex += GetObjectByNameCjson(&_buffer[dataIndex], "height", &temp[layersCount].height, INT);
			dataIndex += GetObjectByNameCjson(&_buffer[dataIndex], "id", &temp[layersCount].id, INT);
			dataIndex += GetObjectByNameCjson(&_buffer[dataIndex], "name", &temp[layersCount].name, CHAR_PTR);
			dataIndex += GetObjectByNameCjson(&_buffer[dataIndex], "opacity", &temp[layersCount].opacity, FLOAT);
			dataIndex += GetObjectByNameCjson(&_buffer[dataIndex], "type", &temp[layersCount].type, CHAR_PTR);
			dataIndex += GetObjectByNameCjson(&_buffer[dataIndex], "visible", &temp[layersCount].visible, BOOL);
			dataIndex += GetObjectByNameCjson(&_buffer[dataIndex], "width", &temp[layersCount].width, INT);
			dataIndex += GetObjectByNameCjson(&_buffer[dataIndex], "x", &temp[layersCount].x, INT);
			dataIndex += GetObjectByNameCjson(&_buffer[dataIndex], "y", &temp[layersCount].y, INT);
		}
		else if (dataIndex >  draworderIndex)
		{
			draworderIndex += GetObjectByNameCjson(&_buffer[draworderIndex], "draworder", &temp[layersCount].draworder, CHAR_PTR);
			draworderIndex += GetObjectByNameCjson(&_buffer[draworderIndex], "id", &temp[layersCount].id, INT);
			draworderIndex += GetObjectByNameCjson(&_buffer[draworderIndex], "name", &temp[layersCount].name, CHAR_PTR);

			temp[layersCount].objectsCount = GetObjectStructObjectCjson(&_buffer[draworderIndex], &temp[layersCount].objects);

			draworderIndex += GetObjectByNameCjson(&_buffer[draworderIndex], "opacity", &temp[layersCount].opacity, FLOAT);
			draworderIndex += GetObjectByNameCjson(&_buffer[draworderIndex], "type", &temp[layersCount].type, CHAR_PTR);
			draworderIndex += GetObjectByNameCjson(&_buffer[draworderIndex], "visible", &temp[layersCount].visible, BOOL);
			draworderIndex += GetObjectByNameCjson(&_buffer[draworderIndex], "x", &temp[layersCount].x, INT);
			draworderIndex += GetObjectByNameCjson(&_buffer[draworderIndex], "y", &temp[layersCount].y, INT);


		}
		*_layers = temp;
		layersCount++;

		dataIndex = GetIndexValueCjson(_buffer, "data");
		draworderIndex = GetIndexValueCjson(_buffer, "draworder");
	}
	return layersCount;
}

int GetObjectStructObjectCjson(char* _buffer, Object** _object)
{
	Object* ALLOC(temp, 1, sizeof(Object)) 0;

	int first = 0;

	int objectCount = 0;
	while (first = GetIndexValueCjson(_buffer, "\"height\"") < GetIndexValueCjson(_buffer, "id"))
	{
		REALLOC(temp, *_object, (size_t)(objectCount + 1) * sizeof(Object)) objectCount;
		temp[objectCount] = (Object){ 0 };

		GetObjectByNameCjson(&_buffer[first], "height", &temp[objectCount].height, INT);
		GetObjectByNameCjson(&_buffer[first], "id", &temp[objectCount].id, INT);
		GetObjectByNameCjson(&_buffer[first], "name", &temp[objectCount].name, CHAR_PTR);
		GetObjectByNameCjson(&_buffer[first], "rotation", &temp[objectCount].rotation, INT);
		GetObjectByNameCjson(&_buffer[first], "type", &temp[objectCount].type, CHAR_PTR);
		GetObjectByNameCjson(&_buffer[first], "visible", &temp[objectCount].visible, BOOL);
		GetObjectByNameCjson(&_buffer[first], "width", &temp[objectCount].width, INT);
		GetObjectByNameCjson(&_buffer[first], "x", &temp[objectCount].x, INT);
		GetObjectByNameCjson(&_buffer[first], "y", &temp[objectCount].y, INT);
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