#include "Bullet.h"
#include "Ennemy.h"
#include "Boss.h"
#include "Player.h"

sfTexture* bulletTexture;
sfTexture* mistealTexture;
Bullet bulletListAlly[BULLET_ALLY_MAX];
Bullet bulletListEnemy[BULLET_ENEMY_MAX];
Misteal mistealList[MISTEAL_ALLY_MAX];
unsigned mistealCount;
unsigned bulletCountAlly;
unsigned bulletCountEnemy;

void SortBulletListAlly(unsigned _index);
void SortBulletListEnemy(unsigned _index);
void SortMistealList(unsigned _index);

void LoadBullet(void)
{
	bulletTexture = GetAsset("Assets/Sprites/Bullet_Placeholder.png");
	mistealTexture = GetAsset("Assets/Sprites/Misteal_Ammo_Placeholder.png");
	mistealCount = 0;
	bulletCountAlly = 0;
	bulletCountEnemy = 0;
}

void UpdateBullet(float _dt)
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

			if (reactionWall.x || reactionWall.y || ColisionBox(hitboxBullet, sfTrue, AXIS_BOTH).x || HitEnemy(9.f, hitboxBullet) || HitBoss(9.f, hitboxBullet))
			{
				DeleteBulletAlly(i);
			}
			else
			{
				bulletListAlly[i].velocity.y += G * _dt;
				sfSprite_move(bulletListAlly[i].sprite, (sfVector2f) { bulletListAlly[i].velocity.x* _dt, bulletListAlly[i].velocity.y* _dt });
				sfSprite_setRotation(bulletListAlly[i].sprite, RAD_DEG(atan2f(bulletListAlly[i].velocity.y, bulletListAlly[i].velocity.x)));
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

			if (reactionWall.x || reactionWall.y || ColisionBox(hitboxBullet, sfTrue, AXIS_BOTH).x || ColisionWithPlayer(hitboxBullet))
			{
				DamagePlayer(1);
				DeleteBulletEnemy(i);
			}
			else
			{
				bulletListEnemy[i].velocity.y += G * _dt;
				sfSprite_move(bulletListEnemy[i].sprite, (sfVector2f) { bulletListEnemy[i].velocity.x* _dt, bulletListEnemy[i].velocity.y* _dt });
				sfSprite_setRotation(bulletListEnemy[i].sprite, RAD_DEG(atan2f(bulletListEnemy[i].velocity.y, bulletListEnemy[i].velocity.x)));
			}
		}

	}
}

void UpdateMisteal(float _dt)
{
	sfFloatRect hitboxMisteal = { 0 };
	sfVector2f reactionWall = { 0 };
	for (int i = (int)mistealCount - 1; i >= 0; i--)
	{
		
		if (!mistealList[i].isSticked)
		{
			hitboxMisteal.left = sfSprite_getPosition(mistealList[i].sprite).x;
			hitboxMisteal.top = sfSprite_getPosition(mistealList[i].sprite).y;
			hitboxMisteal.width = 1;
			hitboxMisteal.height = 1;
			reactionWall = Colision(hitboxMisteal, AXIS_BOTH);
			mistealList[i].timerOutMap += _dt;
			if (mistealList[i].timerOutMap > MISTEAL_TIMER_OUTMAP)
			{
				DeleteMisteal(i);
				continue;
			}
			

			if (reactionWall.x || reactionWall.y || HitBoss(36.f, hitboxMisteal))
			{
				mistealList[i].isSticked = sfTrue;
			}
			if (!mistealList[i].isAlreadyHit)
			{
				if (HitEnemy(9.f, hitboxMisteal))
				{
					if (!mistealList[i].isAlreadyHit)
					{
						if (mistealList[i].velocity.x > 0)
						{
							mistealList[i].velocity.x = -20.f;
							mistealList[i].velocity.y = 180.f;
							mistealList[i].isAlreadyHit = sfTrue;
						}
						else if (mistealList[i].velocity.x < 0)
						{
							mistealList[i].velocity.x = 20.f;
							mistealList[i].velocity.y = 180.f;
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
				sfSprite_setRotation(mistealList[i].sprite, RAD_DEG(atan2f(mistealList[i].velocity.y, mistealList[i].velocity.x)));
			}
		}
		else if (mistealList[i].isSticked)
		{
			hitboxMisteal = sfSprite_getGlobalBounds(mistealList[i].sprite);
			if (HitPlayer(hitboxMisteal, 0))
			{
				DeleteMisteal(i);
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
	newMisteal.sprite = CreateSprite(mistealTexture, (sfVector2f) { 0, 0 }, 1.f, 39.f);
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
	newMisteal.timerOutMap = 0.f;
	mistealList[mistealCount] = newMisteal;
	mistealCount++;
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