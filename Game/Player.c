#include "Player.h"

Player player;

float timerDash = 0;

void MovePlayer(float _dt);

void LoadPlayer(void)
{
	player = (Player){ 0 };

	sfTexture* texture = GetAsset("D:/GitHub/final-project-prog1-2025-2026/x64/Debug/Assets/Sprites/capsul.png");
	player.sprite = CreateSprite(texture, (sfVector2f) { SCREEN_WIDTH, TILE_SIZE * 119 }, 2.f, 40);
	SetSpriteOriginFoot(player.sprite);
}

void UpdatePlayer(float _dt)
{
	MovePlayer(_dt);
}

void MovePlayer(float _dt)
{
	if (timerDash <= PLAYER_DASH_COOLDOWN)
	{
		timerDash += _dt;
	}

	KeySave tempKey1 = KEY_RIGHT;
	KeySave tempKey2 = KEY_LEFT;

	if (player.isDashing == sfFalse)
	{
		if ((sfKeyboard_isKeyPressed(GetKeyFromSave(tempKey1)) || sfMouse_isButtonPressed(GetMouseKeyFromSave(tempKey1))) && (sfKeyboard_isKeyPressed(GetKeyFromSave(tempKey2)) || sfMouse_isButtonPressed(GetMouseKeyFromSave(tempKey2))))
		{
			player.velocity.x = 0;
		}
		else if (sfKeyboard_isKeyPressed(GetKeyFromSave(tempKey1)) || sfMouse_isButtonPressed(GetMouseKeyFromSave(tempKey1)))
		{
			player.velocity.x = 1;
			player.direction = sfTrue;
		}
		else if (sfKeyboard_isKeyPressed(GetKeyFromSave(tempKey2)) || sfMouse_isButtonPressed(GetMouseKeyFromSave(tempKey2)))
		{
			player.velocity.x = -1;
			player.direction = sfFalse;
		}
		else
		{
			player.velocity.x = 0;
		}
	}

	tempKey1 = KEY_JUMP;
	tempKey2 = KEY_DOWN;

	if (player.isGrounded == sfTrue)
	{
		if ((sfKeyboard_isKeyPressed(GetKeyFromSave(tempKey1)) || sfMouse_isButtonPressed(GetMouseKeyFromSave(tempKey1))) && (sfKeyboard_isKeyPressed(GetKeyFromSave(tempKey2)) || sfMouse_isButtonPressed(GetMouseKeyFromSave(tempKey2))))
		{
			player.velocity.y = 0;
		}
		else if (sfKeyboard_isKeyPressed(GetKeyFromSave(tempKey1)) || sfMouse_isButtonPressed(GetMouseKeyFromSave(tempKey1)))
		{
			sfSprite_move(player.sprite, (sfVector2f) { 0, -10 });
			player.velocity.y = -PLAYER_JUMP_POWER;
			player.isGrounded = sfFalse;
		}
		else if (sfKeyboard_isKeyPressed(GetKeyFromSave(tempKey2)) || sfMouse_isButtonPressed(GetMouseKeyFromSave(tempKey2)))

		{
			player.velocity.y = 1;
		}
	}

	if (timerDash >= PLAYER_DASH_DURATION)
	{
		tempKey1 = KEY_DASH;
		if ((sfKeyboard_isKeyPressed(GetKeyFromSave(tempKey1)) || sfMouse_isButtonPressed(GetMouseKeyFromSave(tempKey1))) && timerDash >= PLAYER_DASH_COOLDOWN)
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

			player.isDashing = sfTrue;
		}
		else
		{
			player.isDashing = sfFalse;
		}
	}

	if (player.isGrounded == sfFalse)
	{
		player.velocity.y += 9.81f * _dt;
	}

	if (sfSprite_getPosition(player.sprite).y >= TILE_SIZE * 121 * 2)
	{
		player.isGrounded = sfTrue;
		player.velocity.y = 0;

		sfFloatRect bound = sfSprite_getGlobalBounds(player.sprite);
		sfSprite_move(player.sprite, (sfVector2f) { 0, TILE_SIZE * 121 * 2 - bound.height - bound.top });
	}

	sfSprite_move(player.sprite, (sfVector2f) { PLAYER_WALK_SPEED_MAX* player.velocity.x* _dt, PLAYER_WALK_SPEED_MAX* player.velocity.y* _dt });

	sfVector2f reaction = Colision(sfSprite_getGlobalBounds(player.sprite));
	if (reaction.y < 0)
	{
		player.isGrounded = sfTrue;
		player.velocity.y = 0;
	}
	sfSprite_move(player.sprite, reaction);
}

void KillPlayer(void)
{

}

sfVector2f GetPlayerPosition(void)
{
	return sfSprite_getPosition(player.sprite);
}