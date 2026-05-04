#include "Missile.h"
#include "Ennemy.h"
#include "Boss.h"
#include "Player.h"

Missile missileList[MISSILE_MAX] = { 0 };
ColdBreath coldBreath = { 0 };
Secondary secondary;
sfTexture* missileTexture;
void MoveMissile(unsigned _index, sfVector2f _playerPos, float _dt);

void LoadMissile(void)
{
	missileTexture = GetAsset("Assets/Sprites/Drone_Placeholder.png");

	coldBreath.sprite = CreateSprite(GetAsset("Assets/Sprites/ColdBreath_Placeholder.png"), (sfVector2f) { 0, 0 }, 1.f, 39);
	SetSpriteOriginMiddle(coldBreath.sprite);
	coldBreath.lifetime = 0.f;
	coldBreath.isAlive = sfFalse;
	//missileList[i].music = CreateMusic("Assets/Musics/FREEZESOUNDAAJOUTER.ogg", 5.f, sfFalse);

	for (unsigned i = 0; i < MISSILE_MAX; i++)
	{
		missileList[i].sprite = CreateSprite(missileTexture, (sfVector2f) { 0, 0 }, 1.f, 39);
		SetSpriteOriginMiddle(missileList[i].sprite);

		sfVector2f missilePosition = { 0 };

		missileList[i].lifetime = 0;

		missileList[i].rotation = 0;

		missileList[i].isAlive = sfFalse;

		missileList[i].music = CreateMusic("Assets/Musics/drone_sound.ogg", 15.f, sfFalse);

		secondary = DRONE;
	}
}

void AddMissile(sfVector2f _pos, sfBool _isRighted)
{
	for (unsigned i = 0; i < MISSILE_MAX; i++)
	{
		if (missileList[i].isAlive == sfFalse)
		{
			sfMusic_play(missileList[i].music);
			missileList[i].isAlive = sfTrue;
			missileList[i].lifetime = 0;
			missileList[i].rotation = -90;
			if (_isRighted)
			{
				sfSprite_setPosition(missileList[i].sprite, (sfVector2f) { _pos.x - 12.f, _pos.y - WEAPON_ORIGIN });
			}
			else
			{
				sfSprite_setPosition(missileList[i].sprite, (sfVector2f) { _pos.x + 12.f, _pos.y - WEAPON_ORIGIN });
			}
			return;
		}
	}
}

void AddColdBreath(sfVector2f _posShooter, sfVector2f _posTarget, ShooterType _shooterType)
{
	//sfMusic_play(coldBreath.music);
	if (coldBreath.isAlive)
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
	coldBreath.velocity.x = cosf(realAngleRad) * MISTEAL_SPEED;
	coldBreath.velocity.y = sinf(realAngleRad) * MISTEAL_SPEED;

	sfSprite_setPosition(coldBreath.sprite, spawnPos);
	sfSprite_setRotation(coldBreath.sprite, realAngleRad * (180.0f / (float)M_PI));

	coldBreath.lifetime = 0.f;
	coldBreath.isAlive = sfTrue;
}

void UpdateSecondary(sfVector2f _posAim, float _dt)
{
	if (secondary == DRONE)
	{
	UpdateMissile(_posAim, _dt);
	}
	else if (secondary == COLDBREATH)
	{
	UpdateColdBreath(_dt);
	}
}

void UpdateMissile(sfVector2f _posAim, float _dt)
{
	for (unsigned i = 0; i < MISSILE_MAX; i++)
	{
		if (missileList[i].isAlive == sfTrue)
		{
			missileList[i].lifetime += _dt;
			if (missileList[i].lifetime <= SECONDARY_PROJECTILE_DURATION)
			{
				CheckCollisionMissilesList();
				MoveMissile(i, _posAim, _dt);
			}
			else
			{
				missileList[i].isAlive = sfFalse;
				sfSprite_setPosition(missileList[i].sprite, (sfVector2f) { 0, 0 });
				sfMusic_stop(missileList[i].music);
				continue;
			}
			sfVector2f reaction = Colision(sfSprite_getGlobalBounds(missileList[i].sprite), AXIS_BOTH);
			sfVector2f reactionBox = ColisionBox(sfSprite_getGlobalBounds(missileList[i].sprite), sfTrue, AXIS_BOTH);
			reaction.x += reactionBox.x;
			reaction.y += reactionBox.y;
			if (reaction.x != 0 || reaction.y != 0 || HitEnemy(10.f, sfSprite_getGlobalBounds(missileList[i].sprite)) || HitBoss(10.f, sfSprite_getGlobalBounds(missileList[i].sprite)))
			{
				sfMusic_stop(missileList[i].music);
				missileList[i].isAlive = sfFalse;
				sfSprite_setPosition(missileList[i].sprite, (sfVector2f) { 0, 0 });
				continue;
			}
		}
	}
}

