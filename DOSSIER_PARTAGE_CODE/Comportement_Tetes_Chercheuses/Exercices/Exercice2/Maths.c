#include "Maths.h"

int GetRandom(int _min, int _max)
{
	return _min + rand() % (_max - _min + 1);
}

float DegToRad(float _angle)
{
	return (float)(_angle * M_PI / 180);
}

float RadToDeg(float _angle)
{
	return (float)(_angle * 180 / M_PI);
}

sfVector2f GetDirectionFromDegree(float _angle)
{
	float angleRad = DegToRad(_angle);

	return (sfVector2f) { cosf(angleRad), sinf(angleRad) };
}

sfVector2f GetDirectionFromRadian(float _angle)
{
	return (sfVector2f) { cosf(_angle), sinf(_angle) };
}

sfVector2f GetNormalizedVector(sfVector2f _vector)
{
	float vectorLength = GetVectorLength(_vector);

	return (sfVector2f) { _vector.x / vectorLength, _vector.y / vectorLength };
}

float GetVectorLength(sfVector2f _vector)
{
	return sqrtf(_vector.x * _vector.x + _vector.y * _vector.y);
}

float GetDistance(sfVector2f _pos1, sfVector2f _pos2)
{
	sfVector2f vector = { (float)(_pos2.x - _pos1.x) , (float)(_pos2.y - _pos1.y) };

	return GetVectorLength(vector);
}
