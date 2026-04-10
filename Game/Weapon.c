#include "Weapon.h"

Weapon weapon;

void LoadWeapon(void)
{
	sfTexture* textureWeapon = GetAsset("Assets/Sprites/raygun.png");
	weapon.railGun.sprite = CreateSprite(textureWeapon, (sfVector2f) { 0, 0 }, 1.f, 38);
	sfSprite_setOrigin(weapon.railGun.sprite, (sfVector2f) { 4, 6 });

	textureWeapon = GetAsset("Assets/Sprites/Axe_Placeholder.png");
	weapon.steamAxe.sprite = CreateSprite(textureWeapon, (sfVector2f) { 0, 0 }, 1.f, 38);
	sfSprite_setOrigin(weapon.steamAxe.sprite, (sfVector2f) { 7, 1 });

	
	weapon.isRight = sfTrue;
	weapon.weaponType = RAILGUN;
}

void MoveWeapon(sfVector2f _posPlayer, sfVector2f _aimPos, float _dt)
{
	sfSprite* weaponSprite = { 0 };
	float angleOffset = 0;
	switch (weapon.weaponType)
	{
	case RAILGUN:
		weaponSprite = weapon.railGun.sprite;
		angleOffset = WEAPON_ANGLE_RAILGUN_OFFSET;
		break;
	case STEAMAXE:
		weaponSprite = weapon.steamAxe.sprite;
		angleOffset = WEAPON_ANGLE_STEAMAXE_OFFSET;
		break;
	default:
		break;
	}

	sfVector2f gunPosition = _posPlayer;
	gunPosition.y -= WEAPON_ORIGIN;
	sfSprite_setPosition(weaponSprite, gunPosition);

	sfVector2f playerPos = _posPlayer; //start
	sfVector2f aimPosition = _aimPos; // target

	float dx = aimPosition.x - playerPos.x;
	float dy = aimPosition.y - playerPos.y;

	float angleRect = atan2f(dy, dx) * (180.0f / (float)M_PI);
	if (angleRect > 90.0f || angleRect < -90)
	{
		if (weapon.isRight)
		{
			sfSprite_setScale(weaponSprite, (sfVector2f) { 1.f, -1.f });
			weapon.isRight = sfFalse;
		}
	}
	else
	{
		if (!weapon.isRight)
		{
			sfSprite_setScale(weaponSprite, (sfVector2f) { 1.f, 1.f });
			weapon.isRight = sfTrue;
		}
	}
	if (weapon.isRight)
	{
		sfSprite_setRotation(weaponSprite, angleRect + angleOffset);
	}
	else
	{
		sfSprite_setRotation(weaponSprite, angleRect - angleOffset);
	}
}

void UseWeapon(sfVector2f _posPlayer, sfVector2f _aimPos)
{
	AddBullet(_posPlayer, _aimPos, (int){ WEAPON_ORIGIN }, weapon.isRight);
}

Weapon GetWeapon()
{
	return weapon;
}