#include "Player.h"

Player player = { 0 };

void MovePlayer(float _dt);
void CheckCollisionPlayerScreen(void);
void UpdateCooldown(float _dt);
void UpdateFireControl(void);

void LoadPlayer(void)
{
	player.texture = sfTexture_createFromFile("Assets/Sprites/Ship.png", NULL);
	player.sprite = sfSprite_create();
	sfSprite_setTexture(player.sprite, player.texture, sfTrue);
	sfFloatRect playerHitbox = sfSprite_getLocalBounds(player.sprite);
	sfSprite_setOrigin(player.sprite, (sfVector2f) { playerHitbox.width / 2, playerHitbox.height / 2 });
	sfSprite_setPosition(player.sprite, (sfVector2f) { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 });

	player.canShoot = sfTrue;
	player.cooldown = 1.f/FIRE_RATE;
}

void UpdatePlayer(sfRenderWindow* _renderWindow, float _dt)
{
	MovePlayer(_dt);
	CheckCollisionPlayerScreen();
	UpdateCooldown(_dt);
	UpdateFireControl();
}

void DrawPlayer(sfRenderWindow* const _renderWindow)
{
	sfRenderWindow_drawSprite(_renderWindow, player.sprite, NULL);
}

void CleanupPlayer(void)
{
	sfTexture_destroy(player.texture);
	sfSprite_destroy(player.sprite);

	player = (Player){ 0 };
}

void MovePlayer(float _dt)
{
	sfVector2f directionAxis = { 0 };

	if (sfKeyboard_isKeyPressed(sfKeyD))
	{
		directionAxis.x++;
	}
	if (sfKeyboard_isKeyPressed(sfKeyQ))
	{
		directionAxis.x--;
	}

	if (sfKeyboard_isKeyPressed(sfKeyS))
	{
		directionAxis.y++;
	}
	if (sfKeyboard_isKeyPressed(sfKeyZ))
	{
		directionAxis.y--;
	}

	if (directionAxis.x != 0 || directionAxis.y != 0)
	{
		float direction = atan2f(directionAxis.y, directionAxis.x);

		sfVector2f playerMove = { PLAYER_SPEED * cosf(direction) * _dt, PLAYER_SPEED * sinf(direction) * _dt };
		sfSprite_move(player.sprite, playerMove);
	}
}

void CheckCollisionPlayerScreen(void)
{
	sfFloatRect hitbox = sfSprite_getGlobalBounds(player.sprite);
	if (hitbox.left < 0)
	{
		sfSprite_move(player.sprite, (sfVector2f) { 0 - hitbox.left, 0 });
	}
	if (hitbox.left + hitbox.width > SCREEN_WIDTH)
	{
		sfSprite_move(player.sprite, (sfVector2f) { SCREEN_WIDTH - (hitbox.left + hitbox.width), 0 });
	}
	if (hitbox.top < 0)
	{
		sfSprite_move(player.sprite, (sfVector2f) { 0, 0 - hitbox.top });
	}
	if (hitbox.top + hitbox.height > SCREEN_HEIGHT)
	{
		sfSprite_move(player.sprite, (sfVector2f) { 0, SCREEN_HEIGHT - (hitbox.top + hitbox.height) });
	}
}

void UpdateCooldown(float _dt)
{
	if (!player.canShoot)
	{
		player.cooldown -= _dt;
		if (player.cooldown < 0)
		{
			player.cooldown += 1.f/FIRE_RATE;
			player.canShoot = sfTrue;
		}
	}
}

void UpdateFireControl(void)
{
	if (sfKeyboard_isKeyPressed(sfKeySpace))
	{
		if (player.canShoot && GetBulletCount() < BULLET_MAX)
		{
			sfFloatRect playerHitbox = sfSprite_getGlobalBounds(player.sprite);
			sfVector2f shotPosition = { playerHitbox.left + playerHitbox.width / 2, playerHitbox.top };
			AddBullet(shotPosition);

			player.canShoot = sfFalse;
		}
	}
}
