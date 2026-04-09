#include "Player.h"

Player player;
Weapon weapon;

float timerDash = 0;
float timerFaling = 0;

void MovePlayer(float _dt);
void UpdateCooldown(float _dt);
void UpdateFireControl(void);

sfVertexArray* CreateLineOfSight(sfVector2f _pointA, sfVector2f _pointB, sfColor _color);

void LoadPlayer(void)
{
	player = (Player){ 0 };
	weapon = (Weapon){ 0 };

	sfTexture* texture = GetAsset("Assets/Sprites/capsul.png");
	player.sprite = CreateSprite(texture, (sfVector2f) { 0, 0 }, 1.f, 40);
	SetSpriteOriginFoot(player.sprite);

	sfTexture* textureWeapon = GetAsset("Assets/Sprites/gun_Placeholder.png");
	player.weapon.sprite = CreateSprite(textureWeapon, (sfVector2f) { 0, 0 }, 1.f, 38);
	sfSprite_setOrigin(player.weapon.sprite, (sfVector2f) { 0, 2 });
	player.weapon.isRight = sfTrue;


	player.collision = sfRectangleShape_create();
	sfRectangleShape_setSize(player.collision, (sfVector2f) { PLAYER_COLLISION_WIDTH, PLAYER_COLLISION_HEIGHT });
	sfRectangleShape_setPosition(player.collision, (sfVector2f) { 100, 32 });
	sfRectangleShape_setOrigin(player.collision, (sfVector2f) { PLAYER_COLLISION_WIDTH / 2, PLAYER_COLLISION_HEIGHT });

	player.canShoot = sfTrue;
	player.cooldown = 1.f / FIRE_RATE;
}

sfVertexArray* CreateLineOfSight(sfVector2f _pointA, sfVector2f _pointB, sfColor _color)
{
	sfVertexArray* newLine = sfVertexArray_create();
	sfVertexArray_setPrimitiveType(newLine, sfLines);

	sfVertex vertexA = { _pointA, _color };
	sfVertexArray_append(newLine, vertexA);

	sfVertex vertexB = { _pointB, _color };
	sfVertexArray_append(newLine, vertexB);

	return newLine;
}

void UpdatePlayer(float _dt)
{
	MovePlayer(_dt);
	UpdateCooldown(_dt);
	UpdateFireControl();
	SetViewCenter(GetPlayerPosition());
	sfSprite_setPosition(player.sprite, sfRectangleShape_getPosition(player.collision));
}

void MovePlayer(float _dt)
{
	if (timerDash <= PLAYER_DASH_COOLDOWN)
	{
		timerDash += _dt;
	}

	if (timerDash >= PLAYER_DASH_DURATION)
	{
		player.velocity.x = 0;
		if (sfKeyboard_isKeyPressed(GetKeyFromSave(KEY_RIGHT)) || sfMouse_isButtonPressed(GetMouseKeyFromSave(KEY_RIGHT)))
		{
			player.velocity.x++;
		}
		if (sfKeyboard_isKeyPressed(GetKeyFromSave(KEY_LEFT)) || sfMouse_isButtonPressed(GetMouseKeyFromSave(KEY_LEFT)))
		{
			player.velocity.x--;
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

	if (timerDash >= PLAYER_DASH_COOLDOWN && (sfKeyboard_isKeyPressed(GetKeyFromSave(KEY_DASH)) || sfMouse_isButtonPressed(GetMouseKeyFromSave(KEY_DASH))))
	{
		timerDash = 0;
		if (player.direction)
		{
			player.velocity.x = PLAYER_DASH_POWER;
		}
		else
		{
			player.velocity.x = -PLAYER_DASH_POWER;
		}
	}

	sfRectangleShape_move(player.collision, (sfVector2f) { PLAYER_WALK_SPEED_MAX* player.velocity.x* _dt, PLAYER_WALK_SPEED_MAX* player.velocity.y* _dt });

	sfVector2f reaction = Colision(sfRectangleShape_getGlobalBounds(player.collision));
	if (reaction.x != 0)
	{
		player.velocity.x = 0;
	}

	if (reaction.y < 0)
	{
		player.isGrounded = sfTrue;
		timerFaling = 0;
	}

	if (reaction.y >= 0)
	{
		player.isGrounded = sfFalse;
		if (timerFaling <= PLAYER_JUMP_FORGIVE)
		{
			timerFaling += _dt;
		}
	}

	if (reaction.y != 0)
	{
		player.velocity.y = 0;

	}
	// GUN CODE PART WILL BE MODIFIED NEXT WEEK

	sfRectangleShape_move(player.collision, reaction);
	sfVector2f gunPosition = GetPlayerPosition();
	gunPosition.y -= PLAYER_COLLISION_HEIGHT / 2;
	sfSprite_setPosition(player.weapon.sprite, gunPosition);

	sfVector2f playerPos = GetPlayerPosition(); //start
	sfVector2f aimPosition = GetAimPosition(); // target

	float dx = aimPosition.x - playerPos.x;
	float dy = aimPosition.y - playerPos.y;

	float angleRect = atan2f(dy, dx) * (180.0f / (float)M_PI);
	if (angleRect > 90.0f || angleRect < -90)
	{
		if (player.weapon.isRight)
		{
			sfSprite_setScale(player.weapon.sprite, (sfVector2f) { 1.f, -1.f });
			player.weapon.isRight = sfFalse;
		}
	}
	else
	{
		if (!player.weapon.isRight)
		{
			sfSprite_setScale(player.weapon.sprite, (sfVector2f) { 1.f, 1.f });
			player.weapon.isRight = sfTrue;
		}
	}
	sfSprite_setRotation(player.weapon.sprite, angleRect);
}

void KillPlayer(void)
{

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
			player.cooldown += 1.f / FIRE_RATE;
			player.canShoot = sfTrue;
		}
	}
}

void UpdateFireControl(void)
{
	if (sfKeyboard_isKeyPressed(GetKeyFromSave(KEY_GUN)))
	{
		if (player.canShoot && GetBulletCount() < BULLET_MAX)
		{

			AddBullet();
			player.canShoot = sfFalse;
		}
	}
	if (sfKeyboard_isKeyPressed(GetKeyFromSave(KEY_SECOND)))
	{
		if (player.canShoot)
		{
			AddMissile();
			player.canShoot = sfFalse;
		}
	}
}