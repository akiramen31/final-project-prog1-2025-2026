#ifndef CAMERA_H
#define CAMERA_H

#include "Common.h"
#include "Player.h"

#define CAMERA_SPEED 1
#define CAMERA_LOCK 5

void UpdateCamera(float _dt);

void MoveViewSlow(sfVector2f _pos, float _dt);
void SetDefaultView(float _dt);

float GetCameraZoom(void);
#endif