#include "Boss.h"

void MoveBoss1(sfVector2f _move);

Boss boss;

void LoadBoss(void)
{
	boss = (Boss){ 0 };
}

void SwitchBoss(char _index, sfVector2f _position)
{
	if (boss.boss1)
	{
		DestroyVisualEntity(boss.boss1->track);
		DestroyVisualEntity(boss.boss1->gunCariage);
		DestroyVisualEntity(boss.boss1->steamTank);
		//DestroyVisualEntity(boss.boss1->spriteCanon);
		DestroyVisualEntity(boss.boss1->spriteTurret1Case);
		DestroyVisualEntity(boss.boss1->spriteTurret1Base);
		DestroyVisualEntity(boss.boss1->spriteTurret1Canon);
		DestroyVisualEntity(boss.boss1->spriteTurret2Case);
		DestroyVisualEntity(boss.boss1->spriteTurret2Base);
		DestroyVisualEntity(boss.boss1->spriteTurret2Canon);
		Free(boss.boss1);
		boss.boss1 = NULL;
	}

	sfFloatRect hitbox = { 0 };

	if (_index == 0)
	{
		boss.boss1 = Calloc(1, sizeof(Boss1));

		sfVector2f Offset = { 48 ,-46 };
		sfVector2f positionTurret2 = ADD_VECTOR(_position, Offset);
		Offset.x *= -1;
		sfVector2f positionTurret1 = ADD_VECTOR(_position, Offset);

		//Offset = (sfVector2f){ 0, Offset.y + hitbox.height / 2.f };
		sfVector2f positionTrack = _position;


		boss.boss1->spriteTurret1Canon = CreateSprite(GetAsset("Assets/Boss/1/TurretCanon_Placeholder.png"), positionTurret1, 1.f, 1.f);
		hitbox = sfSprite_getLocalBounds(boss.boss1->spriteTurret1Canon);
		sfSprite_setOrigin(boss.boss1->spriteTurret1Canon, (sfVector2f) { hitbox.width, hitbox.height / 2.f });

		boss.boss1->spriteTurret2Canon = CreateSprite(GetAsset("Assets/Boss/1/TurretCanon_Placeholder.png"), positionTurret2, -1.f, 1.f);
		hitbox = sfSprite_getLocalBounds(boss.boss1->spriteTurret2Canon);
		sfSprite_setOrigin(boss.boss1->spriteTurret2Canon, (sfVector2f) { hitbox.width, hitbox.height / 2.f });

		boss.boss1->spriteTurret1Base = CreateSprite(GetAsset("Assets/Boss/1/TurretBase_Placeholder.png"), positionTurret1, 1.f, 1.f);
		hitbox = sfSprite_getLocalBounds(boss.boss1->spriteTurret1Base);
		sfSprite_setOrigin(boss.boss1->spriteTurret1Base, (sfVector2f) { hitbox.width, hitbox.height / 2.f });

		boss.boss1->spriteTurret2Base = CreateSprite(GetAsset("Assets/Boss/1/TurretBase_Placeholder.png"), positionTurret2, -1.f, 1.f);
		hitbox = sfSprite_getLocalBounds(boss.boss1->spriteTurret2Base);
		sfSprite_setOrigin(boss.boss1->spriteTurret2Base, (sfVector2f) { hitbox.width, hitbox.height / 2.f });

		boss.boss1->spriteTurret1Case = CreateSprite(GetAsset("Assets/Boss/1/TurretChamber_Placeholder.png"), positionTurret1, 1.f, 1.f);
		hitbox = sfSprite_getLocalBounds(boss.boss1->spriteTurret1Case);
		sfSprite_setOrigin(boss.boss1->spriteTurret1Case, (sfVector2f) { 0, hitbox.height / 2.f });

		boss.boss1->spriteTurret2Case = CreateSprite(GetAsset("Assets/Boss/1/TurretChamber_Placeholder.png"), positionTurret2, -1.f, 1.f);
		hitbox = sfSprite_getLocalBounds(boss.boss1->spriteTurret2Case);
		sfSprite_setOrigin(boss.boss1->spriteTurret2Case, (sfVector2f) { 0, hitbox.height / 2.f });

		Offset = (sfVector2f){ 0, Offset.y + hitbox.height / 2.f };
		sfVector2f positionTopBoss = ADD_VECTOR(_position, Offset);

		boss.boss1->steamTank = CreateSprite(GetAsset("Assets/Boss/1/SteamTank_Placeholder.png"), positionTopBoss, 1.f, 1.f);
		SetSpriteOriginFoot(boss.boss1->steamTank);

		boss.boss1->gunCariage = CreateSprite(GetAsset("Assets/Boss/1/GunCariage_Placeholder.png"), positionTopBoss, 1.f, 1.f);
		SetSpriteOriginFoot(boss.boss1->gunCariage);

		boss.boss1->track = CreateSprite(GetAsset("Assets/Boss/1/Track_Placeholder.png"), positionTrack, 1.f, 1.f);
		SetSpriteOriginFoot(boss.boss1->track);



	}
}


