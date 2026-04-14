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

void AddBullet(sfVector2f _posPlayer, sfVector2f _posAim, int _weaponPos,sfBool _isRighted)
{
    if (bulletCount >= BULLET_MAX) return;

    Bullet newBullet = { 0 };
    newBullet.sprite = CreateSprite(bulletTexture, (sfVector2f) { 0, 0 }, 1.f, 39);
    SetSpriteOriginMiddel(newBullet.sprite);


    sfVector2f pivotPos = { _posPlayer.x, _posPlayer.y - (float)_weaponPos };


    float dxInitial = _posAim.x - pivotPos.x;
    float dyInitial = _posAim.y - pivotPos.y;
    float angleRadInitial = atan2f(dyInitial, dxInitial);


    float gunLength = 10.0f;
    float sideOffset = 7.0f;


    if (dxInitial < 0) {
        sideOffset = -sideOffset;
    }

    sfVector2f spawnPos;
    spawnPos.x = pivotPos.x + cosf(angleRadInitial) * gunLength - sinf(angleRadInitial) * sideOffset;
    spawnPos.y = pivotPos.y + sinf(angleRadInitial) * gunLength + cosf(angleRadInitial) * sideOffset;

    float realDx = _posAim.x - spawnPos.x;
    float realDy = _posAim.y - spawnPos.y;
    float realAngleRad = atan2f(realDy, realDx);

    newBullet.velocity.x = cosf(realAngleRad) * BULLET_SPEED;
    newBullet.velocity.y = sinf(realAngleRad) * BULLET_SPEED;

    sfSprite_setPosition(newBullet.sprite, spawnPos);
    sfSprite_setRotation(newBullet.sprite, realAngleRad * (180.0f / (float)M_PI));

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
