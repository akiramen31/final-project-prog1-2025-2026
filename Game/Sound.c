#include "Sound.h"

//BuffersList soundList;
Sounds sounds;

void LoadSounds()
{
	sounds = (Sounds){ 0 };

	sounds.buffer[PLAYER_DASH] = (SoundBufferAndVolume){ GetAsset("Assets/Sounds/dash.ogg"), 0.3f };
	sounds.buffer[PLAYER_JUMP] = (SoundBufferAndVolume){ GetAsset("Assets/Sounds/jump.ogg"), 0.1f };
	sounds.buffer[PLAYER_WALK] = (SoundBufferAndVolume){ GetAsset("Assets/Sounds/walk.ogg"), 10.f };
	sounds.buffer[PLAYER_LAND] = (SoundBufferAndVolume){ GetAsset("Assets/Sounds/land.ogg"), 10.f };
	//sounds.buffer[PLAYER_MELEE] = GetAsset("Assets/Sounds/land.ogg");
	//sounds.buffer[PLAYER_HURT] = GetAsset("Assets/Sounds/land.ogg");
	//sounds.buffer[ENNEMY_HURT_LIGHT] = GetAsset("Assets/Sounds/land.ogg");
	sounds.buffer[ENNEMY_HURT_MEDIUM] = (SoundBufferAndVolume){ GetAsset("Assets/Sounds/medium_armor.ogg"), 0.25f };
	sounds.buffer[ENNEMY_HURT_HEAVY] = (SoundBufferAndVolume){ GetAsset("Assets/Sounds/high_armor_hit.ogg"), 0.25f };
	//sounds.buffer[ENNEMY_SHOOT] = GetAsset("Assets/Sounds/land.ogg");
	//sounds.buffer[BOSS_DAMAGE] = GetAsset("Assets/Sounds/land.ogg");
	sounds.buffer[BOSS_MOVE] = (SoundBufferAndVolume){ GetAsset("Assets/Sounds/tracks_move.ogg"), 1.f };
	sounds.buffer[BOSS_SHOOT_MISSILE] = (SoundBufferAndVolume){ GetAsset("Assets/Sounds/missile_launch.ogg"), 1.f };
	//sounds.buffer[BOSS_MISSILE_EXPLODE] = GetAsset("Assets/Sounds/land.ogg");
	sounds.buffer[WEAPON_RAYGUN_SHOOT] = (SoundBufferAndVolume){ GetAsset("Assets/Sounds/raygun_shoot.ogg"), 1.f };
	//sounds.buffer[WEAPON_RAYGUN_HIT] = GetAsset("Assets/Sounds/land.ogg");
	sounds.buffer[WEAPON_MISTEEL_NO_AMMO] = (SoundBufferAndVolume){ GetAsset("Assets/Sounds/mi-steel_no_ammo.ogg"), 1.f };
	sounds.buffer[WEAPON_MISTEEL_RELOAD] = (SoundBufferAndVolume){ GetAsset("Assets/Sounds/mi-steel_reload.ogg"), 1.f };
	//sounds.buffer[WEAPON_MISTEEL_SHOOT] = GetAsset("Assets/Sounds/land.ogg");
	sounds.buffer[WEAPON_AXE_MISS] = (SoundBufferAndVolume){ GetAsset("Assets/Sounds/axe_miss.ogg"), 1.f };
	sounds.buffer[WEAPON_AXE_SMALL] = (SoundBufferAndVolume){ GetAsset("Assets/Sounds/axe_hit.ogg"), 1.f };
	sounds.buffer[WEAPON_AXE_MEDIUM] = (SoundBufferAndVolume){ GetAsset("Assets/Sounds/axe_hit.ogg"), 1.f };
	sounds.buffer[WEAPON_AXE_LONG] = (SoundBufferAndVolume){ GetAsset("Assets/Sounds/axe_hit.ogg"), 1.f };
	sounds.buffer[DRONE_PASSIVE] = (SoundBufferAndVolume){ GetAsset("Assets/Sounds/drone_sound.ogg"), 1.f };
	//sounds.buffer[COLDBREATH_SHOOT] = GetAsset("Assets/Sounds/land.ogg");
	sounds.buffer[GENERAL_EXPLOSION] = (SoundBufferAndVolume){ GetAsset("Assets/Sounds/explosion.ogg"), 0.25f };
	sounds.buffer[GENERAL_BOX] = (SoundBufferAndVolume){ GetAsset("Assets/Sounds/box_break.ogg"), 0.125f };
}

sfSound* PlaySound(SoundIndex _index)
{
	if (sounds.currentPlayingCount < MAX_SOUNDS && _index < SOUND_INDEX_COUNT && sounds.buffer[_index].buffer && _index >= 0)
	{
		sounds.sound[sounds.currentPlayingCount] = CreateSound(sounds.buffer[_index].buffer, sounds.buffer[_index].volume, sfTrue);
		sounds.currentPlayingCount++;
		return sounds.sound[sounds.currentPlayingCount - 1];
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