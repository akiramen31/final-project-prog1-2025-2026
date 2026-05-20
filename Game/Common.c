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

	if ((int)(_animation->timeActualy / _animation->frameDuration) < _animation->frameCount)
	{
		sfSprite_setTextureRect(_sprite, (sfIntRect) { _animation->rectActualy.left + _animation->rectActualy.width * (int)(_animation->timeActualy / _animation->frameDuration), _animation->rectActualy.top, _animation->rectActualy.width, _animation->rectActualy.height });
	}
	else
	{
		_animation->timeActualy = 0;
		return !_animation->isLooping;
	}
	return sfFalse;
}

sfBool UpdateAdvencedAnimationAndGiveIfStop(sfSprite* const _sprite, AdvencedAnimation* const _animation, const float _dt)
{
	_animation->timeActualy += _dt;

	if ((int)_animation->timeActualy / (int)_animation->frameDuration < _animation->frameCount)
	{
		sfSprite_setTextureRect(_sprite, _animation->rectList[(int)_animation->timeActualy / (int)_animation->frameDuration]);
	}
	else
	{
		_animation->timeActualy = 0;
		return !_animation->isLooping;
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

void SetTextOriginFoot(sfText* _text)
{
	sfFloatRect box = sfText_getLocalBounds(_text);
	sfText_setOrigin(_text, (sfVector2f) { box.width / 2, box.height });
}

void SetTextOriginMiddle(sfText* _text)
{
	sfFloatRect box = sfText_getLocalBounds(_text);
	sfText_setOrigin(_text, (sfVector2f) { box.width / 2, box.height / 2 });
}

void CopyStringToBuffer(char* _buffer, char* _string)
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
	if (_string1 && _string2)
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
	}
	return sfFalse;
}

char* StringCopy(char* _string)
{
	int i = 0;
	while (_string[i])
	{
		i++;
	}
	i++;
	char* stringCopy = Calloc(i, sizeof(char));
	for (int j = 0; j < i; j++)
	{
		stringCopy[j] = _string[j];
	}
	return stringCopy;
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
	if (_image && *_image)
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
}

sfBool IsPointInFloatRect(sfVector2f _pos, sfFloatRect* _rect)
{
	if (_pos.x > _rect->left && _pos.x < _rect->left + _rect->width)
	{
		if (_pos.y > _rect->top && _pos.y < _rect->top + _rect->height)
		{
			return sfTrue;
		}
	}

	return sfFalse;
}

void FusionString(char* _buffer, int _count, char** _strings)
{
	int size = 0;
	for (int i = 0; i < _count; i++)
	{
		int j = 0;
		while (_strings[i][j])
		{
			_buffer[size] = _strings[i][j];
			j++;
			size++;
		}
	}
}

void TransformIntToString(char* _buffer, int _value)
{
	if (_value)
	{
		int power = 1000000000;
		int index = 0;
		for (int i = 0; i < 10; i++)
		{
			if (_value / power)
			{
				_buffer[index] = _value / power % 10 + '0';
				index++;
			}
			power /= 10;
		}
		while (_buffer[index])
		{
			_buffer[index] = 0;
			index++;
		}
	}
	else
	{
		_buffer[0] = '0';
		_buffer[1] = 0;
	}
}

int GetSizeString(char* _string)
{
	int size = 0;

	while (_string[size])
	{
		size++;
	}
	return size;
}

sfBool UpdateTextHighlightTextColor(sfText* _text, sfColor _base, sfColor _highlight, sfVector2f _posMouse)
{
	sfFloatRect rect = sfText_getGlobalBounds(_text);
	if (rect.left < _posMouse.x && rect.left + rect.width > _posMouse.x && rect.top < _posMouse.y && rect.top + rect.height > _posMouse.y)
	{
		sfText_setColor(_text, _highlight);
		return 1;
	}
	else
	{
		sfText_setColor(_text, _base);
		return 0;
	}
	return 0;
}