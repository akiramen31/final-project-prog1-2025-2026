#ifndef COMMON_FONCTION_H
#define COMMON_FONCTION_H

#include "Common.h"

int GetRandomInRange(int _min, int _max);
float GetAngleToVector(sfVector2f _vector);
float RadToDeg(float _angleRad);
float DegToRad(float _angleDeg);
void UpdateText(sfText* _text, char* _format, char* _string, int _value);
sfBool IsColidingPionHitbox(sfFloatRect* _hitbox, sfVector2f _position);
#endif // !COMMON_FONCTION_H