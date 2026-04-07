#include "Cjson.h"

#define FREE(_x) if (_x) free(_x)
#define REALLOC(_exitBlock, _block, _size) _exitBlock = realloc(_block, _size); if (!temp) return
#define ALLOC(_exitBlock, _count, _size) _exitBlock = calloc(_count, _size); if(!_exitBlock) return

void GetObjectByNameCjson(char* _name, void* _saveValue, TypeValue _typeValue);
Bool StringCompareCjson(char* _string1, char* _string2);
int GetObjectStructLayerCjson(Layers** _layers);
int GetObjectStructObjectCjson(Object** _object);
int GetObjectStructTilesetCjson(Tilesets** _tilesets);
int GetObjectStructWangsetsCjson(Wangsets** _wangsets);
int GetObjectStructColorsCjson(Colors** _colors);
int GetObjectStructWangtilesCjson(Wangtiles** _wangtiles);
int GetIndexValueCjson(char* _id1);
int GetIndexPoxiCjson(int _index1, int _index2);

unsigned index;
char* buffer;

Cjson* LoadCjson(char* _file)
{
	index = 0;
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

	buffer = calloc((size_t)(fileSize + 1), sizeof(char));
	if (!buffer)
	{
		fclose(file);
		return NULL;
	}

	fread(buffer, sizeof(char), (size_t)(fileSize + 1), file);
	GetObjectByNameCjson("compressionlevel", &cjson->compressionlevel, INT);
	GetObjectByNameCjson("height", &cjson->height, INT);
	GetObjectByNameCjson("infinite", &cjson->infinite, BOOL);

	cjson->layersCount = GetObjectStructLayerCjson(&cjson->layers);

	GetObjectByNameCjson("nextlayerid", &cjson->nextLayerId, INT);
	GetObjectByNameCjson("nextobjectid", &cjson->nextObjectId, INT);
	GetObjectByNameCjson("orientation", &cjson->orientation, CHAR_PTR);
	GetObjectByNameCjson("renderorder", &cjson->renderOrder, CHAR_PTR);
	GetObjectByNameCjson("tiledversion", &cjson->tiledVersion, CHAR_PTR);
	GetObjectByNameCjson("tileheight", &cjson->tileHeight, INT);

	cjson->tilesetsCount = GetObjectStructTilesetCjson(&cjson->tilesets);

	GetObjectByNameCjson("tilewidth", &cjson->tileWidth, INT);
	GetObjectByNameCjson("type", &cjson->type, CHAR_PTR);
	GetObjectByNameCjson("version", &cjson->version, CHAR_PTR);
	GetObjectByNameCjson("width", &cjson->width, INT);

	free(buffer);
	return cjson;
}

