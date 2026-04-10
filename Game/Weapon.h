#ifndef WEAPON_H
#define WEAPON_H

#include "Common.h"

typedef enum WeaponType 
{
	RAILGUN,
	AXE,
	OTHER
}WeaponType;

typedef struct Weapon 
{

	sfSprite* sprite;
	WeaponType type;
	sfVector2f velocity;
	sfBool isRight;

}Weapon;

#endif // !WEAPON_H
