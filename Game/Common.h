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
#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 904
#define BPP 32

#define FIRST_CASE (sfIntRect) {32, 160, 64, 64}
#define NB_GRID_ROW 9
#define NB_GRID_COLUMN 15 

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
sfBool IsColidingPionHitbox(sfFloatRect* _hitbox, sfVector2f _position);
sfBool UpdateAnimationAndGiveIfStop(sfSprite* _sprite, Animation* const _animation, float _dt);
sfVector2f TransformVector2iToVector2f(sfVector2i _vector);
void SetSpriteOrigineFoot(sfSprite* _sprite);
void SetSpriteOrigineMiddel(sfSprite* _sprite);

#endif // !COMMON_H
