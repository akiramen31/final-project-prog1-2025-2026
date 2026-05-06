#include "Secondaries.h"
#include "Ennemy.h"
#include "Boss.h"
#include "Player.h"

SecondaryData secondaryData;

void LoadSecondary(void)
{

	secondaryData.droneTexture = GetAsset("Assets/Sprites/drone.png");

	secondaryData.coldBreath.sprite = CreateSprite(GetAsset("Assets/Sprites/ColdBreath_Placeholder.png"), (sfVector2f) { 0, 0 }, 1.f, 39);
	SetSpriteOriginMiddle(secondaryData.coldBreath.sprite);
	secondaryData.coldBreath.lifetime = 0.f;
	secondaryData.coldBreath.isAlive = sfFalse;
	//missileList[i].music = CreateMusic("Assets/Musics/FREEZESOUNDAAJOUTER.ogg", 5.f, sfFalse);

	for (unsigned i = 0; i < MISSILE_MAX; i++)
	{
		secondaryData.droneList[i].sprite = CreateSprite(secondaryData.droneTexture, (sfVector2f) { 0 }, 0.f, 39);
		SetSpriteOriginMiddle(secondaryData.droneList[i].sprite);
		secondaryData.droneList[i].lifetime = 0;
		secondaryData.droneList[i].rotation = 0;
		secondaryData.droneList[i].isAlive = sfFalse;

		secondaryData.droneList[i].ambientSound = CreateMusic("Assets/Musics/drone_sound.ogg", 15.f, sfFalse);
	}
}

void AddDrone(sfVector2f _pos, sfBool _isRighted)
{
	for (unsigned i = 0; i < MISSILE_MAX; i++)
	{
		if (secondaryData.droneList[i].isAlive == sfFalse)
		{
			sfMusic_play(secondaryData.droneList[i].ambientSound);
			secondaryData.droneList[i].isAlive = sfTrue;
			secondaryData.droneList[i].lifetime = 0;
			secondaryData.droneList[i].rotation = -90;
			sfSprite_setScale(secondaryData.droneList[i].sprite, (sfVector2f) { 1 ,1});
			if (_isRighted)
			{
				sfSprite_setPosition(secondaryData.droneList[i].sprite, (sfVector2f) { _pos.x - 12.f, _pos.y - WEAPON_ORIGIN });
			}
			else
			{
				sfSprite_setPosition(secondaryData.droneList[i].sprite, (sfVector2f) { _pos.x + 12.f, _pos.y - WEAPON_ORIGIN });
			}
			return;
		}
	}
}

void AddColdBreath(sfVector2f _posShooter, sfVector2f _posTarget, ShooterType _shooterType)
{
	//sfMusic_play(coldBreath.music);
	if (secondaryData.coldBreath.isAlive)
	{
		return;
	}

	sfVector2f pivotPos = { _posShooter.x, _posShooter.y - _shooterType.weaponPos };

	float dxInitial = _posTarget.x - pivotPos.x;
	float dyInitial = _posTarget.y - pivotPos.y;
	float angleRadInitial = atan2f(dyInitial, dxInitial);


	if (!_shooterType.isRighted)
	{
		_shooterType.shootPosition.y = -_shooterType.shootPosition.y;
	}

	sfVector2f spawnPos;
	spawnPos.x = pivotPos.x + cosf(angleRadInitial) * _shooterType.shootPosition.x - sinf(angleRadInitial) * _shooterType.shootPosition.y;
	spawnPos.y = pivotPos.y + sinf(angleRadInitial) * _shooterType.shootPosition.x + cosf(angleRadInitial) * _shooterType.shootPosition.y;

	float realDx = _posTarget.x - spawnPos.x;
	float realDy = _posTarget.y - spawnPos.y;
	float realAngleRad = atan2f(realDy, realDx);
	secondaryData.coldBreath.velocity.x = cosf(realAngleRad) * MISTEAL_SPEED;
	secondaryData.coldBreath.velocity.y = sinf(realAngleRad) * MISTEAL_SPEED;

	sfSprite_setPosition(secondaryData.coldBreath.sprite, spawnPos);
	sfSprite_setRotation(secondaryData.coldBreath.sprite, realAngleRad * (180.0f / (float)M_PI));

	secondaryData.coldBreath.lifetime = 0.f;
	secondaryData.coldBreath.isAlive = sfTrue;
}