void GetObjectByNameCjson(char* _name, void* _saveValue, TypeValue _typeValue)
{
	int tempCalcule = 0;
	int i = index;
	int j = 0;
	while (buffer[i])
	{
		j = 0;
		tempCalcule = i + j;
		while (buffer[tempCalcule] == _name[j])
		{
			j++;
			tempCalcule = i + j;
			if (_name[j] == 0 && buffer[i - 1] == '\"' && buffer[tempCalcule] == '\"')
			{
				if (_typeValue == BOOL)
				{
					if (buffer[i + j + 2] == 't')
					{
						*(Bool*)_saveValue = TRUE;
					}
					else if (buffer[i + j + 2] == 'f')
					{
						*(Bool*)_saveValue = FALSE;
					}
				}
				else if (_typeValue == CHAR)
				{
					sscanf_s(&buffer[i + j], "%*[^0-9-]%d", (int*)_saveValue);
				}
				else if (_typeValue == INT)
				{
					sscanf_s(&buffer[i + j], "%*[^0-9-]%d", (int*)_saveValue);
				}
				else if (_typeValue == FLOAT)
				{
					sscanf_s(&buffer[i + j], "%*[^0-9-]%f", (float*)_saveValue);
				}
				else if (_typeValue == CHAR_PTR)
				{
					char* value = calloc(30, sizeof(char));
					if (!value)
					{
						return;
					}

					int h = 3;
					do {
						value[h - 3] = buffer[i + j + h];
						h++;
					} while (buffer[i + j + h] != '\"');

					*(char**)_saveValue = realloc(value, (size_t)(h - 2));
				}
				else if (_typeValue == INT_PTR)
				{
					int count = 0;
					int nbColumn = 1;
					int nbRow = 1;

					while (buffer[i + j + count] != ']')
					{
						if (buffer[i + j + count] == '\n')
						{
							nbRow++;
						}
						if (buffer[i + j + count] == ',')
						{
							nbColumn++;
						}
						count++;
					}
					nbColumn /= nbRow;

					int** value = *(int***)_saveValue = calloc(nbRow, sizeof(int*));
					if (!value)
					{
						return;
					}

					for (int g = 0; g < nbRow; g++)
					{
						value[g] = calloc(nbColumn, sizeof(int*));
						if (!value[g])
						{
							return;
						}
					}

					int indexChar = 0;
					for (int row = 0; row < nbRow; row++)
					{
						for (int column = 0; column < nbColumn; column++)
						{
							if (buffer[i + j + indexChar - 1] == ',' || buffer[i + j + indexChar - 1] == '[')
							{
								if (buffer[i + j + indexChar] == '\n')
								{
									indexChar += 6;
								}
								sscanf_s(&buffer[i + j + indexChar], "%d", &value[row][column]);

							}
							else
							{
								column--;
							}
							indexChar++;
						}
					}
				}

				while (buffer[i] != '\0' && buffer[i] != '\t' && buffer[i] != ',')
				{
					buffer[i] = 'N';
					i++;
				}
				index = i;
				return;
			}
		}
		i += j + 1;
	}
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

int GetObjectStructLayerCjson(Layers** _layers)
{
	Layers* ALLOC(temp, 1, sizeof(Layers)) 0;

	int layersCount = 0;

	int dataIndex = GetIndexValueCjson("data");
	int draworderIndex = GetIndexValueCjson("draworder");
	int imageIndex = GetIndexValueCjson("image");

	while (dataIndex != draworderIndex || dataIndex != imageIndex)
	{
		REALLOC(temp, *_layers, (size_t)(layersCount + 1) * sizeof(Layers)) layersCount;
		temp[layersCount] = (Layers){ 0 };
		if (dataIndex < draworderIndex && dataIndex < imageIndex)
		{
			GetObjectByNameCjson("data", &temp[layersCount].data, INT_PTR);
			GetObjectByNameCjson("height", &temp[layersCount].height, INT);
			GetObjectByNameCjson("id", &temp[layersCount].id, INT);
			GetObjectByNameCjson("name", &temp[layersCount].name, CHAR_PTR);
			GetObjectByNameCjson("opacity", &temp[layersCount].opacity, FLOAT);
			GetObjectByNameCjson("type", &temp[layersCount].type, CHAR_PTR);
			GetObjectByNameCjson("visible", &temp[layersCount].visible, BOOL);
			GetObjectByNameCjson("width", &temp[layersCount].width, INT);
		}
		else if (draworderIndex < imageIndex)
		{
			GetObjectByNameCjson("draworder", &temp[layersCount].draworder, CHAR_PTR);
			GetObjectByNameCjson("id", &temp[layersCount].id, INT);
			GetObjectByNameCjson("name", &temp[layersCount].name, CHAR_PTR);

			temp[layersCount].objectsCount = GetObjectStructObjectCjson(&temp[layersCount].objects);

			GetObjectByNameCjson("opacity", &temp[layersCount].opacity, FLOAT);
			GetObjectByNameCjson("type", &temp[layersCount].type, CHAR_PTR);
			GetObjectByNameCjson("visible", &temp[layersCount].visible, BOOL);
		}
		else
		{
			GetObjectByNameCjson("id", &temp[layersCount].id, INT);
			GetObjectByNameCjson("image", &temp[layersCount].draworder, CHAR_PTR);
			GetObjectByNameCjson("imageheight", &temp[layersCount].height, INT);
			GetObjectByNameCjson("imagewidth", &temp[layersCount].width, INT);
			GetObjectByNameCjson("locked", &temp[layersCount].locked, BOOL);
			GetObjectByNameCjson("name", &temp[layersCount].name, CHAR_PTR);
			GetObjectByNameCjson("offsetx", &temp[layersCount].offsetx, INT);
			GetObjectByNameCjson("offsety", &temp[layersCount].offsety, INT);
			GetObjectByNameCjson("opacity", &temp[layersCount].opacity, FLOAT);
			GetObjectByNameCjson("type", &temp[layersCount].type, CHAR_PTR);
			GetObjectByNameCjson("visible", &temp[layersCount].visible, BOOL);
		}
		GetObjectByNameCjson("x", &temp[layersCount].x, INT);
		GetObjectByNameCjson("y", &temp[layersCount].y, INT);
		*_layers = temp;
		layersCount++;

		dataIndex = GetIndexValueCjson("data");
		draworderIndex = GetIndexValueCjson("draworder");
		imageIndex = GetIndexValueCjson("image");
	}
	return layersCount;
}

int GetObjectStructObjectCjson(Object** _object)
{
	Object* ALLOC(temp, 1, sizeof(Object)) 0;
	int objectCount = 0;
	while (GetIndexValueCjson("height") < GetIndexValueCjson("type"))
	{
		REALLOC(temp, *_object, (size_t)(objectCount + 1) * sizeof(Object)) objectCount;
		temp[objectCount] = (Object){ 0 };

		GetObjectByNameCjson("height", &temp[objectCount].height, INT);
		GetObjectByNameCjson("id", &temp[objectCount].id, INT);
		GetObjectByNameCjson("name", &temp[objectCount].name, CHAR_PTR);
		GetObjectByNameCjson("rotation", &temp[objectCount].rotation, INT);
		GetObjectByNameCjson("type", &temp[objectCount].type, CHAR_PTR);
		GetObjectByNameCjson("visible", &temp[objectCount].visible, BOOL);
		GetObjectByNameCjson("width", &temp[objectCount].width, INT);
		GetObjectByNameCjson("x", &temp[objectCount].x, INT);
		GetObjectByNameCjson("y", &temp[objectCount].y, INT);
		*_object = temp;
		objectCount++;
	}
	return objectCount;
}

int GetObjectStructTilesetCjson(Tilesets** _tilesets)
{
	Tilesets* ALLOC(temp, 1, sizeof(Tilesets)) 0;

	int tilesetsCount = 0;

	while (GetIndexValueCjson("tilewidth") > GetIndexValueCjson("columns"))
	{
		REALLOC(temp, *_tilesets, (size_t)(tilesetsCount + 1) * sizeof(Tilesets)) tilesetsCount;
		temp[tilesetsCount] = (Tilesets){ 0 };

		GetObjectByNameCjson("columns", &temp[tilesetsCount].columns, INT);
		GetObjectByNameCjson("firstgid", &temp[tilesetsCount].firstgid, INT);
		GetObjectByNameCjson("image", &temp[tilesetsCount].image, CHAR_PTR);
		GetObjectByNameCjson("imageheight", &temp[tilesetsCount].imageheight, INT);
		GetObjectByNameCjson("imagewidth", &temp[tilesetsCount].imagewidth, INT);
		GetObjectByNameCjson("margin", &temp[tilesetsCount].margin, INT);
		GetObjectByNameCjson("name", &temp[tilesetsCount].name, CHAR_PTR);
		GetObjectByNameCjson("spacing", &temp[tilesetsCount].spacing, INT);
		GetObjectByNameCjson("tilecount", &temp[tilesetsCount].tilecount, INT);
		GetObjectByNameCjson("tileheight", &temp[tilesetsCount].tileheight, INT);
		GetObjectByNameCjson("tilewidth", &temp[tilesetsCount].tileheight, INT);

		temp[tilesetsCount].wangsetsCount = GetObjectStructWangsetsCjson(&temp[tilesetsCount].wangsets);

		*_tilesets = temp;
		tilesetsCount++;
	}
	return tilesetsCount;
}

int GetObjectStructWangsetsCjson(Wangsets** _wangsets)
{
	Wangsets* ALLOC(temp, 1, sizeof(Wangsets)) 0;
	int wangsetsCount = 0;
	int colorIndex = GetIndexValueCjson("colors");
	int wangtiles = GetIndexValueCjson("wangtiles");

	while (colorIndex != wangtiles)
	{
		REALLOC(temp, *_wangsets, (size_t)(wangsetsCount + 1) * sizeof(Wangsets)) wangsetsCount;
		temp[wangsetsCount] = (Wangsets){ 0 };

		if (colorIndex < wangtiles)
		{
			buffer[colorIndex] = 'N';
			GetObjectStructColorsCjson(&temp[wangsetsCount].colors);
			GetObjectByNameCjson("name", &temp[wangsetsCount].name, CHAR_PTR);
			GetObjectByNameCjson("tile", &temp[wangsetsCount].tile, INT);
			GetObjectByNameCjson("type", &temp[wangsetsCount].type, CHAR_PTR);
		}
		else if (colorIndex > wangtiles)
		{
			buffer[wangtiles] = 'N';
			temp[wangsetsCount].wangtilesCount = GetObjectStructWangtilesCjson(&temp[wangsetsCount].wangtiles);
		}
		*_wangsets = temp;
		wangsetsCount++;
		colorIndex = GetIndexValueCjson("colors");
		wangtiles = GetIndexValueCjson("wangtiles");
	}
	return wangsetsCount;
}

int GetObjectStructColorsCjson(Colors** _colors)
{
	Colors* ALLOC(temp, 1, sizeof(Colors)) 0;
	int colorsCount = 0;

	while (GetIndexValueCjson("probability") > GetIndexValueCjson("name"))
	{
		REALLOC(temp, *_colors, (size_t)(colorsCount + 1) * sizeof(Colors)) colorsCount;
		temp[colorsCount] = (Colors){ 0 };

		GetObjectByNameCjson("color", &temp[colorsCount].color, CHAR_PTR);
		GetObjectByNameCjson("name", &temp[colorsCount].name, CHAR_PTR);
		GetObjectByNameCjson("tile", &temp[colorsCount].tile, INT);
		GetObjectByNameCjson("probability", &temp[colorsCount].probability, FLOAT);

		*_colors = temp;
		colorsCount++;
	}
	return colorsCount;
}

int GetObjectStructWangtilesCjson(Wangtiles** _wangtiles)
{
	Wangtiles* ALLOC(temp, 1, sizeof(Wangtiles)) 0;
	int wangtilesCount = 0;

	while (GetIndexValueCjson("type") > GetIndexValueCjson("tileid"))
	{
		REALLOC(temp, *_wangtiles, (size_t)(wangtilesCount + 1) * sizeof(Wangtiles)) wangtilesCount;
		temp[wangtilesCount] = (Wangtiles){ 0 };

		GetObjectByNameCjson("tileid", &temp[wangtilesCount].tileid, INT);
		GetObjectByNameCjson("wangid", &temp[wangtilesCount].wangid, INT_PTR);

		*_wangtiles = temp;
		wangtilesCount++;
	}
	return  wangtilesCount;
}

int GetIndexValueCjson(char* _id1)
{
	int i = index;
	int j = 0;
	while (buffer[i])
	{
		j = 0;
		while (buffer[i + j] == _id1[j])
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