#include "Player.h"
#include "Entity.h"
#include "Enemy.h"
#include "Elevator.h"
#include "Boss.h"

Player player;

float timerDash = 0;
float timerFaling = 0;
float timerLastEnergyConso = 0;
float timerlastDamageReceive = PLAYER_DAMAGE_IMUNITY_DURATION;
float timerHit = 0;

sfBool playerInvicible = sfFalse;

void UpdateMovePlayer(sfBool _intro, float _dt);

void ColisionMapPlayer(float _dt);
void MoveZonePlayer(float _dt);

void UpdateAnimation(float _dt);

void UpdateCooldown(float _dt);
void UpdateFireControl(float _dt);
void UpdateFireControlRailgun(void);
void UpdateFireControlSteamAxe(float _dt);
void UpdateFireControlMisteal(void);

void UpdateSteamAxe(float _dt);
void UpdateEnergy(float _dt);

void UpdateWeaponPlayer(float _dt);

void UpdateLockPlayerInRoomIfEnemyAlive(void);

sfVector2f posFly;

sfRectangleShape* lockZone;

void LoadPlayer(void)
{
	player = (Player){ 0 };
	sfTexture* texture = GetAsset("Assets/Sprites/player_sprite_sheet.png");
	player.sprite = CreateSprite(texture, (sfVector2f) { 0, 0 }, 1.f, 40.f);
	texture = GetAsset("Assets/Sprites/player_punch.png");
	player.punch = CreateSprite(texture, (sfVector2f) { 0, 0 }, 1.f, 40.f);
	player.direction = sfTrue;

	player.collision = sfRectangleShape_create();
	sfRectangleShape_setSize(player.collision, (sfVector2f) { PLAYER_COLLISION_WIDTH, PLAYER_COLLISION_HEIGHT });
	sfRectangleShape_setPosition(player.collision, (sfVector2f) { 100, 32 });
	sfRectangleShape_setOrigin(player.collision, (sfVector2f) { PLAYER_COLLISION_WIDTH / 2, PLAYER_COLLISION_HEIGHT });

	lockZone = CreateRectangleShape((sfFloatRect) { 0 }, sfTransparent, (sfColor) { 203, 115, 51, 255 }, 60.f);
	sfRectangleShape_setOutlineThickness(lockZone, 8.f);

	player.running.frameCount = 8;
	player.running.frameDuration = 0.1f;
	player.running.isLooping = sfTrue;
	player.running.rectActualy = (sfIntRect){ 0,0,32,32 };

	player.jumping.frameCount = 2;
	player.jumping.frameDuration = 0.2f;
	player.jumping.isLooping = sfTrue;
	player.jumping.rectActualy = (sfIntRect){ 0,32,32,32 };

	player.falling.frameCount = 2;
	player.falling.frameDuration = 0.3f;
	player.falling.isLooping = sfTrue;
	player.falling.rectActualy = (sfIntRect){ 64,32,32,32 };

	player.idling.frameCount = 4;
	player.idling.frameDuration = 0.2f;
	player.idling.isLooping = sfTrue;
	player.idling.rectActualy = (sfIntRect){ 0,64,16,32 };

	player.dashing.frameCount = 1;
	player.dashing.frameDuration = 1.f;
	player.dashing.isLooping = sfFalse;
	player.dashing.rectActualy = (sfIntRect){ 0,96,32,32 };

	player.canShoot = sfTrue;
	player.cooldown = 1.f / FIRE_RATE_RAILGUN;
	player.isAttacking = sfFalse;
	player.pressTime = 0.f;

	player.hitCollisionSize = CreateRectangleShape((sfFloatRect) { 0, 0, 18, 16 }, sfTransparent, sfTransparent, 40.f);
	timerHit = PLAYER_HIT_COOLDOWN;
	player.punching.frameCount = 10;
	player.punching.frameDuration = PLAYER_HIT_COOLDOWN / player.punching.frameCount;
	player.punching.isLooping = sfFalse;
	player.punching.rectActualy = (sfIntRect){ 0,0,16,8 };

	posFly.x = 100;
	posFly.y = 32;

	player.lifeMax = PLAYER_MAX_HEALTH;
	player.life = player.lifeMax;

	player.ener.energyMax = 100;
	player.ener.energy = player.ener.energyMax;
	player.ener.energyRegen = 10;
	player.ener.energyRegenCooldown = 0.5f;
	player.ener.dashConsuption = 5.f;
}

