#ifndef SOUND_H
#define SOUND_H

#include "Common.h"

typedef enum SoundCategory
{
	CATEGORY_PLAYER,
	CATEGORY_ENNEMY,
	CATEGORY_BOSS,
	CATEGORY_WEAPON,
	CATEGORY_SECONDARY,
	CATEGORY_GENERAL,
	SOUND_CATEGORY_COUNT
}SoundCategory;

typedef enum PlayerSound
{
	PLAYER_DASH,
	PLAYER_JUMP,
	PLAYER_LAND,
	PLAYER_MELEE,
	PLAYER_HURT,
	PLAYER_SOUND_COUNT
}PlayerSound;

typedef enum EnnemySound
{
	ENNEMY_HURT_LIGHT,
	ENNEMY_HURT_MEDIUM,
	ENNEMY_HURT_HEAVY,
	ENNEMY_SHOOT,
	ENNEMY_SOUND_COUNT
}EnnemySound;

typedef enum BossSound
{
	BOSS_DAMAGE,
	BOSS_MOVE,
	BOSS_SHOOT_MISSILE,
	BOSS_MISSILE_EXPLODE,
	BOSS_SOUND_COUNT
}BossSound;

typedef enum WeaponSound
{
	WEAPON_RAYGUN_SHOOT,
	WEAPON_RAYGUN_HIT,
	WEAPON_MISTEEL_RELOAD,
	WEAPON_MISTEEL_SHOOT,
	WEAPON_AXE_MISS,
	WEAPON_AXE_SMALL,
	WEAPON_AXE_MEDIUM,
	WEAPON_AXE_LONG,
	WEAPON_SOUND_COUNT
}WeaponSound;

typedef enum SecondarySound
{
	DRONE_PASSIVE,
	COLDBREATH,
	SECONDARY_SOUND_COUNT
}SecondarySound;

typedef enum GeneralSound
{
	GENERAL_EXPLOSION,
	GENERAL_SOUND_COUNT
}GeneralSound;

typedef struct Sounds
{
	sfSound* player[PLAYER_SOUND_COUNT];
	sfSound* ennemy[ENNEMY_SOUND_COUNT];
	sfSound* boss[BOSS_SOUND_COUNT];
	sfSound* weapon[WEAPON_SOUND_COUNT];
	sfSound* secondary[SECONDARY_SOUND_COUNT];
	sfSound* general[GENERAL_SOUND_COUNT] ;
}Sounds;

void LoadSounds(void);
void PlaySound(SoundCategory _category, int _index);

#endif // !SOUND_H