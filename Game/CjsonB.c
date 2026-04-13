#include "CjsonB.h"

#define ALLOC(_size) allocationFree; allocationFree += _size; allocCount += _size;
#define START_GET_STRUCT(type, value) GoNextDataInStructCjsonB();unsigned count = GetStructCountCjsonB();type* temp = *value = (type*)ALLOC(sizeof(type) * count)for (unsigned i = 0; i < count; i++){	while (GoNextDataInStructCjsonB()){
#define END_GET_STRUCT }}return count;

void* __stdcall VirtualAlloc(void* addr, unsigned long long size, unsigned long type, unsigned long protect);
int   __stdcall VirtualFree(void* addr, unsigned long long size, unsigned long type);

int GetObjectStructLayerCjsonB(Layers** _layers);
int GetObjectStructObjectCjsonB(Object** _object);
int GetObjectStructTilesetCjsonB(Tilesets** _tilesets);
int GetObjectStructWangsetsCjsonB(Wangsets** _wangsets);
int GetObjectStructColorsCjsonB(Colors** _colors);
int GetObjectStructWangtilesCjsonB(Wangtiles** _wangtiles);

char StringCompareCjsonB(char* _string);
char GoNextDataInStructCjsonB(void);
unsigned GetStructCountCjsonB(void);

void GetValueInBufferCjsonB(TypeValue _type, void* _value);

char* buffer;
char* allocationFree;
unsigned allocCount;

Cjson* LoadCjsonB(char* _file)
{
	//open file
	int file = open(_file, 0, 0644);
	if (file == -1)
	{
		return 0;
	}
	unsigned long long fileSize = lseek(file, 0, 2);
	lseek(file, 0, 0);
	char* fullBuffer = buffer = VirtualAlloc(0, fileSize + 1, 0x3000, 4);
	read(file, buffer, fileSize);
	close(file);
	allocationFree = VirtualAlloc(0, fileSize * 2, 0x3000, 4);
	Cjson* cjson = (Cjson*)ALLOC(sizeof(Cjson));

	while (GoNextDataInStructCjsonB())
	{
		if (StringCompareCjsonB("infinite"))
		{
			GetValueInBufferCjsonB(BOOL, &cjson->infinite);
		}
		else if (StringCompareCjsonB("width"))
		{
			GetValueInBufferCjsonB(INT, &cjson->width);
		}
		else if (StringCompareCjsonB("height"))
		{
			GetValueInBufferCjsonB(INT, &cjson->height);
		}
		else if (StringCompareCjsonB("tilewidth"))
		{
			GetValueInBufferCjsonB(INT, &cjson->tilewidth);
		}
		else if (StringCompareCjsonB("tileheight"))
		{
			GetValueInBufferCjsonB(INT, &cjson->tileheight);
		}
		else if (StringCompareCjsonB("nextlayerid"))
		{
			GetValueInBufferCjsonB(INT, &cjson->nextlayerid);
		}
		else if (StringCompareCjsonB("nextobjectid"))
		{
			GetValueInBufferCjsonB(INT, &cjson->nextobjectid);
		}
		else if (StringCompareCjsonB("compressionlevel"))
		{
			GetValueInBufferCjsonB(FLOAT, &cjson->compressionlevel);
		}
		else if (StringCompareCjsonB("orientation"))
		{
			GetValueInBufferCjsonB(CHAR_PTR, &cjson->orientation);
		}
		else if (StringCompareCjsonB("renderorder"))
		{
			GetValueInBufferCjsonB(CHAR_PTR, &cjson->renderOrder);
		}
		else if (StringCompareCjsonB("tiledversion"))
		{
			GetValueInBufferCjsonB(CHAR_PTR, &cjson->tiledVersion);
		}
		else if (StringCompareCjsonB("type"))
		{
			GetValueInBufferCjsonB(CHAR_PTR, &cjson->type);
		}
		else if (StringCompareCjsonB("version"))
		{
			GetValueInBufferCjsonB(CHAR_PTR, &cjson->version);
		}
		else if (StringCompareCjsonB("layers"))
		{
			cjson->layersCount = GetObjectStructLayerCjsonB(&cjson->layers);
		}
		else if (StringCompareCjsonB("tilesets"))
		{
			cjson->tilesetsCount = GetObjectStructTilesetCjsonB(&cjson->tilesets);
		}
	}

	VirtualFree(buffer, 0, 0x8000);

	//realloc
	buffer = (char*)cjson;
	allocationFree = VirtualAlloc(0, allocCount, 0x3000, 4);
	for (unsigned i = 0; i < allocCount; i++)
	{
		allocationFree[i] = buffer[i];
	}
	cjson = (Cjson*)allocationFree;

	buffer = 0;
	allocationFree = 0;
	allocCount = 0;

	return cjson;
}

