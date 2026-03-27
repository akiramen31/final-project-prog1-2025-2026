#ifndef CUTE_TILED_H
#define CUTE_TILED_H

#include <stdint.h> /* uint32_t */

/* Read this in the event of errors */
extern const char* cute_tiled_error_reason;
extern int cute_tiled_error_line;

typedef struct cute_tiled_map_t cute_tiled_map_t;
typedef struct cute_tiled_tileset_t cute_tiled_tileset_t;

/*!
 * Load a map from disk, placed into heap allocated memory. \p mem_ctx can be
 * NULL. It is used for custom allocations.
 */
cute_tiled_map_t* cute_tiled_load_map_from_file(const char* path, void* mem_ctx);

/*!
 * Load a map from memory. \p mem_ctx can be NULL. It is used for custom allocations.
 */
cute_tiled_map_t* cute_tiled_load_map_from_memory(const void* memory, int size_in_bytes, void* mem_ctx);

/*!
 * Reverses the layers order, so they appear in reverse-order from what is shown in the Tiled editor.
 */
void cute_tiled_reverse_layers(cute_tiled_map_t* map);

/*!
 * Free all dynamic memory associated with this map.
 */
void cute_tiled_free_map(cute_tiled_map_t* map);

/*!
 * Load an external tileset from disk, placed into heap allocated memory. \p mem_ctx can be
 * NULL. It is used for custom allocations.
 */
cute_tiled_tileset_t* cute_tiled_load_external_tileset(const char* path, void* mem_ctx);

/*!
 * Load an external tileset from memory. \p mem_ctx can be NULL. It is used for custom allocations.
 */
cute_tiled_tileset_t* cute_tiled_load_external_tileset_from_memory(const void* memory, int size_in_bytes, void* mem_ctx);

/*!
 * Free all dynamic memory associated with this external tileset.
 */
void cute_tiled_free_external_tileset(cute_tiled_tileset_t* tileset);

/* -----------------------------------------------------------------------
   Macros / Inline helpers
----------------------------------------------------------------------- */

#if !defined(CUTE_TILED_U64)
#define CUTE_TILED_U64 unsigned long long
#endif

#if !defined(CUTE_TILED_INLINE)
#if defined(_MSC_VER)
#define CUTE_TILED_INLINE __inline
#else
#define CUTE_TILED_INLINE __inline__
#endif
#endif

/* -----------------------------------------------------------------------
   Forward declarations
----------------------------------------------------------------------- */

typedef struct cute_tiled_layer_t cute_tiled_layer_t;
typedef struct cute_tiled_object_t cute_tiled_object_t;
typedef struct cute_tiled_frame_t cute_tiled_frame_t;
typedef struct cute_tiled_tile_descriptor_t cute_tiled_tile_descriptor_t;
typedef struct cute_tiled_property_t cute_tiled_property_t;
typedef union cute_tiled_string_t cute_tiled_string_t;

/*!
 * To access a string, simply do: object->name.ptr
 */
union cute_tiled_string_t
{
	const char* ptr;
	CUTE_TILED_U64 hash_id;
};

typedef enum CUTE_TILED_PROPERTY_TYPE
{
	CUTE_TILED_PROPERTY_NONE,
	CUTE_TILED_PROPERTY_INT,
	CUTE_TILED_PROPERTY_BOOL,
	CUTE_TILED_PROPERTY_FLOAT,
	CUTE_TILED_PROPERTY_STRING,
	CUTE_TILED_PROPERTY_FILE,   /* reported as string */
	CUTE_TILED_PROPERTY_COLOR,
} CUTE_TILED_PROPERTY_TYPE;

struct cute_tiled_property_t
{
	union
	{
		int integer;
		int boolean;
		float floating;
		cute_tiled_string_t string;
		cute_tiled_string_t file;
		uint32_t color;
	} data;
	CUTE_TILED_PROPERTY_TYPE type;
	cute_tiled_string_t name;
};

struct cute_tiled_object_t
{
	int ellipse;
	int gid;
	float height;
	int id;
	cute_tiled_string_t name;
	int point;
	int vert_count;
	float* vertices;
	int vert_type;
	int property_count;
	cute_tiled_property_t* properties;
	float rotation;
	/* template */  /* Not currently supported. */
	/* text */      /* Not currently supported. */
	cute_tiled_string_t type;
	/* class */     /* Not currently supported. */
	int visible;
	float width;
	float x;
	float y;
	cute_tiled_object_t* next;
};

/* -----------------------------------------------------------------------
   GID flip flag helpers
----------------------------------------------------------------------- */

#define CUTE_TILED_FLIPPED_HORIZONTALLY_FLAG 0x80000000
#define CUTE_TILED_FLIPPED_VERTICALLY_FLAG   0x40000000
#define CUTE_TILED_FLIPPED_DIAGONALLY_FLAG   0x20000000

static CUTE_TILED_INLINE int cute_tiled_unset_flags(int tile_data_gid)
{
	const int flags = ~(CUTE_TILED_FLIPPED_HORIZONTALLY_FLAG | CUTE_TILED_FLIPPED_VERTICALLY_FLAG | CUTE_TILED_FLIPPED_DIAGONALLY_FLAG);
	return tile_data_gid & flags;
}

