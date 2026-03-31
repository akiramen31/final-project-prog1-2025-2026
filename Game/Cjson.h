#ifndef CJSON_H
#define CJSON_H

#include <stdio.h>
#include <stdlib.h>

typedef char Bool;
#define FALSE (Bool) 0
#define TRUE (Bool) 1

typedef enum TypeValue
{
	BOOL,
	CHAR,
	INT,
	FLOAT,
	CHAR_PTR,
	INT_PTR,
}TypeValue;

typedef struct Object
{
	unsigned id;
	unsigned x;
	unsigned y;
	unsigned width;
	unsigned height;
	unsigned rotation;
	char* name;
	char* type;
	Bool visible;
}Object;

typedef struct Layers
{
	int id;
	int x;
	int y;
    char* name;
	float opacity;
	char* type;
    Bool visible;

	unsigned** data;
    unsigned width;
    unsigned height;

    char* draworder;
    Object* objects;
	int objectsCount;
}Layers;

typedef struct Colors
{
	char* color;
	char* name;
	float probability;
	int tile;
}Colors;

typedef struct Wangtiles
{
	int tileid;
	int* wangid;
}Wangtiles;

typedef struct Wangsets
{
	Colors* colors;
	int colorsCount;
	char* name;
	int tile;
	char* type;
	Wangtiles* wangtiles;
	int wangtilesCount;
}Wangsets;

typedef struct Tilesets
{
	unsigned columns;
	unsigned firstgid;
	unsigned imagewidth;
	unsigned imageheight;
	unsigned margin;
	unsigned spacing;
	unsigned tilecount;
	unsigned tileheight;
	unsigned tilewidth;
	char* image;
	char* name;
	Wangsets* wangsets;
	int wangsetsCount;
}Tilesets;

typedef struct Cjson
{
	int compressionlevel;
	unsigned width;
	unsigned height;
	unsigned tileWidth;
	unsigned tileHeight;
	unsigned nextLayerId;
	unsigned nextObjectId;
	Bool infinite;
	char* orientation;
	char* renderOrder;
	char* tiledVersion;
	char* type;
	char* version;
	Layers* layers;
	Tilesets* tilesets;
	int layersCount;
	int tilesetsCount;
}Cjson;

Cjson* LoadCjson(char* _file);
void CleanupCjson(Cjson* _cjson);

#endif // CJSON_H