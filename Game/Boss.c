#include "Boss.h"

void MoveBoss1(sfVector2f _move);
void UpdateBossReaction(float _dt);

Boss boss;

void LoadBoss(void)
{
	boss = (Boss){ 0 };
}

void SwitchBoss(char _index, sfVector2f _position)
{
	if (boss.boss1)
	{
		DestroyBoss();
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

		boss.boss1->spriteTurretLCase = CreateSprite(GetAsset("Assets/Boss/1/TurretChamberTEST_Placeholder-export.png"), positionTurret1, 1.f, 1.f);
		hitbox = sfSprite_getLocalBounds(boss.boss1->spriteTurretLCase);
		sfSprite_setOrigin(boss.boss1->spriteTurretLCase, (sfVector2f) { 0, hitbox.height / 2.f });

		boss.boss1->spriteTurretRCase = CreateSprite(GetAsset("Assets/Boss/1/TurretChamberTEST_Placeholder-export.png"), positionTurret2, -1.f, 1.f);
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

		boss.boss1->playerPositionToBoss = PLAYER_NOT_IN_ARENA;
		boss.boss1->bossLife = 72.f;
		boss.boss1->cooldownShoot = 2;
		boss.boss1->state = sfTrue;
	}
}


void UpdateBoss(float _dt)
{
	if (DEV_BOSS)
	{
		if (sfKeyboard_isKeyPressed(sfKeyM))
		{
			MoveBoss1((sfVector2f) { PLAYER_HORIZONTAL_SPEED_MAX * 2 * _dt, 0.f });
		}
		if (sfKeyboard_isKeyPressed(sfKeyL))
		{
			MoveBoss1((sfVector2f) { -PLAYER_HORIZONTAL_SPEED_MAX * 2 * _dt, 0.f });
		}

	}
	sfFloatRect pRect = GetPlayerRect();

	sfFloatRect bossSolids[3] = { 0 };
	bossSolids[0] = sfSprite_getGlobalBounds(boss.boss1->steamTank);
	bossSolids[1] = sfSprite_getGlobalBounds(boss.boss1->spriteTurretLCase);
	bossSolids[2] = sfSprite_getGlobalBounds(boss.boss1->spriteTurretRCase);

	sfVector2f push = TestCollisionBossPlayer(pRect, bossSolids, 3, AXIS_BOTH);
	if (boss.boss1->state)
	{
		HandlePlayerBossCollision(push);
		CheckBossPlayerState(_dt);
		UpdateBossReaction(_dt);
		UpdateTurret(_dt);
		BossShoot(_dt);
	}
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

sfBool HitBoss(float _degat, sfFloatRect _hitbox)
{
	if (boss.boss1->state)
	{
		sfFloatRect bossSolids[3] = { 0 };
		bossSolids[0] = sfSprite_getGlobalBounds(boss.boss1->steamTank);
		bossSolids[1] = sfSprite_getGlobalBounds(boss.boss1->spriteTurretLCase);
		bossSolids[2] = sfSprite_getGlobalBounds(boss.boss1->spriteTurretRCase);
		for (unsigned i = 0; i < 3; i++)
		{
			if (sfFloatRect_intersects(&_hitbox, &bossSolids[i], NULL))
			{
				if (i == 0)
				{
					boss.boss1->bossLife -= _degat;
					if (sfSprite_getPosition(boss.boss1->track).x > ARENA_CENTER)
					{
						boss.boss1->bossReactionToPlayer = GO_LEFT;
						boss.boss1->bossReacting = sfTrue;
					}
					else if (sfSprite_getPosition(boss.boss1->track).x < ARENA_CENTER)
					{
						boss.boss1->bossReactionToPlayer = GO_RIGHT;
						boss.boss1->bossReacting = sfTrue;
					}
					if (boss.boss1->bossLife <= 0)
					{
						DestroyBoss();
					}
					return sfTrue;
				}
				return sfTrue;
			}
		}
	}
	return sfFalse;
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

void CheckBossPlayerState(float _dt)
{
	sfVector2f trackPosition = sfSprite_getPosition(boss.boss1->track);
	sfVector2f playerPos = GetPlayerPosition();

	sfVector2f posFar = (trackPosition.x > playerPos.x) ? trackPosition : playerPos;
	sfVector2f posClose = (trackPosition.x <= playerPos.x) ? trackPosition : playerPos;

	sfVector2f distance = (sfVector2f){ posFar.x - posClose.x, trackPosition.y - playerPos.y };

	if (boss.boss1->playerPositionToBoss == PLAYER_NOT_IN_ARENA && GetPlayerPosition().x > ARENA_ENTRY)
	{
		boss.boss1->playerPositionToBoss = PLAYER_AWAY_LEFT;
	}
	if (!boss.boss1->bossReacting)
	{
		if (boss.boss1->playerPositionToBoss != PLAYER_NOT_IN_ARENA)
		{
			if (distance.x > TARGET_DISTANCE_MAX && playerPos.x < trackPosition.x)
			{
				boss.boss1->playerPositionToBoss = PLAYER_AWAY_LEFT;
				boss.boss1->bossReactionToPlayer = GO_LEFT;
			}
			else if (distance.x > TARGET_DISTANCE_MAX && playerPos.x > trackPosition.x)
			{
				boss.boss1->playerPositionToBoss = PLAYER_AWAY_RIGHT;
				boss.boss1->bossReactionToPlayer = GO_RIGHT;
			}
			if (distance.x < TARGET_DISTANCE_MAX && distance.x > SHOOT_DISTANCE_MIN)
			{
				if (playerPos.x < trackPosition.x)
				{
					boss.boss1->playerPositionToBoss = PLAYER_RANGE_SHOOT_LEFT;
					boss.boss1->bossReactionToPlayer = NONE;

				}
				else if (playerPos.x > trackPosition.x)
				{
					boss.boss1->playerPositionToBoss = PLAYER_RANGE_SHOOT_RIGHT;
					boss.boss1->bossReactionToPlayer = NONE;
				}
			}
			if (distance.x < 46 && distance.y < 32)
			{
				boss.boss1->playerPositionToBoss = PLAYER_UNDER;
				boss.boss1->runAwayTiming += _dt * DT_SLOW;
				if (boss.boss1->runAwayTiming >= RUNAWAY_TIMER)
				{
					if (trackPosition.x > ARENA_CENTER)
					{
						boss.boss1->bossReactionToPlayer = GO_LEFT;
						boss.boss1->bossReacting = sfTrue;
					}
					else if (trackPosition.x < ARENA_CENTER)
					{
						boss.boss1->bossReactionToPlayer = GO_RIGHT;
						boss.boss1->bossReacting = sfTrue;
					}
					boss.boss1->runAwayTiming = 0;
				}
			}
			if (distance.x < TARGET_DISTANCE_MAX && distance.y > 80)
			{
				boss.boss1->playerPositionToBoss = PLAYER_ON_TOP;
				boss.boss1->runAwayTiming += _dt * DT_SLOW;
				if (boss.boss1->runAwayTiming >= RUNAWAY_TIMER)
				{
					if (trackPosition.x > ARENA_CENTER)
					{
						boss.boss1->bossReactionToPlayer = GO_LEFT;
						boss.boss1->bossReacting = sfTrue;
					}
					else if (trackPosition.x < ARENA_CENTER)
					{
						boss.boss1->bossReactionToPlayer = GO_RIGHT;
						boss.boss1->bossReacting = sfTrue;
					}
					boss.boss1->runAwayTiming = 0;
				}
			}
			else if (distance.x < TARGET_DISTANCE_MAX && distance.y > 30)
			{
				if (playerPos.x < trackPosition.x)
				{
					boss.boss1->playerPositionToBoss = PLAYER_TURRET_LEFT;
					boss.boss1->runAwayTiming += _dt * DT_SLOW;
					if (boss.boss1->runAwayTiming >= RUNAWAY_TIMER)
					{
						if (trackPosition.x > ARENA_CENTER)
						{
							boss.boss1->bossReactionToPlayer = GO_LEFT;
							boss.boss1->bossReacting = sfTrue;
						}
						else if (trackPosition.x < ARENA_CENTER)
						{
							boss.boss1->bossReactionToPlayer = GO_RIGHT;
							boss.boss1->bossReacting = sfTrue;
						}
						boss.boss1->runAwayTiming = 0;
					}
				}
				else if (playerPos.x > trackPosition.x)
				{
					boss.boss1->playerPositionToBoss = PLAYER_TURRET_RIGHT;
					boss.boss1->runAwayTiming += _dt;
					if (boss.boss1->runAwayTiming >= RUNAWAY_TIMER)
					{
						if (trackPosition.x > ARENA_CENTER)
						{
							boss.boss1->bossReactionToPlayer = GO_LEFT;
							boss.boss1->bossReacting = sfTrue;
						}
						else if (trackPosition.x < ARENA_CENTER)
						{
							boss.boss1->bossReactionToPlayer = GO_RIGHT;
							boss.boss1->bossReacting = sfTrue;
						}
						boss.boss1->runAwayTiming = 0;
					}
				}
			}

		}
	}
}

void UpdateTurret(float _dt)
{
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

void UpdateBossReaction(float _dt)
{
	switch (boss.boss1->bossReactionToPlayer)
	{
	case GO_LEFT:
		if (sfSprite_getPosition(boss.boss1->track).x > ARENA_LIMITE_LEFT && boss.boss1->bossReacting)
		{
			MoveBoss1((sfVector2f) { -(BOSS_SPEED_RUNAWAY * _dt), 0 });
		}
		else if (sfSprite_getPosition(boss.boss1->track).x > ARENA_LIMITE_LEFT)
		{
			MoveBoss1((sfVector2f) { -(BOSS_SPEED * _dt), 0 });
		}
		else
		{
			boss.boss1->bossReactionToPlayer = NONE;
			if (boss.boss1->bossReacting)
			{
				boss.boss1->bossReacting = sfFalse;
			}
		}
		break;
	case GO_RIGHT:
		if (sfSprite_getPosition(boss.boss1->track).x < ARENA_LIMITE_RIGHT && boss.boss1->bossReacting)
		{
			MoveBoss1((sfVector2f) { (BOSS_SPEED_RUNAWAY * _dt), 0 });
		}
		else if (sfSprite_getPosition(boss.boss1->track).x < ARENA_LIMITE_RIGHT)
		{
			MoveBoss1((sfVector2f) { (BOSS_SPEED * _dt), 0 });
		}
		else
		{
			boss.boss1->bossReactionToPlayer = NONE;
			if (boss.boss1->bossReacting)
			{
				boss.boss1->bossReacting = sfFalse;
			}
		}
		break;
	default:
		break;
	}
}

void BossShoot(float _dt)
{
	if (boss.boss1->cooldownShoot <= 0)
	{
		boss.boss1->cooldownShoot += 1.f / BOSS_FIRERATE;
		if (boss.boss1->playerPositionToBoss == PLAYER_RANGE_SHOOT_LEFT)
		{
			ShooterType shooterType = { 0 };
			shooterType.shootPosition.x = 0.f;
			shooterType.shootPosition.y = 0.f;
			shooterType.bulletType = LIGHT;
			shooterType.isRighted = sfTrue;
			shooterType.isAlly = sfFalse;
			shooterType.weaponPos = 0;

			sfVector2f playerPos = GetPlayerPosition();

			playerPos.y -= TILE_SIZE;

			AddBullet(sfSprite_getPosition(boss.boss1->spriteTurretLCanon), playerPos, shooterType);
		}
		else if (boss.boss1->playerPositionToBoss == PLAYER_RANGE_SHOOT_RIGHT)
		{
			ShooterType shooterType = { 0 };
			shooterType.shootPosition.x = 0.f;
			shooterType.shootPosition.y = 0.f;
			shooterType.bulletType = LIGHT;
			shooterType.isRighted = sfTrue;
			shooterType.isAlly = sfFalse;
			shooterType.weaponPos = 0;

			sfVector2f playerPos = GetPlayerPosition();

			playerPos.y -= TILE_SIZE;

			AddBullet(sfSprite_getPosition(boss.boss1->spriteTurretRCanon), playerPos, shooterType);
		}
	}
	else
	{
		boss.boss1->cooldownShoot -= _dt;
	}
}

void DestroyBoss(void)
{
	DestroyVisualEntity(boss.boss1->track);
	DestroyVisualEntity(boss.boss1->gunCariage);
	DestroyVisualEntity(boss.boss1->steamTank);
	DestroyVisualEntity(boss.boss1->spriteTurretLCase);
	DestroyVisualEntity(boss.boss1->spriteTurretLBase);
	DestroyVisualEntity(boss.boss1->spriteTurretLCanon);
	DestroyVisualEntity(boss.boss1->spriteTurretRCase);
	DestroyVisualEntity(boss.boss1->spriteTurretRBase);
	DestroyVisualEntity(boss.boss1->spriteTurretRCanon);
	Free(boss.boss1);
	boss.boss1 = NULL;
	boss.boss1->state = sfFalse;
}