static CUTE_TILED_INLINE void cute_tiled_get_flags(int tile_data_gid, int* flip_horizontal, int* flip_vertical, int* flip_diagonal)
{
	*flip_horizontal = !!(tile_data_gid & CUTE_TILED_FLIPPED_HORIZONTALLY_FLAG);
	*flip_vertical = !!(tile_data_gid & CUTE_TILED_FLIPPED_VERTICALLY_FLAG);
	*flip_diagonal = !!(tile_data_gid & CUTE_TILED_FLIPPED_DIAGONALLY_FLAG);
}

/* -----------------------------------------------------------------------
   Structs
----------------------------------------------------------------------- */

struct cute_tiled_layer_t
{
	/* chunks */                        /* Not currently supported. */
	cute_tiled_string_t class_;
	/* compression */                   /* Not currently supported. */
	int data_count;
	int* data;
	cute_tiled_string_t draworder;
	/* encoding */                      /* Not currently supported. */
	int height;
	cute_tiled_layer_t* layers;
	cute_tiled_string_t name;
	cute_tiled_object_t* objects;
	float offsetx;
	float offsety;
	float opacity;
	int property_count;
	cute_tiled_property_t* properties;
	uint32_t transparentcolor;
	uint32_t tintcolor;
	cute_tiled_string_t type;
	cute_tiled_string_t image;
	int visible;
	int width;
	int x;
	int y;
	float parallaxx;
	float parallaxy;
	int repeatx;
	int repeaty;
	int id;
	cute_tiled_layer_t* next;
};

struct cute_tiled_frame_t
{
	int duration;
	int tileid;
};

struct cute_tiled_tile_descriptor_t
{
	int tile_index;
	cute_tiled_string_t type;
	int frame_count;
	cute_tiled_frame_t* animation;
	cute_tiled_string_t image;
	int imageheight;
	int imagewidth;
	cute_tiled_layer_t* objectgroup;
	int property_count;
	cute_tiled_property_t* properties;
	/* terrain */                       /* Not currently supported. */
	float probability;
	cute_tiled_tile_descriptor_t* next;
};

struct cute_tiled_tileset_t
{
	uint32_t backgroundcolor;
	cute_tiled_string_t class_;
	int columns;
	int firstgid;
	/* grid */                          /* Not currently supported. */
	cute_tiled_string_t image;
	int imagewidth;
	int imageheight;
	int margin;
	cute_tiled_string_t name;
	cute_tiled_string_t objectalignment;
	int property_count;
	cute_tiled_property_t* properties;
	int spacing;
	/* terrains */                      /* Not currently supported. */
	int tilecount;
	cute_tiled_string_t tiledversion;
	int tileheight;
	int tileoffset_x;
	int tileoffset_y;
	cute_tiled_tile_descriptor_t* tiles;
	int tilewidth;
	uint32_t transparentcolor;
	cute_tiled_string_t type;
	cute_tiled_string_t source;
	cute_tiled_tileset_t* next;
	float version;
	void* _internal;
};

struct cute_tiled_map_t
{
	uint32_t backgroundcolor;
	cute_tiled_string_t class_;
	int height;
	/* hexsidelength */                 /* Not currently supported. */
	int infinite;
	cute_tiled_layer_t* layers;
	int nextobjectid;
	cute_tiled_string_t orientation;
	int property_count;
	cute_tiled_property_t* properties;
	cute_tiled_string_t renderorder;
	/* staggeraxis */                   /* Not currently supported. */
	/* staggerindex */                  /* Not currently supported. */
	cute_tiled_string_t tiledversion;
	int tileheight;
	cute_tiled_tileset_t* tilesets;
	int tilewidth;
	cute_tiled_string_t type;
	float version;
	int width;
	int nextlayerid;
};

#endif /* CUTE_TILED_H */

/*
	------------------------------------------------------------------------------
	This software is available under 2 licenses - you may choose the one you like.
	------------------------------------------------------------------------------
	ALTERNATIVE A - zlib license
	Copyright (c) 2017 Randy Gaul http://www.randygaul.net
	This software is provided 'as-is', without any express or implied warranty.
	In no event will the authors be held liable for any damages arising from
	the use of this software.
	Permission is granted to anyone to use this software for any purpose,
	including commercial applications, and to alter it and redistribute it
	freely, subject to the following restrictions:
	  1. The origin of this software must not be misrepresented; you must not
		 claim that you wrote the original software. If you use this software
		 in a product, an acknowledgment in the product documentation would be
		 appreciated but is not required.
	  2. Altered source versions must be plainly marked as such, and must not
		 be misrepresented as being the original software.
	  3. This notice may not be removed or altered from any source distribution.
	------------------------------------------------------------------------------
	ALTERNATIVE B - Public Domain (www.unlicense.org)
	This is free and unencumbered software released into the public domain.
	Anyone is free to copy, modify, publish, use, compile, sell, or distribute this
	software, either in source code form or as a compiled binary, for any purpose,
	commercial or non-commercial, and by any means.
	In jurisdictions that recognize copyright laws, the author or authors of this
	software dedicate any and all copyright interest in the software to the public
	domain. We make this dedication for the benefit of the public at large and to
	the detriment of our heirs and successors. We intend this dedication to be an
	overt act of relinquishment in perpetuity of all present and future rights to
	this software under copyright law.
	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
	ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
	WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
	------------------------------------------------------------------------------
*/