void UpdatePlayer(sfBool _intro, float _dt)
{
	player.weapon = GetWeapon();

	if (!_intro)
	{
		UpdateWeaponPlayer(_dt);
	}
	UpdateEnergy(_dt);

	if (GetIntFromSave(DEV_MODE_FLY))
	{
		posFly = GetPlayerPosition();

		int flySpeed = 500;

		SetViewCenter(posFly);

		if (IfControlKeyPressed(KEY_RIGHT))
		{
			posFly.x += flySpeed * _dt;
		}
		else if (IfControlKeyPressed(KEY_LEFT))
		{
			posFly.x -= flySpeed * _dt;
		}
		if (IfControlKeyPressed(KEY_DOWN))
		{
			posFly.y += flySpeed * _dt;
		}
		else if (IfControlKeyPressed(KEY_JUMP))
		{
			posFly.y -= flySpeed * _dt;
		}
		sfRectangleShape_setPosition(player.collision, posFly);
		sfSprite_setScale(player.sprite, (sfVector2f) { 0 });
	}
	else
	{
		MoveZonePlayer(_dt);
		UpdateMovePlayer(_intro, _dt);
		UpdateLockPlayerInRoomIfEnemyAlive();
		UpdateAnimation(_dt);
		if (GetWeapon().weaponType !=NO_WEAPON)
		{
		MoveWeapon(GetPlayerPosition(), GetMousePositionToOrigin(), _dt, player.isAttacking);
		}
	}

	if (timerHit > PLAYER_HIT_COOLDOWN)
	{
		if (IfControlKeyPressed(KEY_HIT))
		{
			timerHit = 0;
			player.hitBoss = sfFalse;
			player.hitEnemy = sfFalse;

			if (player.weapon.isRight)
			{
				sfRectangleShape_setScale(player.hitCollisionSize, (sfVector2f) { 1, 1 });
				sfSprite_setScale(player.punch, (sfVector2f) { 1, 1 });
			}
			else
			{
				sfRectangleShape_setScale(player.hitCollisionSize, (sfVector2f) { -1, 1 });
				sfSprite_setScale(player.punch, (sfVector2f) { -1, 1 });
			}
		}
		else
		{
			sfRectangleShape_setScale(player.hitCollisionSize, (sfVector2f) { 0, 0 });
			sfSprite_setScale(player.punch, (sfVector2f) { 0, 0 });
		}
	}
	if (timerHit <= PLAYER_HIT_COOLDOWN)
	{
		timerHit += _dt;
		UpdateAnimationAndGiveIfStop(player.punch, &player.punching, _dt);

		sfVector2f posPlayer = GetPlayerPosition();
		posPlayer.y -= WEAPON_ORIGIN + 4;
		sfRectangleShape_setPosition(player.hitCollisionSize, posPlayer);
		sfSprite_setPosition(player.punch, posPlayer);

		sfFloatRect rect = sfRectangleShape_getGlobalBounds(player.hitCollisionSize);
		ColisionBox(rect, sfTrue, AXIS_BOTH);
		ColisionElevatorButon(rect);

		if (!player.hitBoss)
		{
			if (HitBoss(PLAYER_HIT_DAMAGE, rect, MEDIUM))
			{
				player.hitBoss = sfTrue;
			}
		}
		if (!player.hitEnemy)
		{
			if (HitEnemy(PLAYER_HIT_DAMAGE, rect, MEDIUM))
			{
				player.hitEnemy = sfTrue;
			}
		}
	}


	if (VerificationEntityIsNotInMap(GetPlayerRect()))
	{
		KillPlayer();
		SetPlayerPosition(player.spawn);
	}

	if (timerlastDamageReceive < PLAYER_DAMAGE_IMUNITY_DURATION)
	{
		timerlastDamageReceive += _dt;
	}
}

