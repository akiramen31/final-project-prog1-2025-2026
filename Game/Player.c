#include "Player.h"

Player player;

float timerDash = 0;
float timerFaling = 0;
float timerLastEnergyConso = 0;

void UpdateMovePlayer(float _dt);

void ColisionMapPlayer(float _dt);
void MoveZonePlayer(float _dt);

void UpdateAnimation(float _dt);

void UpdateCooldown(float _dt);
void UpdateFireControl(float _dt);
void UpdateFireControlRailgun(void);
void UpdateFireControlSteamAxe(float _dt);

void UpdateSteamAxe(float _dt);
void UpdateEnergy(float _dt);

void UpdateWeaponPlayer(float _dt);

sfVector2f pos;

void LoadPlayer(void)
{
	player = (Player){ 0 };
	sfTexture* texture = GetAsset("Assets/Sprites/player_sprite_sheet.png");
	player.sprite = CreateSprite(texture, (sfVector2f) { 0, 0 }, 1.f, 40);
	player.direction == sfTrue;

	player.collision = sfRectangleShape_create();
	sfRectangleShape_setSize(player.collision, (sfVector2f) { PLAYER_COLLISION_WIDTH, PLAYER_COLLISION_HEIGHT });
	sfRectangleShape_setPosition(player.collision, (sfVector2f) { 100, 32 });
	sfRectangleShape_setOrigin(player.collision, (sfVector2f) { PLAYER_COLLISION_WIDTH / 2, PLAYER_COLLISION_HEIGHT });

	player.running.frameCount = 8;
	player.running.frameDuration = 0.1;
	player.running.isLooping = sfTrue;
	player.running.rectActualy = (sfIntRect){ 0,0,32,32 };

	player.walking.frameCount = 8;
	player.walking.frameDuration = 0.1;
	player.walking.isLooping = sfTrue;
	player.walking.rectActualy = (sfIntRect){ 0,64,16,32 };

	player.canShoot = sfTrue;
	player.cooldown = 1.f / FIRE_RATE_RAILGUN;
	player.isAttacking = sfFalse;
	player.pressTime = 0.f;
	pos.x = 100;
	pos.y = 32;

	player.lifeMax = 3;
	player.life = player.lifeMax;

	player.ener.energyMax = 100;
	player.ener.energy = player.ener.energyMax;
	player.ener.energyRegen = 5;
	player.ener.energyRegenCooldown = 0.5f;
	player.ener.dashConsuption = 5.f;
}

