#include "Game.h"

void LoadGame(void)
{
	LoadPlayer();
	LoadAim();
	LoadMissile();
	LoadEnemy();
}

void PollEventGame(sfRenderWindow* _renderWindow)
{
	sfEvent event;

	while (sfRenderWindow_pollEvent(_renderWindow, &event))
	{
		switch (event.type)
		{
		case sfEvtClosed:
			sfRenderWindow_close(_renderWindow);
			break;
		case sfEvtKeyPressed:
			KeyPressedGame(_renderWindow, event.key);
			break;
		default:
			break;
		}
	}
}

void KeyPressedGame(sfRenderWindow* _renderWindow, sfKeyEvent _keyEvent)
{
	switch (_keyEvent.code)
	{
	case sfKeyEscape:
		sfRenderWindow_close(_renderWindow);
		break;
	default:
		break;
	}
}

void UpdateGame(sfRenderWindow* _renderWindow, float _dt)
{
	UpdateAim(_renderWindow, _dt);
	UpdatePlayer(_dt, GetPlayerPosition(), GetAimPosition());
	UpdateEnemy(_renderWindow, _dt);
	if (IsPlayerAttacking())
	{
		int index = GetNearestEnemyPosition(GetPlayerPosition());
		if (index != -1)
		{
			if (IsEnemyInMeleeRange(GetAxe(), GetEnemy(index)))
			{
				ChangeStateEnemy(index);

				ChangePlayerState();
			}
		}
		else
		{
			printf("Aucun enemis tué");
		}
	}
	UpdateMissile(_renderWindow, GetAimPosition(), _dt);
	GetAngleBetweenVertices(GetPlayerPosition(), GetAimPosition());
	UpdatePlayerLineOfSight(GetAimPosition());
}

void DrawGame(sfRenderWindow* _renderWindow)
{
	DrawMissile(_renderWindow);
	DrawPlayer(_renderWindow);
	DrawAim(_renderWindow);
	DrawEnemy(_renderWindow);
}

void CleanupGame(void)
{
	CleanupPlayer();
	CleanupMissile();
	CleanupAim();
	CleanupEnemy();
}

sfBool IsEnemyInMeleeRange(sfRectangleShape* _rectShape, sfCircleShape* _circleShape)
{
	sfVector2f axePos = sfRectangleShape_getPosition(_rectShape);
	sfVector2f enemyPos = sfCircleShape_getPosition(_circleShape);
	float dx = enemyPos.x - axePos.x;
	float dy = enemyPos.y - axePos.y;
	float distanceSq = (dx * dx) + (dy * dy);

	// NO SQRT DONE FASTER BUT A LIL.. STRANGE
	if (CheckCollisionAxeEnemy(_rectShape,_circleShape))
	{
		sfVector2f aimPos = GetAimPosition();
		float angleToMouse = atan2f(aimPos.y - axePos.y, aimPos.x - axePos.x) * 180.0f / (float)PI;

		float angleToEnemy = atan2f(dy, dx) * 180.0f / (float)PI;

		float angleDiff = angleToEnemy - angleToMouse;

		while (angleDiff > 180.0f)  angleDiff -= 360.0f;
		while (angleDiff < -180.0f) angleDiff += 360.0f;

		if (fabsf(angleDiff) <= RANGE_ANGLE_LIGHT / 2.0f && GetPlayerAttack() == LIGHT)
		{
			return sfTrue;
		}
		else if (fabsf(angleDiff) <= RANGE_ANGLE_MEDIUM / 2.0f && GetPlayerAttack() == MEDIUM)
		{
			return sfTrue;
		}
		else if (GetPlayerAttack() == HEAVY)
		{
			return sfTrue;
		}
	}
	return sfFalse;
}

sfBool CheckCollisionAxeEnemy(sfRectangleShape* _axe, sfCircleShape* _enemy)
{

	sfVector2f axePos = sfRectangleShape_getPosition(_axe);
	sfVector2f axeSize = sfRectangleShape_getSize(_axe);
	sfVector2f axeOrigin = sfRectangleShape_getOrigin(_axe);
	float axeRotation = sfRectangleShape_getRotation(_axe) * ((float)M_PI / 180.0f); 

	sfVector2f enemyPos = sfCircleShape_getPosition(_enemy);
	float enemyRadius = sfCircleShape_getRadius(_enemy);

	float dx = enemyPos.x - axePos.x;
	float dy = enemyPos.y - axePos.y;

	float localX = dx * cosf(-axeRotation) - dy * sinf(-axeRotation);
	float localY = dx * sinf(-axeRotation) + dy * cosf(-axeRotation);

	float rectLeft = -axeOrigin.x;
	float rectRight = axeSize.x - axeOrigin.x;
	float rectTop = -axeOrigin.y;
	float rectBottom = axeSize.y - axeOrigin.y;

	float closestX = (localX < rectLeft) ? rectLeft : (localX > rectRight ? rectRight : localX);
	float closestY = (localY < rectTop) ? rectTop : (localY > rectBottom ? rectBottom : localY);

	float distX = localX - closestX;
	float distY = localY - closestY;
	float distanceSq = (distX * distX) + (distY * distY);

	return distanceSq < (enemyRadius * enemyRadius);
}