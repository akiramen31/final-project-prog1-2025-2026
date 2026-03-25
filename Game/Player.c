#include "Player.h"

Player player;

void MovePlayer(float _dt);

void LoadPlayer(void)
{
	player = (Player){ 0 };

	sfTexture* texture = GetAsset("D:/GitHub/final-project-prog1-2025-2026/x64/Debug/Assets/Sprites/capsul.png");
	player.sprite = CreateSprite(texture, (sfVector2f) { SCREEN_WIDTH, 3500.f }, 2.f, 40);
	SetSpriteOriginFoot(player.sprite);
}

void UpdatePlayer(float _dt)
{
	MovePlayer(_dt);
}

void MovePlayer(float _dt)
{
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
			player.velocity.y = -8;
			player.isGrounded = sfFalse;
		}
		else if (sfKeyboard_isKeyPressed(GetKeyFromSave(tempKey2)) || sfMouse_isButtonPressed(GetMouseKeyFromSave(tempKey2)))
		{
			player.velocity.y = 1;
		}
	}
	//else
	//{
	//	player.velocity.y = 0;
	//}

	tempKey1 = KEY_DASH;
	if (sfKeyboard_isKeyPressed(GetKeyFromSave(tempKey1)) || sfMouse_isButtonPressed(GetMouseKeyFromSave(tempKey1)))
	{
		if (player.direction)
		{
			sfSprite_move(player.sprite, (sfVector2f) { 20, 0 });
		}
		else
		{
			sfSprite_move(player.sprite, (sfVector2f) { -20, 0 });
		}

		player.isDashing = sfTrue;
	}
	else
	{
		player.isDashing = sfFalse;
	}

	if (player.isGrounded == sfFalse)
	{
		player.velocity.y += 9.81 * _dt;
	}

	if (sfSprite_getPosition(player.sprite).y >= 32 * 129)
	{
		player.isGrounded = sfTrue;
		player.velocity.y = 0;
	}


	{
		float tempAngle = atan2f(player.velocity.y, player.velocity.y);		sfSprite_move(player.sprite, (sfVector2f) { PLAYER_WALK_SPEED_MAX* player.velocity.x* _dt, PLAYER_WALK_SPEED_MAX* player.velocity.y* _dt });
	}
}

void KillPlayer(void)
{

}
