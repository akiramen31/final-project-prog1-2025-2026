#ifndef CAMERA_H
#define CAMERA_H

#include "Common.h"
#include "Player.h"

#define CAMERA_SPEED 300
#define CAMERA_LOCK 2.f
#define CAMERA_ZOOM_SPEED 0.0025f

void UpdateCamera(float _dt);

void MoveViewSlow(sfVector2f _pos, float _dt);
void SetDefaultView(float _dt);

float GetCameraZoom(void);
void SetCameraZoom(float zoom);
sfBool PauseGameCameraMoveRoom(void);
#endif