void UpdateWeaponPlayer(float _dt)
{
	if (player.weapon.weaponType == STEAMAXE)
	{
		UpdateSteamAxe(_dt);
	}
	UpdateCooldown(_dt);
	UpdateFireControl(_dt);
}

void UpdateLockPlayerInRoomIfEnemyAlive(void)
{
	sfFloatRect hitbox = GetPlayerRect();
	int num = GetTrigerCount();
	InfoZone* area = GetInfoZoneTriger(hitbox);

	sfFloatRect areaReaction = { 0 };
	if (area != NULL)
	{
		for (int i = 0; i < num; i++)
		{
			if (sfFloatRect_intersects(&hitbox, &area[i].hitbox, &areaReaction))
			{
				if (StringCompare(area[i].type, "Camera"))
				{
					if (GetEnemyZone() > 0 || IsBossActive())
					{
						//int width = area[i].hitbox.width / TILE_SIZE;
						//int height = area[i].hitbox.height / TILE_SIZE;

						sfRectangleShape_setScale(lockZone, (sfVector2f) { 1, 1 });
						sfRectangleShape_setSize(lockZone, (sfVector2f) { area[i].hitbox.width, area[i].hitbox.height });
						sfRectangleShape_setPosition(lockZone, (sfVector2f) { area[i].hitbox.left, area[i].hitbox.top });


						sfFloatRect velocity = GetPlayerRect();
						velocity.width -= areaReaction.width;
						velocity.height -= areaReaction.height;

						if (velocity.left < area[i].hitbox.left + area[i].hitbox.width / 2)
						{
							MovePlayer((sfVector2f) { velocity.width, 0 });
						}
						else
						{
							MovePlayer((sfVector2f) { -velocity.width, 0 });
						}

						if (velocity.top < area[i].hitbox.top + area[i].hitbox.height / 2)
						{
							MovePlayer((sfVector2f) { 0, velocity.height });
						}
						else
						{
							MovePlayer((sfVector2f) { 0, -velocity.height });
						}
					}
					else
					{
						sfRectangleShape_setScale(lockZone, (sfVector2f) { 0, 0 });
					}
				}
			}
		}
	}
}

void UpdateMovePlayer(sfBool _intro, float _dt)
{
	if (timerDash <= PLAYER_DASH_COOLDOWN)
	{
		timerDash += _dt;
	}

	if (timerDash >= PLAYER_DASH_DURATION)
	{
		player.velocity.x /= 1.8f;

		if (IfControlKeyPressed(KEY_RIGHT) && IfControlKeyPressed(KEY_LEFT))
		{
			player.velocity.x = 0;
		}
		else if (IfControlKeyPressed(KEY_RIGHT) && !_intro)
		{
			if (player.velocity.x <= 1)
			{
				player.velocity.x = 1;
			}

		}
		else if (IfControlKeyPressed(KEY_LEFT) && !_intro)
		{
			if (player.velocity.x >= -1)
			{
				player.velocity.x = -1;
			}
		}

		if (player.velocity.x > -0.1f && player.velocity.x < 0.1f)
		{
			player.velocity.x = 0;
		}

		if (player.velocity.x < 0)
		{
			player.direction = sfFalse;
		}
		else if (player.velocity.x > 0)
		{
			player.direction = sfTrue;
		}

		if (player.isGrounded == sfTrue || timerFaling < PLAYER_JUMP_FORGIVE)
		{
			player.velocity.y = 0;
			if (!(IfControlKeyPressed(KEY_JUMP) && (IfControlKeyPressed(KEY_DOWN))))
			{
				if (IfControlKeyPressed(KEY_JUMP) && !_intro)
				{
					PlaySound(PLAYER_JUMP);
					sfSprite_move(player.sprite, (sfVector2f) { 0, -10 });
					player.velocity.y -= PLAYER_JUMP_POWER;
					timerFaling += PLAYER_JUMP_FORGIVE;
					player.isGrounded = sfFalse;
				}
				else if (IfControlKeyPressed(KEY_DOWN) && !_intro)
				{
					player.velocity.y++;
				}
			}
		}
		if (player.isGrounded == sfFalse)
		{
			player.velocity.y += G * _dt;
			if (player.velocity.y > PLAYER_FALL_SPEED_MAX)
			{
				player.velocity.y = PLAYER_FALL_SPEED_MAX;
			}
		}
	}

	if (timerDash >= PLAYER_DASH_COOLDOWN && player.ener.energy > player.ener.dashConsuption && IfControlKeyPressed(KEY_DASH) && !_intro)
	{
		PlaySound(PLAYER_DASH);
		timerDash = 0;

		player.velocity.y /= 1.2f;

		player.ener.energy -= player.ener.dashConsuption;
		timerLastEnergyConso = 0;

		if (player.direction)
		{
			player.velocity.x = PLAYER_DASH_POWER;
		}
		else
		{
			player.velocity.x = -PLAYER_DASH_POWER;
		}
	}

	ColisionMapPlayer(_dt);
}

