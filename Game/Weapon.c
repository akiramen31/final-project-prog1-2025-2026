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

void MoveWeapon(sfVector2f _posPlayer, sfVector2f _aimPos, float _dt, sfBool _isAttacking)
{
    sfSprite* weaponSprite = { 0 };
    float angleOffset = 0.0f;

    if (weapon.weaponType == RAILGUN)
    {
        weaponSprite = weapon.railGun.sprite;
        _isAttacking = sfFalse;
        angleOffset = (float)WEAPON_ANGLE_RAILGUN_OFFSET;
    }
    else if (weapon.weaponType == STEAMAXE)
    {
        weaponSprite = weapon.steamAxe.sprite;
        angleOffset = (float)WEAPON_ANGLE_STEAMAXE_OFFSET;
    }

    // --- PARTIE 1 : TOUJOURS ACTIVE (La Position) ---
    // On veut que l'arme suive le joueur, même pendant une attaque
    sfVector2f gunPosition = _posPlayer;
    gunPosition.y -= (float)WEAPON_ORIGIN;
    sfSprite_setPosition(weaponSprite, gunPosition);

    // --- PARTIE 2 : CONDITIONNELLE (La Rotation et l'Échelle) ---
    // On n'exécute la visée QUE si on n'est pas en train d'attaquer
    if (!_isAttacking)
    {
        float dx = _aimPos.x - _posPlayer.x;
        float dy = _aimPos.y - _posPlayer.y;
        float angleRect = atan2f(dy, dx) * (180.0f / (float)M_PI);

        // Gestion du retournement (Scale)
        if (angleRect > 90.0f || angleRect < -90.0f)
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

        // Application de la rotation de visée
        if (weapon.isRight)
        {
            sfSprite_setRotation(weaponSprite, angleRect + angleOffset);
        }
        else
        {
            sfSprite_setRotation(weaponSprite, angleRect - angleOffset);
        }
    }
}

void UseWeapon(sfVector2f _posShooter, sfVector2f _posTarget, sfBool _isRighted)
{
    ShooterType shooterType = { 0 };
    shooterType.shootPosition.x = 10.f;
    shooterType.shootPosition.y = 7.f;
    shooterType.bulletType = LIGHT;
    shooterType.isRighted = _isRighted;
    shooterType.isAlly = sfTrue;
    shooterType.weaponPos = WEAPON_ORIGIN;
	AddBullet(_posShooter, _posTarget, shooterType);
}

Weapon GetWeapon()
{
	return weapon;
}