char GoNextDataInStructCjsonB(void)
{
	unsigned i = 0;
	while (buffer[i] != '}')
	{
		i++;
		if (buffer[i] == '\"')
		{
			i++;
			buffer = &buffer[i];
			return 1;
		}
	}
	i++;
	buffer = &buffer[i];
	return 0;
}

void GetValueInBufferCjsonB(TypeValue _type, void* _value)
{
	unsigned i = 0;
	while (buffer[i] != ':')
	{
		i++;
	}
	i++;
	buffer = &buffer[i];

	if (_type == BOOL)
	{
		if (buffer[0] == 't')
		{
			*(char*)_value = 1;
		}
	}
	else if (_type == INT)
	{
		int value = 0;
		while (*buffer >= '0' && *buffer <= '9')
		{
			value = value * 10 + *buffer - '0';
			buffer++;
		}
		*(int*)_value = value;
	}
	else if (_type == FLOAT)
	{
		float value = 0;
		char signe = 1;

		if (*buffer == '-')
		{
			buffer++;
			signe = -1;
		}
		else if (*buffer == '+')
		{
			buffer++;
		}

		while (*buffer >= '0' && *buffer <= '9')
		{
			value = value * 10 + *buffer - '0';
			buffer++;
		}

		if (*buffer == '.')
		{
			float power = 1;
			while (*buffer >= '0' && *buffer <= '9')
			{
				power *= 10;
				value += (*buffer - '0') / power;
				buffer++;
			}
		}
		*(float*)_value = value * signe;
	}
	else if (_type == CHAR_PTR)
	{
		buffer++;
		unsigned size = 0;
		while (buffer[size] != '\"')
		{
			size++;
		}
		char* value = ALLOC(size + 1 * sizeof(char))

			for (unsigned i = 0; i < size; i++)
			{
				value[i] = buffer[i];
			}
		buffer = &buffer[size + 1];
		*(char**)_value = value;
	}
	else if (_type == INT_PTR)
	{
		buffer++;
		unsigned size = 1;
		unsigned i = 1;
		while (buffer[i] != ']')
		{
			if (buffer[i] == ',')
			{
				size++;
			}
			i++;
		}

		unsigned* value = (unsigned*)ALLOC(size * sizeof(unsigned));
		for (i = 0; i < size; i++)
		{
			while (*buffer == ',' || *buffer == ' ' || *buffer == '\t' || *buffer == '\n')
			{
				buffer++;
			}
			while (*buffer >= '0' && *buffer <= '9')
			{
				value[i] = value[i] * 10 + *buffer - '0';
				buffer++;
			}
		}
		*(int**)_value = value;
	}
}

char StringCompareCjsonB(char* _string)
{
	int i = 0;
	while (_string[i] == buffer[i])
	{
		i++;
		if (_string[i] == 0)
		{
			return 1;
		}
	}
	return 0;
}

unsigned GetStructCountCjsonB(void)
{
	int count = 0;
	unsigned i = 0;
	unsigned countC1 = 1;
	unsigned countC2 = 1;
	while (countC2 > 0)
	{
		if (buffer[i] == '{')
		{
			countC1++;
		}
		else if (buffer[i] == '}')
		{
			countC1--;
			if (countC1 == 0)
			{
				count++;
			}
		}
		else if (buffer[i] == '[')
		{
			countC2++;
		}
		else if (buffer[i] == ']')
		{
			countC2--;
		}
		i++;
	}
	return count;
}