void UpdatePlayer(float _dt)
{
	if (!player.isAttacking)
	{
		player.weapon = GetWeapon();
	}
	UpdateWeaponPlayer(_dt);
	UpdateEnergy(_dt);

	if (GetIntFromSave(DEV_MODE_FLY))
	{
		int val = 10;
		if (sfKeyboard_isKeyPressed(GetKeyFromSave(KEY_RIGHT)) || sfMouse_isButtonPressed(GetMouseKeyFromSave(KEY_RIGHT)))
		{
			pos.x += val;
		}
		else if (sfKeyboard_isKeyPressed(GetKeyFromSave(KEY_LEFT)) || sfMouse_isButtonPressed(GetMouseKeyFromSave(KEY_LEFT)))
		{
			pos.x -= val;
		}
		if (sfKeyboard_isKeyPressed(GetKeyFromSave(KEY_DOWN)) || sfMouse_isButtonPressed(GetMouseKeyFromSave(KEY_DOWN)))
		{
			pos.y += val;
		}
		else if (sfKeyboard_isKeyPressed(GetKeyFromSave(KEY_JUMP)) || sfMouse_isButtonPressed(GetMouseKeyFromSave(KEY_JUMP)))
		{
			pos.y -= val;
		}
		sfRectangleShape_setPosition(player.collision, pos);
	}
	else
	{
		MoveZonePlayer(_dt);
		UpdateMovePlayer(_dt);
	}

	UpdateAnimation(_dt);
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

void UpdateMovePlayer(float _dt)
{
	if (timerDash <= PLAYER_DASH_COOLDOWN)
	{
		timerDash += _dt;
	}

	if (timerDash >= PLAYER_DASH_DURATION)
	{
		player.velocity.x /= 1.5f;

		if ((sfKeyboard_isKeyPressed(GetKeyFromSave(KEY_RIGHT)) || sfMouse_isButtonPressed(GetMouseKeyFromSave(KEY_RIGHT))) && (sfKeyboard_isKeyPressed(GetKeyFromSave(KEY_LEFT)) || sfMouse_isButtonPressed(GetMouseKeyFromSave(KEY_LEFT))))
		{
			player.velocity.x = 0;
		}
		else if (sfKeyboard_isKeyPressed(GetKeyFromSave(KEY_RIGHT)) || sfMouse_isButtonPressed(GetMouseKeyFromSave(KEY_RIGHT)))
		{
			if (player.velocity.x <= 1)
			{
				player.velocity.x = 1;
			}
		}
		else if (sfKeyboard_isKeyPressed(GetKeyFromSave(KEY_LEFT)) || sfMouse_isButtonPressed(GetMouseKeyFromSave(KEY_LEFT)))
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
			if (!((sfKeyboard_isKeyPressed(GetKeyFromSave(KEY_JUMP)) || sfMouse_isButtonPressed(GetMouseKeyFromSave(KEY_JUMP))) && (sfKeyboard_isKeyPressed(GetKeyFromSave(KEY_DOWN)) || sfMouse_isButtonPressed(GetMouseKeyFromSave(KEY_DOWN)))))
			{
				if (sfKeyboard_isKeyPressed(GetKeyFromSave(KEY_JUMP)) || sfMouse_isButtonPressed(GetMouseKeyFromSave(KEY_JUMP)))
				{
					sfSprite_move(player.sprite, (sfVector2f) { 0, -10 });
					player.velocity.y -= PLAYER_JUMP_POWER;
					timerFaling += PLAYER_JUMP_FORGIVE;
					player.isGrounded = sfFalse;
				}
				else if (sfKeyboard_isKeyPressed(GetKeyFromSave(KEY_DOWN)) || sfMouse_isButtonPressed(GetMouseKeyFromSave(KEY_DOWN)))
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

	if (timerDash >= PLAYER_DASH_COOLDOWN && player.ener.energy > player.ener.dashConsuption && (sfKeyboard_isKeyPressed(GetKeyFromSave(KEY_DASH)) || sfMouse_isButtonPressed(GetMouseKeyFromSave(KEY_DASH))))
	{
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
	// ==========================================
	// 1. RESOLVE Y-AXIS FIRST (Gravity / Falling)
	// ==========================================
	sfVector2f reactionY = Colision(sfRectangleShape_getGlobalBounds(player.collision), AXIS_Y);
	sfVector2f reactionPassThrough = CollisionPassThrough(sfRectangleShape_getGlobalBounds(player.collision));

	// Assuming ColisionBox also takes an axis parameter now:
	reactionY.y += ColisionBox(sfRectangleShape_getGlobalBounds(player.collision), sfFalse, AXIS_Y).y;

	// Handle Pass-Through Platforms
	if (reactionPassThrough.y < 0)
	{
		if (player.velocity.y >= 0 && !(sfKeyboard_isKeyPressed(GetKeyFromSave(KEY_DOWN)) || sfMouse_isButtonPressed(GetMouseKeyFromSave(KEY_DOWN))))
		{
			player.isGrounded = sfTrue;
			timerFaling = 0;
			player.velocity.y = 0;
			sfRectangleShape_move(player.collision, reactionPassThrough);
		}
	}
	else
	{
		// Handle Normal Grounding
		if (reactionY.y < 0)
		{
			player.isGrounded = sfTrue;
			timerFaling = 0;
		}
		else if (reactionY.y >= 0)
		{
			player.isGrounded = sfFalse;
			if (timerFaling <= PLAYER_JUMP_FORGIVE)
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

	// Apply the Y pushback before starting X checks


	// ==========================================
	// 2. RESOLVE X-AXIS SECOND (Running / Dashing)
	// ==========================================
	if (timerDash <= PLAYER_DASH_DURATION)
	{
		for (int i = 0; i < 10; i++)
		{
			sfRectangleShape_move(player.collision, (sfVector2f) { PLAYER_HORIZONTAL_SPEED_MAX* player.velocity.x* _dt / 10.0f, 0 });

			sfVector2f reactionX = Colision(sfRectangleShape_getGlobalBounds(player.collision), AXIS_X);
			reactionX.x += ColisionBox(sfRectangleShape_getGlobalBounds(player.collision), sfFalse, AXIS_X).x;

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

		if (reactionX.x != 0)
		{
			// Push out of wall and halt X velocity
			sfRectangleShape_move(player.collision, (sfVector2f) { reactionX.x, 0 });
			player.velocity.x = 0;
		}
	}

	// Weapons logic remains unchanged
	MoveWeapon(GetPlayerPosition(), GetAimPosition(), _dt, player.isAttacking);
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
	if (player.velocity.x != 0 && player.velocity.y == 0)
	{
		UpdateAnimationAndGiveIfStop(player.sprite, &player.running, _dt);
	}
	else /*if (player.velocity.x == 0)*/
	{
		UpdateAnimationAndGiveIfStop(player.sprite, &player.walking, _dt);
	}

	if (player.direction)
	{
		sfSprite_setScale(player.sprite, (sfVector2f) { 1, 1 });
	}
	else
	{
		sfSprite_setScale(player.sprite, (sfVector2f) { -1, 1 });
	}
	SetSpriteOriginFoot(player.sprite);

	sfSprite_setPosition(player.sprite, sfRectangleShape_getPosition(player.collision));
}

void DamagePlayer(int _damage)
{
	player.life -= _damage;
}

void KillPlayer(void)
{
	player.life = 0;
}

sfVector2f GetPlayerPosition(void)
{
	return sfRectangleShape_getPosition(player.collision);
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
		if (sfKeyboard_isKeyPressed(GetKeyFromSave(KEY_GUN)) || sfMouse_isButtonPressed(GetMouseKeyFromSave(KEY_GUN)))
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
	if (sfKeyboard_isKeyPressed(GetKeyFromSave(KEY_SECOND)) || sfMouse_isButtonPressed(GetMouseKeyFromSave(KEY_SECOND)))
	{
		if (player.canShoot)
		{
			AddMissile(GetPlayerPosition(), player.weapon.isRight);
			player.canShoot = sfFalse;
		}
	}
	if (DEV_WEAPON)
	{
		static sfBool k_wasPressed = sfFalse;

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
	}
}

void UpdateFireControlRailgun(void)
{
	if (GetBulletCount() < BULLET_MAX)
	{
		UseWeapon(GetPlayerPosition(), GetAimPosition(), player.weapon.isRight);
	}
}

void UpdateFireControlSteamAxe(float _dt)
{
	if (sfKeyboard_isKeyPressed(GetKeyFromSave(KEY_GUN)) || sfMouse_isButtonPressed(GetMouseKeyFromSave(KEY_GUN)))
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
				player.weapon.steamAxe.attackType = MEDIUM;
				player.ener.energy -= 25.f;
			}
			else
			{
				player.weapon.steamAxe.attackType = LIGHT;
			}
		}
		else
		{
			if (player.ener.energy >= 50.0f)
			{
				player.weapon.steamAxe.attackType = HEAVY;
				player.ener.energy -= 50.0f;
			}
			else
			{
				player.weapon.steamAxe.attackType = MEDIUM;
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
		player.weapon.steamAxe.canHit = sfTrue;
		player.cooldown = 1.0f / FIRE_RATE_STEAMAXE;
		player.pressTime = 0.0f;
	}
}

void UpdateSteamAxe(float _dt)
{
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
	if (sfKeyboard_isKeyPressed(sfKeyP))
	{
		player.ener.energy = player.ener.energyMax;
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

float GetPlayerEnergyInfo(int _index)
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