#include "Projectiles.h"
#include "Ennemy.h"
#include "Boss.h"
#include "Player.h"
#include "Elevator.h"

sfTexture* bulletTexture;
sfTexture* mistealTexture;
sfTexture* droneTexture;
Bullet bulletListAlly[BULLET_ALLY_MAX];
Bullet bulletListEnemy[BULLET_ENEMY_MAX];
Misteal mistealList[MISTEAL_ALLY_MAX];
Drone droneList[DRONE_MAX] = { 0 };
ColdBreath coldBreath = { 0 };
BossDrone bossDroneList[MAX_BOSS_DRONE] = { 0 };
DangerZone dangerZoneList[MAX_BOSS_DRONE] = { 0 };


unsigned mistealCount;
unsigned bulletCountAlly;
unsigned bulletCountEnemy;
float groundLevel;

void SortBulletListAlly(unsigned _index);
void SortBulletListEnemy(unsigned _index);
void SortMistealList(unsigned _index);

void LoadProjectiles(float _groundlvl)
{
	bulletTexture = GetAsset("Assets/Sprites/bullet.png");
	mistealTexture = GetAsset("Assets/Sprites/Misteal_Ammo_Placeholder.png");
	mistealCount = 0;
	bulletCountAlly = 0;
	bulletCountEnemy = 0;
	groundLevel = _groundlvl;
	LoadSecondary();
	LoadBossDrone();
}

void LoadSecondary(void)
{
	//COLDBREATH PART

	droneTexture = GetAsset("Assets/Sprites/drone.png");

	coldBreath.sprite = CreateSprite(GetAsset("Assets/Sprites/ColdBreath_Placeholder.png"), (sfVector2f) { 0, 0 }, 1.f, 39);
	SetSpriteOriginMiddle(coldBreath.sprite);
	coldBreath.lifetime = 0.f;
	coldBreath.isAlive = sfFalse;
	//missileList[i].music = CreateMusic("Assets/Musics/FREEZESOUNDAAJOUTER.ogg", 5.f, sfFalse);
	//

	// DRONE PART

	for (unsigned i = 0; i < DRONE_MAX; i++)
	{
		droneList[i].sprite = CreateSprite(droneTexture, (sfVector2f) { 0 }, 0.f, 39);
		SetSpriteOriginMiddle(droneList[i].sprite);
		droneList[i].lifetime = 0;
		droneList[i].rotation = 0;
		droneList[i].isAlive = sfFalse;
		droneList[i].ambientSound = CreateMusic("Assets/Musics/drone_sound.ogg", 15.f, sfFalse);
	}
	//
	LoadBossDrone();
}

void LoadBossDrone(void)
{
	sfTexture* bossDroneTexture = GetAsset("Assets/Boss/1/Boss_Drone_Placeholder.png");
	for (unsigned i = 0; i < MAX_BOSS_DRONE; i++)
	{
		bossDroneList[i].sprite = CreateSprite(bossDroneTexture, (sfVector2f) { 0, 0 }, 1.f, 39);
		SetSpriteOriginMiddle(bossDroneList[i].sprite);

		sfVector2f missilePosition = { 0 };
		bossDroneList[i].velocity = (sfVector2f){ 0 };
		bossDroneList[i].bossDroneState = BDRONE_IS_OUT;
		bossDroneList[i].bossDroneTimer = 0;
		//bossDroneList[i].music = CreateMusic("Assets/Musics/BOSSDRONESOUNDAPLACER.ogg", 0.5f, sfFalse);
	}
	LoadDangerZone();
}

void LoadDangerZone(void)
{
	sfTexture* dangerZone = GetAsset("Assets/Boss/1/Danger_Zone_Placeholder.png");
	for (unsigned i = 0; i < MAX_BOSS_DRONE; i++)
	{
		dangerZoneList[i].sprite = CreateSprite(dangerZone, (sfVector2f) { 0, 0 }, 1.f, 39);
		SetSpriteOriginMiddle(dangerZoneList[i].sprite);
		dangerZoneList[i].isShowed = sfFalse;
		sfVector2f missilePosition = { 0 };

	}
}

