#include "Player.h"

Player player;

float timerDash = 0;
float timerFaling = 0;

void MovePlayer(float _dt);
void ColisionMapPlayer(float _dt);
void MoveZonePlayer(float _dt);

sfVertexArray* CreateLineOfSight(sfVector2f _pointA, sfVector2f _pointB, sfColor _color);

void LoadPlayer(void)
{
	{ player = (Player){ 0 }; }

	sfTexture* texture = GetAsset("Assets/Sprites/capsul.png");
	player.sprite = CreateSprite(texture, (sfVector2f) { 0, 0 }, 1.f, 40);
	SetSpriteOriginFoot(player.sprite);

	player.collision = sfRectangleShape_create();
	sfRectangleShape_setSize(player.collision, (sfVector2f) { PLAYER_COLLISION_WIDTH, PLAYER_COLLISION_HEIGHT });
	sfRectangleShape_setPosition(player.collision, (sfVector2f) { 100, 32 });
	sfRectangleShape_setOrigin(player.collision, (sfVector2f) { PLAYER_COLLISION_WIDTH / 2, PLAYER_COLLISION_HEIGHT });

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
	MoveZonePlayer(_dt);
	MovePlayer(_dt);

	//MoveCameraSlow(GetPlayerPosition(), _dt);

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

	if (timerDash >= PLAYER_DASH_COOLDOWN && (sfKeyboard_isKeyPressed(GetKeyFromSave(KEY_DASH)) || sfMouse_isButtonPressed(GetMouseKeyFromSave(KEY_DASH))))
	{
		timerDash = 0;

		player.velocity.y /= 1.5f;

		if (player.direction)
		{
			player.velocity.x += PLAYER_DASH_POWER;
		}
		else
		{
			player.velocity.x += -PLAYER_DASH_POWER;
		}
	}

	sfRectangleShape_move(player.collision, (sfVector2f) { 0, PLAYER_WALK_SPEED_MAX* player.velocity.y* _dt });

	ColisionMapPlayer(_dt);
}

void ColisionMapPlayer(float _dt)
{
	sfVector2f reaction = Colision(sfRectangleShape_getGlobalBounds(player.collision));
	sfVector2f reactionPassThrough = CollisionPassThrough(sfRectangleShape_getGlobalBounds(player.collision));

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
		if (reaction.y < 0)
		{
			player.isGrounded = sfTrue;
			timerFaling = 0;
		}
		else if (reaction.y >= 0)
		{
			player.isGrounded = sfFalse;

			if (timerFaling <= PLAYER_JUMP_FORGIVE)
			{
				timerFaling += _dt;
			}
		}

		if (reaction.y != 0 || reactionPassThrough.y != 0)
		{
			player.velocity.y = 0;
		}
	}


	if (timerDash <= PLAYER_DASH_DURATION)
	{
		for (int i = 0; i < 10; i++)
		{
			sfRectangleShape_move(player.collision, (sfVector2f) { PLAYER_WALK_SPEED_MAX* player.velocity.x* _dt / 10, 0 });
			reaction = Colision(sfRectangleShape_getGlobalBounds(player.collision));

			if (reaction.x != 0)
			{
				player.velocity.x = 0;
			}
		}
	}
	else
	{
		sfRectangleShape_move(player.collision, (sfVector2f) { PLAYER_WALK_SPEED_MAX* player.velocity.x* _dt, 0 });
		reaction = Colision(sfRectangleShape_getGlobalBounds(player.collision));

		if (reaction.x != 0)
		{
			player.velocity.x = 0;
		}
	}
	sfRectangleShape_move(player.collision, reaction);
}

void MoveZonePlayer(float _dt)
{
	InfoZone* zone = GetInfoZoneMove(sfRectangleShape_getGlobalBounds(player.collision));

	if (zone != NULL)
	{
		int num = GetMoveCount();
		sfFloatRect hitbox = GetPlayerRect();

		for (int i = 0; i < num; i++)
		{
			int speed = 0;
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
}

void KillPlayer(void)
{

}

sfVector2f GetPlayerPosition(void)
{
	return sfRectangleShape_getPosition(player.collision);
}

sfFloatRect GetPlayerRect(void)
{
	return sfRectangleShape_getGlobalBounds(player.collision);
}

void SetPlayerPosition(sfVector2f _pos)
{
	sfRectangleShape_setPosition(player.collision, _pos);
}