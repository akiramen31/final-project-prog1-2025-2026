#ifndef SOUND_H
#define SOUND_H

#include "Common.h"

#define MAX_SOUNDS 100

typedef enum SoundIndex
{
	PLAYER_DASH,
	PLAYER_JUMP,
	PLAYER_WALK,
	PLAYER_LAND,
	PLAYER_MELEE,
	PLAYER_HURT, 
	ENNEMY_HURT_LIGHT,
	ENNEMY_HURT_MEDIUM,
	ENNEMY_HURT_HEAVY,
	ENNEMY_SHOOT,
	BOSS_DAMAGE,
	BOSS_MOVE,
	BOSS_SHOOT_MISSILE,
	BOSS_MISSILE_EXPLODE,
	WEAPON_RAYGUN_SHOOT,
	WEAPON_RAYGUN_HIT,
	WEAPON_MISTEEL_NO_AMMO,
	WEAPON_MISTEEL_RELOAD,
	WEAPON_MISTEEL_SHOOT,
	WEAPON_AXE_MISS,
	WEAPON_AXE_SMALL,
	WEAPON_AXE_MEDIUM,
	WEAPON_AXE_LONG,
	DRONE_PASSIVE,
	COLDBREATH_SHOOT,
	GENERAL_EXPLOSION,
	GENERAL_BOX,
	SOUND_INDEX_COUNT,
}SoundIndex;

typedef struct SoundBufferAndVlolume
{
	sfSoundBuffer* buffer;
	float volume;
}SoundBufferAndVolume;

typedef struct Sounds
{
	sfSound* sound[MAX_SOUNDS];
	SoundBufferAndVolume buffer[SOUND_INDEX_COUNT];
	int currentPlayingCount;
}Sounds;

void LoadSounds(void);
void StopSound(sfSound* _sound);
sfSound* PlaySound(SoundIndex _index);
void CleanUpFinishedSounds(void);

#endif // !SOUND_H