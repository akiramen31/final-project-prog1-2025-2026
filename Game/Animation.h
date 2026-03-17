#ifndef ANIMATION_H
#define ANIMATION_H

#include "Common.h"

typedef struct Animation
{
	sfIntRect firstFrame;
	sfBool isLooping;
	unsigned frameCount;
	float frameDuration;
	float timeActualy;
}Animation;

sfBool UpdateAnimationAndGiveIfStop(sfSprite* _sprite, Animation* const _animation, float _dt);

#endif