void UpdateProjectiles(sfVector2f _posAim, float _dt)
{
	sfFloatRect hitboxBullet = { 0 };
	sfVector2f reactionWall = { 0 };
	for (int i = (int)bulletCountAlly - 1; i >= 0; i--)
	{
		bulletListAlly[i].lifetime -= _dt;

		if (bulletListAlly[i].lifetime <= 0)
		{
			DeleteBulletAlly(i);
		}
		else
		{
			hitboxBullet = sfSprite_getGlobalBounds(bulletListAlly[i].sprite);
			reactionWall = Colision(hitboxBullet, AXIS_BOTH);

			if (reactionWall.x || reactionWall.y || ColisionBox(hitboxBullet, sfTrue, AXIS_BOTH).x || HitEnemy(9.f, hitboxBullet, LIGHT) || HitBoss(9.f, hitboxBullet, LIGHT) || ColisionElevatorButon(hitboxBullet) || HitBossDrone(sfTrue, hitboxBullet))
			{
				DeleteBulletAlly(i);
			}
			else
			{
				bulletListAlly[i].velocity.y += G * _dt;
				sfSprite_move(bulletListAlly[i].sprite, (sfVector2f) { bulletListAlly[i].velocity.x* _dt, bulletListAlly[i].velocity.y* _dt });
				sfSprite_setRotation(bulletListAlly[i].sprite, (float)RAD_DEG(atan2f(bulletListAlly[i].velocity.y, bulletListAlly[i].velocity.x)));
			}
		}

	}
	for (int i = (int)bulletCountEnemy - 1; i >= 0; i--)
	{
		bulletListEnemy[i].lifetime -= _dt;

		if (bulletListEnemy[i].lifetime <= 0)
		{
			DeleteBulletEnemy(i);
		}
		else
		{
			hitboxBullet = sfSprite_getGlobalBounds(bulletListEnemy[i].sprite);
			reactionWall = Colision(hitboxBullet, AXIS_BOTH);

			if (reactionWall.x || reactionWall.y || ColisionBox(hitboxBullet, sfTrue, AXIS_BOTH).x || ColisionWithPlayer(hitboxBullet, 1))
			{
				DeleteBulletEnemy(i);
			}
			else
			{
				bulletListEnemy[i].velocity.y += G * _dt;
				sfSprite_move(bulletListEnemy[i].sprite, (sfVector2f) { bulletListEnemy[i].velocity.x* _dt, bulletListEnemy[i].velocity.y* _dt });
				sfSprite_setRotation(bulletListEnemy[i].sprite, (float)RAD_DEG(atan2f(bulletListEnemy[i].velocity.y, bulletListEnemy[i].velocity.x)));
			}
		}

	}
	UpdateMisteal(_dt);
	UpdateBossDrone(_dt);
	UpdateSecondary(_posAim, _dt);
}

void UpdateSecondary(sfVector2f _posAim, float _dt)
{
	UpdateColdBreath(_dt);
	UpdateDrone(_posAim, _dt);

}

