#include "Common.h"

sfBool CompareColor(sfColor _color1, sfColor _color2)
{
	if (_color1.r == _color2.r && _color1.g == _color2.g && _color1.b == _color2.b)
	{
		return sfTrue;
	}
	return sfFalse;
}

void UpdateText(sfText* _text, char* _format, char* _string, int _value)
{
	char buffer[50];
	sprintf_s(buffer, sizeof(buffer), _format, _string, _value);
	sfText_setString(_text, buffer);
}

sfBool IsColidingPointHitbox(sfFloatRect* _hitbox, sfVector2f _position)
{
	if (_hitbox->left < _position.x && _hitbox->top < _position.y && _hitbox->left + _hitbox->width > _position.x && _hitbox->top + _hitbox->height > _position.y)
	{
		return sfTrue;
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

void CopyStingToBuffer(char* _buffer, char* _string)
{
	int i = 0;
	while (_string[i] > 0)
	{
		_buffer[i] = _string[i];
		i++;
	}
}

void** CreateGrid(unsigned long _columnCount, unsigned long _rowCount, size_t _typeSize)
{
	char** grid = Calloc(_columnCount, sizeof(char*));
	if (!grid)
	{
		return NULL;
	}

	char* temp = Calloc((size_t)_rowCount * _columnCount, _typeSize);
	if (!temp)
	{
		Free(grid);
		return NULL;
	}

	for (int i = 0; i < _rowCount; i++)
	{
		grid[i] = &temp[i * _typeSize * _columnCount];
	}

	return grid;
}

void FreeGrid(void** grid)
{
	if (grid)
	{
		Free(grid[0]);
		Free(grid);
	}
}

sfBool StringCompare(char* _string1, char* _string2)
{
	int i = 0;
	while (_string1[i] == _string2[i])
	{
		i++;
		if (_string1[i] == 0 && _string2[i] == 0)
		{
			return sfTrue;
		}
		else if (_string1[i] == 0 || _string2[i] == 0)
		{
			return sfFalse;
		}
	}
	return sfFalse;
}