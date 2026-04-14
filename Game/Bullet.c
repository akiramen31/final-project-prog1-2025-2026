#include "Bullet.h"
#include "Ennemy.h"

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
	sfFloatRect hitboxBullet = { 0 };
	sfVector2f reaction = { 0 };
	sfVector2f reactionBox = { 0 };
	for (int i = (int)bulletCount - 1; i >= 0; i--)
	{
		bulletList[i].lifetime -= _dt;

		if (bulletList[i].lifetime <= 0)
		{
			DeleteBullet(i);
			continue;
		}
		hitboxBullet = sfSprite_getGlobalBounds(bulletList[i].sprite);

		reaction = Colision(sfSprite_getGlobalBounds(bulletList[i].sprite), AXIS_BOTH);
		reactionBox = ColisionBox(sfSprite_getGlobalBounds(bulletList[i].sprite), sfTrue, AXIS_BOTH);
		reaction.x += reactionBox.x;
		reaction.y += reactionBox.y;

		if (reaction.x != 0 || reaction.y != 0 || IfHitEnemy(hitboxBullet))
		{
			DeleteBullet(i);
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

void AddBullet(sfVector2f _posShooter, sfVector2f _posTarget, ShooterType _shooterType)
{
	if (bulletCount >= BULLET_MAX) return;

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

	newBullet.velocity.x = cosf(realAngleRad) * BULLET_SPEED;
	newBullet.velocity.y = sinf(realAngleRad) * BULLET_SPEED;

	sfSprite_setPosition(newBullet.sprite, spawnPos);
	sfSprite_setRotation(newBullet.sprite, realAngleRad * (180.0f / (float)M_PI));

	newBullet.isAlive = sfTrue;
	newBullet.isAlly = _shooterType.isAlly;
	newBullet.bulletType = _shooterType.bulletType;
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

void DeleteBullet(unsigned _index)
{
	DestroyVisualEntity(bulletList[_index].sprite);
	SortBulletList(_index);
	bulletCount--;
}