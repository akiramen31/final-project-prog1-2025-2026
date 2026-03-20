#include "Player.h"

#define PLAYER_HEIGHT 30
#define PLAYER_WIDTH 20
#define INVINCIBLE_DURATION 3

Player player;

float lastMove;
float invincibleTime;

void MovePlayer(CasePosibility _GetMovePosibility, float _dt);

void LoadPlayer(void)
{
	player = (Player){ 0 };

	player.killSound = CreateSound(GetAsset("Assets/Sounds/Kill.wav"), 100000.f, sfFalse);

	sfTexture* playerTexture = GetAsset("Assets/Sprites/Player/Player.png");
	player.sprite = CreateSprite(playerTexture, (sfVector2f) { 200, 200 }, 4.f, 40);
	player.animation.rectActualy = (sfIntRect){ 0, (player.direction + player.state) * PLAYER_HEIGHT, PLAYER_WIDTH, PLAYER_HEIGHT };
	sfSprite_setTextureRect(player.sprite, (sfIntRect) { 0, (player.direction + player.state)* PLAYER_HEIGHT, PLAYER_WIDTH, PLAYER_HEIGHT });
	sfSprite_setOrigin(player.sprite, (sfVector2f) { PLAYER_WIDTH / 2, PLAYER_HEIGHT });
	player.animation.isLooping = sfTrue;

	sfSprite_setPosition(player.sprite, (sfVector2f) { 64 + 64 * player.posGrid.x, 224 + 64 * player.posGrid.y });
}

void UpdatePlayer(CasePosibility _GetMovePosibility, float _dt)
{
	MovePlayer(_GetMovePosibility, _dt);

	if (player.isInvincible)
	{
		invincibleTime += _dt;
		sfSprite_setColor(player.sprite, sfColor_fromRGBA(255, 255, 255, 127));

		if (invincibleTime >= INVINCIBLE_DURATION)
		{
			player.isInvincible = sfFalse;
		}
	}
	else
	{
		sfSprite_setColor(player.sprite, sfColor_fromRGBA(255, 255, 255, 255));
	}

	if (DEV_MODE)
	{
		if (sfKeyboard_isKeyPressed(sfKeyUp))
		{
			if (player.playerMoveSpeedGrid < 8)
			{
				player.playerMoveSpeedGrid++;
			}
		}
		else if (sfKeyboard_isKeyPressed(sfKeyDown))
		{

			if (player.playerMoveSpeedGrid > 1)
			{
				player.playerMoveSpeedGrid--;
			}
		}
	}
	else
	{
		player.playerMoveSpeedGrid = GetIntFromSave(SPEED);
	}
	player.animation.frameDuration = (float)1 / (8 + player.playerMoveSpeedGrid);
}

void MovePlayer(CasePosibility _GetMovePosibility, float _dt)
{
	lastMove += _dt;

	if (player.isWalking == sfFalse)
	{
		State temp = player.state;

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
			if (_GetMovePosibility.left == 0)
			{
				player.state = IDLE;
			}
		}
		else if (sfKeyboard_isKeyPressed(sfKeyD))
		{
			player.direction = RIGHT;
			player.state = WALK;
			if (_GetMovePosibility.right == 0)
			{
				player.state = IDLE;
			}
		}
		else if (sfKeyboard_isKeyPressed(sfKeyZ))
		{
			player.direction = UP;
			player.state = WALK;
			if (_GetMovePosibility.up == 0)
			{
				player.state = IDLE;
			}
		}
		else if (sfKeyboard_isKeyPressed(sfKeyS))
		{
			player.direction = DOWN;
			player.state = WALK;
			if (_GetMovePosibility.down == 0)
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

		if (temp == player.state)
		{
			player.animation.rectActualy = (sfIntRect){ player.animation.rectActualy.left, (player.direction + player.state) * PLAYER_HEIGHT, PLAYER_WIDTH, PLAYER_HEIGHT };
		}
		else
		{
			player.animation.rectActualy = (sfIntRect){ 0, (player.direction + player.state) * PLAYER_HEIGHT, PLAYER_WIDTH, PLAYER_HEIGHT };
		}

		if (player.state == WALK)
		{
			if (lastMove > (1.f / (10 + player.playerMoveSpeedGrid)))
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


				sfVector2i pos = (sfVector2i){ sfSprite_getPosition(player.sprite).x,sfSprite_getPosition(player.sprite).y };
				lastMove = 0;
				player.moveLength = (sfVector2f){ (64 + 64 * player.posGrid.x - pos.x) / (30 - player.playerMoveSpeedGrid*2), (224 + 64 * player.posGrid.y - pos.y) / (30 - player.playerMoveSpeedGrid*2) };
				player.isWalking = sfTrue;
			}
		}
	}
	else
	{
		sfVector2i pos = (sfVector2i){ sfSprite_getPosition(player.sprite).x,sfSprite_getPosition(player.sprite).y };

		sfSprite_move(player.sprite, player.moveLength);

		switch (player.direction)
		{
		case DOWN:
			if (pos.y >= 224 + 64 * player.posGrid.y)
			{
				player.isWalking = sfFalse;
				sfSprite_setPosition(player.sprite, TransformVector2iToVector2f(player.posGrid));
			}
			break;
		case LEFT:
			if (pos.x <= 64 + 64 * player.posGrid.x)
			{
				player.isWalking = sfFalse;
				sfSprite_setPosition(player.sprite, TransformVector2iToVector2f(player.posGrid));
			}
			break;
		case RIGHT:
			if (pos.x >= 64 + 64 * player.posGrid.x)
			{
				player.isWalking = sfFalse;
				sfSprite_setPosition(player.sprite, TransformVector2iToVector2f(player.posGrid));
			}
			break;
		case UP:
			if (pos.y <= 224 + 64 * player.posGrid.y)
			{
				player.isWalking = sfFalse;
				sfSprite_setPosition(player.sprite, TransformVector2iToVector2f(player.posGrid));
			}
			break;
		default:
			break;
		}
	}
	UpdateAnimationAndGiveIfStop(player.sprite, &player.animation, _dt);
}

sfVector2i GetPlayerPositionGrid(void)
{
	return player.posGrid;
}

sfBool AskPlayerIdle(void)
{
	if (player.state == IDLE)
	{
		return sfTrue;
	}
	else
	{
		return sfFalse;
	}
}

sfBool AskPlayerInvincible(void)
{
	if (player.isInvincible)
	{
		return sfTrue;
	}
	else
	{
		return sfFalse;
	}
}

void KillPlayer(void)
{

	invincibleTime = 0;
	player.isInvincible = sfTrue;
	player.posGrid = (sfVector2i){ 0 };
	sfSprite_setPosition(player.sprite, TransformVector2iToVector2f(player.posGrid));
	AddIntToSave(LIFE, -1);
	if (GetIntFromSave(LIFE) < 0)
	{
		SetIntToSave(LIFE, 0);
	}

	sfSound_play(player.killSound);
}
