#ifndef WEAPON_H
#define WEAPON_H

#include "Common.h"
#include "Bullet.h"

#define WEAPON_ANGLE_RAILGUN_OFFSET 11.5f
#define WEAPON_ANGLE_STEAMAXE_OFFSET -70.f

#define FIRE_RATE_STEAMAXE 2.f

#define STEAMAXE_ANGLE_LIGHT 120.f
#define STEAMAXE_ANGLE_MEDIUM 180.f
#define STEAMAXE_ANGLE_HEAVY 360.f


typedef enum WeaponType 
{
	RAILGUN,
	STEAMAXE,
	OTHER
}WeaponType;

typedef struct RailGun
{
	sfSprite* sprite;

}RailGun;

typedef struct SteamAxe
{
	sfSprite* sprite;
	AttackType attackType;

}SteamAxe;

typedef struct Weapon 
{
	SteamAxe steamAxe;
	RailGun railGun;
	WeaponType weaponType;
	sfBool isRight;

}Weapon;

void LoadWeapon(void);
void MoveWeapon(sfVector2f _posPlayer, sfVector2f _aimPos, float _dt, sfBool _isAttacking);
void UseWeapon(sfVector2f _posShooter, sfVector2f _posTarget, sfBool _isRighted);

Weapon GetWeapon(void);

#endif // !WEAPON_H
