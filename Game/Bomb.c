#include "Bomb.h"

sfTexture* bombTexture;
Bomb bombList[NUM_MAX_BOMB];
sfSound* bombExplode;
sfSound* bombPLace;
sfTexture* explosionTexture;
Deflagration deflagrationList[NUM_MAX_DEFLAGRATION];

int bombCount = 0;
int deflagrationCount = 0;

void SortBombList(int _index);
void SortDeflagrationList(int _index);

void LoadBomb(void)
{
	bombCount = 0;
	deflagrationCount = 0;

	for (int i = 0; i < NUM_MAX_BOMB; i++)
	{
		bombList[i] = (Bomb){ 0 };
	}
	for (int i = 0; i < NUM_MAX_DEFLAGRATION; i++)
	{
		deflagrationList[i] = (Deflagration){ 0 };
	}

	bombTexture = GetAsset("Assets/Sprites/Bomb/Bomb.png");

	explosionTexture = GetAsset("Assets/Sprites/Bomb/Explosion_Coded.png");

	bombExplode = CreateSound(GetAsset("Assets/Sounds/Bomb.wav"), GAME_VOLUME, sfFalse);
	bombPLace = CreateSound(GetAsset("Assets/Sounds/Placed.wav"), GAME_VOLUME, sfFalse);
}

void SpawnBomb(sfVector2i _bombPos)
{
	bombCount++;
	if (bombCount > GetIntFromSave(bombCount))
	{
		bombCount--;
		return;
	}
	sfSound_play(bombPLace);
	for (int i = 0; i < bombCount; i++)
	{
		if (bombList[i].placed == sfFalse)
		{
			bombList[i].position = _bombPos;

			bombList[i].sprite = CreateSprite(bombTexture, TransformVector2iToVector2f(_bombPos), 4.f, 70.f);

			sfSprite_setTextureRect(bombList[i].sprite, (sfIntRect) { 0, 0, 16, 16 });
			sfSprite_setOrigin(bombList[i].sprite, (sfVector2f) { 8, 16 });

			bombList[i].animation.frameCount = 3;
			bombList[i].animation.frameDuration = 0.1f;
			bombList[i].animation.isLooping = sfTrue;
			bombList[i].animation.rectActualy = (sfIntRect){ 0,0,16,16 };

			bombList[i].duration = 0;

			bombList[i].placed = sfTrue;
			return;
		}
		else
		{
			if (_bombPos.x == bombList[i].position.x && _bombPos.y == bombList[i].position.y)
			{
				bombCount--;
				return;
			}
		}
	}
}

void BlowBomb(int _num, CasePosibility _colision)
{
	sfSound_play(bombExplode);
	bombList[_num].blowDirectionCode = 0;

	if (_colision.downBox)
	{
		_colision.down++;
	}
	if (_colision.rightBox)
	{
		_colision.right++;
	}
	if (_colision.upBox)
	{
		_colision.up++;
	}
	if (_colision.leftBox)
	{
		_colision.left++;
	}

	if (_colision.down)
	{
		bombList[_num].blowDirectionCode += BLOW_DOWN;
		CreateDeflagration(BLOW_DOWN, _colision.down, bombList[_num].position);
	}
	if (_colision.right)
	{
		bombList[_num].blowDirectionCode += BLOW_RIGHT;
		CreateDeflagration(BLOW_RIGHT, _colision.right, bombList[_num].position);
	}
	if (_colision.up)
	{
		bombList[_num].blowDirectionCode += BLOW_UP;
		CreateDeflagration(BLOW_UP, _colision.up, bombList[_num].position);
	}
	if (_colision.left)
	{
		bombList[_num].blowDirectionCode += BLOW_LEFT;
		CreateDeflagration(BLOW_LEFT, _colision.left, bombList[_num].position);
	}

	bombList[_num].blowDirectionCode--;

	for (int i = 0; i < NUM_MAX_DEFLAGRATION; i++)
	{
		if (deflagrationList[i].placed == sfFalse)
		{
			deflagrationList[i].sprite = CreateSprite(explosionTexture, TransformVector2iToVector2f(bombList[_num].position), 4.f, 20.f);
			sfSprite_setOrigin(deflagrationList[i].sprite, (sfVector2f) { 8, 16 });
			deflagrationList[i].animation.frameCount = 3;
			deflagrationList[i].animation.frameDuration = 0.1f;
			deflagrationList[i].placed = sfTrue;
			deflagrationList[i].position = bombList[_num].position;
			deflagrationList[i].position = bombList[_num].position;
			sfSprite_setTextureRect(deflagrationList[i].sprite, (sfIntRect) { 0, 16 * bombList[_num].blowDirectionCode, 16, 16 });
			deflagrationList[i].animation.rectActualy = (sfIntRect){ 0, 16 * bombList[_num].blowDirectionCode,16,16 };
			deflagrationCount++;
			break;
		}
	}

	DestroyVisualEntity(bombList[_num].sprite);

	SortBombList(_num);
	bombCount--;
}