void UpdateBoss(float _dt)
{
	if (DEV_BOSS)
	{
		if (sfKeyboard_isKeyPressed(sfKeyM))
		{
			MoveBoss1((sfVector2f) { 10.f, 0.f });
		}
		if (sfKeyboard_isKeyPressed(sfKeyL))
		{
			MoveBoss1((sfVector2f) { -10.f, 0.f });
		}
	}
	sfFloatRect pRect = GetPlayerRect();

	sfFloatRect bossSolids[3];
	bossSolids[0] = sfSprite_getGlobalBounds(boss.boss1->steamTank);
	bossSolids[1] = sfSprite_getGlobalBounds(boss.boss1->spriteTurret1Case);
	bossSolids[2] = sfSprite_getGlobalBounds(boss.boss1->spriteTurret2Case);

	sfVector2f push = TestCollisionBossPlayer(pRect, bossSolids, 3, AXIS_BOTH);

	MovePlayer(push);
}

void MoveBoss1(sfVector2f _move)
{
	sfSprite_move(boss.boss1->track, _move);
	sfSprite_move(boss.boss1->gunCariage, _move);
	sfSprite_move(boss.boss1->steamTank, _move);
	sfSprite_move(boss.boss1->spriteTurret1Case, _move);
	sfSprite_move(boss.boss1->spriteTurret1Base, _move);
	sfSprite_move(boss.boss1->spriteTurret1Canon, _move);
	sfSprite_move(boss.boss1->spriteTurret2Case, _move);
	sfSprite_move(boss.boss1->spriteTurret2Base, _move);
	sfSprite_move(boss.boss1->spriteTurret2Canon, _move);
}

void HitBoss(sfFloatRect _hitbox)
{

}

sfVector2f TestCollisionBossPlayer(sfFloatRect _hitbox, sfFloatRect* _bossParts, int _partCount, int _axis)
{
	sfVector2f vectorMove = { 0, 0 };
	sfFloatRect reaction = { 0, 0, 0, 0 };

	for (int i = 0; i < _partCount; i++)
	{
		if (sfFloatRect_intersects(&_hitbox, &_bossParts[i], &reaction))
		{
			int resolveX = 0;
			if (_axis == AXIS_X)
			{
				resolveX = 1;
			}
			else if (_axis == AXIS_Y)
			{
				resolveX = 0;
			}
			else
			{
				resolveX = (reaction.width < reaction.height);
			}

			if (resolveX)
			{
				float hitboxCenterX = _hitbox.left + (_hitbox.width / 2.0f);
				float coliderCenterX = _bossParts[i].left + (_bossParts[i].width / 2.0f);
				float push = (hitboxCenterX < coliderCenterX) ? -reaction.width : reaction.width;
				vectorMove.x += push;
				_hitbox.left += push;
			}
			else
			{
				float hitboxCenterY = _hitbox.top + (_hitbox.height / 2.0f);
				float coliderCenterY = _bossParts[i].top + (_bossParts[i].height / 2.0f);
				float push = (hitboxCenterY < coliderCenterY) ? -reaction.height : reaction.height;
				vectorMove.y += push;
				_hitbox.top += push;
			}
		}
	}
	return vectorMove;
}
