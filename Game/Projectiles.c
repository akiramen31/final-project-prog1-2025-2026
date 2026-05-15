#include "Projectiles.h"
#include "Enemy.h"
#include "Boss.h"
#include "Player.h"
#include "Elevator.h"

sfTexture* bulletTexture;
sfTexture* bulletBossTexture;
sfTexture* mistealTexture;
sfTexture* droneTexture;
sfTexture* grenadeTexture;
sfTexture* explosionTexture;
Bullet bulletListAlly[BULLET_ALLY_MAX];
Bullet bulletListEnemy[BULLET_ENEMY_MAX];
Misteal mistealList[MISTEAL_ALLY_MAX];
Drone droneList[PLAYER_DRONE_MAX];
ColdBreath coldBreath;
BossMissile bossMissileList[MAX_BOSS_MISSILE];
DangerZone dangerZoneList[MAX_BOSS_MISSILE];
Explosion* explosionList = NULL;
UnhiddingExplosion unhiddingExplosion;
UnhiddingBomb unhiddingBomb;
Grenade* grenadeList = NULL;
Animation explosionAnimation;

unsigned mistealCount;
unsigned bulletCountAlly;
unsigned bulletCountEnemy;
unsigned explosionCount;
unsigned grenadeCount;
float groundLevel;

void SortBulletListAlly(unsigned _index);
void SortBulletListEnemy(unsigned _index);
void SortMistealList(unsigned _index);

void LoadProjectiles(float _groundlevel)
{
	bulletTexture = GetAsset("Assets/Sprites/bullet.png");
	bulletBossTexture = GetAsset("Assets/Sprites/bullet_boss.png");
	mistealTexture = GetAsset("Assets/Sprites/mi-steel_ammo.png");
	mistealCount = 0;
	bulletCountAlly = 0;
	bulletCountEnemy = 0;
	groundLevel = _groundlevel;
	LoadGrenade();
	LoadSecondary();
	LoadBossProjectiles();
	LoadExplosion();
	LoadUnhiddingExplosion();
}

void LoadGrenade(void)
{
	grenadeTexture = GetAsset("Assets/Sprites/Grenade_Boss.png");
	grenadeList = Calloc(1, sizeof(Grenade));
	grenadeCount = 0;
}

void LoadSecondary(void)
{
	//COLDBREATH PART
	droneTexture = GetAsset("Assets/Sprites/drone.png");

	coldBreath.sprite = CreateSprite(GetAsset("Assets/Sprites/cold_breath.png"), (sfVector2f) { 0, 0 }, 1.f, 39);
	SetSpriteOriginMiddle(coldBreath.sprite);
	coldBreath.lifetime = 0.f;
	coldBreath.isAlive = sfFalse;
	//missileList[i].music = CreateMusic("Assets/Musics/FREEZESOUNDAAJOUTER.ogg", 5.f, sfFalse);

	// DRONE PART

	for (unsigned i = 0; i < PLAYER_DRONE_MAX; i++)
	{
		droneList[i].sprite = CreateSprite(droneTexture, (sfVector2f) { 0 }, 0.f, 39);
		SetSpriteOriginMiddle(droneList[i].sprite);
		droneList[i].lifetime = 0;
		droneList[i].rotation = 0;
		droneList[i].isAlive = sfFalse;
		droneList[i].ambientSound = CreateMusic("Assets/Musics/drone_sound.ogg", 15.f, sfFalse);
	}
}

void LoadBossProjectiles(void)
{
	unhiddingBomb.sprite = CreateSprite(GetAsset("Assets/Sprites/UnlocatingBomb_boss"), (sfVector2f) { 0.f, 0.f }, 1.f, 16.f);
	SetSpriteOriginFoot(unhiddingBomb.sprite);
	unhiddingBomb.droptiming = 2.f;
	unhiddingBomb.lifetime = 2.f;

	sfTexture* bossDroneTexture = GetAsset("Assets/Sprites/tank_ballistic_missile.png");
	for (unsigned i = 0; i < MAX_BOSS_MISSILE; i++)
	{
		bossMissileList[i].sprite = CreateSprite(bossDroneTexture, (sfVector2f) { 0, 0 }, 1.f, 39);
		SetSpriteOriginMiddle(bossMissileList[i].sprite);

		sfVector2f missilePosition = { 0 };
		bossMissileList[i].velocity = (sfVector2f){ 0 };
		bossMissileList[i].bossMissileState = BOSS_MISSILE_OUT;
		bossMissileList[i].bossMissileTimer = 0;
		//bossDroneList[i].music = CreateMusic("Assets/Musics/BOSSDRONESOUNDAPLACER.ogg", 0.5f, sfFalse);
	}
	LoadDangerZone();
}