void ColisionMapPlayer(float _dt)
{
	sfRectangleShape_move(player.collision, (sfVector2f) { 0, PLAYER_VERTICAL_SPEED_MAX* player.velocity.y* _dt });
	sfVector2f reactionY = Colision(GetPlayerRect(), AXIS_Y);
	sfVector2f reactionPassThrough = CollisionPassThrough(GetPlayerRect());

	reactionY.y += ColisionBox(GetPlayerRect(), sfFalse, AXIS_Y).y;
	reactionY.y += ColisionElevator(GetPlayerRect(), AXIS_Y).y;
	reactionY.y += ColisionBossplayer(GetPlayerRect(), AXIS_Y).y;

	if (reactionPassThrough.y < 0)
	{
		if (player.velocity.y >= 0 && !IfControlKeyPressed(KEY_DOWN))
		{
			player.isGrounded = sfTrue;
			timerFaling = 0;
			player.velocity.y = 0;
			sfRectangleShape_move(player.collision, reactionPassThrough);
		}
	}
	else
	{
		if (reactionY.y < 0)
		{
			player.isGrounded = sfTrue;
			timerFaling = 0;
		}
		else if (reactionY.y >= 0)
		{
			player.isGrounded = sfFalse;
			if (timerFaling <= PLAYER_JUMP_FORGIVE && timerDash >= PLAYER_DASH_DURATION)
			{
				timerFaling += _dt;
			}
		}

		if (reactionY.y != 0 || reactionPassThrough.y != 0)
		{
			player.velocity.y = 0;
		}
		sfRectangleShape_move(player.collision, (sfVector2f) { 0, reactionY.y });
	}

	if (timerDash <= PLAYER_DASH_DURATION)
	{
		for (int i = 0; i < 10; i++)
		{
			sfRectangleShape_move(player.collision, (sfVector2f) { PLAYER_HORIZONTAL_SPEED_MAX* player.velocity.x* _dt / 10.0f, 0 });

			sfVector2f reactionX = Colision(sfRectangleShape_getGlobalBounds(player.collision), AXIS_X);
			reactionX.x += ColisionBox(sfRectangleShape_getGlobalBounds(player.collision), sfFalse, AXIS_X).x;
			reactionX.x += ColisionElevator(sfRectangleShape_getGlobalBounds(player.collision), AXIS_X).x;
			reactionX.x += ColisionBossplayer(GetPlayerRect(), AXIS_X).x;

			if (reactionX.x != 0)
			{
				// Push out of wall and halt X velocity immediately
				sfRectangleShape_move(player.collision, (sfVector2f) { reactionX.x, 0 });
				player.velocity.x = 0;
				break;
			}
		}
	}
	else
	{
		sfRectangleShape_move(player.collision, (sfVector2f) { PLAYER_HORIZONTAL_SPEED_MAX* player.velocity.x* _dt, 0 });

		sfVector2f reactionX = Colision(sfRectangleShape_getGlobalBounds(player.collision), AXIS_X);
		reactionX.x += ColisionBox(sfRectangleShape_getGlobalBounds(player.collision), sfFalse, AXIS_X).x;
		reactionX.x += ColisionBossplayer(GetPlayerRect(), AXIS_X).x;

		if (reactionX.x != 0)
		{
			sfRectangleShape_move(player.collision, (sfVector2f) { reactionX.x, 0 });
			player.velocity.x = 0;
		}
	}
}