int GetObjectStructLayerCjsonB(Layers** _layers)
{
	GoNextDataInStructCjsonB();
	GoNextDataInStructCjsonB();
	unsigned count = GetStructCountCjsonB();
	buffer-= 2;
	Layers* temp = *_layers = (Layers*)ALLOC(sizeof(Layers) * count);
	for (unsigned i = 0; i < count; i++)
	{
		while (GoNextDataInStructCjsonB())
		{
			if (StringCompareCjsonB("visible"))
			{
				GetValueInBufferCjsonB(BOOL, &temp[i].visible);
			}
			else if (StringCompareCjsonB("locked"))
			{
				GetValueInBufferCjsonB(BOOL, &temp[i].locked);
			}
			else if (StringCompareCjsonB("id"))
			{
				GetValueInBufferCjsonB(INT, &temp[i].id);
			}
			else if (StringCompareCjsonB("width"))
			{
				GetValueInBufferCjsonB(INT, &temp[i].width);
			}
			else if (StringCompareCjsonB("height"))
			{
				GetValueInBufferCjsonB(INT, &temp[i].height);
			}
			else if (StringCompareCjsonB("imageheight"))
			{
				GetValueInBufferCjsonB(INT, &temp[i].imageheight);
			}
			else if (StringCompareCjsonB("imagewidth"))
			{
				GetValueInBufferCjsonB(INT, &temp[i].imagewidth);
			}
			else if (StringCompareCjsonB("offsetx"))
			{
				GetValueInBufferCjsonB(FLOAT, &temp[i].offsetx);
			}
			else if (StringCompareCjsonB("offsety"))
			{
				GetValueInBufferCjsonB(FLOAT, &temp[i].offsety);
			}
			else if (StringCompareCjsonB("x"))
			{
				GetValueInBufferCjsonB(FLOAT, &temp[i].x);
			}
			else if (StringCompareCjsonB("y"))
			{
				GetValueInBufferCjsonB(FLOAT, &temp[i].y);
			}
			else if (StringCompareCjsonB("opacity"))
			{
				GetValueInBufferCjsonB(FLOAT, &temp[i].opacity);
			}
			else if (StringCompareCjsonB("name"))
			{
				GetValueInBufferCjsonB(CHAR_PTR, &temp[i].name);
			}
			else if (StringCompareCjsonB("type"))
			{
				GetValueInBufferCjsonB(CHAR_PTR, &temp[i].type);
			}
			else if (StringCompareCjsonB("draworder"))
			{
				GetValueInBufferCjsonB(CHAR_PTR, &temp[i].draworder);
			}
			else if (StringCompareCjsonB("image"))
			{
				GetValueInBufferCjsonB(CHAR_PTR, &temp[i].image);
			}
			else if (StringCompareCjsonB("data"))
			{
				GetValueInBufferCjsonB(INT_PTR, &temp[i].data);
			}
			else if (StringCompareCjsonB("objects"))
			{
				temp[i].objectsCount = GetObjectStructObjectCjsonB(&temp[i].objects);
			}
		}
	}
	return count;
}

int GetObjectStructObjectCjsonB(Object** _object)
{
	GoNextDataInStructCjsonB();
	GoNextDataInStructCjsonB();
	unsigned count = GetStructCountCjsonB();
	buffer -= 2;
	Object* temp = *_object = (Object*)ALLOC(sizeof(Object) * count);
	for (unsigned i = 0; i < count; i++)
	{
		while (GoNextDataInStructCjsonB())
		{
			if (StringCompareCjsonB("visible"))
			{
				GetValueInBufferCjsonB(BOOL, &temp[i].visible);
			}
			else if (StringCompareCjsonB("id"))
			{
				GetValueInBufferCjsonB(INT, &temp[i].id);
			}
			else if (StringCompareCjsonB("width"))
			{
				GetValueInBufferCjsonB(FLOAT, &temp[i].width);
			}
			else if (StringCompareCjsonB("height"))
			{
				GetValueInBufferCjsonB(FLOAT, &temp[i].height);
			}
			else if (StringCompareCjsonB("x"))
			{
				GetValueInBufferCjsonB(FLOAT, &temp[i].x);
			}
			else if (StringCompareCjsonB("y"))
			{
				GetValueInBufferCjsonB(FLOAT, &temp[i].y);
			}
			else if (StringCompareCjsonB("rotation"))
			{
				GetValueInBufferCjsonB(FLOAT, &temp[i].rotation);
			}
			else if (StringCompareCjsonB("opacity"))
			{
				GetValueInBufferCjsonB(FLOAT, &temp[i].opacity);
			}
			else if (StringCompareCjsonB("name"))
			{
				GetValueInBufferCjsonB(CHAR_PTR, &temp[i].name);
			}
			else if (StringCompareCjsonB("type"))
			{
				GetValueInBufferCjsonB(CHAR_PTR, &temp[i].type);
			}
		}
	}
	return count;
}

