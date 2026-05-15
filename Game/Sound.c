#include "Sound.h"

//BuffersList soundList;
Sounds sounds;

void LoadSounds()
{
	sounds = (Sounds){ 0 };
	sounds.buffer[CATEGORY_PLAYER][PLAYER_DASH] = GetAsset("Assets/Sounds/dash.ogg");
	sounds.buffer[CATEGORY_PLAYER][PLAYER_LAND] = GetAsset("Assets/Sounds/land.ogg");
	sounds.buffer[CATEGORY_PLAYER][PLAYER_WALK] = GetAsset("Assets/Sounds/land.ogg");

	sounds.buffer[CATEGORY_BOSS][BOSS_MOVE] = GetAsset("Assets/Sounds/tracks_move.ogg");
	sounds.buffer[CATEGORY_BOSS][BOSS_SHOOT_MISSILE] = GetAsset("Assets/Sounds/missile_launch.ogg");

	sounds.buffer[CATEGORY_ENNEMY][ENNEMY_HURT_MEDIUM] = GetAsset("Assets/Sounds/medium_armor.ogg");
	sounds.buffer[CATEGORY_ENNEMY][ENNEMY_HURT_HEAVY] = GetAsset("Assets/Sounds/heavy_armor.ogg");
	sounds.buffer[CATEGORY_ENNEMY][GENERAL_EXPLOSION] = GetAsset("Assets/Sounds/explosion.ogg");
	/*
	soundList.player[PLAYER_DASH] = GetAsset("Assets/Sounds/dash.ogg");
	soundList.player[PLAYER_LAND] = GetAsset("Assets/Sounds/land.ogg");
	soundList.player[PLAYER_WALK] = GetAsset("Assets/Sounds/land.ogg");
	//soundList.boss[BOSS_DAMAGE];
	soundList.boss[BOSS_MOVE] = GetAsset("Assets/Sounds/tracks_move.ogg");
	soundList.boss[BOSS_SHOOT_MISSILE] = GetAsset("Assets/Sounds/missile_launch.ogg");
	//soundList.ennemy[ENNEMY_SHOOT] = ;
	//soundList.ennemy[ENNEMY_HURT_LIGHT];
	soundList.ennemy[ENNEMY_HURT_MEDIUM] = GetAsset("Assets/Sounds/medium_armor.ogg");
	soundList.ennemy[ENNEMY_HURT_HEAVY] = GetAsset("Assets/Sounds/heavy_armor.ogg");
	soundList.general[GENERAL_EXPLOSION] = GetAsset("Assets/Sounds/explosion.ogg");
	sounds.currentPlayingCount = 0;
	*/
}

sfSound* PlaySound(SoundCategory _category, int _index)
{
	if (sounds.currentPlayingCount <= MAX_SOUNDS)
	{
		sounds.currentPlayingCount++;
		if (sounds.buffer[_category][_index])
		{
			sounds.sound[sounds.currentPlayingCount] = CreateSound(sounds.buffer[_category][_index], GAME_VOLUME, sfTrue);
			if (_category == CATEGORY_PLAYER && _index == PLAYER_WALK)
			{
				sfSound_setLoop(sounds.sound[sounds.currentPlayingCount], sfTrue);
			}
			return sounds.buffer[sounds.currentPlayingCount];
		}

		/*
		switch (_category)
		{
		case CATEGORY_PLAYER:
			if (_index < PLAYER_SOUND_COUNT)
			{
				sounds.buffer[sounds.currentPlayingCount] = CreateSound(soundList.player[_index], GAME_VOLUME, sfTrue);
				if (_index == PLAYER_JUMP)
				{
					sfSound_setLoop(sounds.buffer[sounds.currentPlayingCount], sfTrue);
				}
				return sounds.buffer[sounds.currentPlayingCount];
			}
			else
			{
				return;
			}
			break;
		case CATEGORY_ENNEMY:
			if (_index < PLAYER_SOUND_COUNT)
			{
				sounds.buffer[sounds.currentPlayingCount] = CreateSound(soundList.ennemy[_index], GAME_VOLUME, sfTrue);
			}
			else
			{
				return;
			}
			return sounds.buffer[sounds.currentPlayingCount];
			break;
		case CATEGORY_BOSS:
			if (_index < PLAYER_SOUND_COUNT)
			{
				sounds.buffer[sounds.currentPlayingCount] = CreateSound(soundList.boss[_index], GAME_VOLUME, sfTrue);
			}
			else
			{
				return;
			}
			return sounds.buffer[sounds.currentPlayingCount];
			break;
		case CATEGORY_WEAPON:
			if (_index < PLAYER_SOUND_COUNT)
			{
				sounds.buffer[sounds.currentPlayingCount] = CreateSound(soundList.weapon[_index], GAME_VOLUME, sfTrue);
			}
			else
			{
				return;
			}
			return sounds.buffer[sounds.currentPlayingCount];
			break;
		case CATEGORY_SECONDARY:
			if (_index < SECONDARY_SOUND_COUNT)
			{
				sounds.buffer[sounds.currentPlayingCount] = CreateSound(soundList.secondary[_index], GAME_VOLUME, sfTrue);
			}
			else
			{
				return;
			}
			return sounds.buffer[sounds.currentPlayingCount];
			break;
		case CATEGORY_GENERAL:
			if (_index < GENERAL_SOUND_COUNT)
			{
				sounds.buffer[sounds.currentPlayingCount] = CreateSound(soundList.general[_index], GAME_VOLUME, sfTrue);
			}
			else
			{
				return;
			}
			return sounds.buffer[sounds.currentPlayingCount];
			break;
		default:
			break;
		}
		*/
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
		if (sfSound_getStatus(sounds.sound[i]) == sfStopped)
		{
			sfSound_destroy(sounds.sound[i]);
			sfSound* temp;
			for (int j = i + 1; j < sounds.currentPlayingCount; j++)
			{
				temp = sounds.sound[j + 1];
				sounds.sound[j] = temp;
			}
			sounds.currentPlayingCount -= 1;
		}
	}
}