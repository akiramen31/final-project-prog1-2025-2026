#include "Player.h"

Player player;

float timerDash = 0;
float timerFaling = 0;

void MovePlayer(float _dt);

void LoadPlayer(void)
{
	player = (Player){ 0 };

	sfTexture* texture = GetAsset("Assets/Sprites/capsul.png");
	player.sprite = CreateSprite(texture, (sfVector2f) { 0, 0 }, 1.f, 40);
	SetSpriteOriginFoot(player.sprite);

	player.collision = sfRectangleShape_create();
	sfRectangleShape_setSize(player.collision, (sfVector2f) { PLAYER_COLLISION_WIDTH, PLAYER_COLLISION_HEIGHT });
	sfRectangleShape_setPosition(player.collision, (sfVector2f) { 100, 32 });
	sfRectangleShape_setOrigin(player.collision, (sfVector2f) { PLAYER_COLLISION_WIDTH / 2, PLAYER_COLLISION_HEIGHT });
}

void UpdatePlayer(float _dt)
{
	MovePlayer(_dt);
	SetViewCenter(GetPlayerPosition());
	sfSprite_setPosition(player.sprite, sfRectangleShape_getPosition(player.collision));
}

void MovePlayer(float _dt)
{
	if (timerDash <= PLAYER_DASH_COOLDOWN)
	{
		timerDash += _dt;
	}

	KeySave tempKey1;
	KeySave tempKey2;

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

		tempKey1 = KEY_JUMP;
		tempKey2 = KEY_DOWN;

		if (player.isGrounded == sfTrue || timerFaling < PLAYER_JUMP_FORGIVE)
		{
			player.velocity.y = 0;
			if (!((sfKeyboard_isKeyPressed(GetKeyFromSave(tempKey1)) || sfMouse_isButtonPressed(GetMouseKeyFromSave(tempKey1))) && (sfKeyboard_isKeyPressed(GetKeyFromSave(tempKey2)) || sfMouse_isButtonPressed(GetMouseKeyFromSave(tempKey2)))))
			{
				if (sfKeyboard_isKeyPressed(GetKeyFromSave(tempKey1)) || sfMouse_isButtonPressed(GetMouseKeyFromSave(tempKey1)))
				{
					sfSprite_move(player.sprite, (sfVector2f) { 0, -10 });
					player.velocity.y -= PLAYER_JUMP_POWER;
					timerFaling += PLAYER_JUMP_FORGIVE;
					player.isGrounded = sfFalse;
				}
				else if (sfKeyboard_isKeyPressed(GetKeyFromSave(tempKey2)) || sfMouse_isButtonPressed(GetMouseKeyFromSave(tempKey2)))
				{
					player.velocity.y++;
				}
			}
		}
		else
		{
			player.velocity.y += 9.81f * _dt;
			if (player.velocity.y > PLAYER_FALL_SPEED_MAX)
			{
				player.velocity.y = PLAYER_FALL_SPEED_MAX;
			}
		}
	}

	if (timerDash >= PLAYER_DASH_COOLDOWN && (sfKeyboard_isKeyPressed(GetKeyFromSave(KEY_DASH)) || sfMouse_isButtonPressed(GetMouseKeyFromSave(KEY_DASH))))
	{
		timerDash = 0;
		player.velocity.y = 0;
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

	sfRectangleShape_move(player.collision, reaction);
}

void KillPlayer(void)
{

}

sfVector2f GetPlayerPosition(void)
{
	return sfRectangleShape_getPosition(player.collision);
}

