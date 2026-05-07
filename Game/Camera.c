#include "Camera.h"
#include "Parallax.h"

Camera camera;

void UpdatePositionCameraLevel1(float _dt);
void UpdatePositionCameraLevel2(float _dt);
void UpdatePositionCameraLevel3(float _dt);

void LoadCamera(void)
{
	camera.positionName = 0;
	camera.lastPositionName = 0;
	camera.index = 0;
	camera.zoom = 0;
	camera.newZoom = 0;
}

void ChangePosCamera(int _lastCamPos, int _val);

void SetCamPositionName(sfFloatRect* _hitbox, InfoZone* _area, float _dt);

void UpdateCamera(float _dt)
{
	switch (GetCurrentMap())
	{
	case LEVEL1:
		UpdatePositionCameraLevel1(_dt);
		break;
	case LEVEL2:
		UpdatePositionCameraLevel2(_dt);
		break;
	case LEVEL3:
		UpdatePositionCameraLevel3(_dt);
		break;
	default:
		SetDefaultView(_dt);
		break;
	}

	if (camera.zoom != camera.newZoom)
	{
		if (camera.zoom > camera.newZoom)
		{
			camera.zoom -= CAMERA_ZOOM_SPEED * _dt;
		}
		else
		{
			camera.zoom += CAMERA_ZOOM_SPEED * _dt;
		}

		if (camera.zoom < camera.newZoom + CAMERA_ZOOM_LOCK && camera.zoom > camera.newZoom - CAMERA_ZOOM_LOCK)
		{
			camera.zoom = camera.newZoom;
		}
	}
	SetViewZoom(camera.zoom);
	SizeParallaxToCamera();
}

void MoveViewSlow(sfVector2f _pos, float _dt)
{
	sfVector2f tempPos = GetViewPosition();
	sfVector2f size = GetViewSize();

	tempPos.x += size.x / 2;
	tempPos.y += size.y / 2;

	sfVector2f vectorLength = (sfVector2f){ VECTOR(tempPos, _pos) };

	double angle = atan2f(vectorLength.y, vectorLength.x);

	sfVector2f move = { 0 };

	sfVector2f speed;
	//if (camera.positionName == 0)
	if (camera.lastPositionName == camera.positionName)
	{
		speed.x = CAMERA_SPEED_ON_PLAYER_X;
		speed.y = CAMERA_SPEED_ON_PLAYER_Y;
	}
	else
	{
		speed = (sfVector2f){ CAMERA_SPEED };
	}

	move.x = _dt * (float)cos(angle) * speed.x;
	move.y = _dt * (float)sin(angle) * speed.y;


	if (POW2(vectorLength.x) > POW2(1000))
	{
		if (vectorLength.x < 0)
		{
			move.x = -1000;
		}
		else
		{
			move.x = 1000;
		}
	}
	if (POW2(vectorLength.y) > POW2(1000))
	{
		if (vectorLength.y < 0)
		{
			move.y = -1000;
		}
		else
		{
			move.y = 1000;
		}
	}

	if (POW2(move.x) > POW2(vectorLength.x))
	{
		move.x = vectorLength.x;
	}
	if (POW2(move.y) > POW2(vectorLength.y))
	{
		move.y = vectorLength.y;
	}

	if (POW2(move.x) < CAMERA_SPEED_LOCK && POW2(move.y) < CAMERA_SPEED_LOCK)
	{
		SetViewCenter(_pos);
		camera.lastPositionName = camera.positionName;
	}
	else
	{
		MoveView(move);
	}
}