void MoveZonePlayer(float _dt)
{
	InfoZone* zone = GetInfoZoneMove();
	int num = GetMoveCount();
	sfFloatRect hitbox = GetPlayerRect();
	int speed = 0;

	for (int i = 0; i < num; i++)
	{
		sscanf_s(zone[i].name, "%d", &speed);

		if (sfFloatRect_intersects(&hitbox, &zone[i].hitbox, NULL))
		{
			if (StringCompare(zone[i].type, "RIGHT"))
			{
				sfRectangleShape_move(player.collision, (sfVector2f) { speed* _dt, 0 });
			}
			else if (StringCompare(zone[i].type, "LEFT"))
			{
				sfRectangleShape_move(player.collision, (sfVector2f) { -speed * _dt, 0 });
			}
			else if (StringCompare(zone[i].type, "DOWN"))
			{
				sfRectangleShape_move(player.collision, (sfVector2f) { 0, speed* _dt });
			}
			else if (StringCompare(zone[i].type, "UP"))
			{
				sfRectangleShape_move(player.collision, (sfVector2f) { 0, -speed * _dt });
			}
		}
	}
}

void UpdateAnimation(float _dt)
{
	if (timerDash >= PLAYER_DASH_DURATION)
	{
		if (player.velocity.y == 0)
		{
			if (player.velocity.x != 0)
			{
				if (!player.running.timeActualy)
				{
					player.soundWalk = PlaySound(PLAYER_WALK);
				}
				else
				{
					StopSound(player.soundWalk);
				}
				UpdateAnimationAndGiveIfStop(player.sprite, &player.running, _dt);
			}
			else if (player.velocity.x == 0)
			{
				UpdateAnimationAndGiveIfStop(player.sprite, &player.idling, _dt);
			}
		}
		else if (player.velocity.y < 0)
		{
			UpdateAnimationAndGiveIfStop(player.sprite, &player.jumping, _dt);
		}
		else if (player.velocity.y > 0)
		{
			UpdateAnimationAndGiveIfStop(player.sprite, &player.falling, _dt);
		}

	}
	if (timerDash < PLAYER_DASH_DURATION)
	{
		UpdateAnimationAndGiveIfStop(player.sprite, &player.dashing, _dt);
	}

	if (timerlastDamageReceive < PLAYER_DAMAGE_IMUNITY_DURATION)
	{
		if (((int)(timerlastDamageReceive * 100) % 2) == 0)
		{
			sfSprite_setScale(player.sprite, (sfVector2f) { 0, 0 });
		}
		else
		{
			if (player.direction)
			{
				sfSprite_setScale(player.sprite, (sfVector2f) { 1, 1 });
			}
			else
			{
				sfSprite_setScale(player.sprite, (sfVector2f) { -1, 1 });
			}
		}
	}
	else
	{
		if (player.direction)
		{
			sfSprite_setScale(player.sprite, (sfVector2f) { 1, 1 });
		}
		else
		{
			sfSprite_setScale(player.sprite, (sfVector2f) { -1, 1 });
		}
	}
	SetSpriteOriginFoot(player.sprite);

	sfSprite_setPosition(player.sprite, GetPlayerPosition());
}

void DamagePlayer(int _damage)
{
	if (_damage > 0)
	{
		if (!playerInvicible)
		{
			if (timerDash >= PLAYER_DASH_COOLDOWN && timerlastDamageReceive >= PLAYER_DAMAGE_IMUNITY_DURATION)
			{
				PlaySound(PLAYER_HURT);
				player.life -= _damage;
				timerlastDamageReceive = 0;
			}
		}
	}
}

void KillPlayer(void)
{
	player.life = 0;
}

sfVector2f GetPlayerPosition(void)
{
	return sfRectangleShape_getPosition(player.collision);
}

sfVector2f GetPlayerCenterPosition(void)
{
	sfFloatRect rect = GetPlayerRect();
	return (sfVector2f) { rect.left + rect.width / 2, rect.top + rect.height / 2 };
}

