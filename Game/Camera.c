#include "Camera.h"

int lastCamPositonName;
int val;
float cameraZoom;

void UpdatePositionCameraLevel1(float _dt);
void UpdatePositionCameraLevel2(float _dt);
void UpdatePositionCameraLevel3(float _dt);

void ChangeLastPos(int _lastCamPos, int _val);

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
}


void MoveViewSlow(sfVector2f _pos, float _dt)
{
	sfVector2f tempPos = GetViewPosition();
	sfVector2f size = GetViewSize();

	tempPos.x += size.x / 2;
	tempPos.y += size.y / 2;

	sfVector2f newPos = (sfVector2f){ VECTOR(tempPos, _pos) };

	newPos.x *= _dt;
	newPos.y *= _dt;

	newPos.x *= CAMERA_SPEED;
	newPos.y *= CAMERA_SPEED;

	if (newPos.x > CAMERA_SPEED_MAX)
	{
		newPos.x = CAMERA_SPEED_MAX;
	}
	else if (newPos.x < -CAMERA_SPEED_MAX)
	{
		newPos.x = -CAMERA_SPEED_MAX;
	}
	if (newPos.y > CAMERA_SPEED_MAX)
	{
		newPos.y = CAMERA_SPEED_MAX;
	}
	else if (newPos.y < -CAMERA_SPEED_MAX)
	{
		newPos.y = -CAMERA_SPEED_MAX;
	}


	if (POW2(newPos.x) < CAMERA_LOCK && POW2(newPos.y) < CAMERA_LOCK)
	{
		SetViewCenter(_pos);
	}
	else
	{
		MoveView(newPos);
	}
}

