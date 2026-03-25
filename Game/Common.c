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

sfBool IsColidingPointHitbox(sfFloatRect* _hitbox, sfVector2f _position)
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

void SetSpriteOriginFoot(sfSprite* _sprite)
{
	sfFloatRect box = sfSprite_getLocalBounds(_sprite);
	sfSprite_setOrigin(_sprite, (sfVector2f) { box.width / 2, box.height });
}

void SetSpriteOriginMiddel(sfSprite* _sprite)
{
	sfFloatRect box = sfSprite_getLocalBounds(_sprite);
	sfSprite_setOrigin(_sprite, (sfVector2f) { box.width / 2, box.height / 2 });
}

sfVector2f GetColliderMove(sfIntRect _staticObject, sfIntRect _moveObject)
{
	return (sfVector2f) { 0 };
}

void CopyStingToBuffer(char* _buffer, char* _string)
{
	int i = 0;
	while (_string[i] > 0)
	{
		_buffer[i] = _string[i];
		i++;
	}
}

void** CreateGrid(sfVector2u _size, size_t _typeSize)
{
	void** grid = Calloc(_size.y, sizeof(void*));
	if (!grid)
	{
		return grid;
	}

	for (unsigned i = 0; i < _size.y; i++)
	{
		grid[i] = Calloc(_size.x, _typeSize);
		if (!grid[i])
		{
			return grid;
		}
	}
	return grid;
}