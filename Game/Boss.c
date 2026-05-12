#include "Boss.h"

void MoveBoss(sfVector2f _move);
void UpdateBossReaction(float _dt);
void FreezeBoss(void);

Boss boss;

void LoadBoss(int _index, sfVector2f _position)
{

	boss = (Boss){ 0 };
	boss.currentBoss = _index + 1;
	switch (boss.currentBoss)
	{
	case 1:
		if (boss.boss1 != NULL)
		{
			DestroyBoss(1);
		}
		boss.boss1 = Calloc(1, sizeof(Boss1));
		sfTexture* textureList[PART_COUNT_BOSS1 - 2];
		textureList[TRACK] = GetAsset("Assets/Sprites/tank_track.png");
		textureList[STEAM_TANK_BOSS1] = GetAsset("Assets/Sprites/tank_steamtank.png");
		textureList[CARIAGE] = GetAsset("Assets/Sprites/tank_carriage.png");
		textureList[MISSILE_LAUNCHER] = GetAsset("Assets/Sprites/tank_missile_launcher.png");
		textureList[L_CHAMBER] = GetAsset("Assets/Sprites/tank_chamber.png");
		textureList[L_CANNON] = GetAsset("Assets/Sprites/tank_cannon.png");
		sfVector2f positionListBoss1[PART_COUNT_BOSS1];
		positionListBoss1[TRACK] = _position;
		positionListBoss1[STEAM_TANK_BOSS1] = (sfVector2f){ _position.x, _position.y - sfTexture_getSize(textureList[TRACK]).y };
		positionListBoss1[CARIAGE] = (sfVector2f){ _position.x, _position.y - (sfTexture_getSize(textureList[TRACK]).y / 2) };
		positionListBoss1[MISSILE_LAUNCHER] = (sfVector2f){ _position.x, positionListBoss1[CARIAGE].y - sfTexture_getSize(textureList[CARIAGE]).y };
		positionListBoss1[L_CHAMBER] = (sfVector2f){ _position.x - (sfTexture_getSize(textureList[L_CHAMBER]).x / 2) - (sfTexture_getSize(textureList[STEAM_TANK_BOSS1]).x / 2), positionListBoss1[CARIAGE].y };
		positionListBoss1[L_CANNON] = (sfVector2f){ positionListBoss1[L_CHAMBER].x - (sfTexture_getSize(textureList[L_CHAMBER]).x / 2), positionListBoss1[CARIAGE].y - 20 };
		positionListBoss1[R_CHAMBER] = (sfVector2f){ _position.x + (sfTexture_getSize(textureList[L_CHAMBER]).x / 2) + (sfTexture_getSize(textureList[STEAM_TANK_BOSS1]).x / 2), positionListBoss1[CARIAGE].y };
		positionListBoss1[R_CANNON] = (sfVector2f){ positionListBoss1[R_CHAMBER].x + (sfTexture_getSize(textureList[L_CHAMBER]).x / 2), positionListBoss1[CARIAGE].y - 20 };
		for (int i = 0; i < PART_COUNT_BOSS1; i++)
		{
			if (i > PART_COUNT_BOSS1 - 3)
			{
				boss.boss1->sprites[i] = CreateSprite(textureList[i - 2], positionListBoss1[i], 1.f, 10.f + (0.1 * i));
			}
			else
			{
				boss.boss1->sprites[i] = CreateSprite(textureList[i], positionListBoss1[i], 1.f, 10.f + (0.1 * i));
			}
			SetSpriteOriginFoot(boss.boss1->sprites[i]);
		}
		boss.boss1->cooldownBullet = 1.f / BOSS1_FIRERATE_BULLET;
		boss.boss1->cooldownBallistic = 1.f / BOSS1_FIRERATE_DRONE;
		boss.boss1->playerPositionToBoss1 = NOT_IN_ARENA1;
		boss.boss1->hitboxes[0] = sfSprite_getGlobalBounds(boss.boss1->sprites[STEAM_TANK_BOSS1]);
		boss.boss1->hitboxes[1] = sfSprite_getGlobalBounds(boss.boss1->sprites[L_CHAMBER]);
		boss.boss1->hitboxes[2] = sfSprite_getGlobalBounds(boss.boss1->sprites[R_CHAMBER]);
		boss.boss1->hitboxes[3] = sfSprite_getGlobalBounds(boss.boss1->sprites[MISSILE_LAUNCHER]);
		sfSprite_setRotation(boss.boss1->sprites[L_CANNON], 90);
		sfSprite_setRotation(boss.boss1->sprites[R_CANNON], -90);
		boss.life = -1;

		break;
	case 2:
		printf("Position du boss : %f et %f", _position.x, _position.y);
		boss.boss2 = Calloc(1, sizeof(Boss2));
		if (boss.boss2 != NULL)
		{
			DestroyBoss(2);
		}
		sfTexture* bossSpritesheet = GetAsset("Assets/Sprites/Boss_2.png");
		sfIntRect textureRectList[PART_COUNT_BOSS2 - 6];
		textureRectList[BODY] = (sfIntRect){ 16, 32, 80, 32 };
		textureRectList[STEAM_TANK_BOSS2] = (sfIntRect){ 0,32,16,16 };
		textureRectList[STEAM_TANK_COVERING] = (sfIntRect){ 64, 0, 18,2 };
		textureRectList[STEAM_TANK_CHIMNEY] = (sfIntRect){ 48, 0, 4,8 };
		textureRectList[TURRET_BASE] = (sfIntRect){ 0, 0, 20, 20 };
		textureRectList[TURRET_CANNON] = (sfIntRect){ 32, 0, 4, 8 };
		textureRectList[BOMB_BAY] = (sfIntRect){ 48, 18, 28, 7 };
		textureRectList[LEFT_ROTOR] = (sfIntRect){ 96, 32, 32, 32 };
		textureRectList[LEFT_KICKSTAND] = (sfIntRect){ 96, 0, 8, 32 };
		textureRectList[TOP_LEFT_CHIMNEY] = (sfIntRect){ 32, 16, 8, 4 };

		sfVector2f positionListBoss2[PART_COUNT_BOSS2];
		positionListBoss2[BODY] = _position;
		positionListBoss2[STEAM_TANK_BOSS2] = (sfVector2f){ _position.x, _position.y - 32.f };
		positionListBoss2[STEAM_TANK_COVERING] = (sfVector2f){ _position.x, _position.y - 48.f };
		positionListBoss2[STEAM_TANK_CHIMNEY] = (sfVector2f){ _position.x, _position.y - 50.f };
		positionListBoss2[TURRET_BASE] = (sfVector2f){ _position.x, _position.y - 6.f };
		positionListBoss2[TURRET_CANNON] = (sfVector2f){ _position.x, _position.y - 16.f };
		positionListBoss2[BOMB_BAY] = (sfVector2f){ _position.x, _position.y + 7.f };
		positionListBoss2[LEFT_ROTOR] = (sfVector2f){ _position.x - 56.f, _position.y };
		positionListBoss2[RIGHT_ROTOR] = (sfVector2f){ _position.x + 56.f, _position.y };
		positionListBoss2[LEFT_KICKSTAND] = (sfVector2f){ _position.x - 76.f, _position.y + 16.f };
		positionListBoss2[MIDDLE_KICKSTAND] = (sfVector2f){ positionListBoss2[LEFT_KICKSTAND].x + 76.f, positionListBoss2[LEFT_KICKSTAND].y };
		positionListBoss2[RIGHT_KICKSTAND] = (sfVector2f){ positionListBoss2[MIDDLE_KICKSTAND].x + 76.f, positionListBoss2[MIDDLE_KICKSTAND].y };
		positionListBoss2[TOP_LEFT_CHIMNEY] = (sfVector2f){ positionListBoss2[STEAM_TANK_BOSS2].x - 28.f, positionListBoss2[STEAM_TANK_BOSS2].y };
		positionListBoss2[TOP_RIGHT_CHIMNEY] = (sfVector2f){ positionListBoss2[TOP_LEFT_CHIMNEY].x + 60, positionListBoss2[TOP_LEFT_CHIMNEY].y };
		positionListBoss2[BOTTOM_LEFT_CHIMNEY] = (sfVector2f){ positionListBoss2[BOMB_BAY].x - 20, _position.y + 4 };
		positionListBoss2[BOTTOM_RIGHT_CHIMNEY] = (sfVector2f){ positionListBoss2[BOMB_BAY].x + 20, _position.y + 4 };
		for (int i = 0; i < PART_COUNT_BOSS2; i++)
		{
			if (i > PART_COUNT_BOSS2 - 4)		  // CHIMNEY
			{
				boss.boss2->sprites[i] = CreateSprite(bossSpritesheet, positionListBoss2[i], 1.f, 10.f + (0.1 * i));
				sfSprite_setTextureRect(boss.boss2->sprites[i], textureRectList[TOP_LEFT_CHIMNEY]);
			}
			else if (i > PART_COUNT_BOSS2 - 6)	  // KICKSTAND
			{
				boss.boss2->sprites[i] = CreateSprite(bossSpritesheet, positionListBoss2[i], 1.f, 10.f + (0.1 * i));
				sfSprite_setTextureRect(boss.boss2->sprites[i], textureRectList[LEFT_KICKSTAND]);
			}
			else if (i > PART_COUNT_BOSS2 - 7)	 // ROTOR
			{
				boss.boss2->sprites[i] = CreateSprite(bossSpritesheet, positionListBoss2[i], 1.f, 10.f + (0.1 * i));
				sfSprite_setTextureRect(boss.boss2->sprites[i], textureRectList[LEFT_ROTOR]);
			}
			else								 // NORMAL LOOP + DIE SEHR GROßE KANONE
			{
				boss.boss2->sprites[i] = CreateSprite(bossSpritesheet, positionListBoss2[i], 1.f, 10.f + (0.1 * i));
				sfSprite_setTextureRect(boss.boss2->sprites[i], textureRectList[i]);

			}
			SetSpriteOriginFoot(boss.boss2->sprites[i]);
		}
		boss.boss2->playerPositionToBoss2 = NOT_IN_ARENA2;
		boss.boss2->hitboxes[0] = sfSprite_getGlobalBounds(boss.boss2->sprites[BODY]);
		boss.boss2->hitboxes[1] = sfSprite_getGlobalBounds(boss.boss2->sprites[STEAM_TANK_COVERING]);
		boss.boss2->hitboxes[2] = sfSprite_getGlobalBounds(boss.boss2->sprites[STEAM_TANK_BOSS2]);
		boss.boss2->hitboxes[3] = sfSprite_getGlobalBounds(boss.boss2->sprites[BOMB_BAY]);
		boss.boss2->hitboxes[4] = sfSprite_getGlobalBounds(boss.boss2->sprites[RIGHT_ROTOR]);
		boss.boss2->hitboxes[5] = sfSprite_getGlobalBounds(boss.boss2->sprites[LEFT_ROTOR]);
		boss.boss2->hitboxes[6] = sfSprite_getGlobalBounds(boss.boss2->sprites[RIGHT_KICKSTAND]);
		boss.boss2->hitboxes[7] = sfSprite_getGlobalBounds(boss.boss2->sprites[LEFT_KICKSTAND]);
		boss.boss2->hitboxes[8] = sfSprite_getGlobalBounds(boss.boss2->sprites[STEAM_TANK_CHIMNEY]);
		boss.boss2->hitboxes[9] = sfSprite_getGlobalBounds(boss.boss2->sprites[TOP_LEFT_CHIMNEY]);
		boss.boss2->hitboxes[10] = sfSprite_getGlobalBounds(boss.boss2->sprites[TOP_RIGHT_CHIMNEY]);
		boss.boss2->hitboxes[11] = sfSprite_getGlobalBounds(boss.boss2->sprites[BOTTOM_LEFT_CHIMNEY]);
		boss.boss2->hitboxes[12] = sfSprite_getGlobalBounds(boss.boss2->sprites[BOTTOM_RIGHT_CHIMNEY]);

		boss.life = MAX_BOSS2_LIFE;
		boss.boss2->aimDestination = (sfVector2f){ GetBossPosition().x, GetBossPosition().y + 40.f };
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
				for (int i = PART_COUNT_BOSS1 - 1; i >= 0; i--)
				{
					sfSprite_setColor(boss.boss1->sprites[i], (sfColor) { 255, 255, 255, 255 });
				}
				break;
			case 2:
				for (int i = PART_COUNT_BOSS2 - 1; i >= 0; i--)
				{
					sfSprite_setColor(boss.boss2->sprites[i], (sfColor) { 255, 255, 255, 255 });
				}
				break;
			default:
				break;
			}
		}
	}
	sfFloatRect playerRect = GetPlayerRect();
	sfVector2f push = { 0 };
	CheckBossPlayerState(_posPlayer, _dt);
	if (boss.life != -1)
	{
		switch (boss.currentBoss)
		{
		case 1:
			//push = TestCollisionBossPlayer(playerRect, boss.boss1->hitboxes, 3, AXIS_BOTH);
			break;
		case 2:

			//push = TestCollisionBossPlayer(playerRect, boss.boss2->hitboxes, 8, AXIS_BOTH);
			UpdateTurret(_posPlayer, _dt);
			FloatingHandlerBoss2(_dt);
			//push = TestCollisionBossPlayer(playerRect, boss.boss2->hitboxes, 3, AXIS_BOTH);
			//UpdateTurret(_posPlayer, _dt);
			break;
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
		UpdateTurret(_posPlayer, _dt);
		HandlePlayerBossCollision(push);
		UpdateBossReaction(_dt);
		BossShoot(_posPlayer, _dt);
	}
	else if (_posPlayer.x > ARENA1_ENTRY)
	{
 		boss.life = MAX_BOSS1_LIFE;
	}
}

