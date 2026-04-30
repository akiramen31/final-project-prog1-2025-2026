#include "Camera.h"

int CamPositonName;
int LastCamPositonName;
int index;
float cameraZoom;
float cameraNewZoom;

void UpdatePositionCameraLevel1(float _dt);
void UpdatePositionCameraLevel2(float _dt);
void UpdatePositionCameraLevel3(float _dt);

void ChangePosCamera(int _lastCamPos, int _val);

void UpdateCamera(float _dt)
{
	switch (GetActualyMap())
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

	if (cameraZoom != cameraNewZoom)
	{
		if (cameraZoom > cameraNewZoom)
		{
			cameraZoom -= CAMERA_ZOOM_SPEED;
		}
		else
		{
			cameraZoom += CAMERA_ZOOM_SPEED;
		}

		if (cameraZoom < cameraNewZoom + CAMERA_ZOOM_SPEED && cameraZoom > cameraNewZoom - CAMERA_ZOOM_SPEED)
		{
			cameraZoom = cameraNewZoom;
		}
	}
	SetViewZoom(cameraZoom);
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


	float speed;
	if (CamPositonName == 0)
	{
		speed = CAMERA_SPEED_ON_PLAYER;
	}
	else
	{
		speed = CAMERA_SPEED;
	}

	move.x = _dt * (float)cos(angle) * speed;
	move.y = _dt * (float)sin(angle) * speed;

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

	if (POW2(move.x) < CAMERA_LOCK && POW2(move.y) < CAMERA_LOCK)
	{
		SetViewCenter(_pos);
		LastCamPositonName = CamPositonName;
	}
	else
	{
		MoveView(move);
	}
}

