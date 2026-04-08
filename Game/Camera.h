#ifndef CAMERA_H
#define CAMERA_H

#include "Common.h"

#define CAMERA_SPEED 10

void UpdateCamera(float _dt);

void MoveViewSlow(sfVector2f _pos, float _dt);
void SetDefaultView(float _dt);

#endif