void UpdateColdBreath(float _dt)
{
	if (coldBreath.isAlive)
	{
		coldBreath.lifetime += _dt;
		if (coldBreath.lifetime > SECONDARY_PROJECTILE_DURATION)
		{
			coldBreath.isAlive = sfFalse;
			sfSprite_setPosition(coldBreath.sprite, (sfVector2f) { 0 });
			return;
		}
		sfFloatRect hitboxColdBreath = { 0 };
		sfVector2f reactionWall = { 0 };
		hitboxColdBreath = sfSprite_getGlobalBounds(coldBreath.sprite);
		reactionWall = Colision(hitboxColdBreath, AXIS_BOTH);
		if (reactionWall.x || reactionWall.y || ColisionBox(hitboxColdBreath, sfFalse, AXIS_BOTH).x || HitEnemy(FREEZE_DMG, hitboxColdBreath) || HitBoss(FREEZE_DMG, hitboxColdBreath))
		{
			coldBreath.isAlive = sfFalse;
			sfSprite_setPosition(coldBreath.sprite, (sfVector2f) { 0 });
			return;
		}
		else
		{
			coldBreath.velocity.y += G * _dt;
			sfSprite_move(coldBreath.sprite, (sfVector2f) { coldBreath.velocity.x* _dt, coldBreath.velocity.y* _dt });
			sfSprite_setRotation(coldBreath.sprite, (float)RAD_DEG(atan2f(coldBreath.velocity.y, coldBreath.velocity.x)));
		}
	}
}

void MoveMissile(unsigned _index, sfVector2f _playerPos, float _dt)
{
	if (!missileList[_index].isAlive) return;

	sfVector2f currentPos = sfSprite_getPosition(missileList[_index].sprite);

	float dx = _playerPos.x - currentPos.x;
	float dy = _playerPos.y - currentPos.y;

	float targetAngle = atan2f(dy, dx) * 180.0f / (float)PI;

	float angleDiff = targetAngle - missileList[_index].rotation;

	while (angleDiff > 180.0f)  angleDiff -= 360.0f;
	while (angleDiff < -180.0f) angleDiff += 360.0f;

	float maxRotationThisFrame = DEGRE_ROTATION * _dt;

	if (fabsf(angleDiff) <= maxRotationThisFrame)
	{
		missileList[_index].rotation = targetAngle;
	}
	else
	{
		if (angleDiff > 0)
			missileList[_index].rotation += maxRotationThisFrame;
		else
			missileList[_index].rotation -= maxRotationThisFrame;
	}

	float angleInRadians = missileList[_index].rotation * (float)PI / 180.0f;

	sfVector2f movement;
	movement.x = cosf(angleInRadians) * SPEED_MISSILE * _dt;
	movement.y = sinf(angleInRadians) * SPEED_MISSILE * _dt;

	// 7. Application à la forme SFML
	sfSprite_setRotation(missileList[_index].sprite, missileList[_index].rotation);
	sfSprite_move(missileList[_index].sprite, movement);
}

void CheckCollisionMissilesList(void)
{
	for (unsigned i = 0; i < MISSILE_MAX; i++)
	{
		if (!missileList[i].isAlive) continue;

		// On commence à j = i + 1 pour ne pas tester deux fois la même paire
		// et ne pas tester le missile contre lui-même
		for (unsigned j = i + 1; j < MISSILE_MAX; j++)
		{
			if (!missileList[j].isAlive) continue;

			sfVector2f posA = sfSprite_getPosition(missileList[i].sprite);
			sfVector2f posB = sfSprite_getPosition(missileList[j].sprite);

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

				sfSprite_move(missileList[i].sprite, moveA);
				sfSprite_move(missileList[j].sprite, moveB);
			}
		}
	}
}

Secondary GetSecondary(void)
{
	return secondary;
}