void LoadDangerZone(void)
{
	sfTexture* dangerZone = GetAsset("Assets/Sprites/danger_zone.png");
	for (unsigned i = 0; i < MAX_BOSS_MISSILE; i++)
	{
		dangerZoneList[i].sprite = CreateSprite(dangerZone, (sfVector2f) { 0 }, 1.f, 39);
		SetSpriteOriginMiddle(dangerZoneList[i].sprite);
		dangerZoneList[i].isShown = sfFalse;
	}
}

void LoadExplosion(void)
{
	explosionTexture = GetAsset("Assets/Sprites/explosion.png");
	explosionAnimation.rectActualy = (sfIntRect){ 0,0,16,16 };
	explosionAnimation.frameCount = 4;
	explosionAnimation.frameDuration = EXPLOSION_FRAME_DURATION;
	explosionAnimation.isLooping = sfFalse;
	explosionAnimation.timeActualy = 0;
	explosionList = Calloc(1, sizeof(Explosion));
	explosionCount = 0;
}

void LoadUnhiddingExplosion(void)
{
	unhiddingExplosion.sprite = CreateSprite(("Assets/Sprites/unhidding_explosion.png"), (sfVector2f) { 0, 0 }, 1.f, 0.01f);
	sfSprite_setTextureRect(unhiddingExplosion.sprite, (sfIntRect) {0, 0, 512, 16});
	SetSpriteOriginFoot(unhiddingExplosion.sprite);
	unhiddingExplosion.isOn;
	unhiddingExplosion.unhiddingExplosionAnim.rectActualy = (sfIntRect){ 0,0,512,16 };
	unhiddingExplosion.unhiddingExplosionAnim.frameCount = 4;
	unhiddingExplosion.unhiddingExplosionAnim.frameDuration = EXPLOSION_FRAME_DURATION;
	unhiddingExplosion.unhiddingExplosionAnim.isLooping = sfFalse;
	unhiddingExplosion.unhiddingExplosionAnim.timeActualy = 0;
}

void UpdateProjectiles(sfVector2f _posAim, float _dt)
{
	sfFloatRect hitboxBullet = { 0 };
	sfVector2f reactionWall = { 0 };
	sfVector2f reactionPassThrough = { 0 };
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

			if (reactionWall.x || reactionWall.y || ColisionBox(hitboxBullet, sfTrue, AXIS_BOTH).x || HitEnemy(9.f, hitboxBullet, LIGHT) || HitBoss(9.f, hitboxBullet, LIGHT) || ColisionElevatorButon(hitboxBullet) || HitBossMissile(hitboxBullet) || HitGrenade(hitboxBullet))
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
			reactionPassThrough = CollisionPassThrough(hitboxBullet);
			reactionWall = Colision(hitboxBullet, AXIS_BOTH);
			reactionWall.x += reactionPassThrough.x;
			reactionWall.y += reactionPassThrough.y;

			if (reactionWall.x || reactionWall.y || ColisionBox(hitboxBullet, sfTrue, AXIS_BOTH).x || ColisionWithPlayer(hitboxBullet, sfTrue))
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
	UpdateBossMissile(_dt);
	UpdateSecondary(_posAim, _dt);
	UpdateGrenade(_dt);
	UpdateExplosion(_dt);
	UpdateUnhiddingExplosion(_dt);
}

