#include "Bullet.h"

sfTexture* bulletTexture;
Bullet bulletList[BULLET_MAX];
unsigned bulletCount;

void SortBulletList(unsigned _index);

void LoadBullet(void)
{
	bulletTexture = sfTexture_createFromFile("Assets/Sprites/Shot.png", NULL);
	bulletCount = 0;
}

void UpdateBullet(sfRenderWindow* _renderWindow, float _dt)
{
	for (int i = bulletCount - 1; i >= 0; i--)
	{
		// Bullet move
		bulletList[i].velocity.y += GRAVITY * _dt;

		sfSprite_move(bulletList[i].sprite, (sfVector2f) { bulletList[i].velocity.x * _dt, bulletList[i].velocity.y * _dt });

		float angle = atan2f(bulletList[i].velocity.y, bulletList[i].velocity.x) * (180.0f / 3.14159f);
		sfSprite_setRotation(bulletList[i].sprite, angle + 90.0f);

		// The bullet is outside the screen
		sfFloatRect hitbox = sfSprite_getGlobalBounds(bulletList[i].sprite);
		if (hitbox.left + hitbox.width < 0 || hitbox.left > SCREEN_WIDTH || hitbox.top + hitbox.height < 0 || hitbox.top > SCREEN_HEIGHT)         
		{
			sfSprite_destroy(bulletList[i].sprite);
			SortBulletList(i);
			bulletCount--;
		}
	}

	printf("nombre de tirs : %u\n", bulletCount);
}

void DrawBullet(sfRenderWindow* const _renderWindow)
{
	for (unsigned i = 0; i < bulletCount; i++)
	{
		sfRenderWindow_drawSprite(_renderWindow, bulletList[i].sprite, NULL);
	}
}

void CleanupBullet(void)
{
	sfTexture_destroy(bulletTexture);
	bulletTexture = NULL;

	for (unsigned i = 0; i < bulletCount; i++)
	{
		sfSprite_destroy(bulletList[i].sprite);
		bulletList[i] = (Bullet){ 0 };
	}
}

unsigned GetBulletCount(void)
{
	return bulletCount;
}

void AddBullet(sfVector2f _start, sfVector2f _target)
{
	Bullet newBullet = { 0 };

	newBullet.sprite = sfSprite_create();
	sfSprite_setTexture(newBullet.sprite, bulletTexture, sfTrue);
	sfFloatRect hitbox = sfSprite_getLocalBounds(newBullet.sprite);
	sfSprite_setOrigin(newBullet.sprite, (sfVector2f) { hitbox.width / 2, hitbox.height / 2 });
	sfSprite_setPosition(newBullet.sprite, _start);

	float dx = _target.x - _start.x;
	float dy = _target.y - _start.y;

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