void BlowBombByDeflagration(int _num, CasePosibility _colision, BlowDirection _direction)
{
	if (_colision.downBox)
	{
		_colision.down++;
	}
	if (_colision.rightBox)
	{
		_colision.right++;
	}
	if (_colision.upBox)
	{
		_colision.up++;
	}
	if (_colision.leftBox)
	{
		_colision.left++;
	}

	bombList[_num].blowDirectionCode = 0;
	if (_colision.down)
	{
		bombList[_num].blowDirectionCode += BLOW_DOWN;
		if (_direction != BLOW_UP)
		{
			CreateDeflagration(BLOW_DOWN, _colision.down, bombList[_num].position);
		}
	}
	if (_colision.right)
	{
		bombList[_num].blowDirectionCode += BLOW_RIGHT;
		if (_direction != BLOW_LEFT)
		{
			CreateDeflagration(BLOW_RIGHT, _colision.right, bombList[_num].position);
		}
	}
	if (_colision.up)
	{
		bombList[_num].blowDirectionCode += BLOW_UP;
		if (_direction != BLOW_DOWN)
		{
			CreateDeflagration(BLOW_UP, _colision.up, bombList[_num].position);
		}
	}
	if (_colision.left)
	{
		bombList[_num].blowDirectionCode += BLOW_LEFT;
		if (_direction != BLOW_RIGHT)
		{
			CreateDeflagration(BLOW_LEFT, _colision.left, bombList[_num].position);
		}
	}

	bombList[_num].blowDirectionCode--;

	for (int i = 0; i < NUM_MAX_DEFLAGRATION; i++)
	{
		if (deflagrationList[i].placed == sfFalse)
		{
			deflagrationList[i].sprite = CreateSprite(explosionTexture, TransformVector2iToVector2f(bombList[_num].position), 4.f, 20.f);
			sfSprite_setOrigin(deflagrationList[i].sprite, (sfVector2f) { 8, 16 });
			deflagrationList[i].animation.frameCount = 3;
			deflagrationList[i].animation.frameDuration = 0.1f;
			deflagrationList[i].placed = sfTrue;
			deflagrationList[i].position = bombList[_num].position;
			deflagrationList[i].position = bombList[_num].position;
			sfSprite_setTextureRect(deflagrationList[i].sprite, (sfIntRect) { 0, 16 * bombList[_num].blowDirectionCode, 16, 16 });
			deflagrationList[i].animation.rectActualy = (sfIntRect){ 0, 16 * bombList[_num].blowDirectionCode,16,16 };
			deflagrationCount++;
			break;
		}
	}

	DestroyVisualEntity(bombList[_num].sprite);

	SortBombList(_num);
	bombCount--;
}

Explosion UpdateBomb(CasePosibility _colision[], float _dt)
{

	for (int i = deflagrationCount - 1; i >= 0; i--)
	{
		if (UpdateAnimationAndGiveIfStop(deflagrationList[i].sprite, &deflagrationList[i].animation, _dt))
		{
			DestroyVisualEntity(deflagrationList[i].sprite);

			SortDeflagrationList(i);
			deflagrationCount--;
		}
	}

	for (int i = 0; i < bombCount; i++)
	{
		for (int j = deflagrationCount - 1; j >= 0; j--)
		{
			if (bombList[i].position.x == deflagrationList[j].position.x && bombList[i].position.y == deflagrationList[j].position.y)
			{
				BlowBombByDeflagration(i, _colision[i], deflagrationList[j].direction);
			}
		}
	}
	for (int i = 0; i < bombCount; i++)
	{
		UpdateAnimationAndGiveIfStop(bombList[i].sprite, &bombList[i].animation, _dt);
		bombList[i].duration += _dt;

		if (bombList[i].duration >= BLOW_TIMER_BOMB)
		{
			BlowBomb(i, _colision[i]);
			return (Explosion) {sfTrue, i};
		}
	}
	return (Explosion) { sfFalse, -1 };
}

sfBool CheckAtLocationBomb(sfVector2i _pos)
{
	for (int i = 0; i < bombCount; i++)
	{
		if (_pos.x == bombList[i].position.x && _pos.y == bombList[i].position.y)
		{
			return sfTrue;
		}
		else
		{
			return sfFalse;
		}
	}
	return sfFalse;
}

int GetBombCount(void)
{
	return bombCount;
}

sfVector2i GetBombPositionGrid(int _num)
{
	return bombList[_num].position;
}

void SortBombList(int _index)
{
	for (int i = _index; i < bombCount - 1; i++)
	{
		bombList[i] = bombList[i + 1];
	}
	bombList[bombCount - 1] = (Bomb){ 0 };
}

