#include "Player.h"

#define PLAYER_HEIGHT 30
#define PLAYER_WIDTH 20

Player player;

void MovePlayer(float _dt);

void LoadPlayer(void)
{
	player = (Player){ 0 };

	sfTexture* playerTexture = GetAsset("Assets/Sprites/Player/Player.png");
	player.sprite = CreateSprite(playerTexture, (sfVector2f) { 200, 200 }, 4.f, 40);
	sfSprite_setTextureRect(player.sprite, (sfIntRect) { 0, (player.direction + player.state)* PLAYER_HEIGHT, PLAYER_WIDTH, PLAYER_HEIGHT });
	sfSprite_setOrigin(player.sprite, (sfVector2f) { PLAYER_WIDTH / 2, PLAYER_HEIGHT });
	player.animation.frameDuration = 0.1f;
	player.animation.isLooping = sfTrue;
	player.animation.rectActualy = (sfIntRect){ 0, (player.direction + player.state) * PLAYER_HEIGHT, PLAYER_WIDTH, PLAYER_HEIGHT };
}

void UpdatePlayer(float _dt)
{
	MovePlayer(_dt);
}

void MovePlayer(float _dt)
{
	if (sfKeyboard_isKeyPressed(sfKeyQ) && sfKeyboard_isKeyPressed(sfKeyD))
	{
		player.direction = DOWN;
		player.state = IDLE;
	}
	else if (sfKeyboard_isKeyPressed(sfKeyQ))
	{
		player.direction = LEFT;
		//if (player.posGrid.x > 0)
		{
			player.state = WALKING;
			player.posGrid.x--;
		}
		//else
		//{
		//	player.state = IDLE;

		//}
	}
	else if (sfKeyboard_isKeyPressed(sfKeyD))
	{
		player.direction = RIGHT;
		//if (player.posGrid.x < 10)
		{
			player.state = WALKING;
			player.posGrid.x++;
		}
		//else
		//{
		//	player.state = IDLE;

		//}
	}
	else
	{
		player.state = IDLE;

	}

	if (player.state == WALKING)
	{
		player.animation.frameCount = 6;
	}
	else if (player.state == IDLE)
	{
		player.animation.frameCount = 3;
	}

	player.animation.rectActualy = (sfIntRect){ player.animation.rectActualy.left, (player.direction + player.state) * PLAYER_HEIGHT, PLAYER_WIDTH, PLAYER_HEIGHT };

	UpdateAnimationAndGiveIfStop(player.sprite, &player.animation, _dt);
}

sfVector2i GetPlayerPositionGrid(void)
{
	return player.posGrid;
}
