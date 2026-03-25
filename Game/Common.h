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

#define GAME_SCALE 1
#define GAME_VOLUME 10.f
#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080
#define BPP 32
#define DEV_MODE 1

#define G 9.81f

typedef enum State
{
	IDLE = 0,
	WALK = 4
}State;

typedef enum Direction
{
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

int GetRandomInRange(int _min, int _max);
float GetAngleToVector(sfVector2f _vector);
float RadToDeg(float _angleRad);
float DegToRad(float _angleDeg);
void UpdateText(sfText* _text, char* _format, char* _string, int _value);
sfBool IsColidingPointHitbox(sfFloatRect* _hitbox, sfVector2f _position);
sfBool UpdateAnimationAndGiveIfStop(sfSprite* _sprite, Animation* const _animation, float _dt);
void SetSpriteOriginFoot(sfSprite* _sprite);
void SetSpriteOriginMiddel(sfSprite* _sprite);
sfVector2f GetColliderMove(sfIntRect _staticObject, sfIntRect _moveObject);
void CopyStingToBuffer(char* _buffer, char* _string);
void** CreateGrid(sfVector2u _size, size_t _typeSize);

#endif // !COMMON_H
