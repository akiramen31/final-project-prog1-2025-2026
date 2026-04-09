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
	for (int i = bulletCount - 1; i >= 0; i--)
	{
		bulletList[i].lifetime -= _dt;
		if (bulletList[i].lifetime <= 0)
		{
			DestroyVisualEntity(bulletList[i].sprite);
			bulletList[i].isAlive = sfFalse;
			SortBulletList(i);
			bulletCount--;
		}
		if (bulletList[i].isAlive)
		{
			// Bullet move
			bulletList[i].velocity.y += G * _dt;

			sfSprite_move(bulletList[i].sprite, (sfVector2f) { bulletList[i].velocity.x* _dt, bulletList[i].velocity.y* _dt });

			float angle = atan2f(bulletList[i].velocity.y, bulletList[i].velocity.x) * (180.0f / 3.14159f);
			sfSprite_setRotation(bulletList[i].sprite, angle + 90.0f);

			// The bullet is outside the screen
			sfFloatRect hitbox = sfSprite_getGlobalBounds(bulletList[i].sprite);
			if (hitbox.left + hitbox.width < 0 || hitbox.left > SCREEN_WIDTH || hitbox.top + hitbox.height < 0 || hitbox.top > SCREEN_HEIGHT)
			{
				DestroyVisualEntity(bulletList[i].sprite);
				SortBulletList(i);
				bulletCount--;
			}
		}
	}

	printf("nombre de tirs : %u\n", bulletCount);
}


unsigned GetBulletCount(void)
{
	return bulletCount;
}

void AddBullet(sfVector2f _posPlayer, sfVector2f _posAim)
{
	Bullet newBullet = { 0 };

	newBullet.sprite = CreateSprite(bulletTexture, (sfVector2f) { 0, 0 }, 1.f, 39);
	SetSpriteOriginMiddel(newBullet.sprite);
	sfSprite_setPosition(newBullet.sprite, _posPlayer);
	sfVector2f playerPos = _posPlayer;
	sfVector2f aimPosition = _posAim;

	float dx = aimPosition.x - playerPos.x;
	float dy = aimPosition.y - playerPos.y;

	float length = sqrtf(dx * dx + dy * dy);

	if (length != 0)
	{
		newBullet.velocity.x = (dx / length) * BULLET_SPEED;
		newBullet.velocity.y = (dy / length) * BULLET_SPEED;
	}
	else
	{
		newBullet.velocity = (sfVector2f){ 0, 0 };
	}

	float angle = atan2f(dy, dx) * (180.0f / (float)M_PI);
	sfSprite_setRotation(newBullet.sprite, angle);
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
