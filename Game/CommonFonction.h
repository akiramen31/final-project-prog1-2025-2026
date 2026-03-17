#ifndef COMMON_FONCTION_H
#define COMMON_FONCTION_H

#include "Common.h"

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
#endif // !COMMON_FONCTION_H