void UpdateCooldown(float _dt)
{
	if (!player.canShoot)
	{
		player.cooldown -= _dt;
		if (player.cooldown < 0)
		{
			switch (player.weapon.weaponType)
			{
			case RAILGUN:
				player.cooldown += 1.f / FIRE_RATE_RAILGUN;
				break;
			case STEAMAXE:
				player.weapon.steamAxe.attackType = NOATTACK;
				player.cooldown += 1.f / FIRE_RATE_STEAMAXE;
				break;
			case MISTEEL:
				player.cooldown += 1.f / FIRE_RATE_MISTEAL;
				break;
			default:
				break;
			}
			player.canShoot = sfTrue;
			player.isAttacking = sfFalse;
		}
	}
}

void UpdateFireControl(float _dt)
{
	if (player.weapon.weaponType == RAILGUN)
	{
		if (IfControlKeyPressed(KEY_GUN))
		{
			if (player.canShoot)
			{
				UpdateFireControlRailgun();
				player.isAttacking = sfTrue;
				player.canShoot = sfFalse;
			}
		}
	}
	else if (player.weapon.weaponType == STEAMAXE)
	{
		UpdateFireControlSteamAxe(_dt);
	}
	else if (player.weapon.weaponType == MISTEEL)
	{
		if (IfControlKeyPressed(KEY_GUN))
		{
			if (player.canShoot)
			{
				UpdateFireControlMisteal();
				player.isAttacking = sfTrue;
				player.canShoot = sfFalse;
			}
		}
	}
	if (IfControlKeyPressed(KEY_SECOND))
	{
		if (player.canShoot)
		{
			if (player.weapon.secondaryData.secondaryType == DRONE)
			{
				if (player.ener.energy > 50.f)
				{
					AddDrone(GetPlayerPosition(), player.weapon.isRight);
					player.ener.energy -= 50.f;
					player.canShoot = sfFalse;

				}
			}
			else if (player.weapon.secondaryData.secondaryType == COLDBREATH)
			{
				if (player.ener.energy > 40.f)
				{
					ShooterType shooterType = { 0 };
					shooterType.weaponPos = WEAPON_ORIGIN;
					shooterType.shootPosition.x = 5.f;
					shooterType.shootPosition.y = 0.f;
					shooterType.bulletType = LIGHT;
					shooterType.isRighted = player.weapon.isRight;
					shooterType.isAlly = sfTrue;
					AddColdBreath(GetPlayerPosition(), GetMousePositionToOrigin(), shooterType);
					player.ener.energy -= 40.f;
					player.canShoot = sfFalse;
				}
			}
		}
	}
	if (DEV_WEAPON)
	{
		static sfBool k_wasPressed = sfFalse;
		static sfBool j_wasPressed = sfFalse;

		if (sfKeyboard_isKeyPressed(sfKeyK))
		{
			if (k_wasPressed == sfFalse)
			{
				SwitchGunDevMode();
				k_wasPressed = sfTrue;
			}
		}
		else
		{
			k_wasPressed = sfFalse;
		}
		if (sfKeyboard_isKeyPressed(sfKeyJ))
		{
			if (j_wasPressed == sfFalse)
			{
				SwitchSecondaryDevMode();
				j_wasPressed = sfTrue;
			}
		}
		else
		{
			j_wasPressed = sfFalse;
		}
	}
}

void UpdateFireControlRailgun(void)
{
	if (GetBulletCount() < BULLET_ALLY_MAX)
	{
		UseWeaponRailgun(GetPlayerPosition(), GetMousePositionToOrigin(), player.weapon.isRight);
	}
}

void UpdateFireControlMisteal(void)
{
	if (GetBulletCount() < MISTEAL_ALLY_MAX)
	{
		UseWeaponMisteal(GetPlayerPosition(), GetMousePositionToOrigin(), player.weapon.isRight);
	}
}