void UpdatePositionCameraLevel1(float _dt)
{
	sfFloatRect hitbox = GetPlayerRect();
	InfoZone* zone = GetInfoZoneTriger(hitbox);

	if (zone != NULL)
	{
		sfVector2f pos = { 0 };

		if (sfFloatRect_intersects(&hitbox, &zone[val].hitbox, NULL) && lastCamPositonName != 0)
		{
			switch (lastCamPositonName)
			{
			case 1:
				cameraZoom = 0.2f;
				pos.x = zone[val].hitbox.left + zone[val].hitbox.width / 2;
				pos.y = zone[val].hitbox.top + zone[val].hitbox.height / 2;
				break;
			case 2:
				cameraZoom = 0.35f;
				pos.x = zone[val].hitbox.left + zone[val].hitbox.width / 2;
				pos.y = zone[val].hitbox.top + zone[val].hitbox.height / 2;
				break;
			case 3:
				cameraZoom = 0.25f;
				pos.x = zone[val].hitbox.left + zone[val].hitbox.width / 2;
				pos.y = zone[val].hitbox.top + zone[val].hitbox.height / 2;
				break;
				//for biggest room
			case 4:
				cameraZoom = 0.3f;
				if (zone[val].hitbox.left + zone[val].hitbox.width - zone[val].hitbox.width / 3 <= hitbox.left + hitbox.width / 2)
				{
					pos.x = zone[val].hitbox.left + zone[val].hitbox.width - zone[val].hitbox.width / 3;
				}
				else if (zone[val].hitbox.left + zone[val].hitbox.width / 3 >= hitbox.left + hitbox.width / 2)
				{
					pos.x = zone[val].hitbox.left + zone[val].hitbox.width / 3;
				}
				else
				{
					pos.x = hitbox.left + hitbox.width / 2;
				}

				if (zone[val].hitbox.top + zone[val].hitbox.height - zone[val].hitbox.height / 3 <= hitbox.top + hitbox.height / 2)
				{
					pos.y = zone[val].hitbox.top + zone[val].hitbox.height - zone[val].hitbox.height / 3;
				}
				else if (zone[val].hitbox.top + zone[val].hitbox.height / 3 >= hitbox.top + hitbox.height / 2)
				{
					pos.y = zone[val].hitbox.top + zone[val].hitbox.height / 3;
				}
				else
				{
					pos.y = hitbox.top + hitbox.height / 2;
				}
				break;
				//for boss room
			case 5:
				cameraZoom = 0.42f;
				pos.x = zone[val].hitbox.left + zone[val].hitbox.width / 2;
				pos.y = zone[val].hitbox.top + zone[val].hitbox.height / 2;
				break;
			default:
				pos = GetPlayerPosition();
				break;
			}
			MoveViewSlow(pos, _dt);
			SetViewZoom(cameraZoom);
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
						ChangeLastPos(camName, i);
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

		if (sfFloatRect_intersects(&hitbox, &zone[val].hitbox, NULL) && lastCamPositonName != 0)
		{
			switch (lastCamPositonName)
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
				cameraZoom = 0.2f;
				pos.x = zone[val].hitbox.left + zone[val].hitbox.width / 2;
				pos.y = zone[val].hitbox.top + zone[val].hitbox.height / 2;
				break;
				//for bigger room
			case 5:
			case 6:
			case 15:
			case 18:
			case 22:
			case 23:
			case 25:
				cameraZoom = 0.3f;
				pos.x = zone[val].hitbox.left + zone[val].hitbox.width / 2;
				pos.y = zone[val].hitbox.top + zone[val].hitbox.height / 2;
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
				cameraZoom = 0.3f;
				pos.x = zone[val].hitbox.left + zone[val].hitbox.width / 2;
				if (zone[val].hitbox.top + zone[val].hitbox.height - zone[val].hitbox.height / 3 <= hitbox.top + hitbox.height / 2)
				{
					pos.y = zone[val].hitbox.top + zone[val].hitbox.height - zone[val].hitbox.height / 3;
				}
				else if (zone[val].hitbox.top + zone[val].hitbox.height / 3 >= hitbox.top + hitbox.height / 2)
				{
					pos.y = zone[val].hitbox.top + zone[val].hitbox.height / 3;
				}
				else
				{
					pos.y = hitbox.top + hitbox.height / 2;
				}
				break;
				//for wide room
			case 27:
			case 29:
				cameraZoom = 0.3f;
				if (zone[val].hitbox.left + zone[val].hitbox.width - zone[val].hitbox.width / 3 <= hitbox.left + hitbox.width / 2)
				{
					pos.x = zone[val].hitbox.left + zone[val].hitbox.width - zone[val].hitbox.width / 3;
				}
				else if (zone[val].hitbox.left + zone[val].hitbox.width / 3 >= hitbox.left + hitbox.width / 2)
				{
					pos.x = zone[val].hitbox.left + zone[val].hitbox.width / 3;
				}
				else
				{
					pos.x = hitbox.left + hitbox.width / 2;
				}
				pos.y = zone[val].hitbox.top + zone[val].hitbox.height / 2;
				break;
				//for biggest room
			case 28:
				cameraZoom = 0.3f;
				if (zone[val].hitbox.left + zone[val].hitbox.width - zone[val].hitbox.width / 3 <= hitbox.left + hitbox.width / 2)
				{
					pos.x = zone[val].hitbox.left + zone[val].hitbox.width - zone[val].hitbox.width / 3;
				}
				else if (zone[val].hitbox.left + zone[val].hitbox.width / 3 >= hitbox.left + hitbox.width / 2)
				{
					pos.x = zone[val].hitbox.left + zone[val].hitbox.width / 3;
				}
				else
				{
					pos.x = hitbox.left + hitbox.width / 2;
				}

				if (zone[val].hitbox.top + zone[val].hitbox.height - zone[val].hitbox.height / 3 <= hitbox.top + hitbox.height / 2)
				{
					pos.y = zone[val].hitbox.top + zone[val].hitbox.height - zone[val].hitbox.height / 3;
				}
				else if (zone[val].hitbox.top + zone[val].hitbox.height / 3 >= hitbox.top + hitbox.height / 2)
				{
					pos.y = zone[val].hitbox.top + zone[val].hitbox.height / 3;
				}
				else
				{
					pos.y = hitbox.top + hitbox.height / 2;
				}
				break;
				//for boss room
			case 30:
				cameraZoom = 0.42f;
				pos.x = zone[val].hitbox.left + zone[val].hitbox.width / 2;
				pos.y = zone[val].hitbox.top + zone[val].hitbox.height / 2;
				break;
			default:
				pos = GetPlayerPosition();
				break;
			}
			MoveViewSlow(pos, _dt);
			SetViewZoom(cameraZoom);
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
						ChangeLastPos(camName, i);
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

void ChangeLastPos(int _lastCamPos, int _val)
{
	lastCamPositonName = _lastCamPos;
	val = _val;
}

void SetDefaultView(float _dt)
{
	sfVector2f pos = GetPlayerPosition();
	MoveViewSlow((sfVector2f) { pos.x, pos.y - TILE_SIZE * 3 }, _dt);
	cameraZoom = 0.2f;
	SetViewZoom(cameraZoom);
	ChangeLastPos(0, 0);
}

float GetCameraZoom(void)
{
	return cameraZoom;
}
