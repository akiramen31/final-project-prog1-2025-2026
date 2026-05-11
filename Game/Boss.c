#include "Boss.h"

void MoveBoss(sfVector2f _move);
void UpdateBossReaction(float _dt);
void FreezeBoss(void);

Boss boss;

void LoadBoss(int _index, sfVector2f _position)
{
	if (boss.boss1 != NULL)
	{
		DestroyBoss();
	}
	boss = (Boss){ 0 };
	boss.boss1 = Calloc(1, sizeof(Boss1));
	boss.timerFrozen = -1;
	boss.life = -1;

	boss.currentBoss = _index + 1;
	switch (boss.currentBoss)
	{
	case 1:
		boss.boss1 = Calloc(1, sizeof(Boss1));
		sfTexture* textureList[PART_COUNT - 2];
		textureList[TRACK] = GetAsset("Assets/Sprites/tank_track.png");
		textureList[STEAM_TANK] = GetAsset("Assets/Sprites/tank_steamtank.png");
		textureList[CARIAGE] = GetAsset("Assets/Sprites/tank_carriage.png");
		textureList[MISSILE_LAUNCHER] = GetAsset("Assets/Sprites/tank_missile_launcher.png");
		textureList[L_CHAMBER] = GetAsset("Assets/Sprites/tank_chamber.png");
		textureList[L_CANNON] = GetAsset("Assets/Sprites/tank_cannon.png");
		sfVector2f positionList[PART_COUNT];
		positionList[TRACK] = _position;
		positionList[STEAM_TANK] = (sfVector2f){ _position.x, _position.y - sfTexture_getSize(textureList[TRACK]).y };
		positionList[CARIAGE] = (sfVector2f){ _position.x, _position.y - (sfTexture_getSize(textureList[TRACK]).y / 3) };
		positionList[MISSILE_LAUNCHER] = (sfVector2f){ _position.x, positionList[CARIAGE].y - sfTexture_getSize(textureList[CARIAGE]).y };
		positionList[L_CHAMBER] = (sfVector2f){ _position.x - (sfTexture_getSize(textureList[L_CHAMBER]).x / 2) - (sfTexture_getSize(textureList[STEAM_TANK]).x/2), positionList[CARIAGE].y - 10 };
		positionList[L_CANNON] = (sfVector2f){ positionList[L_CHAMBER].x - (sfTexture_getSize(textureList[L_CHAMBER]).x / 2), positionList[L_CHAMBER].y - (sfTexture_getSize(textureList[L_CHAMBER]).y / 2) };
		positionList[R_CHAMBER] = (sfVector2f){ _position.x + (sfTexture_getSize(textureList[L_CHAMBER]).x / 2) + (sfTexture_getSize(textureList[STEAM_TANK]).x/2), positionList[CARIAGE].y - 10 };
		positionList[R_CANNON] = (sfVector2f){ positionList[R_CHAMBER].x + (sfTexture_getSize(textureList[L_CHAMBER]).x / 2), positionList[L_CANNON].y};
		for (int i = 0; i < PART_COUNT; i++)
		{
			if (i > PART_COUNT - 3)
			{
				boss.boss1->sprites[i] = CreateSprite(textureList[i - 2], positionList[i], 1.f, 10.f + (0.1 * i));
			}
			else
			{
				boss.boss1->sprites[i] = CreateSprite(textureList[i], positionList[i], 1.f, 10.f + (0.1 * i));
			}
			SetSpriteOriginFoot(boss.boss1->sprites[i]);
		}
		boss.boss1->playerPositionToBoss = NOT_IN_ARENA;
		boss.boss1->cooldownBullet = 1.f / BOSS_FIRERATE_BULLET;
		boss.boss1->cooldownBallistic = 1.f / BOSS_FIRERATE_DRONE;
		boss.boss1->hitboxes[0] = sfSprite_getGlobalBounds(boss.boss1->sprites[STEAM_TANK]);
		boss.boss1->hitboxes[1] = sfSprite_getGlobalBounds(boss.boss1->sprites[L_CHAMBER]);
		boss.boss1->hitboxes[2] = sfSprite_getGlobalBounds(boss.boss1->sprites[R_CHAMBER]);
		boss.boss1->hitboxes[3] = sfSprite_getGlobalBounds(boss.boss1->sprites[MISSILE_LAUNCHER]);

		//sfSprite_setScale(boss.boss1->sprites[CARIAGE], (sfVector2f) { 0 });
		break;
	default:
		break;
	}
}