void UpdateFireControlSteamAxe(float _dt)
{
	if (IfControlKeyPressed(KEY_GUN))
	{
		if (player.canShoot)
		{
			player.pressTime += _dt;
		}
	}
	else if (player.pressTime > 0.0f)
	{
		if (player.pressTime < 0.5f)
		{
			if (player.ener.energy >= 25.f)
			{
				ChangeAttackType(MEDIUM);
				player.ener.energy -= 25.f;
			}
			else
			{
				ChangeAttackType(LIGHT);
			}
		}
		else
		{
			if (player.ener.energy >= 50.0f)
			{
				ChangeAttackType(HEAVY);
				player.ener.energy -= 50.0f;
			}
			else
			{
				ChangeAttackType(MEDIUM);
				player.ener.energy -= 25.f;
			}
		}
		float angleShift = 0.0f;
		if (player.weapon.steamAxe.attackType == LIGHT)
		{
			angleShift = STEAMAXE_ANGLE_LIGHT / 2.0f;
		}
		else if (player.weapon.steamAxe.attackType == MEDIUM)
		{
			angleShift = STEAMAXE_ANGLE_MEDIUM / 2.0f;
		}
		else
		{
			angleShift = STEAMAXE_ANGLE_HEAVY / 2.0f;
		}
		if (player.weapon.isRight)
		{
			sfSprite_rotate(player.weapon.steamAxe.sprite, -angleShift);
		}
		else
		{
			sfSprite_rotate(player.weapon.steamAxe.sprite, angleShift);
		}
		player.isAttacking = sfTrue;
		player.canShoot = sfFalse;
		CanHitBoss(sfTrue);
		player.cooldown = 1.0f / FIRE_RATE_STEAMAXE;
		player.pressTime = 0.0f;
	}
}

void UpdateSteamAxe(float _dt)
{
	sfBool testBoss = 0;

	if (player.isAttacking)
	{
		float range = 0;
		if (player.weapon.steamAxe.attackType == LIGHT)
		{
			range = (float)STEAMAXE_ANGLE_LIGHT;
		}
		else if (player.weapon.steamAxe.attackType == MEDIUM)
		{
			range = (float)STEAMAXE_ANGLE_MEDIUM;
		}
		else if (player.weapon.steamAxe.attackType == HEAVY)
		{
			range = (float)STEAMAXE_ANGLE_HEAVY;
		}
		float frameRotation = (range * (float)FIRE_RATE_STEAMAXE * _dt);
		if (player.weapon.isRight)
		{
			sfSprite_rotate(player.weapon.steamAxe.sprite, frameRotation);
		}
		else
		{
			sfSprite_rotate(player.weapon.steamAxe.sprite, -frameRotation);
		}
		if (player.weapon.steamAxe.canHit == sfTrue)
		{
			sfFloatRect axeHitbox = sfSprite_getGlobalBounds(player.weapon.steamAxe.sprite);
			ColisionBox(axeHitbox, sfTrue, AXIS_BOTH);
			ColisionElevatorButon(axeHitbox);

			if (player.weapon.steamAxe.attackType == LIGHT)
			{
				if (HitEnemy(1.f, axeHitbox, LIGHT))
				{
					PlaySound(WEAPON_AXE_SMALL);
				}
				if (HitBoss(5.f, axeHitbox, LIGHT))
				{
					PlaySound(WEAPON_AXE_MEDIUM);
					CanHitBoss(sfFalse);
				}
				else
				{
					PlaySound(WEAPON_AXE_MISS);
				}
			}
			else if (player.weapon.steamAxe.attackType == MEDIUM)
			{
				if (HitEnemy(2.f, axeHitbox, MEDIUM))
				{
					PlaySound(WEAPON_AXE_MEDIUM);
				}
				else if (HitBoss(8.f, axeHitbox, MEDIUM))
				{
					PlaySound(WEAPON_AXE_MEDIUM);
					CanHitBoss(sfFalse);
				}
				else
				{
					PlaySound(WEAPON_AXE_MISS);
				}
			}
			else if (player.weapon.steamAxe.attackType == HEAVY)
			{
				if (HitEnemy(3.f, axeHitbox, HEAVY))
				{
					PlaySound(WEAPON_AXE_LONG);
				}
				else if (HitBoss(14.f, axeHitbox, HEAVY))
				{
					PlaySound(WEAPON_AXE_LONG);
					CanHitBoss(sfFalse);
				}
				else
				{
					PlaySound(WEAPON_AXE_MISS);
				}
			}
		}
	}
}