int GetObjectStructTilesetCjsonB(Tilesets** _tilesets)
{
	GoNextDataInStructCjsonB();
	GoNextDataInStructCjsonB();
	unsigned count = GetStructCountCjsonB();
	buffer -= 2;
	Tilesets* temp = *_tilesets = (Tilesets*)ALLOC(sizeof(Tilesets) * count);
	for (unsigned i = 0; i < count; i++)
	{
		while (GoNextDataInStructCjsonB())
		{
			if (StringCompareCjsonB("columns"))
			{
				GetValueInBufferCjsonB(INT, &temp[i].columns);
			}
			else if (StringCompareCjsonB("firstgid"))
			{
				GetValueInBufferCjsonB(INT, &temp[i].firstgid);
			}
			else if (StringCompareCjsonB("imagewidth"))
			{
				GetValueInBufferCjsonB(INT, &temp[i].imagewidth);
			}
			else if (StringCompareCjsonB("imageheight"))
			{
				GetValueInBufferCjsonB(FLOAT, &temp[i].imageheight);
			}
			else if (StringCompareCjsonB("margin"))
			{
				GetValueInBufferCjsonB(FLOAT, &temp[i].margin);
			}
			else if (StringCompareCjsonB("spacing"))
			{
				GetValueInBufferCjsonB(FLOAT, &temp[i].spacing);
			}
			else if (StringCompareCjsonB("tilecount"))
			{
				GetValueInBufferCjsonB(CHAR_PTR, &temp[i].tilecount);
			}
			else if (StringCompareCjsonB("tileheight"))
			{
				GetValueInBufferCjsonB(CHAR_PTR, &temp[i].tileheight);
			}
			else if (StringCompareCjsonB("tilewidth"))
			{
				GetValueInBufferCjsonB(CHAR_PTR, &temp[i].tilewidth);
			}
			else if (StringCompareCjsonB("image"))
			{
				GetValueInBufferCjsonB(CHAR_PTR, &temp[i].image);
			}
			else if (StringCompareCjsonB("name"))
			{
				GetValueInBufferCjsonB(CHAR_PTR, &temp[i].name);
			}
			else if (StringCompareCjsonB("source"))
			{
				GetValueInBufferCjsonB(CHAR_PTR, &temp[i].source);
			}
			else if (StringCompareCjsonB("wangsets"))
			{
				temp[i].wangsetsCount = GetObjectStructWangsetsCjsonB(&temp[count].wangsets);
			}
		}
	}
	return count;
}

int GetObjectStructWangsetsCjsonB(Wangsets** _wangsets)
{
	GoNextDataInStructCjsonB();
	GoNextDataInStructCjsonB();
	unsigned count = GetStructCountCjsonB();
	buffer -= 2;
	Wangsets* temp = *_wangsets = (Wangsets*)ALLOC(sizeof(Wangsets) * count);
	for (unsigned i = 0; i < count; i++)
	{
		while (GoNextDataInStructCjsonB())
		{
			if (StringCompareCjsonB("tile"))
			{
				GetValueInBufferCjsonB(INT, &temp[i].tile);
			}
			else if (StringCompareCjsonB("name"))
			{
				GetValueInBufferCjsonB(CHAR_PTR, &temp[i].name);
			}
			else if (StringCompareCjsonB("type"))
			{
				GetValueInBufferCjsonB(CHAR_PTR, &temp[i].type);
			}
			else if (StringCompareCjsonB("colors"))
			{
				temp[i].colorsCount = GetObjectStructColorsCjsonB(&temp[count].colors);
			}
			else if (StringCompareCjsonB("wangtiles"))
			{
				temp[i].wangtilesCount = GetObjectStructWangtilesCjsonB(&temp[count].wangtiles);
			}
		}
	}
	return count;
}

int GetObjectStructColorsCjsonB(Colors** _colors)
{
	GoNextDataInStructCjsonB();
	GoNextDataInStructCjsonB();
	unsigned count = GetStructCountCjsonB();
	buffer -= 2;
	Colors* temp = *_colors = (Colors*)ALLOC(sizeof(Colors) * count);
	for (unsigned i = 0; i < count; i++)
	{
		while (GoNextDataInStructCjsonB())
		{
			if (StringCompareCjsonB("tile"))
			{
				GetValueInBufferCjsonB(INT, &temp[i].tile);
			}
			else if (StringCompareCjsonB("probability"))
			{
				GetValueInBufferCjsonB(FLOAT, &temp[i].probability);
			}
			else if (StringCompareCjsonB("color"))
			{
				GetValueInBufferCjsonB(CHAR_PTR, &temp[i].color);
			}
			else if (StringCompareCjsonB("name"))
			{
				GetValueInBufferCjsonB(CHAR_PTR, &temp[i].name);
			}
		}
	}
	return count;
}

int GetObjectStructWangtilesCjsonB(Wangtiles** _wangtiles)
{
	GoNextDataInStructCjsonB();
	GoNextDataInStructCjsonB();
	unsigned count = GetStructCountCjsonB();
	buffer -= 2;
	Wangtiles* temp = *_wangtiles = (Wangtiles*)ALLOC(sizeof(Wangtiles) * count);
	for (unsigned i = 0; i < count; i++)
	{
		while (GoNextDataInStructCjsonB())
		{
			if (StringCompareCjsonB("tileid"))
			{
				GetValueInBufferCjsonB(INT, &temp[i].tileid);
			}
			else if (StringCompareCjsonB("wangid"))
			{
				GetValueInBufferCjsonB(INT_PTR, &temp[i].wangid);
			}
		}
	}
	return count;
}
void CleanupCjsonB(Cjson* _cjson)
{
	VirtualFree(_cjson, 0, 0x8000);
}
