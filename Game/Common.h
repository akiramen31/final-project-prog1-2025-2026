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
#define BPP 32

#define DEV_MODE sfTrue
#define DEV_ENNEMY sfTrue
#define DEV_MAP_COLIDER sfFalse
#define DEV_WEAPON sfFalse
#define DEV_MODE_FLY sfFalse

#define TILE_SIZE 16.f

#define G 9.81f

typedef enum State
{
	IDLE = 0,
	WALK = 4
}State;

typedef enum Direction
{
	NO_DIRECTION,
	UP,
	DOWN,
	LEFT,
	RIGHT,
	UP_LEFT,
	UP_RIGHT,
	DOWN_LEFT,
	DOWN_RIGHT
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

sfBool CompareColor(sfColor _color1, sfColor _color2);
void UpdateText(sfText* _text, char* _format, char* _string, int _value);
sfBool IsColidingPointHitbox(sfFloatRect* _hitbox, sfVector2f _position);
sfBool UpdateAnimationAndGiveIfStop(sfSprite* _sprite, Animation* const _animation, float _dt);
void SetSpriteOriginFoot(sfSprite* _sprite);
void SetSpriteOriginMiddel(sfSprite* _sprite);
void CopyStingToBuffer(char* _buffer, char* _string);
void** CreateGrid(sfVector2u _size, size_t _typeSize);
sfBool StringCompare(char* _string1, char* _string2);
#endif // !COMMON_H
