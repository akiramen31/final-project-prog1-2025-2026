#ifndef WEAPON_H
#define WEAPON_H

#include "Common.h"
#include "Bullet.h"

#define WEAPON_ANGLE_RAILGUN_OFFSET 11.5f
#define WEAPON_ANGLE_STEAMAXE_OFFSET -70.f

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

}SteamAxe;

typedef struct Weapon 
{
	SteamAxe steamAxe;
	RailGun railGun;
	WeaponType weaponType;
	sfBool isRight;

}Weapon;

void LoadWeapon(void);
void MoveWeapon(sfVector2f _posPlayer, sfVector2f _aimPos, float _dt);

void UseWeapon(sfVector2f _posPlayer, sfVector2f _aimPos);

Weapon GetWeapon(void);

#endif // !WEAPON_H