void UpdateSecondary(sfVector2f _posAim, float _dt)
{
	switch (secondaryData.secondaryType)
	{
	case COLDBREATH:
		UpdateColdBreath(_dt);
		break;
	case DRONE:
		UpdateDrone(_posAim, _dt);
		break;
	}
}

void UpdateDrone(sfVector2f _posAim, float _dt)
{
	for (unsigned i = 0; i < MISSILE_MAX; i++)
	{
		if (secondaryData.droneList[i].isAlive == sfTrue)
		{
			secondaryData.droneList[i].lifetime += _dt;
			if (secondaryData.droneList[i].lifetime <= SECONDARY_PROJECTILE_DURATION)
			{
				CheckCollisionMissilesList();
				MoveDrone(i, _posAim, _dt);
			}
			else
			{
				secondaryData.droneList[i].isAlive = sfFalse;
				sfSprite_setPosition(secondaryData.droneList[i].sprite, (sfVector2f) { 0, 0 });
				sfMusic_stop(secondaryData.droneList[i].ambientSound);
				continue;
			}
			sfVector2f reaction = Colision(sfSprite_getGlobalBounds(secondaryData.droneList[i].sprite), AXIS_BOTH);
			sfVector2f reactionBox = ColisionBox(sfSprite_getGlobalBounds(secondaryData.droneList[i].sprite), sfTrue, AXIS_BOTH);
			reaction.x += reactionBox.x;
			reaction.y += reactionBox.y;
			if (reaction.x != 0 || reaction.y != 0 || HitEnemy(10.f, sfSprite_getGlobalBounds(secondaryData.droneList[i].sprite)) || HitBoss(10.f, sfSprite_getGlobalBounds(secondaryData.droneList[i].sprite), HEAVY))
			{
				sfMusic_stop(secondaryData.droneList[i].ambientSound);
				secondaryData.droneList[i].isAlive = sfFalse;
				sfSprite_setPosition(secondaryData.droneList[i].sprite, (sfVector2f) { 0, 0 });
				continue;
			}
		}
	}
}

void UpdateColdBreath(float _dt)
{
	if (secondaryData.coldBreath.isAlive)
	{
		secondaryData.coldBreath.lifetime += _dt;
		if (secondaryData.coldBreath.lifetime > SECONDARY_PROJECTILE_DURATION)
		{
			secondaryData.coldBreath.isAlive = sfFalse;
			sfSprite_setPosition(secondaryData.coldBreath.sprite, (sfVector2f) { 0 });
			return;
		}
		sfFloatRect hitboxColdBreath = { 0 };
		sfVector2f reactionWall = { 0 };
		hitboxColdBreath = sfSprite_getGlobalBounds(secondaryData.coldBreath.sprite);
		if (reactionWall.x || reactionWall.y || ColisionBox(hitboxColdBreath, sfFalse, AXIS_BOTH).x || HitEnemy(FREEZE_DMG, hitboxColdBreath) || HitBoss(FREEZE_DMG, hitboxColdBreath, FREEZE))
		{
			secondaryData.coldBreath.isAlive = sfFalse;
			sfSprite_setPosition(secondaryData.coldBreath.sprite, (sfVector2f) { 0 });
			return;
		}
		else
		{
			secondaryData.coldBreath.velocity.y += G * _dt;
			sfSprite_move(secondaryData.coldBreath.sprite, (sfVector2f) { secondaryData.coldBreath.velocity.x* _dt, secondaryData.coldBreath.velocity.y* _dt });
			sfSprite_setRotation(secondaryData.coldBreath.sprite, (float)RAD_DEG(atan2f(secondaryData.coldBreath.velocity.y, secondaryData.coldBreath.velocity.x)));
		}
	}
}

