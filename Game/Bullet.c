#include "Bullet.h"

sfTexture* bulletTexture;
Bullet bulletList[BULLET_MAX];
unsigned bulletCount;

void SortBulletList(unsigned _index);

void LoadBullet(void)
{
	bulletTexture = GetAsset("Assets/Sprites/Bullet_Placeholder.png");
	bulletCount = 0;
}

void UpdateBullet(float _dt)
{
	for (int i = (int)bulletCount - 1; i >= 0; i--)
	{
		bulletList[i].lifetime -= _dt;

		if (bulletList[i].lifetime <= 0)
		{
			DestroyVisualEntity(bulletList[i].sprite);
			SortBulletList(i);
			bulletCount--;
			continue;
		}

		bulletList[i].velocity.y += G * _dt;
		sfSprite_move(bulletList[i].sprite, (sfVector2f) { bulletList[i].velocity.x* _dt, bulletList[i].velocity.y* _dt });

		float angle = atan2f(bulletList[i].velocity.y, bulletList[i].velocity.x) * (180.0f / (float)M_PI);
		sfSprite_setRotation(bulletList[i].sprite, angle);
	}
}


unsigned GetBulletCount(void)
{
	return bulletCount;
}
// a changer quand je ferais une struct weapon
void AddBullet(sfVector2f _posPlayer, sfVector2f _posAim, int _weaponPos, sfBool _isRighted)
{
	if (bulletCount >= BULLET_MAX)
	{
		return;
	}

	Bullet newBullet = { 0 };

	newBullet.sprite = CreateSprite(bulletTexture, (sfVector2f) { 0, 0 }, 1.f, 39);
	SetSpriteOriginMiddel(newBullet.sprite);

	sfVector2f pivotPos = { _posPlayer.x, _posPlayer.y - _weaponPos };

	float dx = _posAim.x - _posPlayer.x;
	float dy = _posAim.y - _posPlayer.y;

	float angleRad = atan2f(dy, dx);
	float angleDeg = angleRad * (180.0f / (float)M_PI);
	float sideOffset;

	if (_isRighted)
	{
		sideOffset = 10.0f;
	}
	else
	{
		sideOffset = -10.0f;
	}
	float gunLength = 6.0f;  
	
	sfVector2f spawnPos;
	spawnPos.x = pivotPos.x + cosf(angleRad) * gunLength - sinf(angleRad) * sideOffset;
	spawnPos.y = pivotPos.y + sinf(angleRad) * gunLength + cosf(angleRad) * sideOffset;

	dx = _posAim.x - spawnPos.x;
	dy = _posAim.y - spawnPos.y;
	angleRad = atan2f(dy, dx);
	
	newBullet.velocity.x = cosf(angleRad) * BULLET_SPEED;
	newBullet.velocity.y = sinf(angleRad) * BULLET_SPEED;

	angleDeg = angleRad * (180.0f / (float)M_PI);

	sfSprite_setPosition(newBullet.sprite, spawnPos);
	sfSprite_setRotation(newBullet.sprite, angleDeg); 

	newBullet.isAlive = sfTrue;
	newBullet.lifetime = BULLET_LIFETIME;
	bulletList[bulletCount] = newBullet;
	bulletCount++;
}

void SortBulletList(unsigned _index)
{
	for (unsigned i = _index; i < bulletCount - 1; i++)
	{
		bulletList[i] = bulletList[i + 1];
	}
	bulletList[bulletCount - 1] = (Bullet){ 0 };
}
