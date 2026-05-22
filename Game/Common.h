#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <process.h>
#include <math.h>
#include "SFML/Graphics.h"
#include "SFML/Audio.h"

#define GAME_SCALE 1
#define GAME_VOLUME 10.f
#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

// NEEDED FOR BULLET AND WEAPON
#define PLAYER_COLLISION_WIDTH 12
#define PLAYER_COLLISION_HEIGHT 30
#define WEAPON_ORIGIN PLAYER_COLLISION_HEIGHT * 0.75f - 1.0f

#define FONT "Assets/Fonts/GallaeciaForte.ttf"

#define BPP 32

#define MAP_COUNT 3
#define WEAPON_COUNT 3
#define SECONDARY_COUNT 2

#define DEV_MODE sfFalse
#define DEV_WEAPON sfFalse
#define DEBUG_MODE_A_STAR sfFalse
#define DEV_MODE_CAMERA sfFalse
#define DEV_PRINT_ERROR sfFalse

#define TILE_SIZE 16
#define FREEZE_TIMER 4.f

#define COLOR_ORANGE (sfColor){ 255, 165, 0 , 255 }

#define G 5.81f
//#define G 9.81f

typedef enum CollisionAxis
{
	AXIS_X,
	AXIS_Y,
	AXIS_BOTH
} CollisionAxis;

typedef enum MapState
{
	LEVEL1,
	LEVEL2,
	LEVEL3,
	LEVEL_TEST
}MapState;

typedef enum AttackType
{
	NOATTACK,
	FREEZE,
	LIGHT,
	MEDIUM,
	HEAVY
}AttackType;

typedef enum ArmorType
{
	LIGHT_ARMOR,
	MEDIUM_ARMOR,
	HEAVY_ARMOR
}ArmorType;

typedef struct ShooterType
{
	float weaponPos;
	sfVector2f shootPosition;
	AttackType bulletType;
	sfBool isAlly;
	sfBool isRighted;
	sfBool isBoss2;
}ShooterType;

typedef enum EnergyEnum
{
	ENERGY,
	ENERGY_MAX,
	ENERGY_REGEN,
	ENERGY_REGEN_COLDBREATH,
	ENERGY_REGEN_COOLDOWN,
}EnergyEnum;

typedef enum Direction
{
	EMPTY_DIRECTION,
	DOWN_LEFT,
	DOWN,
	DOWN_RIGHT,
	LEFT,
	NO_DIRECTION,
	RIGHT,
	UP_LEFT,
	UP,
	UP_RIGHT,
}Direction;

typedef struct Animation
{
	sfIntRect rectActualy;
	sfBool isLooping;
	int frameCount;
	float frameDuration;
	float timeActualy;
}Animation;

typedef struct AdvencedAnimation
{
	sfIntRect* rectList;
	sfBool isLooping;
	int frameCount;
	float frameDuration;
	float timeActualy;
}AdvencedAnimation;

#define POW2(x) (x * x)
#define VECTOR(u,v) v.x - u.x, v.y - u.y
#define NORM_POW2(u,v) (POW2((v.x - u.x))) + (POW2((v.y - u.y)))
#define RAD_DEG(r) r * 180 / (float)M_PI
#define DEG_RAD(d) d * (float)M_PI / 180
#define RAND_RANGE(_min, _max) _min + rand() % (_max - _min + 1)
#define ANGLE_VECTOR_RAD(v) atan2f(v.y, v.x)
#define ANGLE_VECTOR_DEG(v) RAD_DEG(ANGLE_VECTOR_RAD(v))
#define ADD_VECTOR(u,v) {(u.x + v.x), (u.y + v.y)}
#define RECT_TRANSFORME(newType, rect) (newType) {rect.left, rect.top, rect.width, rect.height}
#define IF_RECT_EQUAL(_rect1, _rect2) (_rect1.left == _rect2.left && _rect1.top == _rect2.top && _rect1.height == _rect2.height && _rect1.width == _rect2.width)
#define IF_VECT_EQUAL(u,v) (u.x == v.x && u.y == v.y)

sfBool CompareColor(sfColor _color1, sfColor _color2);
void UpdateText(sfText* _text, char* _format, char* _string, int _value);
sfBool IsColidingPointHitbox(sfFloatRect* _hitbox, sfVector2f _position);
sfBool UpdateAnimationAndGiveIfStop(sfSprite* _sprite, Animation* const _animation, float _dt);
sfBool UpdateAdvencedAnimationAndGiveIfStop(sfSprite* const _sprite, AdvencedAnimation* const _animation, const float _dt);
void SetSpriteOriginFoot(sfSprite* _sprite);
void SetSpriteOriginMiddle(sfSprite* _sprite);
void SetTextOriginFoot(sfText* _text);
void SetTextOriginMiddle(sfText* _text);
void CopyStringToBuffer(char* _buffer, char* _string);
void** CreateGrid(unsigned long _columnCount, unsigned long _rowCount, size_t _typeSize);
void** ReallocGrid(void** _previousGrid, unsigned long _previousColumnCount, unsigned long _previousRowCount, unsigned long _columnCount, unsigned long _rowCount, size_t _typeSize);
void FreeGrid(void** grid);
sfBool StringCompare(char* _string1, char* _string2);
char* StringCopy(char* _string);
float MoveTowardsAngle(float _current, float _target, float _speed, float _dt);
sfBool VerificationEntityIsNotInMap(sfFloatRect _rect);
void ScaleImage(sfImage** _image, int _scale);
sfBool IsPointInFloatRect(sfVector2f _pos, sfFloatRect* _rect);
void FusionString(char* _buffer, int _count, char** _strings);
void TransformIntToString(char* _buffer, int _value);
int GetSizeString(char* _string);
sfBool UpdateTextHighlightTextColor(sfText* _text, sfColor _base, sfColor _highlight, sfVector2f _posMouse);

#include "Backup.h"
#include "EntityManager.h"
#include "Sound.h"
#include "Map.h"

#endif // !COMMON_H
