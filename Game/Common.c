#include "Common.h"

sfBool CompareColor(sfColor _color1, sfColor _color2)
{
	return (_color1.r == _color2.r && _color1.g == _color2.g && _color1.b == _color2.b);
}

void UpdateText(sfText* _text, char* _format, char* _string, int _value)
{
	char buffer[50];
	sprintf_s(buffer, sizeof(buffer), _format, _string, _value);
	sfText_setString(_text, buffer);
}

sfBool IsColidingPointHitbox(sfFloatRect* _hitbox, sfVector2f _position)
{
	return (_hitbox->left < _position.x && _hitbox->top < _position.y && _hitbox->left + _hitbox->width > _position.x && _hitbox->top + _hitbox->height > _position.y);
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
			return !_animation->isLooping;
		}
	}
	return sfFalse;
}

void SetSpriteOriginFoot(sfSprite* _sprite)
{
	sfFloatRect box = sfSprite_getLocalBounds(_sprite);
	sfSprite_setOrigin(_sprite, (sfVector2f) { box.width / 2, box.height });
}

void SetSpriteOriginMiddle(sfSprite* _sprite)
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
	char** grid = Calloc(_rowCount * (sizeof(char*) + _columnCount * _typeSize), sizeof(char));
	if (!grid)
	{
		return NULL;
	}

	char* temp = &grid[_rowCount];
	unsigned tempValue = _columnCount * _typeSize;
	for (unsigned i = 0; i < _rowCount; i++)
	{
		grid[i] = &temp[i * tempValue];
	}
	return grid;
}

void** ReallocGrid(void** _previousGrid, unsigned long _previousColumnCount, unsigned long _previousRowCount, unsigned long _columnCount, unsigned long _rowCount, size_t _typeSize)
{
	char** grid = (char**)CreateGrid(_columnCount, _rowCount, _typeSize);
	char** previousGrid = (char**)_previousGrid;
	if (!grid)
	{
		return NULL;
	}
	unsigned columnToCopy = _previousColumnCount;
	if (_previousColumnCount > _columnCount)
	{
		columnToCopy = _columnCount;
	}
	unsigned rowToCopy = (unsigned)(_previousRowCount * _typeSize);
	if (_previousRowCount > _rowCount)
	{
		rowToCopy = (int)(_rowCount * _typeSize);
	}

	for (unsigned i = 0; i < columnToCopy; i++)
	{
		for (unsigned j = 0; j < rowToCopy; j++)
		{
			grid[i][j] = previousGrid[i][j];
		}
	}
	return grid;
}

void FreeGrid(void** grid)
{
	if (grid)
	{
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
	}
	return sfFalse;
}

float MoveTowardsAngle(float _current, float _target, float _speed, float _dt)
{
	/*float diff = _target - _current;
	if (diff < -180.0f)
	{
		diff += (int)(diff / 180.f) * 360.f;
	}
	else if (diff > 180.0f)
	{
		diff -= (int)(diff / 180.f) * 360.f;
	}

	float step = _speed * _dt;

	if (fabsf(diff) <= step)
	{
		return _target;
	}
	else if (diff > 0)
	{
		return _current + step;
	}
	else
	{
		return _current - step;
	}
	return 0;*/


		// 1. On ramène l'angle SFML (0/360) vers le format mathématique (-180/180)
		if (_current > 180.0f) _current -= 360.0f;

		float diff = _target - _current;

		// 2. On prend toujours le chemin le plus court
		while (diff < -180.0f) diff += 360.0f;
		while (diff > 180.0f) diff -= 360.0f;

		float step = _speed * _dt;

		if (fabsf(diff) <= step) return _target;

		return _current + (diff > 0 ? step : -step);
}

sfBool VerificationEntityIsNotInMap(sfFloatRect _rect)
{
	MapData data = *GetMapData();
	return ((_rect.left <= 0) || (_rect.left + _rect.width >= data.size.x * TILE_SIZE) || (_rect.top <= 0) || (_rect.top + _rect.height >= data.size.y * TILE_SIZE));
}

void ScaleImage(sfImage** _image, int _scale)
{
	sfImage* image = *_image;
	sfVector2u imageSize = sfImage_getSize(image);
	sfVector2u newSize = { imageSize.x * _scale, imageSize.y * _scale };
	*_image = sfImage_create(newSize.x, newSize.y);

	for (unsigned int y = 0; y < newSize.y; y++)
	{
		for (unsigned int x = 0; x < newSize.x; x++)
		{
			sfColor color = sfImage_getPixel(image, (int)(x / _scale), (int)(y / _scale));
			sfImage_setPixel(*_image, x, y, color);
		}
	}
}