/*
#include "Player.h"

			Player player;

		float timerDash = 0;
		float timerFaling = 0;

		void MovePlayer(float _dt);

		void LoadPlayer(void)
		{
			player = (Player){ 0 };

			sfTexture* texture = GetAsset("Assets/Sprites/capsul.png");
			player.sprite = CreateSprite(texture, (sfVector2f) { 0, 0 }, 1.f, 40);
			SetSpriteOriginFoot(player.sprite);

			player.collision = sfRectangleShape_create();
			sfRectangleShape_setSize(player.collision, (sfVector2f) { PLAYER_COLLISION_WIDTH, PLAYER_COLLISION_HEIGHT });
			sfRectangleShape_setPosition(player.collision, (sfVector2f) { 100, 32 });
			sfRectangleShape_setOrigin(player.collision, (sfVector2f) { PLAYER_COLLISION_WIDTH / 2, PLAYER_COLLISION_HEIGHT });
		}

		void UpdatePlayer(float _dt)
		{
			MovePlayer(_dt);
			SetViewCenter(GetPlayerPosition());
			sfSprite_setPosition(player.sprite, sfRectangleShape_getPosition(player.collision));
		}

		void MovePlayer(float _dt)
		{
			if (timerDash <= PLAYER_DASH_COOLDOWN)
			{
				timerDash += _dt;
			}

			KeySave tempKey1;
			KeySave tempKey2;

			if (player.isDashing == sfFalse)
			{
				tempKey1 = KEY_RIGHT;
				tempKey2 = KEY_LEFT;
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
				else /*if (player.isGrounded)
				{
					player.velocity.x = 0;
				}

				tempKey1 = KEY_JUMP;
				tempKey2 = KEY_DOWN;

				if (player.isGrounded == sfTrue || timerFaling < PLAYER_JUMP_FORGIVE)
				{

					if ((sfKeyboard_isKeyPressed(GetKeyFromSave(tempKey1)) || sfMouse_isButtonPressed(GetMouseKeyFromSave(tempKey1))) && (sfKeyboard_isKeyPressed(GetKeyFromSave(tempKey2)) || sfMouse_isButtonPressed(GetMouseKeyFromSave(tempKey2))))
					{
						player.velocity.y = 0;
					}
					else if (sfKeyboard_isKeyPressed(GetKeyFromSave(tempKey1)) || sfMouse_isButtonPressed(GetMouseKeyFromSave(tempKey1)))
					{
						player.velocity.y = 0;
						sfSprite_move(player.sprite, (sfVector2f) { 0, -10 });
						player.velocity.y -= PLAYER_JUMP_POWER;
						timerFaling += PLAYER_JUMP_FORGIVE;
						player.isGrounded = sfFalse;
					}
					else if (sfKeyboard_isKeyPressed(GetKeyFromSave(tempKey2)) || sfMouse_isButtonPressed(GetMouseKeyFromSave(tempKey2)))

					{
						player.velocity.y += 1;
					}
				}

				if (player.isGrounded == sfFalse)
				{
					player.velocity.y += 9.81f * _dt;
				}
			}


			if (timerDash >= PLAYER_DASH_DURATION)
			{
				tempKey1 = KEY_DASH;
				if ((sfKeyboard_isKeyPressed(GetKeyFromSave(tempKey1)) || sfMouse_isButtonPressed(GetMouseKeyFromSave(tempKey1))) && timerDash >= PLAYER_DASH_COOLDOWN)
				{
					timerDash = 0;
					//player.velocity.y = 0;
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
			if (player.velocity.y > 1)
			{
				player.velocity.y = 1;
			}


			sfRectangleShape_move(player.collision, (sfVector2f) { PLAYER_WALK_SPEED_MAX* player.velocity.x* _dt, PLAYER_WALK_SPEED_MAX* player.velocity.y* _dt });

			sfVector2f reaction = Colision(sfRectangleShape_getGlobalBounds(player.collision));
			if (reaction.y < 0)
			{
				player.isGrounded = sfTrue;
				player.velocity.y = 0;
				timerFaling = 0;
			}
			else if (reaction.y >= 0.f)
			{
				player.isGrounded = sfFalse;
			}

			//printf("%f %f\n", reaction.x, reaction.y);


			if (reaction.x != 0)
			{
				player.velocity.x = 0;

				player.isDashing = sfFalse;
			}
			if (reaction.y != 0)
			{
				player.velocity.y = 0;
				if (timerFaling <= PLAYER_JUMP_FORGIVE)
				{
					timerFaling += _dt;
				}
			}

			sfRectangleShape_move(player.collision, reaction);
		}

		void KillPlayer(void)
		{

		}

		sfVector2f GetPlayerPosition(void)
		{
			return sfRectangleShape_getPosition(player.collision);
		}
*/