void UpdateMisteal(float _dt)
{
	sfFloatRect hitboxMisteal = { 0 };
	sfVector2f reactionWall = { 0 };
	for (int i = (int)mistealCount - 1; i >= 0; i--)
	{
		mistealList[i].timer += _dt;
		if (!mistealList[i].isSticked)
		{
			hitboxMisteal.left = sfSprite_getPosition(mistealList[i].sprite).x;
			hitboxMisteal.top = sfSprite_getPosition(mistealList[i].sprite).y;
			hitboxMisteal.width = 1;
			hitboxMisteal.height = 1;
			reactionWall = Colision(hitboxMisteal, AXIS_BOTH);
			if (mistealList[i].timer > MISTEAL_TIMER_OUTMAP_NOTSTICKED)
			{
				DeleteMisteal(i);
				continue;
			}


			if (reactionWall.x || reactionWall.y)
			{
				mistealList[i].isSticked = sfTrue;
			}
			if (!mistealList[i].isAlreadyHit)
			{
				if (HitEnemy(9.f, hitboxMisteal, HEAVY) || HitBoss(36.f, hitboxMisteal, HEAVY) || HitBossDrone(sfTrue, hitboxMisteal))
				{
					if (!mistealList[i].isAlreadyHit)
					{
						if (mistealList[i].velocity.x > 0)
						{
							mistealList[i].velocity.x = -20.f;
							mistealList[i].velocity.y = 280.f;
							mistealList[i].isAlreadyHit = sfTrue;
						}
						else if (mistealList[i].velocity.x < 0)
						{
							mistealList[i].velocity.x = 20.f;
							mistealList[i].velocity.y = 280.f;
							mistealList[i].isAlreadyHit = sfTrue;
						}

					}
				}
			}
			if (ColisionBox(hitboxMisteal, sfTrue, AXIS_BOTH).x)
			{
				if (mistealList[i].velocity.x > 0)
				{
					mistealList[i].velocity.x = 20.f;
					mistealList[i].velocity.y = 180.f;
				}
				else if (mistealList[i].velocity.x < 0)
				{
					mistealList[i].velocity.x = -20.f;
					mistealList[i].velocity.y = 180.f;
				}
			}
			else
			{
				mistealList[i].velocity.y += G * _dt;
				sfSprite_move(mistealList[i].sprite, (sfVector2f) { mistealList[i].velocity.x* _dt, mistealList[i].velocity.y* _dt });
				sfSprite_setRotation(mistealList[i].sprite, (float)RAD_DEG(atan2f(mistealList[i].velocity.y, mistealList[i].velocity.x)));
			}
		}
		else if (mistealList[i].isSticked)
		{
			hitboxMisteal = sfSprite_getGlobalBounds(mistealList[i].sprite);
			if (ColisionWithPlayer(hitboxMisteal, 0))
			{
				DeleteMisteal(i);
				continue;
			}
			else if (mistealList[i].timer > MISTEAL_TIMER_STICKED)
			{
				DeleteMisteal(i);
				continue;
			}
		}
	}
}

void UpdateBossDrone(float _dt)
{
	for (unsigned i = 0; i < MAX_BOSS_DRONE; i++)
	{
		if (bossDroneList[i].bossDroneState == BDRONE_IS_SPAWNING)
		{
			bossDroneList[i].bossDroneTimer -= _dt;
			if (bossDroneList[i].bossDroneTimer < 0)
			{
				bossDroneList[i].bossDroneState = BDRONE_IS_ASCENDING;
			}
		}
		else if (bossDroneList[i].bossDroneState == BDRONE_IS_ASCENDING)
		{
			if (sfSprite_getPosition(bossDroneList[i].sprite).y <= 620)
			{
				bossDroneList[i].bossDroneState = BDRONE_IS_STASIC_IN_SKY;
				AddDangerZone((sfVector2f) { bossDroneList[i].destination, groundLevel }, i);
				bossDroneList[i].bossDroneTimer = BOSS_DRONE_IN_SKY_TIME;
				bossDroneList[i].velocity.y = 0;
				sfSprite_rotate(bossDroneList[i].sprite, 180.f);
				sfSprite_setPosition(bossDroneList[i].sprite, (sfVector2f) { bossDroneList[i].destination, sfSprite_getPosition(bossDroneList[i].sprite).y });
			}
		}
		else if (bossDroneList[i].bossDroneState == BDRONE_IS_STASIC_IN_SKY)
		{
			bossDroneList[i].bossDroneTimer -= _dt;
			if (bossDroneList[i].bossDroneTimer < 0)
			{
				bossDroneList[i].velocity.y = BOSS_DRONE_SPEED;
				bossDroneList[i].bossDroneState = BDRONE_IS_FALLING;
			}
		}
		sfSprite_move(bossDroneList[i].sprite, (sfVector2f) { 0, bossDroneList[i].velocity.y* _dt });
		sfFloatRect hitboxBossDrone = sfSprite_getGlobalBounds(bossDroneList[i].sprite);
		sfVector2f reactionWall = Colision(hitboxBossDrone, AXIS_BOTH);
		if (bossDroneList[i].bossDroneState == BDRONE_IS_SPAWNING)
		{
			if (reactionWall.x || reactionWall.y || ColisionBox(hitboxBossDrone, sfTrue, AXIS_BOTH).x || ColisionWithPlayer(hitboxBossDrone, 1))
			{
				DeleteBossDrone(i);
				continue;
			}
		}
		else if (bossDroneList[i].bossDroneState == BDRONE_IS_ASCENDING || bossDroneList[i].bossDroneState == BDRONE_IS_FALLING)
		{
			if (reactionWall.x || reactionWall.y || ColisionBox(hitboxBossDrone, sfTrue, AXIS_BOTH).x || ColisionWithPlayer(hitboxBossDrone, 1) || HitBoss(50.f, hitboxBossDrone, HEAVY))
			{
				DeleteBossDrone(i);
				continue;
			}
		}
	}
}