void UpdatePositionCameraLevel1(float _dt)
{
	sfFloatRect hitbox = GetPlayerRect();
	InfoZone* zone = GetInfoZoneTriger(hitbox);

	if (zone != NULL)
	{
		sfVector2f pos = { 0 };

		if (sfFloatRect_intersects(&hitbox, &zone[index].hitbox, NULL) && CamPositonName != 0)
		{
			switch (CamPositonName)
			{
			case 1:
				cameraNewZoom = 0.2f;
				pos.x = zone[index].hitbox.left + zone[index].hitbox.width / 2;
				pos.y = zone[index].hitbox.top + zone[index].hitbox.height / 2;
				break;
			case 2:
				cameraNewZoom = 0.35f;
				pos.x = zone[index].hitbox.left + zone[index].hitbox.width / 2;
				pos.y = zone[index].hitbox.top + zone[index].hitbox.height / 2;
				break;
			case 3:
				cameraNewZoom = 0.25f;
				pos.x = zone[index].hitbox.left + zone[index].hitbox.width / 2;
				pos.y = zone[index].hitbox.top + zone[index].hitbox.height / 2;
				break;
				//for biggest room
			case 4:
				cameraNewZoom = 0.3f;
				if (zone[index].hitbox.left + zone[index].hitbox.width - zone[index].hitbox.width / 3 <= hitbox.left + hitbox.width / 2)
				{
					pos.x = zone[index].hitbox.left + zone[index].hitbox.width - zone[index].hitbox.width / 3;
				}
				else if (zone[index].hitbox.left + zone[index].hitbox.width / 3 >= hitbox.left + hitbox.width / 2)
				{
					pos.x = zone[index].hitbox.left + zone[index].hitbox.width / 3;
				}
				else
				{
					pos.x = hitbox.left + hitbox.width / 2;
				}

				if (zone[index].hitbox.top + zone[index].hitbox.height - zone[index].hitbox.height / 3 <= hitbox.top + hitbox.height / 2)
				{
					pos.y = zone[index].hitbox.top + zone[index].hitbox.height - zone[index].hitbox.height / 3;
				}
				else if (zone[index].hitbox.top + zone[index].hitbox.height / 3 >= hitbox.top + hitbox.height / 2)
				{
					pos.y = zone[index].hitbox.top + zone[index].hitbox.height / 3;
				}
				else
				{
					pos.y = hitbox.top + hitbox.height / 2;
				}
				break;
				//for boss room
			case 5:
				cameraNewZoom = 0.42f;
				pos.x = zone[index].hitbox.left + zone[index].hitbox.width / 2;
				pos.y = zone[index].hitbox.top + zone[index].hitbox.height / 2;
				break;
			default:
				pos = GetPlayerPosition();
				break;
			}
			MoveViewSlow(pos, _dt);
		}
		else
		{
			int num = GetTrigerCount();
			sfFloatRect area = { 0 };

			for (int i = 0; i < num; i++)
			{
				if (sfFloatRect_intersects(&hitbox, &zone[i].hitbox, &area))
				{
					if (StringCompare(zone[i].type, "Camera"))
					{
						int camName = 0;
						sscanf_s(zone[i].name, "%d", &camName);
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
	}
	else
	{
		SetDefaultView(_dt);
	}
}

void UpdatePositionCameraLevel2(float _dt)
{
	sfFloatRect hitbox = GetPlayerRect();
	InfoZone* zone = GetInfoZoneTriger(hitbox);

	if (zone != NULL)
	{
		sfVector2f pos = { 0 };

		if (sfFloatRect_intersects(&hitbox, &zone[index].hitbox, NULL) && CamPositonName != 0)
		{
			switch (CamPositonName)
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
				cameraNewZoom = 0.2f;
				pos.x = zone[index].hitbox.left + zone[index].hitbox.width / 2;
				pos.y = zone[index].hitbox.top + zone[index].hitbox.height / 2;
				break;
				//for bigger room
			case 5:
			case 6:
			case 15:
			case 18:
			case 22:
			case 23:
			case 25:
				cameraNewZoom = 0.3f;
				pos.x = zone[index].hitbox.left + zone[index].hitbox.width / 2;
				pos.y = zone[index].hitbox.top + zone[index].hitbox.height / 2;
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
				cameraNewZoom = 0.3f;
				pos.x = zone[index].hitbox.left + zone[index].hitbox.width / 2;
				if (zone[index].hitbox.top + zone[index].hitbox.height - zone[index].hitbox.height / 3 <= hitbox.top + hitbox.height / 2)
				{
					pos.y = zone[index].hitbox.top + zone[index].hitbox.height - zone[index].hitbox.height / 3;
				}
				else if (zone[index].hitbox.top + zone[index].hitbox.height / 3 >= hitbox.top + hitbox.height / 2)
				{
					pos.y = zone[index].hitbox.top + zone[index].hitbox.height / 3;
				}
				else
				{
					pos.y = hitbox.top + hitbox.height / 2;
				}
				break;
				//for wide room
			case 27:
			case 29:
				cameraNewZoom = 0.3f;
				if (zone[index].hitbox.left + zone[index].hitbox.width - zone[index].hitbox.width / 3 <= hitbox.left + hitbox.width / 2)
				{
					pos.x = zone[index].hitbox.left + zone[index].hitbox.width - zone[index].hitbox.width / 3;
				}
				else if (zone[index].hitbox.left + zone[index].hitbox.width / 3 >= hitbox.left + hitbox.width / 2)
				{
					pos.x = zone[index].hitbox.left + zone[index].hitbox.width / 3;
				}
				else
				{
					pos.x = hitbox.left + hitbox.width / 2;
				}
				pos.y = zone[index].hitbox.top + zone[index].hitbox.height / 2;
				break;
				//for biggest room
			case 28:
				cameraNewZoom = 0.3f;
				if (zone[index].hitbox.left + zone[index].hitbox.width - zone[index].hitbox.width / 3 <= hitbox.left + hitbox.width / 2)
				{
					pos.x = zone[index].hitbox.left + zone[index].hitbox.width - zone[index].hitbox.width / 3;
				}
				else if (zone[index].hitbox.left + zone[index].hitbox.width / 3 >= hitbox.left + hitbox.width / 2)
				{
					pos.x = zone[index].hitbox.left + zone[index].hitbox.width / 3;
				}
				else
				{
					pos.x = hitbox.left + hitbox.width / 2;
				}

				if (zone[index].hitbox.top + zone[index].hitbox.height - zone[index].hitbox.height / 3 <= hitbox.top + hitbox.height / 2)
				{
					pos.y = zone[index].hitbox.top + zone[index].hitbox.height - zone[index].hitbox.height / 3;
				}
				else if (zone[index].hitbox.top + zone[index].hitbox.height / 3 >= hitbox.top + hitbox.height / 2)
				{
					pos.y = zone[index].hitbox.top + zone[index].hitbox.height / 3;
				}
				else
				{
					pos.y = hitbox.top + hitbox.height / 2;
				}
				break;
				//for boss room
			case 30:
				cameraNewZoom = 0.42f;
				pos.x = zone[index].hitbox.left + zone[index].hitbox.width / 2;
				pos.y = zone[index].hitbox.top + zone[index].hitbox.height / 2;
				break;
			default:
				pos = GetPlayerPosition();
				break;
			}
			MoveViewSlow(pos, _dt);
		}
		else
		{
			int num = GetTrigerCount();
			sfFloatRect area = { 0 };

			for (int i = 0; i < num; i++)
			{
				if (sfFloatRect_intersects(&hitbox, &zone[i].hitbox, &area))
				{
					if (StringCompare(zone[i].type, "Camera"))
					{
						int camName = 0;
						sscanf_s(zone[i].name, "%d", &camName);
						ChangePosCamera(camName, i);
						return;
					}
				}
			}
		}
	}
	else
	{
		SetDefaultView(_dt);
	}
}

void UpdatePositionCameraLevel3(float _dt)
{
	SetDefaultView(_dt);
}

void ChangePosCamera(int _lastCamPos, int _val)
{
	CamPositonName = _lastCamPos;
	index = _val;
}

void SetDefaultView(float _dt)
{
	sfVector2f pos = GetPlayerPosition();
	MoveViewSlow((sfVector2f) { pos.x, pos.y - TILE_SIZE * 3 }, _dt);
	cameraNewZoom = 0.2f;
	ChangePosCamera(0, 0);
}

float GetCameraZoom(void)
{
	return cameraZoom;
}

void SetCameraZoom(float zoom)
{
	cameraZoom = zoom;
	cameraNewZoom = zoom;
}

sfBool PauseGameCameraMoveRoom(void)
{
	if (cameraZoom != cameraNewZoom || LastCamPositonName != CamPositonName)
	{
		return sfTrue;
	}
	else
	{
		return sfFalse;
	}
}