void MoveDrone(unsigned _index, sfVector2f _playerPos, float _dt)
{
	if (!secondaryData.droneList[_index].isAlive) return;

	sfVector2f currentPos = sfSprite_getPosition(secondaryData.droneList[_index].sprite);

	float dx = _playerPos.x - currentPos.x;
	float dy = _playerPos.y - currentPos.y;

	float targetAngle = atan2f(dy, dx) * 180.0f / (float)PI;

	float angleDiff = targetAngle - secondaryData.droneList[_index].rotation;

	while (angleDiff > 180.0f)  angleDiff -= 360.0f;
	while (angleDiff < -180.0f) angleDiff += 360.0f;

	float maxRotationThisFrame = DEGRE_ROTATION * _dt;

	if (fabsf(angleDiff) <= maxRotationThisFrame)
	{
		secondaryData.droneList[_index].rotation = targetAngle;
	}
	else
	{
		if (angleDiff > 0)
			secondaryData.droneList[_index].rotation += maxRotationThisFrame;
		else
			secondaryData.droneList[_index].rotation -= maxRotationThisFrame;
	}

	float angleInRadians = secondaryData.droneList[_index].rotation * (float)PI / 180.0f;

	sfVector2f movement;
	movement.x = cosf(angleInRadians) * SPEED_MISSILE * _dt;
	movement.y = sinf(angleInRadians) * SPEED_MISSILE * _dt;

	// 7. Application à la forme SFML
	sfSprite_setRotation(secondaryData.droneList[_index].sprite, secondaryData.droneList[_index].rotation);
	sfSprite_move(secondaryData.droneList[_index].sprite, movement);
}

void CheckCollisionMissilesList(void)
{
	for (unsigned i = 0; i < MISSILE_MAX; i++)
	{
		if (!secondaryData.droneList[i].isAlive) continue;

		// On commence à j = i + 1 pour ne pas tester deux fois la même paire
		// et ne pas tester le missile contre lui-même
		for (unsigned j = i + 1; j < MISSILE_MAX; j++)
		{
			if (!secondaryData.droneList[j].isAlive) continue;

			sfVector2f posA = sfSprite_getPosition(secondaryData.droneList[i].sprite);
			sfVector2f posB = sfSprite_getPosition(secondaryData.droneList[j].sprite);

			float dx = posB.x - posA.x;
			float dy = posB.y - posA.y;
			float distance = sqrtf(dx * dx + dy * dy);
			float minDistance = MISSILE_WEIGHT * 2.0f; // Somme des deux rayons

			if (distance < minDistance)
			{
				// 1. Calcul du décalage (pénétration)
				float overlap = minDistance - distance;

				// 2. Normalisation du vecteur de direction (pour savoir où pousser)
				// On évite la division par zéro si les missiles sont exactement au même point
				if (distance == 0) distance = 0.1f;
				float nx = dx / distance;
				float ny = dy / distance;

				// 3. On décale chaque missile de la moitié de l'overlap dans des directions opposées
				sfVector2f moveA = { -nx * overlap / 2.0f, -ny * overlap / 2.0f };
				sfVector2f moveB = { nx * overlap / 2.0f,  ny * overlap / 2.0f };

				sfSprite_move(secondaryData.droneList[i].sprite, moveA);
				sfSprite_move(secondaryData.droneList[j].sprite, moveB);
			}
		}
	}
}

SecondaryType GetSecondaryType(void)
{
	return secondaryData.secondaryType;
}

void SetSecondaryType(SecondaryType _type)
{
	secondaryData.secondaryType = _type;
}