void UpdateSecondary(sfVector2f _posAim, float _dt)
{
	switch (GetSecondaryType())
	{
	case COLDBREATH:
		UpdateColdBreath(_dt);
		break;
	case DRONE:
		UpdateDrone(_posAim, _dt);
		break;
	}
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
			if (mistealList[i].timer > MISTEAL_TIMER_GENERAL)
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
				if (HitEnemy(9.f, hitboxMisteal, HEAVY) || HitBoss(36.f, hitboxMisteal, HEAVY) || HitBossMissile(hitboxMisteal) || HitGrenade(hitboxMisteal))
				{
					if (!mistealList[i].isAlreadyHit)
					{
						if (mistealList[i].velocity.x > 0)
						{
							mistealList[i].velocity.x = -20.f;
							mistealList[i].velocity.y = 280.f;
							mistealList[i].isAlreadyHit = sfTrue;
							mistealList[i].timer = MISTEAL_TIMER_IN_WALL;
						}
						else if (mistealList[i].velocity.x < 0)
						{
							mistealList[i].velocity.x = 20.f;
							mistealList[i].velocity.y = 280.f;
							mistealList[i].isAlreadyHit = sfTrue;
							mistealList[i].timer = MISTEAL_TIMER_IN_WALL;
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
			if (ColisionWithPlayer(hitboxMisteal, sfFalse))
			{
				DeleteMisteal(i);
				continue;
			}
			else if (mistealList[i].timer > MISTEAL_TIMER_IN_WALL)
			{
				DeleteMisteal(i);
				continue;
			}
		}
	}
}

void UpdateBossMissile(float _dt)
{
	for (unsigned i = 0; i < MAX_BOSS_MISSILE; i++)
	{
		if (bossMissileList[i].bossMissileState == BOSS_MISSILE_SPAWNING)
		{
			bossMissileList[i].bossMissileTimer -= _dt;
			if (bossMissileList[i].bossMissileTimer < 0)
			{
				bossMissileList[i].bossMissileState = BOSS_MISSILE_ASCENDING;
			}
		}
		else if (bossMissileList[i].bossMissileState == BOSS_MISSILE_ASCENDING)
		{
			if (sfSprite_getPosition(bossMissileList[i].sprite).y <= 620)
			{
				bossMissileList[i].bossMissileState = BOSS_MISSILE_STASIC;
				AddDangerZone((sfVector2f) { bossMissileList[i].destination, groundLevel }, i);
				bossMissileList[i].bossMissileTimer = BOSS_MISSILE_AIR_TIME;
				bossMissileList[i].velocity.y = 0;
				sfSprite_rotate(bossMissileList[i].sprite, 180.f);
				sfSprite_setPosition(bossMissileList[i].sprite, (sfVector2f) { bossMissileList[i].destination, sfSprite_getPosition(bossMissileList[i].sprite).y });
			}
		}
		else if (bossMissileList[i].bossMissileState == BOSS_MISSILE_STASIC)
		{
			bossMissileList[i].bossMissileTimer -= _dt;
			if (bossMissileList[i].bossMissileTimer < 0)
			{
				bossMissileList[i].velocity.y = BOSS_MISSILE_SPEED;
				bossMissileList[i].bossMissileState = BOSS_MISSILE_FALLING;
			}
		}
		sfSprite_move(bossMissileList[i].sprite, (sfVector2f) { 0, bossMissileList[i].velocity.y* _dt });
		sfFloatRect hitboxBossDrone = sfSprite_getGlobalBounds(bossMissileList[i].sprite);
		sfVector2f reactionWall = Colision(hitboxBossDrone, AXIS_BOTH);
		if (bossMissileList[i].bossMissileState == BOSS_MISSILE_SPAWNING)
		{
			if (reactionWall.x || reactionWall.y || ColisionBox(hitboxBossDrone, sfTrue, AXIS_BOTH).x || ColisionWithPlayer(hitboxBossDrone, sfTrue))
			{
				SpawnExplosion(sfSprite_getPosition(bossMissileList[i].sprite), sfFalse, BOSS_EXPLOSION_RANGE);
				DeleteBossMissile(i);
				continue;
			}
		}
		else if (bossMissileList[i].bossMissileState == BOSS_MISSILE_ASCENDING || bossMissileList[i].bossMissileState == BOSS_MISSILE_FALLING)
		{
			if (reactionWall.x || reactionWall.y || ColisionBox(hitboxBossDrone, sfTrue, AXIS_BOTH).x || ColisionWithPlayer(hitboxBossDrone, sfTrue) || HitBoss(0, hitboxBossDrone, NOATTACK))
			{
				SpawnExplosion(sfSprite_getPosition(bossMissileList[i].sprite), sfFalse, BOSS_EXPLOSION_RANGE);
				DeleteBossMissile(i);
				continue;
			}
		}
	}
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
	if (_shooterType.isAlly == sfTrue)
	{
		newBullet.sprite = CreateSprite(bulletTexture, (sfVector2f) { 0, 0 }, 1.f, 39.f);
	}
	else
	{
		newBullet.sprite = CreateSprite(bulletBossTexture, (sfVector2f) { 0, 0 }, 2.f, 0.01f);
	}
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
	else if (_shooterType.isBoss2)
	{
		newBullet.velocity.x = cosf(realAngleRad) * BULLET_SPEED_ENEMY * 2.f;
		newBullet.velocity.y = sinf(realAngleRad) * BULLET_SPEED_ENEMY * 2.f;
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

void SpawnBossMissile(sfVector2f _posShooter, float _destination)
{
	for (unsigned i = 0; i < MAX_BOSS_MISSILE; i++)
	{
		if (bossMissileList[i].bossMissileState == BOSS_MISSILE_OUT)
		{
			sfSprite_setPosition(bossMissileList[i].sprite, _posShooter);
			bossMissileList[i].destination = _destination;
			bossMissileList[i].bossMissileTimer = BOSS_MISSILE_SPAWN_TIME;
			bossMissileList[i].velocity.y = -BOSS_MISSILE_SPEED;
			bossMissileList[i].bossMissileState = BOSS_MISSILE_SPAWNING;
			return;
		}
	}
}

void AddDangerZone(sfVector2f _destination, unsigned _index)
{
	sfSprite_setPosition(dangerZoneList[_index].sprite, _destination);
	dangerZoneList[_index].isShown = sfTrue;
	return;
}

sfBool HitBossMissile(sfFloatRect _hitbox)
{
	sfFloatRect hitboxMissile;
	for (int i = 0; i < MAX_BOSS_MISSILE; i++)
	{
		hitboxMissile = sfSprite_getGlobalBounds(bossMissileList[i].sprite);
		if (sfFloatRect_intersects(&_hitbox, &hitboxMissile, NULL))
		{
			SpawnExplosion(sfSprite_getPosition(bossMissileList[i].sprite), sfFalse, BOSS_EXPLOSION_RANGE);
			DeleteBossMissile(i);
			return sfTrue;
		}
	}
	return sfFalse;
}

sfBool HitGrenade(sfFloatRect _hitbox)
{
	if (grenadeCount > 0)
	{
		sfFloatRect hitboxGrenade;
		for (int i = grenadeCount - 1; i >= 0; i--)
		{
			hitboxGrenade = sfSprite_getGlobalBounds(grenadeList[i].sprite);
			if (sfFloatRect_intersects(&_hitbox, &hitboxGrenade, NULL))
			{
				DeleteGrenade(i);
				return sfTrue;
			}
		}
		return sfFalse;
	}
	return sfFalse;
}

void AddDrone(sfVector2f _pos, sfBool _isRighted)
{
	for (unsigned i = 0; i < PLAYER_DRONE_MAX; i++)
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

void SpawnGrenade(sfVector2f _spawnZone, float _sizeGrenade, float _rangeGrenade)
{
	Grenade* temp = Realloc(grenadeList, (size_t)(grenadeCount + 1) * sizeof(Grenade));
	if (!temp)
	{
		return;
	}
	grenadeList = temp;
	temp = NULL;
	Grenade newGrenade = { 0 };
	
	newGrenade.sprite = CreateSprite(grenadeTexture, _spawnZone, _sizeGrenade, 16.f);
	newGrenade.lifetime = GRENADE_LIFETIME;
	newGrenade.velocity.y = GRENADE_FALL_SPEED;
	newGrenade.sizeGrenade = _sizeGrenade;
	newGrenade.rangeGrenade = _rangeGrenade;
	grenadeList[grenadeCount] = newGrenade;
	grenadeCount++;
}

void SpawnUnhiddingBomb(sfVector2f _spawnZone)
{
	sfSprite_setPosition(unhiddingBomb.sprite, _spawnZone);
}

void SpawnExplosion(sfVector2f _explosionZone, sfBool _isAlly, float _range)
{
	Explosion* temp = Realloc(explosionList, (size_t)(explosionCount + 1) * sizeof(Explosion));
	if (!temp)
	{
		return;
	}
	explosionList = temp;
	temp = NULL;
	Explosion newExplosion = { 0 };

	newExplosion.sprite = CreateSprite(explosionTexture, _explosionZone, _range, 16.f);
	sfSprite_setTextureRect(newExplosion.sprite, explosionAnimation.rectActualy);
	SetSpriteOriginMiddle(newExplosion.sprite);
	explosionAnimation.timeActualy = 0.f;
	newExplosion.explosionAnim = explosionAnimation;
	newExplosion.isAlly = _isAlly;
	newExplosion.range = _range;
	explosionList[explosionCount] = newExplosion;
	TestCollisionExplosionList(explosionCount, _range);
	explosionCount++;
}

void SpawnUnhiddingExplosion(sfVector2f _explosionSpawn)
{
	if (unhiddingExplosion.isOn)
	{
		return;
	}
	sfSprite_setPosition(unhiddingExplosion.sprite, _explosionSpawn);
	unhiddingExplosion.isOn = sfTrue;
	TestCollisionUnhiddingExplosion();
}

void SortExplosionList(unsigned _index)
{
	explosionCount--;
	explosionList[_index] = explosionList[explosionCount];
	if (explosionCount)
	{
		explosionList = Realloc(explosionList, explosionCount * sizeof(Explosion));
	}
}

void SortGrenadeList(unsigned _index)
{
	grenadeCount--;
	grenadeList[_index] = grenadeList[grenadeCount];
	if (grenadeCount)
	{
		grenadeList = Realloc(grenadeList, grenadeCount * sizeof(Grenade));
	}
}

void UpdateDrone(sfVector2f _mousePos, float _dt)
{
	for (unsigned i = 0; i < PLAYER_DRONE_MAX; i++)
	{
		if (droneList[i].isAlive == sfTrue)
		{
			droneList[i].lifetime += _dt;
			if (droneList[i].lifetime <= SECONDARY_PROJECTILE_DURATION)
			{
				CheckCollisionDronesList();
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
			sfFloatRect hitboxDrone = sfSprite_getGlobalBounds(droneList[i].sprite);
			reaction.x += reactionBox.x;
			reaction.y += reactionBox.y;
			if (reaction.x != 0 || reaction.y != 0 || HitEnemy(10.f, hitboxDrone, HEAVY) || HitBoss(10.f, hitboxDrone, HEAVY) || HitGrenade(hitboxDrone))
			{
				SpawnExplosion(sfSprite_getPosition(droneList[i].sprite), sfTrue, 3.f);
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

void UpdateExplosion(float _dt)
{
	for (int i = explosionCount - 1; i >= 0; i--)
	{
		if (UpdateAnimationAndGiveIfStop(explosionList[i].sprite, &explosionList[i].explosionAnim, _dt))
		{
			DestroyVisualEntity(explosionList[i].sprite);
			SortExplosionList(i);
		}
	}
}

void UpdateUnhiddingExplosion(float _dt)
{
	if (UpdateAnimationAndGiveIfStop(unhiddingExplosion.sprite, &unhiddingExplosion.unhiddingExplosionAnim, _dt))
	{
		unhiddingExplosion.isOn = sfFalse;
		sfSprite_setPosition(unhiddingExplosion.sprite, (sfVector2f) { 0, 0 });
	}
}

void UpdateGrenade(float _dt)
{
	sfFloatRect hitboxGrenade = { 0 };
	for (int i = grenadeCount - 1; i >= 0; i--)
	{
		if (!grenadeList[i].sprite)
		{
			continue;
		}
		hitboxGrenade = sfSprite_getGlobalBounds(grenadeList[i].sprite);
		sfVector2f reactionPassThrough = CollisionPassThrough(hitboxGrenade);
		sfVector2f reactionWall = Colision(hitboxGrenade, AXIS_BOTH);
		sfVector2f reactionBox = ColisionBox(hitboxGrenade, sfFalse, AXIS_BOTH);
		reactionWall.y += reactionPassThrough.y + reactionBox.y;
		if (reactionWall.y)
		{
			sfSprite_move(grenadeList[i].sprite, reactionWall);
			grenadeList[i].velocity.y = 0.f;
		}
		grenadeList[i].lifetime -= _dt;
		if (grenadeList[i].lifetime < 0.f)
		{
			DeleteGrenade(i);
			continue;
		}
		sfSprite_move(grenadeList[i].sprite, (sfVector2f) { grenadeList[i].velocity.x* _dt, grenadeList[i].velocity.y* _dt });
	}
}

void UpdateUnhiddingBomb(float _dt)
{
	if (!(sfSprite_getPosition(unhiddingBomb.sprite).x < 3.f) && !(sfSprite_getPosition(unhiddingBomb.sprite).y < 3.f))
	{
		if (unhiddingBomb.isGrounded)
		{
			unhiddingBomb.lifetime -= _dt;
			if (unhiddingBomb.lifetime < 0.f)
			{
				SpawnUnhiddingExplosion(sfSprite_getPosition(unhiddingBomb.sprite));
				sfSprite_setPosition(unhiddingBomb.sprite, (sfVector2f) { 0.f, 0.f });
				unhiddingBomb.isGrounded = sfFalse;
			}
		}
	}
}

void DeleteGrenade(unsigned _index)
{
	SpawnExplosion(sfSprite_getPosition(grenadeList[_index].sprite), sfFalse, grenadeList[_index].rangeGrenade);
	DestroyVisualEntity(grenadeList[_index].sprite);
	SortGrenadeList(_index);
}

void MoveDrone(unsigned _index, sfVector2f _mousePos, float _dt)
{
	if (!droneList[_index].isAlive) return;

	sfVector2f currentPos = sfSprite_getPosition(droneList[_index].sprite);

	float dx = _mousePos.x - currentPos.x;
	float dy = _mousePos.y - currentPos.y;

	float targetAngle = atan2f(dy, dx) * 180.f / 3.14159f;

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

	float angleInRadians = droneList[_index].rotation * 3.14159f / 180.0f;

	sfVector2f movement;
	movement.x = cosf(angleInRadians) * SPEED_PLAYER_DRONE * _dt;
	movement.y = sinf(angleInRadians) * SPEED_PLAYER_DRONE * _dt;

	// 7. Application à la forme SFML
	sfSprite_setRotation(droneList[_index].sprite, droneList[_index].rotation);
	sfSprite_move(droneList[_index].sprite, movement);
}

void CheckCollisionDronesList(void)
{
	for (unsigned i = 0; i < PLAYER_DRONE_MAX; i++)
	{
		if (!droneList[i].isAlive) continue;

		// On commence à j = i + 1 pour ne pas tester deux fois la même paire
		// et ne pas tester le missile contre lui-même
		for (unsigned j = i + 1; j < PLAYER_DRONE_MAX; j++)
		{
			if (!droneList[j].isAlive) continue;

			sfVector2f posA = sfSprite_getPosition(droneList[i].sprite);
			sfVector2f posB = sfSprite_getPosition(droneList[j].sprite);

			float dx = posB.x - posA.x;
			float dy = posB.y - posA.y;
			float distance = sqrtf(dx * dx + dy * dy);
			float minDistance = PLAYER_DRONE_WEIGHT * 2.0f; // Somme des deux rayons

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

void TestCollisionExplosionList(unsigned _index, float _range)
{
	sfFloatRect hitboxExplosion = { 0 };
	hitboxExplosion.left = sfSprite_getPosition(explosionList[_index].sprite).x;
	hitboxExplosion.top = sfSprite_getPosition(explosionList[_index].sprite).y;
	hitboxExplosion.width = EXPLOSION_BASIC_RANGE * _range;
	hitboxExplosion.height = EXPLOSION_BASIC_RANGE * _range;
	sfVector2f reaction = ColisionBox(hitboxExplosion, sfTrue, AXIS_BOTH);
	if (reaction.x != 0 || reaction.y != 0)
	{
		if (explosionList[_index].isAlly)
		{
			HitEnemy(PLAYER_DRONE_DAMAGE, hitboxExplosion, HEAVY);
		}
		else
		{
			ColisionWithPlayer(hitboxExplosion, sfTrue);
		}
		HitBoss(BOSS_SELF_DAMAGE, hitboxExplosion, HEAVY);
	}
}

void TestCollisionUnhiddingExplosion(void)
{
	sfFloatRect hitboxUnhidding = { 0 };
	hitboxUnhidding = sfSprite_getGlobalBounds(unhiddingExplosion.sprite);
	ColisionWithPlayer(hitboxUnhidding, sfTrue);
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

void DeleteBossMissile(unsigned _index)
{
	if (_index >= MAX_BOSS_MISSILE)
	{
		return;
	}
	sfSprite_setPosition(bossMissileList[_index].sprite, (sfVector2f) { 0 });
	bossMissileList[_index].velocity = (sfVector2f){ 0 };
	bossMissileList[_index].bossMissileState = BOSS_MISSILE_OUT;
	bossMissileList[_index].bossMissileTimer = 0;
	DeleteDangerZone(_index);
}

void DeleteDangerZone(unsigned _index)
{
	sfSprite_setPosition(dangerZoneList[_index].sprite, (sfVector2f) { 0 });
	dangerZoneList[_index].isShown = sfFalse;
	return;
}


unsigned GetBulletCount(void)
{
	return bulletCountAlly;
}

unsigned GetMistealCount(void)
{
	return mistealCount;
}

unsigned GetExplosionCount(void)
{
	return explosionCount;
}