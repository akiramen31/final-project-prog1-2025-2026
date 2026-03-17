#ifndef ANIMATION_H
#define ANIMATION_H

#include "Common.h"

typedef struct Animation
{
	sfIntRect firstFrame;
	sfBool isLooping;
	float frameDuration;
	unsigned frameCount;
	float timeActualy;
}Animation;

sfBool UpdateAnimationAndGiveIfStop(sfSprite* _sprite, Animation* const _animation, float _dt);

#endif
