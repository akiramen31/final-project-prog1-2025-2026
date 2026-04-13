#ifndef CJSONB_H
#define CJSONB_H

typedef enum TypeValue
{
	BOOL,
	INT,
	FLOAT,
	CHAR_PTR,
	INT_PTR,
}TypeValue;

typedef struct Object
{
	char visible;
	unsigned id;
	float x;
	float y;
	float width;
	float height;
	float rotation;
	float opacity;
	char* name;
	char* type;
}Object;

typedef struct Layers
{
	char visible;
	char locked;
	unsigned id;
	unsigned width;
	unsigned height;
	unsigned imageheight;
	unsigned imagewidth;
	float offsetx;
	float offsety;
	float x;
	float y;
	float opacity;
	char* name;
	char* type;
	char* draworder;
	char* image;
	unsigned* data;

	Object* objects;
	unsigned objectsCount;
}Layers;

typedef struct Colors
{
	unsigned tile;
	float probability;
	char* color;
	char* name;
}Colors;

typedef struct Wangtiles
{
	unsigned tileid;
	int* wangid;
}Wangtiles;

typedef struct Wangsets
{
	unsigned tile;
	char* name;
	char* type;

	Colors* colors;
	unsigned colorsCount;

	Wangtiles* wangtiles;
	unsigned wangtilesCount;
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
	char* source;

	Wangsets* wangsets;
	unsigned wangsetsCount;
}Tilesets;

typedef struct Cjson
{
	char infinite;
	unsigned width;
	unsigned height;
	unsigned tilewidth;
	unsigned tileheight;
	unsigned nextlayerid;
	unsigned nextobjectid;
	float compressionlevel;
	char* orientation;
	char* renderOrder;
	char* tiledVersion;
	char* type;
	char* version;

	Layers* layers;
	unsigned layersCount;

	Tilesets* tilesets;
	unsigned tilesetsCount;
}Cjson;

Cjson* LoadCjsonB(char* _file);
void CleanupCjsonB(Cjson* _cjson);

#endif // CJSONB_H