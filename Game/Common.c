#include "Common.h"

int GetRandomInRange(int _min, int _max)
{
	if (_min > _max)
	{
		int temp = _max;
		_max = _min;
		_min = temp;
	}
	return _min + rand() % (_max - _min + 1);
}

float GetAngleToVector(sfVector2f _vector)
{
	return RadToDeg(atan2f(_vector.y, _vector.x));
}

float RadToDeg(float _angleRad)
{
	return(float)(_angleRad * 180 / (float)M_PI);
}

float DegToRad(float _angleDeg)
{
	return(float)(_angleDeg * M_PI / 180);
}

void UpdateText(sfText* _text, char* _format, char* _string, int _value)
{
	char buffer[50];
	sprintf_s(buffer, sizeof(buffer), _format, _string, _value);
	sfText_setString(_text, buffer);
}

sfBool IsColidingPionHitbox(sfFloatRect* _hitbox, sfVector2f _position)
{
	if (_hitbox->left < _position.x)
	{
		if (_hitbox->top < _position.y)
		{
			if (_hitbox->left + _hitbox->width > _position.x)
			{
				if (_hitbox->top + _hitbox->height > _position.y)
				{
					return sfTrue;
				}
			}
		}
	}
	return sfFalse;
}

sfBool UpdateAnimationAndGiveIfStop(sfSprite* const _sprite, Animation* const _animation, const float _dt)
{
	_animation->timeActualy += _dt;

	sfSprite_setTextureRect(_sprite, _animation->rectActualy);
	if (_animation->timeActualy > _animation->frameDuration)
	{
		_animation->timeActualy = 0.f;
		_animation->rectActualy.left += _animation->rectActualy.width;
		if (_animation->rectActualy.left > (_animation->rectActualy.width * (_animation->frameCount - 1)))
		{
			_animation->rectActualy.left = 0;
			if (!_animation->isLooping)
			{
				return sfTrue;
			}
		}
	}
	return sfFalse;
}

sfVector2f TransformVector2iToVector2f(sfVector2i _vector)
{
	return (sfVector2f) { FIRST_CASE.left + FIRST_CASE.width * ((float)_vector.x + 0.5f),
		FIRST_CASE.top + FIRST_CASE.height * ((float)_vector.y + 1.f) };
}