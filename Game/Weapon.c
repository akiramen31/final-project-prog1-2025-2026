#include "Weapon.h"

Weapon weapon;

void LoadWeapon(void)
{
	sfTexture* textureWeapon = GetAsset("Assets/Sprites/raygun.png");
	weapon.railGun.sprite = CreateSprite(textureWeapon, (sfVector2f) { 0, 0 }, 1.f, 38);
	sfSprite_setOrigin(weapon.railGun.sprite, (sfVector2f) { 4, 6 });

	textureWeapon = GetAsset("Assets/Sprites/Axe_Placeholder.png");
	weapon.steamAxe.sprite = CreateSprite(textureWeapon, (sfVector2f) { 0, 0 }, 1.f, 38);
	sfSprite_setOrigin(weapon.steamAxe.sprite, (sfVector2f) { 1, 1 });

	textureWeapon = GetAsset("Assets/Sprites/Misteal_Launcher_Placeholder.png");
	weapon.miSteal.sprite = CreateSprite(textureWeapon, (sfVector2f) { 0, 0 }, 1.f, 38);
	sfSprite_setOrigin(weapon.miSteal.sprite, (sfVector2f) { 3, 5 });

	weapon.isRight = sfTrue;
	weapon.steamAxe.canHit = sfTrue;
	weapon.weaponType = MISTEAL;
}

void MoveWeapon(sfVector2f _posPlayer, sfVector2f _aimPos, float _dt, sfBool _isAttacking)
{
	sfSprite* weaponSprite = { 0 };
	float angleOffset = 0.0f;

	if (weapon.weaponType == RAILGUN)
	{
		weaponSprite = weapon.railGun.sprite;
		_isAttacking = sfFalse;
	}
	else if (weapon.weaponType == STEAMAXE)
	{
		weaponSprite = weapon.steamAxe.sprite;
	}
	else if (weapon.weaponType == MISTEAL)
	{
		weaponSprite = weapon.miSteal.sprite;
		_isAttacking = sfFalse;
	}

	// --- PARTIE 1 : TOUJOURS ACTIVE (La Position) ---
	// On veut que l'arme suive le joueur, même pendant une attaque
	_posPlayer.y -= (float)WEAPON_ORIGIN;
	sfVector2f gunPosition = _posPlayer;
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

void UseWeaponRailgun(sfVector2f _posShooter, sfVector2f _posTarget, sfBool _isRighted)
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

void UseWeaponMisteal(sfVector2f _posShooter, sfVector2f _posTarget, sfBool _isRighted)
{
	ShooterType shooterType = { 0 };
	shooterType.shootPosition.x = 12.f;
	shooterType.shootPosition.y = -3.f;
	shooterType.bulletType = HEAVY;
	shooterType.isRighted = _isRighted;
	shooterType.isAlly = sfTrue;
	shooterType.weaponPos = WEAPON_ORIGIN;
	AddMisteal(_posShooter, _posTarget, shooterType);
}

Weapon GetWeapon()
{
	return weapon;
}

void SwitchGunDevMode(void)
{
	if (weapon.weaponType == RAILGUN)
	{
		sfSprite_setPosition(weapon.railGun.sprite, (sfVector2f) { 0, 0 });
		weapon.weaponType = STEAMAXE;
	}
	else if (weapon.weaponType == STEAMAXE)
	{
		sfSprite_setPosition(weapon.steamAxe.sprite, (sfVector2f) { 0, 0 });
		weapon.weaponType = MISTEAL;
	}
	else if (weapon.weaponType == MISTEAL)
	{
		sfSprite_setPosition(weapon.miSteal.sprite, (sfVector2f) { 0, 0 });
		weapon.weaponType = RAILGUN;
	}
}

void ChangeAttackType(AttackType _attackType)
{
	if (weapon.weaponType == STEAMAXE)
	{
		switch (_attackType)
		{
		case (LIGHT) :
			weapon.steamAxe.attackType = LIGHT;
			break;
		case (MEDIUM):
			weapon.steamAxe.attackType = MEDIUM;
			break;
		case (HEAVY):
			weapon.steamAxe.attackType = HEAVY;
			break;
		default:
			break;
		}
	}
}