void SortDeflagrationList(int _index)
{
	for (int i = _index; i < deflagrationCount - 1; i++)
	{
		deflagrationList[i] = deflagrationList[i + 1];
	}
	deflagrationList[deflagrationCount - 1] = (Deflagration){ 0 };
}

void CreateDeflagration(BlowDirection _direction, int _length, sfVector2i _position)
{
	if (_length >= GetIntFromSave(FIRE))
	{
		_length = GetIntFromSave(FIRE);
	}

	int test = 2;

	if (_length > test)
	{
		_length = test;
	}
	int tempLength = _length;

	for (int j = 0; j < _length; j++)
	{

		for (int i = 0; i < NUM_MAX_DEFLAGRATION; i++)
		{
			if (deflagrationList[i].placed == sfFalse)
			{
				deflagrationList[i].sprite = CreateSprite(explosionTexture, (sfVector2f) { 0 }, 4.f, 20.f);
				sfSprite_setOrigin(deflagrationList[i].sprite, (sfVector2f) { 8, 16 });
				deflagrationList[i].animation.frameCount = 3;
				deflagrationList[i].animation.frameDuration = 0.1f;
				deflagrationList[i].placed = sfTrue;
				deflagrationList[i].direction = _direction;
				deflagrationCount++;

				switch (_direction)
				{
				case BLOW_DOWN:
					_position.y++;
					sfSprite_setPosition(deflagrationList[i].sprite, TransformVector2iToVector2f(_position));
					deflagrationList[i].position = _position;

					if (tempLength == 1)
					{
						sfSprite_setTextureRect(deflagrationList[i].sprite, (sfIntRect) { 0, 16 * 3, 16, 16 });
						deflagrationList[i].animation.rectActualy = (sfIntRect){ 0, 16 * 3,16,16 };
						return;
					}
					else
					{
						sfSprite_setTextureRect(deflagrationList[i].sprite, (sfIntRect) { 0, 16 * 4, 16, 16 });
						deflagrationList[i].animation.rectActualy = (sfIntRect){ 0, 16 * 4,16,16 };
					}
					break;
				case BLOW_LEFT:
					_position.x--;
					sfSprite_setPosition(deflagrationList[i].sprite, TransformVector2iToVector2f(_position));
					deflagrationList[i].position = _position;

					if (tempLength == 1)
					{
						sfSprite_setTextureRect(deflagrationList[i].sprite, (sfIntRect) { 0, 16 * 1, 16, 16 });
						deflagrationList[i].animation.rectActualy = (sfIntRect){ 0, 16 * 1,16,16 };
						return;
					}
					else
					{
						sfSprite_setTextureRect(deflagrationList[i].sprite, (sfIntRect) { 0, 16 * 9, 16, 16 });
						deflagrationList[i].animation.rectActualy = (sfIntRect){ 0, 16 * 9,16,16 };
					}
					break;
				case BLOW_RIGHT:
					_position.x++;
					sfSprite_setPosition(deflagrationList[i].sprite, TransformVector2iToVector2f(_position));
					deflagrationList[i].position = _position;

					if (tempLength == 1)
					{
						sfSprite_setTextureRect(deflagrationList[i].sprite, (sfIntRect) { 0, 16 * 7, 16, 16 });
						deflagrationList[i].animation.rectActualy = (sfIntRect){ 0, 16 * 7,16,16 };
						return;
					}
					else
					{
						sfSprite_setTextureRect(deflagrationList[i].sprite, (sfIntRect) { 0, 16 * 9, 16, 16 });
						deflagrationList[i].animation.rectActualy = (sfIntRect){ 0, 16 * 9,16,16 };
					}
					break;
				case BLOW_UP:
					_position.y--;
					sfSprite_setPosition(deflagrationList[i].sprite, TransformVector2iToVector2f(_position));
					deflagrationList[i].position = _position;
					if (tempLength == 1)
					{
						sfSprite_setTextureRect(deflagrationList[i].sprite, (sfIntRect) { 0, 16 * 0, 16, 16 });
						deflagrationList[i].animation.rectActualy = (sfIntRect){ 0, 16 * 0,16,16 };
						return;
					}
					else
					{
						sfSprite_setTextureRect(deflagrationList[i].sprite, (sfIntRect) { 0, 16 * 4, 16, 16 });
						deflagrationList[i].animation.rectActualy = (sfIntRect){ 0, 16 * 4,16,16 };
					}
					break;
				default:
					break;
				}
				tempLength--;
			}
		}
	}
}

int GetDeflagrationCount(void)
{
	return deflagrationCount;
}

sfVector2i GetDeflagrationPosition(int _index)
{
	return deflagrationList[_index].position;
}