void UpdatePositionCameraLevel1(float _dt)
{
	sfFloatRect hitbox = GetPlayerRect();
	sfVector2f posPlayer = { hitbox.left + hitbox.width / 2,hitbox.top + hitbox.height / 2 };
	InfoZone* area = GetInfoZoneTriger(hitbox);

	if (area)
	{
		sfVector2f pos = { 0 };

		if (sfFloatRect_intersects(&hitbox, &area[camera.index].hitbox, NULL) && camera.positionName != 0)
		//if (IsPointInFloatRect(posPlayer, &area[camera.index].hitbox) && camera.positionName != 0)
		{
			switch (camera.positionName)
			{
			case 1:
				camera.newZoom = 0.2f;
				pos.x = area[camera.index].hitbox.left + area[camera.index].hitbox.width / 2;
				pos.y = area[camera.index].hitbox.top + area[camera.index].hitbox.height / 2;
				break;
			case 2:
				camera.newZoom = 0.35f;
				pos.x = area[camera.index].hitbox.left + area[camera.index].hitbox.width / 2;
				pos.y = area[camera.index].hitbox.top + area[camera.index].hitbox.height / 2;
				break;
			case 3:
				camera.newZoom = 0.25f;
				pos.x = area[camera.index].hitbox.left + area[camera.index].hitbox.width / 2;
				pos.y = area[camera.index].hitbox.top + area[camera.index].hitbox.height / 2;
				break;
				//for biggest room
			case 4:
				camera.newZoom = 0.3f;
				if (area[camera.index].hitbox.left + area[camera.index].hitbox.width - area[camera.index].hitbox.width / 3 <= hitbox.left + hitbox.width / 2)
				{
					pos.x = area[camera.index].hitbox.left + area[camera.index].hitbox.width - area[camera.index].hitbox.width / 3;
				}
				else if (area[camera.index].hitbox.left + area[camera.index].hitbox.width / 3 >= hitbox.left + hitbox.width / 2)
				{
					pos.x = area[camera.index].hitbox.left + area[camera.index].hitbox.width / 3;
				}
				else
				{
					pos.x = hitbox.left + hitbox.width / 2;
				}

				if (area[camera.index].hitbox.top + area[camera.index].hitbox.height - area[camera.index].hitbox.height / 3 <= hitbox.top + hitbox.height / 2)
				{
					pos.y = area[camera.index].hitbox.top + area[camera.index].hitbox.height - area[camera.index].hitbox.height / 3;
				}
				else if (area[camera.index].hitbox.top + area[camera.index].hitbox.height / 3 >= hitbox.top + hitbox.height / 2)
				{
					pos.y = area[camera.index].hitbox.top + area[camera.index].hitbox.height / 3;
				}
				else
				{
					pos.y = hitbox.top + hitbox.height / 2;
				}
				break;
				//for boss room
			case 5:
				camera.newZoom = 0.42f;
				pos.x = area[camera.index].hitbox.left + area[camera.index].hitbox.width / 2;
				pos.y = area[camera.index].hitbox.top + area[camera.index].hitbox.height / 2;
				break;
			default:
				pos = GetPlayerPosition();
				break;
			}
			MoveViewSlow(pos, _dt);
		}
		else
		{
			SetCamPositionName(&hitbox, area, _dt);
		}
	}
	else
	{
		SetDefaultView(_dt);
	}
}

void UpdatePositionCameraLevel2(float _dt)
{
	sfFloatRect hitbox = GetPlayerRect();
	sfVector2f posPlayer = { hitbox.left + hitbox.width / 2,hitbox.top + hitbox.height / 2 };
	InfoZone* area = GetInfoZoneTriger(hitbox);

	if (area != NULL)
	{
		sfVector2f pos = { 0 };

		if (IsPointInFloatRect(posPlayer, &area[camera.index].hitbox) && camera.positionName != 0)
		{
			switch (camera.positionName)
			{
				//for litle room
			case 1:
			case 3:
			case 4:
			case 7:
			case 8:
			case 9:
			case 10:
			case 12:
			case 13:
			case 16:
			case 19:
			case 26:
				camera.newZoom = 0.2f;
				pos.x = area[camera.index].hitbox.left + area[camera.index].hitbox.width / 2;
				pos.y = area[camera.index].hitbox.top + area[camera.index].hitbox.height / 2;
				break;
				//for bigger room
			case 5:
			case 6:
			case 15:
			case 18:
			case 22:
			case 23:
			case 25:
				camera.newZoom = 0.3f;
				pos.x = area[camera.index].hitbox.left + area[camera.index].hitbox.width / 2;
				pos.y = area[camera.index].hitbox.top + area[camera.index].hitbox.height / 2;
				break;
				// for tall room
			case 2:
			case 11:
			case 14:
			case 17:
			case 20:
			case 21:
			case 24:
			case 31:
			case 32:
				camera.newZoom = 0.3f;
				pos.x = area[camera.index].hitbox.left + area[camera.index].hitbox.width / 2;
				if (area[camera.index].hitbox.top + area[camera.index].hitbox.height - area[camera.index].hitbox.height / 3 <= hitbox.top + hitbox.height / 2)
				{
					pos.y = area[camera.index].hitbox.top + area[camera.index].hitbox.height - area[camera.index].hitbox.height / 3;
				}
				else if (area[camera.index].hitbox.top + area[camera.index].hitbox.height / 3 >= hitbox.top + hitbox.height / 2)
				{
					pos.y = area[camera.index].hitbox.top + area[camera.index].hitbox.height / 3;
				}
				else
				{
					pos.y = hitbox.top + hitbox.height / 2;
				}
				break;
				//for wide room
			case 27:
			case 29:
				camera.newZoom = 0.3f;
				if (area[camera.index].hitbox.left + area[camera.index].hitbox.width - area[camera.index].hitbox.width / 3 <= hitbox.left + hitbox.width / 2)
				{
					pos.x = area[camera.index].hitbox.left + area[camera.index].hitbox.width - area[camera.index].hitbox.width / 3;
				}
				else if (area[camera.index].hitbox.left + area[camera.index].hitbox.width / 3 >= hitbox.left + hitbox.width / 2)
				{
					pos.x = area[camera.index].hitbox.left + area[camera.index].hitbox.width / 3;
				}
				else
				{
					pos.x = hitbox.left + hitbox.width / 2;
				}
				pos.y = area[camera.index].hitbox.top + area[camera.index].hitbox.height / 2;
				break;
				//for biggest room
			case 28:
				camera.newZoom = 0.3f;
				if (area[camera.index].hitbox.left + area[camera.index].hitbox.width - area[camera.index].hitbox.width / 3 <= hitbox.left + hitbox.width / 2)
				{
					pos.x = area[camera.index].hitbox.left + area[camera.index].hitbox.width - area[camera.index].hitbox.width / 3;
				}
				else if (area[camera.index].hitbox.left + area[camera.index].hitbox.width / 3 >= hitbox.left + hitbox.width / 2)
				{
					pos.x = area[camera.index].hitbox.left + area[camera.index].hitbox.width / 3;
				}
				else
				{
					pos.x = hitbox.left + hitbox.width / 2;
				}

				if (area[camera.index].hitbox.top + area[camera.index].hitbox.height - area[camera.index].hitbox.height / 3 <= hitbox.top + hitbox.height / 2)
				{
					pos.y = area[camera.index].hitbox.top + area[camera.index].hitbox.height - area[camera.index].hitbox.height / 3;
				}
				else if (area[camera.index].hitbox.top + area[camera.index].hitbox.height / 3 >= hitbox.top + hitbox.height / 2)
				{
					pos.y = area[camera.index].hitbox.top + area[camera.index].hitbox.height / 3;
				}
				else
				{
					pos.y = hitbox.top + hitbox.height / 2;
				}
				break;
				//for boss room
			case 30:
				camera.newZoom = 0.42f;
				pos.x = area[camera.index].hitbox.left + area[camera.index].hitbox.width / 2;
				pos.y = area[camera.index].hitbox.top + area[camera.index].hitbox.height / 2;
				break;
			default:
				pos = GetPlayerPosition();
				break;
			}
			MoveViewSlow(pos, _dt);
		}
		else
		{
			SetCamPositionName(&hitbox, area, _dt);
		}
	}
	else
	{
		SetDefaultView(_dt);
	}
}

