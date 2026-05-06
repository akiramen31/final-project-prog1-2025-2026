#include "Bullet.h"
#include "Ennemy.h"
#include "Boss.h"
#include "Player.h"
#include "Elevator.h"

sfTexture* bulletTexture;
sfTexture* mistealTexture;
Bullet bulletListAlly[BULLET_ALLY_MAX];
Bullet bulletListEnemy[BULLET_ENEMY_MAX];
Misteal mistealList[MISTEAL_ALLY_MAX];
BossDrone bossDroneList[MAX_BOSS_DRONE] = { 0 };
DangerZone dangerZoneList[MAX_BOSS_DRONE] = { 0 };
unsigned mistealCount;
unsigned bulletCountAlly;
unsigned bulletCountEnemy;

void SortBulletListAlly(unsigned _index);
void SortBulletListEnemy(unsigned _index);
void SortMistealList(unsigned _index);

void LoadProjectiles(void)
{
	bulletTexture = GetAsset("Assets/Sprites/Bullet_Placeholder.png");
	mistealTexture = GetAsset("Assets/Sprites/Misteal_Ammo_Placeholder.png");
	mistealCount = 0;
	bulletCountAlly = 0;
	bulletCountEnemy = 0;
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

		sfVector2f missilePosition = { 0 };
	}
}

void UpdateProjectiles(float _dt)
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

			if (reactionWall.x || reactionWall.y || ColisionBox(hitboxBullet, sfTrue, AXIS_BOTH).x || HitEnemy(9.f, hitboxBullet) || HitBoss(9.f, hitboxBullet) || ColisionElevatorButon(hitboxBullet))
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
				if (HitEnemy(9.f, hitboxMisteal) || HitBoss(36.f, hitboxMisteal))
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
		if (bossDroneList[i].bossDroneState == BDRONE_IS_ASCENDING)
		{
			if (sfSprite_getPosition(bossDroneList[i].sprite).y <= 620)
			{
				bossDroneList[i].bossDroneState = BDRONE_IS_STASIC_IN_SKY;
				bossDroneList[i].bossDroneTimer = BOSS_DRONE_IN_SKY_TIME;
				bossDroneList[i].velocity.y = 0;
				sfSprite_rotate(bossDroneList[i].sprite, 180.f);
				sfSprite_setPosition(bossDroneList[i].sprite, (sfVector2f) { bossDroneList[i].destination, sfSprite_getPosition(bossDroneList[i].sprite).y });
			}
		}
		if (bossDroneList[i].bossDroneState == BDRONE_IS_STASIC_IN_SKY)
		{
			bossDroneList[i].bossDroneTimer -= _dt;
			if (bossDroneList[i].bossDroneTimer < 0)
			{
				bossDroneList[i].velocity.y = BOSS_DRONE_SPEED;
				bossDroneList[i].bossDroneState = BDRONE_IS_FALLING;
			}
		}
		sfSprite_move(bossDroneList[i].sprite, (sfVector2f) { 0, bossDroneList[i].velocity.y* _dt });
		//printf("je suis %d et ma valeur est : %d mon temp est de : %f\n", (i + 1), bossDroneList[i].bossDroneState, bossDroneList[i].bossDroneTimer);
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
			// CHANGER QUAND LE CODE POUR IS SPAWNING SERA GOOD
			bossDroneList[i].bossDroneState = BDRONE_IS_ASCENDING;
			bossDroneList[i].bossDroneTimer = BOSS_DRONE_SPAWN_TIME;
			bossDroneList[i].destination = _destination;
			sfSprite_setPosition(bossDroneList[i].sprite, _posShooter);
			bossDroneList[i].velocity.y = -(BOSS_DRONE_SPEED);
			return;
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