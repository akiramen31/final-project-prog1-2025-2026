#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <process.h>
#include <math.h>
#include "SFML/Graphics.h"
#include "SFML/Audio.h"

#include "Backup.h"
#include "EntityManager.h"

#include "Map.h"

#define GAME_SCALE 1
#define GAME_VOLUME 10.f
#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080


// NEEDED FOR BULLET AND WEAPON
#define PLAYER_COLLISION_WIDTH 12
#define PLAYER_COLLISION_HEIGHT 30
#define WEAPON_ORIGIN PLAYER_COLLISION_HEIGHT * 0.75f - 1.0f

#define BPP 32

#define DEV_MODE sfTrue
#define DEV_ENNEMY sfTrue
#define DEV_MAP_COLIDER sfFalse
#define DEV_WEAPON sfTrue
#define DEV_BOSS sfTrue
#define DEBUG_MODE_A_STAR sfFalse
#define DEV_PIERRE_ENEMY sfTrue
#define DEV_ENEMY_BASIC sfTrue

#define TILE_SIZE 16

#define FREEZE_COLOR ((sfColor) {180, 180, 255, 255})

#define G 5.81f
//#define G 9.81f

typedef enum {
	AXIS_X,
	AXIS_Y,
	AXIS_BOTH
} CollisionAxis;

typedef enum AttackType
{
	LIGHT,
	MEDIUM,
	HEAVY,
	NOATTACK
}AttackType;

typedef struct ShooterType
{
	float weaponPos;
	sfVector2f shootPosition;
	AttackType bulletType;
	sfBool isAlly;
	sfBool isRighted;
}ShooterType;

typedef enum State
{
	IDLE = 0,
	WALK = 4
}State;

enum EnergyEnum
{
	ENERGY,
	ENERGY_MAX,
	ENERGY_REGEN,
	ENERGY_REGEN_COOLDOWN,
};

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

#define POW2(x) (x * x)
#define VECTOR(u,v) v.x - u.x, v.y - u.y
#define NORM_POW2(u,v) (POW2((v.x - u.x))) + (POW2((v.y - u.y)))
#define RAD_DEG(r) r * 180 / M_PI
#define DEG_RAD(d) d * M_PI / 180
#define RAND_RANGE(_min, _max) _min + rand() % (_max - _min + 1)
#define ANGLE_VECTOR(v) RAD_DEG(atan2f(v.y, v.x))
#define ADD_VECTOR(u,v) {(u.x + v.x), (u.y + v.y)}
#define IF_RECT_EGAL(_rect1, _rect2) (_rect1.left == _rect2.left && _rect1.top == _rect2.top && _rect1.height == _rect2.height && _rect1.width == _rect2.width)
#define IF_VECT_EGAL(u,v) (u.x == v.x && u.y == v.y)

sfBool CompareColor(sfColor _color1, sfColor _color2);
void UpdateText(sfText* _text, char* _format, char* _string, int _value);
sfBool IsColidingPointHitbox(sfFloatRect* _hitbox, sfVector2f _position);
sfBool UpdateAnimationAndGiveIfStop(sfSprite* _sprite, Animation* const _animation, float _dt);
void SetSpriteOriginFoot(sfSprite* _sprite);
void SetSpriteOriginMiddle(sfSprite* _sprite);
void CopyStingToBuffer(char* _buffer, char* _string);
void** CreateGrid(unsigned long _columnCount, unsigned long _rowCount, size_t _typeSize);
void** ReallocGrid(void** _previousGrid, unsigned long _previousColumnCount, unsigned long _previousRowCount, unsigned long _columnCount, unsigned long _rowCount, size_t _typeSize);
void FreeGrid(void** grid);
sfBool StringCompare(char* _string1, char* _string2);
float MoveTowardsAngle(float _current, float _target, float _speed, float _dt);
sfBool VerificationEntityIsNotInMap(sfFloatRect _rect);
#endif // !COMMON_H