void UpdateBoss(sfVector2f _posPlayer, float _dt)
{
	if (boss.timerFrozen != -1)
	{
		boss.timerFrozen += _dt;
		if (boss.timerFrozen < FREEZE_TIMER)
		{
			switch (boss.currentBoss)
			{
			case 1:
				for (int i = PART_COUNT - 1; i >= 0; i--)
				{
					sfSprite_setColor(boss.boss1->sprites[i], (sfColor) { 255, 255, 255, 255 });
				}
				break;
			default:
				break;
			}
		}
	}
	sfFloatRect playerRect = GetPlayerRect();
	sfVector2f push;
	CheckBossPlayerState(_posPlayer, _dt);
	if (boss.life != -1)
	{
		switch (boss.currentBoss)
		{
		case 1:
		{
			push = TestCollisionBossPlayer(playerRect, &boss.boss1->hitboxes, 3, AXIS_BOTH);
			UpdateTurret(_posPlayer, _dt);
			break;
		}
		default:
			break;
		}
		if (DEV_BOSS)
		{
			if (sfKeyboard_isKeyPressed(sfKeyM))
			{
				MoveBoss((sfVector2f) { 2.f, 0.f });
			}
			if (sfKeyboard_isKeyPressed(sfKeyL))
			{
				MoveBoss((sfVector2f) { -2.f, 0.f });
			}
		}
		HandlePlayerBossCollision(push);
		UpdateBossReaction(_dt);

		BossShoot(_posPlayer, _dt);
	}
}

void MoveBoss(sfVector2f _move)
{
	switch (boss.currentBoss)
	{
	case 1:
		for (int i = 0; i < PART_COUNT; i++)
		{
			sfSprite_move(boss.boss1->sprites[i], _move);
		}
		for (int i = 0; i < 4; i++)
		{
			boss.boss1->hitboxes[i].left += _move.x;
			boss.boss1->hitboxes[i].top += _move.y;
		}
		break;
	default:
		break;
	}
}

sfBool HitBoss(float _damage, sfFloatRect _hitbox, AttackType _attackType)
{
	if (boss.life != -1)
	{
		switch (boss.currentBoss)
		{
		case 1:
			for (unsigned i = 0; i < 4; i++)
			{
				if (sfFloatRect_intersects(&_hitbox, &boss.boss1->hitboxes[i], NULL))
				{
					if (_attackType == FREEZE)
					{
						if (boss.timerFrozen != -1)
						{
							boss.timerFrozen = 0.f;
						}
						else
						{
							FreezeBoss();
							return sfTrue;
						}
					}
					if (i < 1)
					{
						return DamageBoss(_damage);
					}
					if (i >= 1)
					{
						if (_attackType == MEDIUM || _attackType == HEAVY)
						{
							return DamageBoss(_damage);
						}
						return sfTrue;
					}
				}
			}
			break;
		default:
			break;
		}
	}
	return sfFalse;
}