unsigned GetBulletCount(void)
{
	return bulletCountAlly;
}

unsigned GetMistealCount(void)
{
	return mistealCount;
}

void AddBullet(sfVector2f _posShooter, sfVector2f _posTarget, ShooterType _shooterType)
{
	if (_shooterType.isAlly && bulletCountAlly >= BULLET_ALLY_MAX)
	{
		return;
	}
	else if (!_shooterType.isAlly && bulletCountEnemy >= BULLET_ENEMY_MAX)
	{
		return;
	}


	Bullet newBullet = { 0 };
	newBullet.sprite = CreateSprite(bulletTexture, (sfVector2f) { 0, 0 }, 1.f, 39.f);
	SetSpriteOriginMiddle(newBullet.sprite);


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

	if (_shooterType.isAlly == sfTrue)
	{
		newBullet.velocity.x = cosf(realAngleRad) * BULLET_SPEED_ALLY;
		newBullet.velocity.y = sinf(realAngleRad) * BULLET_SPEED_ALLY;
	}
	else if (_shooterType.isAlly == sfFalse)
	{
		newBullet.velocity.x = cosf(realAngleRad) * BULLET_SPEED_ENEMY;
		newBullet.velocity.y = sinf(realAngleRad) * BULLET_SPEED_ENEMY;
	}

	sfSprite_setPosition(newBullet.sprite, spawnPos);
	sfSprite_setRotation(newBullet.sprite, realAngleRad * (180.0f / (float)M_PI));

	newBullet.isAlive = sfTrue;
	newBullet.isAlly = _shooterType.isAlly;
	newBullet.bulletType = _shooterType.bulletType;
	newBullet.lifetime = BULLET_LIFETIME;
	if (_shooterType.isAlly)
	{
		bulletListAlly[bulletCountAlly] = newBullet;
		bulletCountAlly++;
	}
	else
	{
		bulletListEnemy[bulletCountEnemy] = newBullet;
		bulletCountEnemy++;
	}
}

void AddMisteal(sfVector2f _posShooter, sfVector2f _posTarget, ShooterType _shooterType)
{
	if (mistealCount >= MISTEAL_ALLY_MAX)
	{
		return;
	}

	Misteal newMisteal = { 0 };
	newMisteal.sprite = CreateSprite(mistealTexture, (sfVector2f) { 0, 0 }, 1.f, 71.f);
	sfSprite_setOrigin(newMisteal.sprite, (sfVector2f) { 12, 2 });

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
	newMisteal.velocity.x = cosf(realAngleRad) * MISTEAL_SPEED;
	newMisteal.velocity.y = sinf(realAngleRad) * MISTEAL_SPEED;

	sfSprite_setPosition(newMisteal.sprite, spawnPos);
	sfSprite_setRotation(newMisteal.sprite, realAngleRad * (180.0f / (float)M_PI));

	newMisteal.isSticked = sfFalse;
	newMisteal.isAlreadyHit = sfFalse;
	newMisteal.timer = 0.f;
	mistealList[mistealCount] = newMisteal;
	mistealCount++;
}

void AddBossDrone(sfVector2f _posShooter, float _destination)
{
	for (unsigned i = 0; i < MAX_BOSS_DRONE; i++)
	{
		if (bossDroneList[i].bossDroneState == BDRONE_IS_OUT)
		{
			//sfMusic_play(bossDroneList[i].music);
			sfSprite_setPosition(bossDroneList[i].sprite, _posShooter);
			bossDroneList[i].destination = _destination;
			bossDroneList[i].bossDroneTimer = BOSS_DRONE_SPAWN_TIME;
			bossDroneList[i].velocity.y = -(BOSS_DRONE_SPEED);
			bossDroneList[i].bossDroneState = BDRONE_IS_SPAWNING;
			return;
		}
	}
}

