#include "Bullet.h"
#include "Ennemy.h"
#include "Boss.h"

sfTexture* bulletTexture;
Bullet bulletListAlly[BULLET_ALLY_MAX];
Bullet bulletListEnemy[BULLET_ENEMY_MAX];
unsigned bulletCountAlly;
unsigned bulletCountEnemy;

void SortBulletListAlly(unsigned _index);
void SortBulletListEnemy(unsigned _index);
sfBool HitPlayer(sfFloatRect _rect);

void LoadBullet(void)
{
	bulletTexture = GetAsset("Assets/Sprites/Bullet_Placeholder.png");
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
			if (reactionWall.x || reactionWall.y || ColisionBox(hitboxBullet, sfTrue, AXIS_BOTH).x || HitPlayer(hitboxBullet))
			{
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

unsigned GetBulletCount(void)
{
	return bulletCountAlly;
}

void AddBullet(sfVector2f _posShooter, sfVector2f _posTarget, ShooterType _shooterType)
{
	if (_shooterType.isAlly && bulletCountAlly >= BULLET_ALLY_MAX)
	{
		return;
	}
	else if (!_shooterType.isAlly && bulletCountEnemy >= BULLET_ALLY_MAX)
	{
		return;
	}


	Bullet newBullet = { 0 };
	newBullet.sprite = CreateSprite(bulletTexture, (sfVector2f) { 0, 0 }, 1.f, 39.f);
	SetSpriteOriginMiddel(newBullet.sprite);


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

void SortBulletListAlly(unsigned _index)
{
	for (unsigned i = _index; i < bulletCountAlly - 1; i++)
	{
		bulletListAlly[i] = bulletListAlly[i + 1];
	}
	bulletListAlly[bulletCountAlly - 1] = (Bullet){ 0 };
}

void SortBulletListEnemy(unsigned _index)
{
	for (unsigned i = _index; i < bulletCountEnemy - 1; i++)
	{
		bulletListEnemy[i] = bulletListEnemy[i + 1];
	}
	bulletListEnemy[bulletCountEnemy - 1] = (Bullet){ 0 };
}

void DeleteBulletAlly(unsigned _index)
{
	DestroyVisualEntity(bulletListAlly[_index].sprite);
	SortBulletListAlly(_index);
	bulletCountAlly--;
}

void DeleteBulletEnemy(unsigned _index)
{
	DestroyVisualEntity(bulletListEnemy[_index].sprite);
	SortBulletListEnemy(_index);
	bulletCountEnemy--;
}

sfBool HitPlayer(sfFloatRect _rect)
{
	sfFloatRect playerRect = GetPlayerRect();

	if (sfFloatRect_intersects(&playerRect, &_rect, NULL))
	{
			DamagePlayer(1);
			return sfTrue;
	}
	return sfFalse;
}