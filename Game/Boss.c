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
		DestroyVisualEntity(boss.boss1->spriteTurretLCase);
		DestroyVisualEntity(boss.boss1->spriteTurretLBase);
		DestroyVisualEntity(boss.boss1->spriteTurretLCanon);
		DestroyVisualEntity(boss.boss1->spriteTurretRCase);
		DestroyVisualEntity(boss.boss1->spriteTurretRBase);
		DestroyVisualEntity(boss.boss1->spriteTurretRCanon);
		Free(boss.boss1);
		boss.boss1 = NULL;
	}
	sfFloatRect hitbox = { 0 };

	if (_index == 0)
	{
		boss.boss1 = Calloc(1, sizeof(Boss1));

		sfVector2f Offset = { 58 ,-46 };
		sfVector2f positionTurret2 = ADD_VECTOR(_position, Offset);
		Offset.x *= -1;
		sfVector2f positionTurret1 = ADD_VECTOR(_position, Offset);

		//Offset = (sfVector2f){ 0, Offset.y + hitbox.height / 2.f };
		sfVector2f positionTrack = _position;


		boss.boss1->spriteTurretLCanon = CreateSprite(GetAsset("Assets/Boss/1/TurretCanon_Placeholder.png"), positionTurret1, 1.f, 1.f);
		hitbox = sfSprite_getLocalBounds(boss.boss1->spriteTurretLCanon);
		sfSprite_setOrigin(boss.boss1->spriteTurretLCanon, (sfVector2f) { hitbox.width, hitbox.height / 2.f });

		boss.boss1->spriteTurretRCanon = CreateSprite(GetAsset("Assets/Boss/1/TurretCanon_Placeholder.png"), positionTurret2, -1.f, 1.f);
		hitbox = sfSprite_getLocalBounds(boss.boss1->spriteTurretRCanon);
		sfSprite_setOrigin(boss.boss1->spriteTurretRCanon, (sfVector2f) { hitbox.width, hitbox.height / 2.f });

		boss.boss1->spriteTurretLBase = CreateSprite(GetAsset("Assets/Boss/1/TurretBase_Placeholder.png"), positionTurret1, 1.f, 1.f);
		hitbox = sfSprite_getLocalBounds(boss.boss1->spriteTurretLBase);
		sfSprite_setOrigin(boss.boss1->spriteTurretLBase, (sfVector2f) { hitbox.width, hitbox.height / 2.f });

		boss.boss1->spriteTurretRBase = CreateSprite(GetAsset("Assets/Boss/1/TurretBase_Placeholder.png"), positionTurret2, -1.f, 1.f);
		hitbox = sfSprite_getLocalBounds(boss.boss1->spriteTurretRBase);
		sfSprite_setOrigin(boss.boss1->spriteTurretRBase, (sfVector2f) { hitbox.width, hitbox.height / 2.f });

		boss.boss1->spriteTurretLCase = CreateSprite(GetAsset("Assets/Boss/1/TurretChamber_Placeholder.png"), positionTurret1, 1.f, 1.f);
		hitbox = sfSprite_getLocalBounds(boss.boss1->spriteTurretLCase);
		sfSprite_setOrigin(boss.boss1->spriteTurretLCase, (sfVector2f) { 0, hitbox.height / 2.f });

		boss.boss1->spriteTurretRCase = CreateSprite(GetAsset("Assets/Boss/1/TurretChamber_Placeholder.png"), positionTurret2, -1.f, 1.f);
		hitbox = sfSprite_getLocalBounds(boss.boss1->spriteTurretRCase);
		sfSprite_setOrigin(boss.boss1->spriteTurretRCase, (sfVector2f) { 0, hitbox.height / 2.f });

		Offset = (sfVector2f){ 0, Offset.y + hitbox.height / 2.f };
		sfVector2f positionTopBoss = ADD_VECTOR(_position, Offset);

		boss.boss1->steamTank = CreateSprite(GetAsset("Assets/Boss/1/SteamTank_Placeholder.png"), positionTopBoss, 1.f, 1.f);
		SetSpriteOriginFoot(boss.boss1->steamTank);

		boss.boss1->gunCariage = CreateSprite(GetAsset("Assets/Boss/1/GunCariage_Placeholder.png"), positionTopBoss, 1.f, 1.f);
		SetSpriteOriginFoot(boss.boss1->gunCariage);

		boss.boss1->track = CreateSprite(GetAsset("Assets/Boss/1/Track_Placeholder.png"), positionTrack, 1.f, 1.f);
		SetSpriteOriginFoot(boss.boss1->track);

		boss.boss1->bossReactionToPlayer = PLAYER_RANGE_SHOOT;

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

	sfFloatRect bossSolids[3] = { 0 };
	bossSolids[0] = sfSprite_getGlobalBounds(boss.boss1->steamTank);
	bossSolids[1] = sfSprite_getGlobalBounds(boss.boss1->spriteTurretLCase);
	bossSolids[2] = sfSprite_getGlobalBounds(boss.boss1->spriteTurretRCase);

	sfVector2f push = TestCollisionBossPlayer(pRect, bossSolids, 3, AXIS_BOTH);

	HandlePlayerBossCollision(push);
	UpdateTurret(_dt);
}

void MoveBoss1(sfVector2f _move)
{
	sfSprite_move(boss.boss1->track, _move);
	sfSprite_move(boss.boss1->gunCariage, _move);
	sfSprite_move(boss.boss1->steamTank, _move);
	sfSprite_move(boss.boss1->spriteTurretLCase, _move);
	sfSprite_move(boss.boss1->spriteTurretLBase, _move);
	sfSprite_move(boss.boss1->spriteTurretLCanon, _move);
	sfSprite_move(boss.boss1->spriteTurretRCase, _move);
	sfSprite_move(boss.boss1->spriteTurretRBase, _move);
	sfSprite_move(boss.boss1->spriteTurretRCanon, _move);
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

void CheckPlayerState(void)
{
	
}

void UpdateTurret(float _dt)
{
	if (boss.boss1->bossReactionToPlayer != PLAYER_RANGE_SHOOT) return;

	sfVector2f playerPos = GetPlayerPosition();

	sfVector2f posL = sfSprite_getPosition(boss.boss1->spriteTurretLCanon);
	float angleTargetL = atan2f(-(playerPos.y - posL.y), -(playerPos.x - posL.x)) * (180.0f / 3.14159f);

	if (angleTargetL < -90.0f) angleTargetL = -90.0f;
	if (angleTargetL > 90.0f) angleTargetL = 90.0f;

	float currentAngleL = sfSprite_getRotation(boss.boss1->spriteTurretLCanon);
	float newAngleL = MoveTowardsAngle(currentAngleL, angleTargetL, TURRET_ROTATION_SPEED, _dt);
	sfSprite_setRotation(boss.boss1->spriteTurretLCanon, newAngleL);


	sfVector2f posR = sfSprite_getPosition(boss.boss1->spriteTurretRCanon);
	float angleTargetR = atan2f(playerPos.y - posR.y, playerPos.x - posR.x) * (180.0f / 3.14159f);

	if (angleTargetR > 90.0f) angleTargetR = 90.0f;
	if (angleTargetR < -90.0f) angleTargetR = -90.0f;

	float currentAngleR = sfSprite_getRotation(boss.boss1->spriteTurretRCanon);
	float newAngleR = MoveTowardsAngle(currentAngleR, angleTargetR, TURRET_ROTATION_SPEED, _dt);
	sfSprite_setRotation(boss.boss1->spriteTurretRCanon, newAngleR);
}