void UpdatePositionCameraLevel3(float _dt)
{
	sfFloatRect hitbox = GetPlayerRect();
	sfVector2f posPlayer = { hitbox.left + hitbox.width / 2,hitbox.top + hitbox.height / 2 };
	InfoZone* area = GetInfoZoneTriger(hitbox);

	if (area != NULL)
	{
		sfVector2f pos = { 0 };

		if (IsPointInFloatRect(posPlayer, &area[camera.index].hitbox) && camera.positionName != 0)
		{
			switch (camera.positionName)
			{
			case 1:
				camera.newZoom = 0.42f;
				pos.x = area[camera.index].hitbox.left + area[camera.index].hitbox.width / 2;
				if (area[camera.index].hitbox.top + area[camera.index].hitbox.height - area[camera.index].hitbox.height / 5 <= hitbox.top + hitbox.height / 2)
				{
					pos.y = area[camera.index].hitbox.top + area[camera.index].hitbox.height - area[camera.index].hitbox.height / 5;
				}
				else if (area[camera.index].hitbox.top + area[camera.index].hitbox.height / 5 >= hitbox.top + hitbox.height / 2)
				{
					pos.y = area[camera.index].hitbox.top + area[camera.index].hitbox.height / 5;
				}
				else
				{
					pos.y = hitbox.top + hitbox.height / 2;
				}
				break;

			default:
				pos = GetPlayerPosition();
				break;
			}
			MoveViewSlow(pos, _dt);
		}
		else
		{
			SetCamPositionName(&hitbox, area, _dt);
		}
	}
	else
	{
		SetDefaultView(_dt);
	}
}

void ChangePosCamera(int _lastCamPos, int _val)
{
	camera.positionName = _lastCamPos;
	camera.index = _val;
}

void SetDefaultView(float _dt)
{
	sfVector2f pos = GetPlayerPosition();
	MoveViewSlow((sfVector2f) { pos.x, pos.y - TILE_SIZE * 3 }, _dt);
	camera.newZoom = CAMERA_DEFAULT_ZOOM;
	//camera.newZoom = 5.f;
	ChangePosCamera(0, 0);
}

float GetCameraZoom(void)
{
	return camera.zoom;
}

void SetCameraZoom(float zoom)
{
	camera.zoom = zoom;
	camera.newZoom = zoom;
}

sfBool PauseGameCameraMoveRoom(void)
{
	if (camera.zoom != camera.newZoom || camera.lastPositionName != camera.positionName)
	{
		return sfTrue;
	}
	else
	{
		return sfFalse;
	}
}

void SetCamPositionName(sfFloatRect* _hitbox, InfoZone* _area, float _dt)
{
	int num = GetTrigerCount();
	sfVector2f posPlayer = { _hitbox->left + _hitbox->width / 2,_hitbox->top + _hitbox->height / 2 };

	for (int i = 0; i < num; i++)
	{
		if (IsPointInFloatRect(posPlayer, &_area[i].hitbox))
		{
			if (StringCompare(_area[i].type, "Camera"))
			{
				int camName = 0;
				sscanf_s(_area[i].name, "%d", &camName);
				ChangePosCamera(camName, i);
				return;
			}
			else
			{
				SetDefaultView(_dt);
			}
		}
	}
}