#include "Sound.h"

//BuffersList soundList;
Sounds sounds;

void LoadSounds()
{
	sounds = (Sounds){ 0 };

	sounds.buffer[PLAYER_DASH] = GetAsset("Assets/Sounds/dash.ogg");
	sounds.buffer[PLAYER_JUMP] = GetAsset("Assets/Sounds/jump.ogg");
	sounds.buffer[PLAYER_WALK] = GetAsset("Assets/Sounds/walk.ogg");
	sounds.buffer[PLAYER_LAND] = GetAsset("Assets/Sounds/land.ogg");
	//sounds.buffer[PLAYER_MELEE] = GetAsset("Assets/Sounds/land.ogg");
	//sounds.buffer[PLAYER_HURT] = GetAsset("Assets/Sounds/land.ogg");
	//sounds.buffer[ENNEMY_HURT_LIGHT] = GetAsset("Assets/Sounds/land.ogg");
	sounds.buffer[ENNEMY_HURT_MEDIUM] = GetAsset("Assets/Sounds/medium_armor.ogg");
	sounds.buffer[ENNEMY_HURT_HEAVY] = GetAsset("Assets/Sounds/high_armor_hit.ogg");
	//sounds.buffer[ENNEMY_SHOOT] = GetAsset("Assets/Sounds/land.ogg");
	//sounds.buffer[BOSS_DAMAGE] = GetAsset("Assets/Sounds/land.ogg");
	sounds.buffer[BOSS_MOVE] = GetAsset("Assets/Sounds/tracks_move.ogg");
	sounds.buffer[BOSS_SHOOT_MISSILE] = GetAsset("Assets/Sounds/missile_launch.ogg");
	//sounds.buffer[BOSS_MISSILE_EXPLODE] = GetAsset("Assets/Sounds/land.ogg");
	//sounds.buffer[WEAPON_RAYGUN_SHOOT] = GetAsset("Assets/Sounds/land.ogg");
	//sounds.buffer[WEAPON_RAYGUN_HIT] = GetAsset("Assets/Sounds/land.ogg");
	sounds.buffer[WEAPON_MISTEEL_NO_AMMO] = GetAsset("Assets/Sounds/mi-steel_no_ammo.ogg");
	sounds.buffer[WEAPON_MISTEEL_RELOAD] = GetAsset("Assets/Sounds/mi-steel_reload.ogg");
	//sounds.buffer[WEAPON_MISTEEL_SHOOT] = GetAsset("Assets/Sounds/land.ogg");
	sounds.buffer[WEAPON_AXE_MISS] = GetAsset("Assets/Sounds/axe_miss.ogg");
	sounds.buffer[WEAPON_AXE_SMALL] = GetAsset("Assets/Sounds/axe_hit.ogg");
	sounds.buffer[WEAPON_AXE_MEDIUM] = GetAsset("Assets/Sounds/axe_hit.ogg");
	sounds.buffer[WEAPON_AXE_LONG] = GetAsset("Assets/Sounds/axe_hit.ogg");
	sounds.buffer[DRONE_PASSIVE] = GetAsset("Assets/Sounds/drone_sound.ogg");
	//sounds.buffer[COLDBREATH_SHOOT] = GetAsset("Assets/Sounds/land.ogg");
	sounds.buffer[GENERAL_EXPLOSION] = GetAsset("Assets/Sounds/explosion.ogg");
	//sounds.buffer[GENERAL_BOX] = GetAsset("Assets/Sounds/land.ogg");
}
sfSound* PlaySound(SoundIndex _index)
{
	if (sounds.currentPlayingCount < MAX_SOUNDS && _index < SOUND_INDEX_COUNT && sounds.buffer[_index])
	{
		sounds.sound[sounds.currentPlayingCount] = CreateSound(sounds.buffer[_index], 10000000000000000.f, sfTrue);
		sounds.currentPlayingCount++;
		return sounds.buffer[sounds.currentPlayingCount - 1];
	}
}

void StopSound(sfSound* _sound)
{
	for (int i = 0; i < sounds.currentPlayingCount; i++)
	{
		if (sounds.sound[i] == _sound)
		{
			DestroySoundEntity(_sound);
			sounds.currentPlayingCount--;
			sounds.sound[i] = sounds.sound[sounds.currentPlayingCount];
			return;
		}
	}
}

void CleanUpFinishedSounds(void)
{
	for (int i = 0; i < sounds.currentPlayingCount; i++)
	{
		if (!sfSound_getStatus(sounds.sound[i]))
		{
			DestroySoundEntity(sounds.sound[i]);
			sounds.currentPlayingCount--;
			sounds.sound[i] = sounds.sound[sounds.currentPlayingCount];
		}
	}
}