void MoveBoss(sfVector2f _move)
{
	switch (boss.currentBoss)
	{
	case 1:
		for (int i = 0; i < PART_COUNT_BOSS1; i++)
		{
			sfSprite_move(boss.boss1->sprites[i], _move);
		}
		for (int i = 0; i < 4; i++)
		{
			boss.boss1->hitboxes[i].left += _move.x;
			boss.boss1->hitboxes[i].top += _move.y;
		}
		break;
	case 2:
		for (int i = 0; i < PART_COUNT_BOSS2; i++)
		{
			sfSprite_move(boss.boss2->sprites[i], _move);
		}
		for (int i = 0; i < 14; i++)
		{
			boss.boss2->hitboxes[i].left += _move.x;
			boss.boss2->hitboxes[i].top += _move.y;
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
	switch (boss.currentBoss)
	{
	case 1:
		boss.life -= _damage;
		if (sfSprite_getPosition(boss.boss1->sprites[TRACK]).x > ARENA1_CENTER)
		{
			if (boss.boss1->playerPositionToBoss1 == SHOT_RANGE_LEFT || boss.boss1->playerPositionToBoss1 == SHOT_RANGE_RIGHT)
			{
				boss.boss1->boss1ReactionToPlayer = SLOW_LEFT;
				boss.boss1->boss1Reacting = sfTrue;
			}
		}
		else if (sfSprite_getPosition(boss.boss1->sprites[TRACK]).x <= ARENA1_CENTER)
		{
			if (boss.boss1->playerPositionToBoss1 == SHOT_RANGE_LEFT || boss.boss1->playerPositionToBoss1 == SHOT_RANGE_RIGHT)
			{
				boss.boss1->boss1ReactionToPlayer = SLOW_RIGHT;
				boss.boss1->boss1Reacting = sfTrue;
			}
		}
		if (boss.life <= 0)
		{
			if (boss.currentBoss == 1)
			{
				DestroyBoss(1);
			}
		}
		return sfTrue;
		break;
	case 2:
		break;
	default:
		break;
	}
}

sfVector2f ColisionBossplayer(sfFloatRect _playerHitbox)
{
	sfVector2f push = { 0 };

	if (boss.life != -1)
	{
		switch (boss.currentBoss)
		{
		case 1:

			push = TestCollisionBossPlayer(_playerHitbox, boss.boss1->hitboxes, 3);

			break;

		case 2:

			push = TestCollisionBossPlayer(_playerHitbox, boss.boss2->hitboxes, 8);
			break;
		default:
			break;
		}
	}

	return push;
}

sfVector2f TestCollisionBossPlayer(sfFloatRect _hitbox, sfFloatRect* _bossParts, int _partCount)
{
	sfVector2f vectorMove = { 0, 0 };
	sfFloatRect reaction = { 0, 0, 0, 0 };

	if (boss.currentBoss == 1)
	{
		for (int i = 0; i < _partCount; i++)
		{
			if (sfFloatRect_intersects(&_hitbox, &_bossParts[i], &reaction))
			{
				int resolveX = (reaction.width < reaction.height);

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
	}
	else if (boss.currentBoss == 2)
	{
		for (int i = 0; i < _partCount; i++)
		{
			if (i == 1)
			{
				continue;
			}
			else if (i == 6 || i == 7)
			{
				int resolveX = (reaction.width < reaction.height);

				if (resolveX)
				{
					if (sfFloatRect_intersects(&_hitbox, &_bossParts[i], &reaction))
					{
						if (_hitbox.top + _hitbox.height / 4.f * 3.f + vectorMove.y - _bossParts[i].top < (_bossParts[i].height - _hitbox.height / 2))
						{
							vectorMove.y -= reaction.height;
						}
						_hitbox.top += vectorMove.y;
					}
				}
			}
			else
			{
				if (sfFloatRect_intersects(&_hitbox, &_bossParts[i], &reaction))
				{
					int resolveX = (reaction.width < reaction.height);

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

		if (boss.boss1->playerPositionToBoss1 == NOT_IN_ARENA1 && _posPlayer.x > ARENA1_ENTRY)
		{
			boss.boss1->playerPositionToBoss1 = AWAY_LEFT;
		}
		if (!boss.boss1->boss1Reacting)
		{
			if (boss.boss1->playerPositionToBoss1 != NOT_IN_ARENA1)
			{
				if (distance.x > BOSS1_SHOOT_DISTANCE_MAX && _posPlayer.x < trackPosition.x)
				{
					boss.boss1->playerPositionToBoss1 = AWAY_LEFT;
					boss.boss1->boss1ReactionToPlayer = SLOW_LEFT;
				}
				else if (distance.x > BOSS1_SHOOT_DISTANCE_MAX && _posPlayer.x > trackPosition.x)
				{
					boss.boss1->playerPositionToBoss1 = AWAY_RIGHT;
					boss.boss1->boss1ReactionToPlayer = SLOW_RIGHT;
				}
				if (distance.x < BOSS1_SHOOT_DISTANCE_MAX && distance.x > BOSS1_SHOOT_DISTANCE_MIN)
				{
					if (_posPlayer.x < trackPosition.x)
					{
						boss.boss1->playerPositionToBoss1 = SHOT_RANGE_LEFT;
						boss.boss1->boss1ReactionToPlayer = NONE1;

					}
					else if (_posPlayer.x > trackPosition.x)
					{
						boss.boss1->playerPositionToBoss1 = SHOT_RANGE_RIGHT;
						boss.boss1->boss1ReactionToPlayer = NONE1;
					}
				}
				if (distance.x < 48 && distance.y < 32)
				{
					boss.boss1->playerPositionToBoss1 = UNDER;
					boss.boss1->runAwayTiming += _dt;
					if (boss.boss1->runAwayTiming >= BOSS1_RUNAWAY_TIMER)
					{
						if (trackPosition.x > ARENA1_CENTER)
						{
							boss.boss1->boss1ReactionToPlayer = SLOW_LEFT;
							boss.boss1->boss1Reacting = sfTrue;
						}
						else if (trackPosition.x <= ARENA1_CENTER)
						{
							boss.boss1->boss1ReactionToPlayer = SLOW_RIGHT;
							boss.boss1->boss1Reacting = sfTrue;
						}
						boss.boss1->runAwayTiming = 0;
					}
				}
				if (distance.x < BOSS1_SHOOT_DISTANCE_MAX && distance.y > 80)
				{
					boss.boss1->playerPositionToBoss1 = TOP;
					boss.boss1->runAwayTiming += _dt;
					if (boss.boss1->runAwayTiming >= BOSS1_RUNAWAY_TIMER)
					{
						if (trackPosition.x > ARENA1_CENTER)
						{
							boss.boss1->boss1ReactionToPlayer = SLOW_LEFT;
							boss.boss1->boss1Reacting = sfTrue;
						}
						else if (trackPosition.x <= ARENA1_CENTER)
						{
							boss.boss1->boss1ReactionToPlayer = SLOW_RIGHT;
							boss.boss1->boss1Reacting = sfTrue;
						}
					}
				}
				else if (distance.x < BOSS1_SHOOT_DISTANCE_MIN && distance.y > 30)
				{
					if (_posPlayer.x < trackPosition.x)
					{
						boss.boss1->playerPositionToBoss1 = TURRET_LEFT;
						boss.boss1->runAwayTiming += _dt;
						if (boss.boss1->runAwayTiming >= BOSS1_RUNAWAY_TIMER)
						{
							if (trackPosition.x > ARENA1_CENTER)
							{
								boss.boss1->boss1ReactionToPlayer = SLOW_LEFT;
								boss.boss1->boss1Reacting = sfTrue;
							}
							else if (trackPosition.x <= ARENA1_CENTER)
							{
								boss.boss1->boss1ReactionToPlayer = SLOW_RIGHT;
								boss.boss1->boss1Reacting = sfTrue;
							}
							boss.boss1->runAwayTiming = 0;
						}
					}
					if (distance.x < BOSS1_SHOOT_DISTANCE_MAX && distance.y > 80)
					{
						boss.boss1->playerPositionToBoss1 = TURRET_RIGHT;
						boss.boss1->runAwayTiming += _dt;
						if (boss.boss1->runAwayTiming >= BOSS1_RUNAWAY_TIMER)
						{
							if (trackPosition.x > ARENA1_CENTER)
							{
								boss.boss1->boss1ReactionToPlayer = SLOW_LEFT;
								boss.boss1->boss1Reacting = sfTrue;
							}
							else if (trackPosition.x <= ARENA1_CENTER)
							{
								boss.boss1->boss1ReactionToPlayer = SLOW_RIGHT;
								boss.boss1->boss1Reacting = sfTrue;
							}
							boss.boss1->runAwayTiming = 0;
						}
					}
					else if (distance.x < BOSS1_SHOOT_DISTANCE_MIN && distance.y > 30)
					{
						if (_posPlayer.x < trackPosition.x)
						{
							boss.boss1->playerPositionToBoss1 = TURRET_LEFT;
							boss.boss1->runAwayTiming += _dt;
							if (boss.boss1->runAwayTiming >= BOSS1_RUNAWAY_TIMER)
							{
								if (trackPosition.x > ARENA1_CENTER)
								{
									boss.boss1->boss1ReactionToPlayer = SLOW_LEFT;
									boss.boss1->boss1Reacting = sfTrue;
								}
								else if (trackPosition.x <= ARENA1_CENTER)
								{
									boss.boss1->boss1ReactionToPlayer = SLOW_RIGHT;
									boss.boss1->boss1Reacting = sfTrue;
								}
								boss.boss1->runAwayTiming = 0;
							}
						}
						else if (_posPlayer.x > trackPosition.x)
						{
							boss.boss1->playerPositionToBoss1 = TURRET_RIGHT;
							boss.boss1->runAwayTiming += _dt;
							if (boss.boss1->runAwayTiming >= BOSS1_RUNAWAY_TIMER)
							{
								if (trackPosition.x > ARENA1_CENTER)
								{
									boss.boss1->boss1ReactionToPlayer = SLOW_LEFT;
									boss.boss1->boss1Reacting = sfTrue;
								}
								else if (trackPosition.x <= ARENA1_CENTER)
								{
									boss.boss1->boss1ReactionToPlayer = SLOW_RIGHT;
									boss.boss1->boss1Reacting = sfTrue;
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
}

void UpdateTurret(sfVector2f _posPlayer, float _dt)
{
	if (boss.currentBoss == 1)
	{


		sfVector2f playerPos = _posPlayer;
		if (playerPos.x < sfSprite_getPosition(boss.boss1->sprites[TRACK]).x)
		{
			sfVector2f posL = sfSprite_getPosition(boss.boss1->sprites[L_CANNON]);
			float realDxLeft = playerPos.x - posL.x;
			float realDyLeft = playerPos.y - posL.y;
			float realAngleLeft = RAD_DEG(atan2f(realDyLeft, realDxLeft));
			sfSprite_setRotation(boss.boss1->sprites[L_CANNON], realAngleLeft + 90);
		}
		else
		{
			sfVector2f posR = sfSprite_getPosition(boss.boss1->sprites[R_CANNON]);
			float realDxRight = playerPos.x - posR.x;
			float realDyRight = playerPos.y - posR.y;
			float realAngleRight = RAD_DEG(atan2f(realDyRight, realDxRight));
			sfSprite_setRotation(boss.boss1->sprites[R_CANNON], realAngleRight + 90);
		}
	}
	else if (boss.currentBoss == 2)
	{
		sfVector2f playerPos = _posPlayer;
		sfVector2f posCannon = sfSprite_getPosition(boss.boss2->sprites[TURRET_CANNON]);
		float realDxLeft = playerPos.x - posCannon.x;
		float realDyLeft = playerPos.y - posCannon.y;
		float realAngleLeft = RAD_DEG(atan2f(realDyLeft, realDxLeft));
		sfSprite_setRotation(boss.boss2->sprites[TURRET_CANNON], realAngleLeft + 90.f);
	}
}

void UpdateBossReaction(float _dt)
{
	if (boss.currentBoss == 1)
	{
		switch (boss.boss1->boss1ReactionToPlayer)
		{
		case SLOW_LEFT:
			if (sfSprite_getPosition(boss.boss1->sprites[TRACK]).x > ARENA1_LIMITE_LEFT)
			{
				if (boss.boss1->boss1Reacting)
				{
					MoveBoss((sfVector2f) { -(BOSS1_SPEED_RUNAWAY * _dt), 0 });
				}
				else
				{
					MoveBoss((sfVector2f) { -(BOSS1_SPEED * _dt), 0 });
				}
			}
			else
			{
				boss.boss1->boss1ReactionToPlayer = NONE1;
				if (boss.boss1->boss1Reacting)
				{
					boss.boss1->boss1Reacting = sfFalse;
				}
			}
			break;
		case SLOW_RIGHT:
			if (sfSprite_getPosition(boss.boss1->sprites[TRACK]).x < ARENA1_LIMITE_RIGHT)
			{
				if (boss.boss1->boss1Reacting)
				{
					MoveBoss((sfVector2f) { (BOSS1_SPEED_RUNAWAY * _dt), 0 });
				}
				else
				{
					MoveBoss((sfVector2f) { (BOSS1_SPEED * _dt), 0 });
				}
			}
			else
			{
				boss.boss1->boss1ReactionToPlayer = NONE1;
				if (boss.boss1->boss1Reacting)
				{
					boss.boss1->boss1Reacting = sfFalse;
				}
			}
			break;
		default:
			break;
		}
	}
}

void BossShoot(sfVector2f _posPlayer, float _dt)
{
	switch (boss.currentBoss)
	{
	case 1:
		if (boss.boss1->playerPositionToBoss1 == SHOT_RANGE_LEFT || boss.boss1->playerPositionToBoss1 == SHOT_RANGE_RIGHT)
		{
			if (boss.boss1->cooldownBullet <= 0)
			{
				boss.boss1->cooldownBullet += 1.f / BOSS1_FIRERATE_BULLET;
				ShooterType shooterType = { 0 };
				shooterType.bulletType = LIGHT;
				shooterType.isRighted = sfTrue;
				shooterType.isAlly = sfFalse;
				sfVector2f playerPos = _posPlayer;
				playerPos.y -= TILE_SIZE;
				AddBullet(sfSprite_getPosition(boss.boss1->sprites[5 + ((boss.boss1->playerPositionToBoss1 % 2) * 2)]), playerPos, shooterType);
			}
			else
			{
				boss.boss1->cooldownBullet -= _dt;
			}
		}
		if (boss.boss1->cooldownBallistic <= 0)
		{
			if (boss.boss1->playerPositionToBoss1 == SHOT_RANGE_LEFT || boss.boss1->playerPositionToBoss1 == SHOT_RANGE_RIGHT || boss.boss1->playerPositionToBoss1 == AWAY_LEFT || boss.boss1->playerPositionToBoss1 == AWAY_RIGHT)
			{
				boss.boss1->cooldownBallistic += 1.f / BOSS1_FIRERATE_BULLET;
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

void FloatingHandlerBoss2(float _dt)
{
	if (!boss.boss2->boss2Reaction == RESTARTING || !boss.boss2->boss2Reaction == NONE2 || !boss.boss2->boss2Reaction == UNHIDE || !boss.boss2->boss2Reaction == DROP_PLAYER)
	{

	}
}

void DestroyBoss(int _boss)
{
	if (_boss == 1)
	{
		for (int i = 0; i < PART_COUNT_BOSS1; i++)
		{
			DestroyVisualEntity(boss.boss1->sprites[i]);
		}
		for (int i = 0; i < 4; i++)
		{
			boss.boss1->hitboxes[i] = (sfFloatRect){ 0 };
		}
	}
	else if (_boss == 2)
	{
		for (int i = 0; i < PART_COUNT_BOSS2; i++)
		{
			DestroyVisualEntity(boss.boss2->sprites[i]);
		}
		for (int i = 0; i < 14; i++)
		{
			boss.boss2->hitboxes[i] = (sfFloatRect){ 0 };
		}
	}
	boss.life = -1;
	boss.timerFrozen = -1.f;
}

void FreezeBoss(void)
{
	switch (boss.currentBoss)
	{
	case 1:
		for (int i = PART_COUNT_BOSS1 - 1; i >= 0; i--)
		{
			sfSprite_setColor(boss.boss1->sprites[i], (sfColor) { 180, 180, 255, 255 });
		}
		break;
	case 2:
		for (int i = PART_COUNT_BOSS2 - 1; i >= 0; i--)
		{
			sfSprite_setColor(boss.boss2->sprites[i], (sfColor) { 180, 180, 255, 255 });
		}
		break;
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

float GetBossMaxLife(void)
{
	switch (boss.currentBoss)
	{
	case 1:
		return MAX_BOSS1_LIFE;
		break;
	case 2:
		return MAX_BOSS2_LIFE;
		break;
	}
}

sfVector2f GetBossPosition(void)
{
	switch (boss.currentBoss)
	{
	case 1:
		return sfSprite_getPosition(boss.boss1->sprites[TRACK]);
		break;
	case 2:
		return sfSprite_getPosition(boss.boss2->sprites[BODY]);
		break;
	default:
		return (sfVector2f) { 0 };
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