void AddDangerZone(sfVector2f _destination, unsigned _index)
{

	sfSprite_setPosition(dangerZoneList[_index].sprite, _destination);
	dangerZoneList[_index].isShowed = sfTrue;
	return;
}

sfBool HitBossDrone(sfBool _destroy, sfFloatRect _hitbox)
{
	sfFloatRect hitboxBossDrone = { 0 };
	sfFloatRect hitboxColision = { 0 };
	for (int i = 0; i < MAX_BOSS_DRONE; i++)
	{
		hitboxBossDrone = sfSprite_getGlobalBounds(bossDroneList[i].sprite);
		if (sfFloatRect_intersects(&_hitbox, &hitboxBossDrone, &hitboxColision))
		{
			DeleteBossDrone(i);
			return sfTrue;
		}
	}
	return sfFalse;
}

void AddDrone(sfVector2f _pos, sfBool _isRighted)
{
	for (unsigned i = 0; i < DRONE_MAX; i++)
	{
		if (droneList[i].isAlive == sfFalse)
		{
			sfMusic_play(droneList[i].ambientSound);
			droneList[i].isAlive = sfTrue;
			droneList[i].lifetime = 0;
			droneList[i].rotation = -90;
			sfSprite_setScale(droneList[i].sprite, (sfVector2f) { 1, 1 });
			if (_isRighted)
			{
				sfSprite_setPosition(droneList[i].sprite, (sfVector2f) { _pos.x - 12.f, _pos.y - WEAPON_ORIGIN });
			}
			else
			{
				sfSprite_setPosition(droneList[i].sprite, (sfVector2f) { _pos.x + 12.f, _pos.y - WEAPON_ORIGIN });
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
	coldBreath.velocity.x = cosf(realAngleRad) * SPEED_COLDBREATH;
	coldBreath.velocity.y = sinf(realAngleRad) * SPEED_COLDBREATH;

	sfSprite_setPosition(coldBreath.sprite, spawnPos);
	sfSprite_setRotation(coldBreath.sprite, realAngleRad * (180.0f / (float)M_PI));

	coldBreath.lifetime = 0.f;
	coldBreath.isAlive = sfTrue;
}


void UpdateDrone(sfVector2f _mousePos, float _dt)
{
	for (unsigned i = 0; i < DRONE_MAX; i++)
	{
		if (droneList[i].isAlive == sfTrue)
		{
			droneList[i].lifetime += _dt;
			if (droneList[i].lifetime <= SECONDARY_PROJECTILE_DURATION)
			{
				CheckCollisionMissilesList();
				MoveDrone(i, _mousePos, _dt);
			}
			else
			{
				droneList[i].isAlive = sfFalse;
				sfSprite_setPosition(droneList[i].sprite, (sfVector2f) { 0, 0 });
				sfMusic_stop(droneList[i].ambientSound);
				continue;
			}
			sfVector2f reaction = Colision(sfSprite_getGlobalBounds(droneList[i].sprite), AXIS_BOTH);
			sfVector2f reactionBox = ColisionBox(sfSprite_getGlobalBounds(droneList[i].sprite), sfTrue, AXIS_BOTH);
			reaction.x += reactionBox.x;
			reaction.y += reactionBox.y;
			if (reaction.x != 0 || reaction.y != 0 || HitEnemy(10.f, sfSprite_getGlobalBounds(droneList[i].sprite), HEAVY) || HitBoss(10.f, sfSprite_getGlobalBounds(droneList[i].sprite), HEAVY))
			{
				sfMusic_stop(droneList[i].ambientSound);
				droneList[i].isAlive = sfFalse;
				sfSprite_setPosition(droneList[i].sprite, (sfVector2f) { 0, 0 });
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
		hitboxColdBreath = sfSprite_getGlobalBounds(coldBreath.sprite);
		sfVector2f reactionWall = Colision(sfSprite_getGlobalBounds(coldBreath.sprite), AXIS_BOTH);
		if (reactionWall.x || reactionWall.y || ColisionBox(hitboxColdBreath, sfFalse, AXIS_BOTH).x || HitEnemy(FREEZE_DMG, hitboxColdBreath, FREEZE) || HitBoss(FREEZE_DMG, hitboxColdBreath, FREEZE))
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

void MoveDrone(unsigned _index, sfVector2f _mousePos, float _dt)
{
	if (!droneList[_index].isAlive) return;

	sfVector2f currentPos = sfSprite_getPosition(droneList[_index].sprite);

	float dx = _mousePos.x - currentPos.x;
	float dy = _mousePos.y - currentPos.y;

	float targetAngle = atan2f(dy, dx) * 180.0f / 3.14159;

	float angleDiff = targetAngle - droneList[_index].rotation;

	while (angleDiff > 180.0f)  angleDiff -= 360.0f;
	while (angleDiff < -180.0f) angleDiff += 360.0f;

	float maxRotationThisFrame = DEGRE_ROTATION * _dt;

	if (fabsf(angleDiff) <= maxRotationThisFrame)
	{
		droneList[_index].rotation = targetAngle;
	}
	else
	{
		if (angleDiff > 0)
			droneList[_index].rotation += maxRotationThisFrame;
		else
			droneList[_index].rotation -= maxRotationThisFrame;
	}

	float angleInRadians = droneList[_index].rotation * 3.14159 / 180.0f;

	sfVector2f movement;
	movement.x = cosf(angleInRadians) * SPEED_MISSILE * _dt;
	movement.y = sinf(angleInRadians) * SPEED_MISSILE * _dt;

	// 7. Application à la forme SFML
	sfSprite_setRotation(droneList[_index].sprite, droneList[_index].rotation);
	sfSprite_move(droneList[_index].sprite, movement);
}

void CheckCollisionMissilesList(void)
{
	for (unsigned i = 0; i < DRONE_MAX; i++)
	{
		if (!droneList[i].isAlive) continue;

		// On commence à j = i + 1 pour ne pas tester deux fois la même paire
		// et ne pas tester le missile contre lui-même
		for (unsigned j = i + 1; j < DRONE_MAX; j++)
		{
			if (!droneList[j].isAlive) continue;

			sfVector2f posA = sfSprite_getPosition(droneList[i].sprite);
			sfVector2f posB = sfSprite_getPosition(droneList[j].sprite);

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

				sfSprite_move(droneList[i].sprite, moveA);
				sfSprite_move(droneList[j].sprite, moveB);
			}
		}
	}
}

void DeleteBulletAlly(unsigned _index)
{
	DestroyVisualEntity(bulletListAlly[_index].sprite);
	SortBulletListAlly(_index);
	bulletCountAlly--;
}

void SortBulletListAlly(unsigned _index)
{
	for (unsigned i = _index; i < bulletCountAlly - 1; i++)
	{
		bulletListAlly[i] = bulletListAlly[i + 1];
	}
	bulletListAlly[bulletCountAlly - 1] = (Bullet){ 0 };
}

void DeleteBulletEnemy(unsigned _index)
{
	DestroyVisualEntity(bulletListEnemy[_index].sprite);
	SortBulletListEnemy(_index);
	bulletCountEnemy--;
}

void SortBulletListEnemy(unsigned _index)
{
	for (unsigned i = _index; i < bulletCountEnemy - 1; i++)
	{
		bulletListEnemy[i] = bulletListEnemy[i + 1];
	}
	bulletListEnemy[bulletCountEnemy - 1] = (Bullet){ 0 };
}

void DeleteMisteal(unsigned _index)
{
	DestroyVisualEntity(mistealList[_index].sprite);
	SortMistealList(_index);
	mistealCount--;
}

void SortMistealList(unsigned _index)
{
	for (unsigned i = _index; i < mistealCount - 1; i++)
	{
		mistealList[i] = mistealList[i + 1];
	}
	mistealList[mistealCount - 1] = (Misteal){ 0 };
}

void DeleteBossDrone(unsigned _index)
{
	sfSprite_setPosition(bossDroneList[_index].sprite, (sfVector2f) { 0 });
	bossDroneList[_index].velocity = (sfVector2f){ 0 };
	bossDroneList[_index].bossDroneState = BDRONE_IS_OUT;
	bossDroneList[_index].bossDroneTimer = 0;
	DeleteDangerZone(_index);
}

void DeleteDangerZone(unsigned _index)
{
	sfSprite_setPosition(dangerZoneList[_index].sprite, (sfVector2f) { 0 });
	dangerZoneList[_index].isShowed = sfFalse;
	return;
}

