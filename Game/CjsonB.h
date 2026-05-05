#ifndef CJSONB_H
#define CJSONB_H

typedef enum TypeValueCjsonB
{
	BOOL,
	INT,
	FLOAT,
	CHAR_PTR,
	INT_PTR,
}TypeValueCjsonB;

typedef struct PolylineCjsonB
{
	float x;
	float y;
}PolylineCjsonB;

typedef struct TextCjsonB
{
	char wrap;
	char* text;
}TextCjsonB;

typedef struct ObjectCjsonB
{
	char point;
	char visible;
	char ellipse;
	unsigned id;
	float x;
	float y;
	float width;
	float height;
	float rotation;
	float opacity;
	char* name;
	char* type;
	TextCjsonB text;

	PolylineCjsonB* polygon;
	unsigned polygonCount;

	PolylineCjsonB* polyline;
	unsigned polylineCount;
}ObjectCjsonB;

typedef struct LayersCjsonB
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
	char* tintcolor;
	unsigned* classe;
	unsigned* data;

	ObjectCjsonB* objects;
	unsigned objectsCount;

	struct LayersCjsonB* layers;
	unsigned layersCount;
}LayersCjsonB;

typedef struct ColorsCjsonB
{
	unsigned tile;
	float probability;
	char* color;
	char* name;
}ColorsCjsonB;

typedef struct WangtilesCjsonB
{
	unsigned tileid;
	unsigned* wangid;
}WangtilesCjsonB;

typedef struct WangsetsCjsonB
{
	unsigned tile;
	char* name;
	char* type;

	ColorsCjsonB* colors;
	unsigned colorsCount;

	WangtilesCjsonB* wangtiles;
	unsigned wangtilesCount;
}WangsetsCjsonB;

typedef struct AnimationCjsonB
{
	unsigned tileid;
	float duration;
}AnimationCjsonB;

typedef struct TilesCjsonB
{
	unsigned id;

	AnimationCjsonB* animation;
	unsigned animationCount;
}TilesCjsonB;

typedef struct TilesetsCjsonB
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

	WangsetsCjsonB* wangsets;
	unsigned wangsetsCount;

	TilesCjsonB* tiles;
	unsigned tilesCount;
}TilesetsCjsonB;

typedef struct CjsonB
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

	LayersCjsonB* layers;
	unsigned layersCount;

	TilesetsCjsonB* tilesets;
	unsigned tilesetsCount;
}CjsonB;

CjsonB* LoadCjsonB(char* _file);
void CleanupCjsonB(CjsonB* _cjson);

#endif // CJSONB_H