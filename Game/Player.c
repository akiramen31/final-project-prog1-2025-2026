#include "Player.h"

#define PLAYER_HEIGHT 30
#define PLAYER_WIDTH 20

Player player;

float lastMove;

void MovePlayer(float _dt);

void LoadPlayer(void)
{
	player = (Player){ 0 };

	sfTexture* playerTexture = GetAsset("Assets/Sprites/Player/Player.png");
	player.sprite = CreateSprite(playerTexture, (sfVector2f) { 200, 200 }, 4.f, 40);
	player.animation.rectActualy = (sfIntRect){ 0, (player.direction + player.state) * PLAYER_HEIGHT, PLAYER_WIDTH, PLAYER_HEIGHT };
	sfSprite_setTextureRect(player.sprite, (sfIntRect) { 0, (player.direction + player.state)* PLAYER_HEIGHT, PLAYER_WIDTH, PLAYER_HEIGHT });
	sfSprite_setOrigin(player.sprite, (sfVector2f) { PLAYER_WIDTH / 2, PLAYER_HEIGHT });
	player.animation.frameDuration = 0.1f;
	player.animation.isLooping = sfTrue;
	player.playerMoveSpeedGrid = 1;

	sfSprite_setPosition(player.sprite, (sfVector2f) { 64 + 64 * player.posGrid.x, 224 + 64 * player.posGrid.y });
}

void UpdatePlayer(float _dt)
{
	MovePlayer(_dt);
}

void MovePlayer(float _dt)
{
	lastMove += _dt;

	if (player.isWalking == sfFalse)
	{
		if (sfKeyboard_isKeyPressed(sfKeyQ) && sfKeyboard_isKeyPressed(sfKeyD))
		{
			player.direction = DOWN;
			player.state = IDLE;
		}
		else if (sfKeyboard_isKeyPressed(sfKeyZ) && sfKeyboard_isKeyPressed(sfKeyS))
		{
			player.direction = RIGHT;
			player.state = IDLE;
		}
		else if (sfKeyboard_isKeyPressed(sfKeyQ))
		{
			player.direction = LEFT;
			player.state = WALK;
			if (player.posGrid.x == 0)
			{
				player.state = IDLE;
			}
		}
		else if (sfKeyboard_isKeyPressed(sfKeyD))
		{
			player.direction = RIGHT;
			player.state = WALK;
			if (player.posGrid.x == 14)
			{
				player.state = IDLE;
			}
		}
		else if (sfKeyboard_isKeyPressed(sfKeyZ))
		{
			player.direction = UP;
			player.state = WALK;
			if (player.posGrid.y == 0)
			{
				player.state = IDLE;
			}
		}
		else if (sfKeyboard_isKeyPressed(sfKeyS))
		{
			player.direction = DOWN;
			player.state = WALK;
			if (player.posGrid.y == 8)
			{
				player.state = IDLE;
			}
		}
		else
		{
			player.state = IDLE;
		}

		if (player.state == WALK)
		{
			player.animation.frameCount = 6;
		}
		else if (player.state == IDLE)
		{
			player.animation.frameCount = 3;
		}

		player.animation.rectActualy = (sfIntRect){ player.animation.rectActualy.left, (player.direction + player.state) * PLAYER_HEIGHT, PLAYER_WIDTH, PLAYER_HEIGHT };

		if (player.state == WALK)
		{
			if (lastMove > (1 / (10 + player.playerMoveSpeedGrid)))
			{

				switch (player.direction)
				{
				case DOWN:
					player.posGrid.y++;
					break;
				case LEFT:
					player.posGrid.x--;
					break;
				case RIGHT:
					player.posGrid.x++;
					break;
				case UP:
					player.posGrid.y--;
					break;
				default:
					break;
				}

				sfSprite_setPosition(player.sprite, (sfVector2f) { 64 + 64 * player.posGrid.x, 224 + 64 * player.posGrid.y });
				lastMove = 0;
				player.isWalking = sfTrue;
			}
		}
		player.isWalking = sfFalse;
	}

	UpdateAnimationAndGiveIfStop(player.sprite, &player.animation, _dt);
}

sfVector2i GetPlayerPositionGrid(void)
{
	return player.posGrid;
}