void UpdateEnergy(float _dt)
{
	if (timerLastEnergyConso < player.ener.energyRegenCooldown)
	{
		timerLastEnergyConso += _dt;
	}
	else
	{
		if (player.ener.energy < player.ener.energyMax)
		{
			player.ener.energy += _dt * player.ener.energyRegen;
		}
		else if (player.ener.energy > player.ener.energyMax)
		{
			player.ener.energy = player.ener.energyMax;
		}
	}
	if (player.ener.energy < 0)
	{
		player.ener.energy = 0;
	}
	if (DEV_WEAPON)
	{
		if (sfKeyboard_isKeyPressed(sfKeyP))
		{
			player.ener.energy = player.ener.energyMax;
		}
	}
}

sfFloatRect GetPlayerRect(void)
{
	return sfRectangleShape_getGlobalBounds(player.collision);
}

sfVector2f GetPlayerVelocity(void)
{
	return player.velocity;
}

void SetPlayerVelocity(sfVector2f _velocity)
{
	player.velocity = _velocity;
}

float GetPlayerEnergyInfo(EnergyEnum _index)
{
	switch (_index)
	{
	case ENERGY:
		return player.ener.energy;
		break;
	case ENERGY_MAX:
		return player.ener.energyMax;
		break;
	case ENERGY_REGEN:
		return player.ener.energyRegen;
		break;
	case ENERGY_REGEN_COOLDOWN:
		return player.ener.energyRegenCooldown;
		break;
	default:
		return 0;
		break;
	}
}

void SetPlayerEnergyInfo(float _val, int _index)
{
	switch (_index)
	{
	case ENERGY:
		player.ener.energy = _val;
		break;
	case ENERGY_MAX:
		player.ener.energyMax = _val;
		break;
	case ENERGY_REGEN:
		player.ener.energyRegen = _val;
		break;
	case ENERGY_REGEN_COOLDOWN:
		player.ener.energyRegenCooldown = _val;
		break;
	default:
		break;
	}
}

int GetPlayerLife(void)
{
	return player.life;
}

void SetPlayerLifeMax(int _lifeMax)
{
	player.lifeMax = _lifeMax;
}

void AddPlayerLife(int _life)
{
	player.life += _life;

	if (player.life > player.lifeMax)
	{
		player.life = player.lifeMax;
	}
}

void SetPlayerPosition(sfVector2f _pos)
{
	SetViewCenter(_pos);
	sfRectangleShape_setPosition(player.collision, _pos);
}

void MovePlayer(sfVector2f _move)
{
	sfRectangleShape_move(player.collision, _move);
}

void SetSpawnPlayer(sfVector2f _pos)
{
	SetPlayerPosition(_pos);
	player.spawn = _pos;
}

void SetTpPlayerBoss(sfVector2f _pos)
{
	player.tpBoss = _pos;
}

void TpPlayerBoss(void)
{
	SetPlayerPosition(player.tpBoss);
}

void TpPlayerToSpawn(void)
{
	SetPlayerPosition(player.spawn);
}

void HandlePlayerBossCollision(sfVector2f _push)
{
	if (_push.x != 0.f || _push.y != 0.f)
	{
		sfRectangleShape_move(player.collision, _push);

		if (_push.y < 0.f)
		{
			player.isGrounded = sfTrue;
			player.velocity.y = 0.f;
			timerFaling = 0.f;
		}
		else if (_push.y > 0.f)
		{
			player.velocity.y = 0.f;
		}
		if (_push.x != 0.f)
		{
			player.velocity.x = 0.f;
		}
	}
}

void ChangePlayerInvicibility(void)
{
	if (playerInvicible)
	{
		playerInvicible = sfFalse;
	}
	else
	{
		playerInvicible = sfTrue;
	}
}

sfBool ColisionWithPlayer(sfFloatRect _rect, sfBool _damage)
{
	sfFloatRect playerRect = GetPlayerRect();

	if (sfFloatRect_intersects(&playerRect, &_rect, NULL))
	{
		if (_damage)
		{
			DamagePlayer(1);
		}
		return sfTrue;
	}
	return sfFalse;
}