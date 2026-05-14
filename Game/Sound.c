#include "Sound.h"

Sounds soundList;

void LoadSounds()
{
	soundList.boss[BOSS_DAMAGE];
	soundList.boss[BOSS_MOVE];
	soundList.boss[BOSS_SHOOT_MISSILE];
	soundList.boss[BOSS_MISSILE_EXPLODE];
	soundList.ennemy[ENNEMY_SHOOT];
	soundList.ennemy[ENNEMY_HURT_LIGHT];
	soundList.ennemy[ENNEMY_HURT_MEDIUM];
	soundList.ennemy[ENNEMY_HURT_HEAVY];
	soundList.general[GENERAL_EXPLOSION];
}

void PlaySound(SoundCategory _category, int _index)
{
	switch (_category)
	{
	case CATEGORY_PLAYER:
		break;
	case CATEGORY_ENNEMY:
		break;
	case CATEGORY_BOSS:
		break;
	case CATEGORY_WEAPON:
		break;
	case CATEGORY_SECONDARY:
		break;
	case CATEGORY_GENERAL:
		break;
	default:
		break;
	}
}