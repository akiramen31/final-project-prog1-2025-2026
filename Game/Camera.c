#include "Camera.h"

int lastCamPositonName;

void UpdatePositionCameraLevel1(float _dt);
void UpdatePositionCameraLevel2(float _dt);
void UpdatePositionCameraLevel3(float _dt);

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

	printf("%f %f", _pos.x, _pos.y);
	printf("	%f %f\n", newPos.x, newPos.y);

	newPos.x *= _dt;
	newPos.y *= _dt;

	newPos.x *= CAMERA_SPEED;
	newPos.y *= CAMERA_SPEED;

	if (POW2(newPos.x) < 1 && POW2(newPos.y) < 1)
	{
		SetViewCenter(_pos);
	}
	else
	{
		MoveView(newPos);
	}

	//SetViewCenter(_pos);
}

void UpdatePositionCameraLevel1(float _dt)
{
	SetDefaultView(_dt);
}

void UpdatePositionCameraLevel2(float _dt)
{
	sfFloatRect hitbox = GetPlayerRect();
	InfoZone* zone = GetInfoZoneTriger(hitbox);

	if (zone != NULL)
	{
		if (sfFloatRect_intersects(&hitbox, &zone[lastCamPositonName].hitbox, NULL) && lastCamPositonName != 0)
		{
			switch (lastCamPositonName)
			{
			case 1:
				SetViewZoom(0.5f);
				MoveViewSlow((sfVector2f) { 0, 0 }, _dt);
				break;
			default:
				SetDefaultView(_dt);
				break;
			}
		}
		else
		{
			int num = GetTrigerCount();
			sfFloatRect area = { 0 };

			for (int i = 0; i < num; i++)
			{
				printf("%f ", zone[i].hitbox.height);
			}


			for (int i = 0; i < num; i++)
			{
				if (sfFloatRect_intersects(&hitbox, &zone[i].hitbox, &area))
				{
					printf("ahhhhhhhhhhhhhhh ");
					if (StringCompare(zone[i].type, "Camera"))
					{
						int camName = 0;
						sscanf_s(zone[i].name, "%d", &camName);

						if (camName != 0)
						{
							lastCamPositonName = i;
						}
						else
						{
							lastCamPositonName = 0;
						}
						return;
					}
				}
				else
				{
					lastCamPositonName = 0;
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

void SetDefaultView(float _dt)
{
	MoveViewSlow(GetPlayerPosition(), _dt);
	//SetViewCenter(GetPlayerPosition());
	SetViewZoom(0.5f);
	lastCamPositonName = 0;
}