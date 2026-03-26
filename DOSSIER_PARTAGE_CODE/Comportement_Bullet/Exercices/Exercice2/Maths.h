#ifndef MATHS_H
#define MATHS_H

#include "Common.h"


int GetRandom(int _min, int _max);
float DegToRad(float _angle);
float RadToDeg(float _angle);
sfVector2f GetDirectionFromDegree(float _angle);
sfVector2f GetDirectionFromRadian(float _angle);
sfVector2f GetNormalizedVector(sfVector2f _vector);
float GetVectorLength(sfVector2f _vector);
float GetDistance(sfVector2f _pos1, sfVector2f _pos2);
void GetAngleBetweenVertices(sfVector2f _start, sfVector2f _end);

#endif // !MATHS_H