sfBool DamageBoss(float _damage)
{
	boss.life -= _damage;
	if (sfSprite_getPosition(boss.boss1->sprites[TRACK]).x > ARENA_CENTER)
	{
		if (boss.boss1->playerPositionToBoss == SHOT_RANGE_LEFT || boss.boss1->playerPositionToBoss == SHOT_RANGE_RIGHT)
		{
			boss.boss1->bossReactionToPlayer = SLOW_LEFT;
			boss.boss1->bossReacting = sfTrue;
		}
	}
	else if (sfSprite_getPosition(boss.boss1->sprites[TRACK]).x <= ARENA_CENTER)
	{
		if (boss.boss1->playerPositionToBoss == SHOT_RANGE_LEFT || boss.boss1->playerPositionToBoss == SHOT_RANGE_RIGHT)
		{
			boss.boss1->bossReactionToPlayer = SLOW_RIGHT;
			boss.boss1->bossReacting = sfTrue;
		}
	}
	if (boss.life <= 0)
	{
		DestroyBoss();
	}
	return sfTrue;
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

void CheckBossPlayerState(sfVector2f _posPlayer, float _dt)
{
	switch (boss.currentBoss)
	{
	case 1:
	{
		sfVector2f trackPosition = sfSprite_getPosition(boss.boss1->sprites[TRACK]);

		sfVector2f posFar = (trackPosition.x > _posPlayer.x) ? trackPosition : _posPlayer;
		sfVector2f posClose = (trackPosition.x <= _posPlayer.x) ? trackPosition : _posPlayer;

		sfVector2f distance = (sfVector2f){ posFar.x - posClose.x, trackPosition.y - _posPlayer.y };

		if (boss.life == -1)
		{
			if (boss.boss1->playerPositionToBoss == NOT_IN_ARENA && _posPlayer.x > ARENA_ENTRY)
			{
				boss.boss1->playerPositionToBoss = AWAY_LEFT;
				boss.life = MAX_BOSS_0_LIFE;
			}
		}
		else if (!boss.boss1->bossReacting)
		{
			if (boss.boss1->playerPositionToBoss != NOT_IN_ARENA)
			{
				if (distance.x > SHOOT_DISTANCE_MAX && _posPlayer.x < trackPosition.x)
				{
					boss.boss1->playerPositionToBoss = AWAY_LEFT;
					boss.boss1->bossReactionToPlayer = SLOW_LEFT;
				}
				else if (distance.x > SHOOT_DISTANCE_MAX && _posPlayer.x > trackPosition.x)
				{
					boss.boss1->playerPositionToBoss = AWAY_RIGHT;
					boss.boss1->bossReactionToPlayer = SLOW_RIGHT;
				}
				if (distance.x < SHOOT_DISTANCE_MAX && distance.x > SHOOT_DISTANCE_MIN)
				{
					if (_posPlayer.x < trackPosition.x)
					{
						boss.boss1->playerPositionToBoss = SHOT_RANGE_LEFT;
						boss.boss1->bossReactionToPlayer = NONE;

					}
					else if (_posPlayer.x > trackPosition.x)
					{
						boss.boss1->playerPositionToBoss = SHOT_RANGE_RIGHT;
						boss.boss1->bossReactionToPlayer = NONE;
					}
				}
				if (distance.x < 48 && distance.y < 32)
				{
					boss.boss1->playerPositionToBoss = UNDER;
					boss.boss1->runAwayTiming += _dt;
					if (boss.boss1->runAwayTiming >= RUNAWAY_TIMER)
					{
						if (trackPosition.x > ARENA_CENTER)
						{
							boss.boss1->bossReactionToPlayer = SLOW_LEFT;
							boss.boss1->bossReacting = sfTrue;
						}
						else if (trackPosition.x <= ARENA_CENTER)
						{
							boss.boss1->bossReactionToPlayer = SLOW_RIGHT;
							boss.boss1->bossReacting = sfTrue;
						}
						boss.boss1->runAwayTiming = 0;
					}
				}
				if (distance.x < SHOOT_DISTANCE_MAX && distance.y > 80)
				{
					boss.boss1->playerPositionToBoss = TOP;
					boss.boss1->runAwayTiming += _dt;
					if (boss.boss1->runAwayTiming >= RUNAWAY_TIMER)
					{
						if (trackPosition.x > ARENA_CENTER)
						{
							boss.boss1->bossReactionToPlayer = SLOW_LEFT;
							boss.boss1->bossReacting = sfTrue;
						}
						else if (trackPosition.x <= ARENA_CENTER)
						{
							boss.boss1->bossReactionToPlayer = SLOW_RIGHT;
							boss.boss1->bossReacting = sfTrue;
						}
						boss.boss1->runAwayTiming = 0;
					}
				}
				else if (distance.x < SHOOT_DISTANCE_MIN && distance.y > 30)
				{
					if (_posPlayer.x < trackPosition.x)
					{
						boss.boss1->playerPositionToBoss = TURRET_LEFT;
						boss.boss1->runAwayTiming += _dt;
						if (boss.boss1->runAwayTiming >= RUNAWAY_TIMER)
						{
							if (trackPosition.x > ARENA_CENTER)
							{
								boss.boss1->bossReactionToPlayer = SLOW_LEFT;
								boss.boss1->bossReacting = sfTrue;
							}
							else if (trackPosition.x <= ARENA_CENTER)
							{
								boss.boss1->bossReactionToPlayer = SLOW_RIGHT;
								boss.boss1->bossReacting = sfTrue;
							}
							boss.boss1->runAwayTiming = 0;
						}
					}
					else if (_posPlayer.x > trackPosition.x)
					{
						boss.boss1->playerPositionToBoss = TURRET_RIGHT;
						boss.boss1->runAwayTiming += _dt;
						if (boss.boss1->runAwayTiming >= RUNAWAY_TIMER)
						{
							if (trackPosition.x > ARENA_CENTER)
							{
								boss.boss1->bossReactionToPlayer = SLOW_LEFT;
								boss.boss1->bossReacting = sfTrue;
							}
							else if (trackPosition.x <= ARENA_CENTER)
							{
								boss.boss1->bossReactionToPlayer = SLOW_RIGHT;
								boss.boss1->bossReacting = sfTrue;
							}
							boss.boss1->runAwayTiming = 0;
						}
					}
				}

			}
		}
	}
	break;
	default:
		break;
	}

}

void UpdateTurret(sfVector2f _posPlayer, float _dt)
{
	sfVector2f playerPos = _posPlayer;
	sfVector2f posL = sfSprite_getPosition(boss.boss1->sprites[L_CANNON]);

	//// Sécurité : Si le joueur est trop proche, on ne change pas l'angle
	//float dist = sqrtf(powf(playerPos.x - posL.x, 2) + powf(playerPos.y - posL.y, 2));
	//if (dist < 10.0f) return;

	//// --- TOURELLE GAUCHE ---
	//// 1. Calcul de la cible
	//float angleTargetL = atan2f(-(playerPos.y - posL.y), -(playerPos.x - posL.x)) * (180.0f / 3.14159f);

	//// 2. CLAMP de la cible d'abord !
	//if (angleTargetL > 180.f) angleTargetL = 180.f;
	//if (angleTargetL < 0.F) angleTargetL = 0.f;

	//// 3. Interpolation avec l'angle actuel
	//float currentAngleL = sfSprite_getRotation(boss.boss1->sprites[L_CANNON]);
	//float newAngleL = MoveTowardsAngle(currentAngleL, angleTargetL, TURRET_ROTATION_SPEED, _dt);

	//sfSprite_setRotation(boss.boss1->sprites[L_CANNON], newAngleL);

	//// --- TOURELLE DROITE ---
	sfVector2f posR = sfSprite_getPosition(boss.boss1->sprites[R_CANNON]);
	//float angleTargetR = atan2f(playerPos.y - posR.y, playerPos.x - posR.x) * (180.0f / 3.14159f);

	//// CLAMP de la cible
	//if (angleTargetR > 90.0f) angleTargetR = 90.0f;
	//if (angleTargetR < -90.0f) angleTargetR = -90.0f;

	//float currentAngleR = sfSprite_getRotation(boss.boss1->sprites[R_CANNON]);
	//float newAngleR = MoveTowardsAngle(currentAngleR, angleTargetR, TURRET_ROTATION_SPEED, _dt);


	if (playerPos.x < sfSprite_getPosition(boss.boss1->sprites[TRACK]).x)
	{
		float realDxLeft = playerPos.x - posL.x;
		float realDyLeft = playerPos.y - posL.y;
		float realAngleLeft = RAD_DEG(atan2f(realDyLeft, realDxLeft));
		sfSprite_setRotation(boss.boss1->sprites[L_CANNON], realAngleLeft + 90);
	}
	else
	{
		float realDxRight = playerPos.x - posR.x;
		float realDyRight = playerPos.y - posR.y;
		float realAngleRight = RAD_DEG(atan2f(realDyRight, realDxRight));
		sfSprite_setRotation(boss.boss1->sprites[R_CANNON], realAngleRight + 90);
	}
}

void UpdateBossReaction(float _dt)
{
	switch (boss.boss1->bossReactionToPlayer)
	{
	case SLOW_LEFT:
		if (sfSprite_getPosition(boss.boss1->sprites[TRACK]).x > ARENA_LIMITE_LEFT)
		{
			if (boss.boss1->bossReacting)
			{
				MoveBoss((sfVector2f) { -(BOSS_SPEED_RUNAWAY * _dt), 0 });
			}
			else
			{
				MoveBoss((sfVector2f) { -(BOSS_SPEED * _dt), 0 });
			}
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
	case SLOW_RIGHT:
		if (sfSprite_getPosition(boss.boss1->sprites[TRACK]).x < ARENA_LIMITE_RIGHT)
		{
			if (boss.boss1->bossReacting)
			{
				MoveBoss((sfVector2f) { (BOSS_SPEED_RUNAWAY * _dt), 0 });
			}
			else
			{
				MoveBoss((sfVector2f) { (BOSS_SPEED * _dt), 0 });
			}
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

void BossShoot(sfVector2f _posPlayer, float _dt)
{
	switch (boss.currentBoss)
	{
	case 1:
		if (boss.boss1->cooldownBullet <= 0)
		{
			boss.boss1->cooldownBullet += 1.f / BOSS_FIRERATE_BULLET;
			ShooterType shooterType = { 0 };
			shooterType.bulletType = LIGHT;
			shooterType.isRighted = sfTrue;
			shooterType.isAlly = sfFalse;
			sfVector2f playerPos = _posPlayer;
			playerPos.y -= TILE_SIZE;
			AddBullet(sfSprite_getPosition(boss.boss1->sprites[5 + ((boss.boss1->playerPositionToBoss % 2) * 2)]), playerPos, shooterType);
		}
		else
		{
			boss.boss1->cooldownBullet -= _dt;
		}
		if (boss.boss1->cooldownBallistic <= 0)
		{
			if (boss.boss1->playerPositionToBoss == SHOT_RANGE_LEFT || boss.boss1->playerPositionToBoss == SHOT_RANGE_RIGHT || boss.boss1->playerPositionToBoss == AWAY_LEFT || boss.boss1->playerPositionToBoss == AWAY_RIGHT)
			{
				boss.boss1->cooldownBallistic += 1.f / BOSS_FIRERATE_BULLET;
				SpawnBossMissile(sfSprite_getPosition(boss.boss1->sprites[MISSILE_LAUNCHER]), _posPlayer.x);
			}
		}
		else
		{
			boss.boss1->cooldownBallistic -= _dt;
		}
		break;
	default:
		break;
	}
}

void DestroyBoss(void)
{
	for (int i = 0; i < PART_COUNT; i++)
	{
		DestroyVisualEntity(boss.boss1->sprites[i]);
	}
	for (int i = 0; i < 4; i++)
	{
		boss.boss1->hitboxes[i] = (sfFloatRect){ 0 };
	}
	boss.life = -1;
	boss.timerFrozen = -1.f;
}

void FreezeBoss(void)
{
	for (int i = PART_COUNT - 1; i >= 0; i--)
	{
		sfSprite_setColor(boss.boss1->sprites[i], (sfColor) { 180, 180, 255, 255 });
	}
	boss.timerFrozen = 0.f;
}

float GetBossLife(void)
{
	return boss.life;
}

float* GetBossLifeAdress(void)
{
	return &boss.life;
}

sfVector2f GetBossPosition(void)
{
	switch (boss.currentBoss)
	{
	case 1:
		return sfSprite_getPosition(boss.boss1->sprites[TRACK]);
		break;
	default:
		break;
	}
}

sfBool IsBossActive(void)
{
	if (boss.life == -1)
	{
		return sfFalse;
	}
	else
	{
		return sfTrue;
	}
}

