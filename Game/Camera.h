#ifndef CAMERA_H
#define CAMERA_H

#include "Common.h"
#include "Player.h"

#define CAMERA_SPEED_ON_PLAYER 150
#define CAMERA_SPEED 300
#define CAMERA_SPEED_LOCK 1.f
#define CAMERA_ZOOM_SPEED 0.07f
#define CAMERA_ZOOM_LOCK 0.005f
//#define CAMERA_ZOOM_SPEED 3.07f
//#define CAMERA_ZOOM_LOCK 1.005f

void UpdateCamera(float _dt);

void MoveViewSlow(sfVector2f _pos, float _dt);
void SetDefaultView(float _dt);

float GetCameraZoom(void);
void SetCameraZoom(float zoom);
sfBool PauseGameCameraMoveRoom(void);
#endif