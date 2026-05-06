#ifndef WEAPONS_H
#define WEAPONS_H

#include "Common.h"
#include "Bullet.h"
#include "Secondaries.h"

#define WEAPON_ANGLE_RAILGUN_OFFSET 11.5f
#define WEAPON_ANGLE_STEAMAXE_OFFSET -70.f

#define FIRE_RATE_STEAMAXE 5.5f
#define FIRE_RATE_RAILGUN 2.2f
#define FIRE_RATE_MISTEAL 0.75f

#define STEAMAXE_ANGLE_LIGHT 120.f
#define STEAMAXE_ANGLE_MEDIUM 180.f
#define STEAMAXE_ANGLE_HEAVY 360.f


typedef enum WeaponType 
{
	RAILGUN,
	STEAMAXE,
	MISTEAL,
	OTHER
}WeaponType;

typedef struct RailGun
{
	sfSprite* sprite;

}RailGun;

typedef struct MiSteal
{
	sfSprite* sprite;

}MiSteal;

typedef struct SteamAxe
{
	sfSprite* sprite;
	AttackType attackType;
	sfBool canHit;

}SteamAxe;

typedef struct Weapon 
{
	SteamAxe steamAxe;
	RailGun railGun;
	MiSteal miSteal;
	WeaponType weaponType;
	SecondaryType secondary;
	sfBool isRight;
}Weapon;

void SetWeapon(WeaponType _type);
void LoadWeapons(void);
void MoveWeapon(sfVector2f _posPlayer, sfVector2f _aimPos, float _dt, sfBool _isAttacking);
void UseWeaponRailgun(sfVector2f _posShooter, sfVector2f _posTarget, sfBool _isRighted);
void UseWeaponMisteal(sfVector2f _posShooter, sfVector2f _posTarget, sfBool _isRighted);
void SwitchGunDevMode(void);
void CanHitBoss(sfBool _bool);

void ChangeAttackType(AttackType _attackType);

Weapon GetWeapon(void);

#endif // !WEAPONS_H
