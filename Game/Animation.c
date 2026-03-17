#include "Animation.h"

sfBool UpdateAnimationAndGiveIfStop(sfSprite* const _sprite, Animation* const _animation, const float _dt)
{
	_animation->timeActualy += _dt;



	if (_animation->timeActualy > _animation->frameDuration)
	{
		sfIntRect frameActualy = _animation->firstFrame;
		_animation->timeActualy = 0.f;
		frameActualy.left += frameActualy.width;
		sfSprite_setTextureRect(_sprite, frameActualy);
	}
	return sfFalse;
}