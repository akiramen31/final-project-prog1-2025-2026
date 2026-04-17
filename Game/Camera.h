#ifndef CAMERA_H
#define CAMERA_H

#include "Common.h"
#include "Player.h"

#define CAMERA_SPEED 8
#define CAMERA_SPEED_MAX 50
#define CAMERA_LOCK 0.1f

void UpdateCamera(float _dt);

void MoveViewSlow(sfVector2f _pos, float _dt);
void SetDefaultView(float _dt);

float GetCameraZoom(void);
#endif