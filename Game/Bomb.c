#include "Bomb.h"

sfTexture* bombTexture;
Bomb bombList[NUM_MAX_BOMB];

sfTexture* ExplosionTexture;
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

	ExplosionTexture = GetAsset("Assets/Sprites/Bomb/Explosion_Coded.png");
}

void SpawnBomb(sfVector2i _bombPos)
{
	bombCount++;
	if (bombCount > GetIntToSave(bombCount))
	{
		bombCount--;
		return;
	}
	for (int i = 0; i < bombCount; i++)
	{
		if (bombList[i].placed == sfFalse)
		{
			//printf("spawn bomb num %d\n", i + 1);
			bombList[i].position = _bombPos;

			bombList[i].sprite = CreateSprite(bombTexture, TransformVector2iToVector2f(_bombPos), 4.f, 41);

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
	bombList[_num].blowDirectionCode = 0;
	if (_colision.down)
	{
		bombList[_num].blowDirectionCode += BLOWDOWN;
		CreateDeflagration(BLOWDOWN, _colision.down, bombList[_num].position);
	}
	if (_colision.right)
	{
		bombList[_num].blowDirectionCode += BLOWRIGHT;
		CreateDeflagration(BLOWRIGHT, _colision.right, bombList[_num].position);
	}
	if (_colision.up)
	{
		bombList[_num].blowDirectionCode += BLOWUP;
		CreateDeflagration(BLOWUP, _colision.up, bombList[_num].position);
	}
	if (_colision.left)
	{
		bombList[_num].blowDirectionCode += BLOWLEFT;
		CreateDeflagration(BLOWLEFT, _colision.left, bombList[_num].position);
	}

	bombList[_num].blowDirectionCode--;

	for (int i = 0; i < NUM_MAX_DEFLAGRATION; i++)
	{
		if (deflagrationList[i].placed == sfFalse)
		{
			deflagrationList[i].sprite = CreateSprite(ExplosionTexture, TransformVector2iToVector2f(bombList[_num].position), 4.f, 41);
			sfSprite_setOrigin(deflagrationList[i].sprite, (sfVector2f) { 8, 16 });
			deflagrationList[i].animation.frameCount = 3;
			deflagrationList[i].animation.frameDuration = 0.1f;
			deflagrationList[i].duration = 0;
			deflagrationList[i].placed = sfTrue;
			deflagrationList[i].position = bombList[_num].position;
			deflagrationList[i].position = bombList[_num].position;
			sfSprite_setTextureRect(deflagrationList[i].sprite, (sfIntRect) { 0, 16 * bombList[_num].blowDirectionCode, 16, 16 });
			deflagrationList[i].animation.rectActualy = (sfIntRect){ 0, 16 * bombList[_num].blowDirectionCode,16,16 };
			deflagrationCount++;
		}
	}

	DestroyVisualEntity(bombList[_num].sprite);

	SortBombList(_num);
	bombCount--;
}

void UpdateBomb(CasePosibility _colision[], float _dt)
{
	for (int i = 0; i < bombCount; i++)
	{
		if (bombCount == 0)
		{
			return;
		}

		UpdateAnimationAndGiveIfStop(bombList[i].sprite, &bombList[i].animation, _dt);
		bombList[i].duration += _dt;

		if (bombList[i].duration >= BLOW_TIMER_BOMB)
		{
			BlowBomb(i, _colision[i]);
		}
	}

	for (int i = deflagrationCount - 1; i >= 0; i--)
	{
		if (deflagrationCount == 0)
		{
			return;
		}

		deflagrationList[i].duration += _dt;

		if (UpdateAnimationAndGiveIfStop(deflagrationList[i].sprite, &deflagrationList[i].animation, _dt))
		{
			DestroyVisualEntity(deflagrationList[i].sprite);

			SortDeflagrationList(i);
			deflagrationCount--;
		}
	}
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

void CreateDeflagration(Direction _direction, int _length, sfVector2i _position)
{
	if (_length >= GetIntToSave(FIRE))
	{
		_length = GetIntToSave(FIRE);
	}

	int test = 2;

	//if (_length > test)
	//{
	//	_length = test;
	//}

	printf("%d\n", _length);


	int tempLength = _length;

	for (int j = 0; j < _length; j++)
	{

		for (int i = 0; i < NUM_MAX_DEFLAGRATION; i++)
		{
			if (deflagrationList[i].placed == sfFalse)
			{
				deflagrationList[i].sprite = CreateSprite(ExplosionTexture, (sfVector2f) { 0 }, 4.f, 41);
				sfSprite_setOrigin(deflagrationList[i].sprite, (sfVector2f) { 8, 16 });
				deflagrationList[i].animation.frameCount = 3;
				deflagrationList[i].animation.frameDuration = 0.1f;
				deflagrationList[i].duration = 0;
				deflagrationList[i].placed = sfTrue;
				deflagrationCount++;

				switch (_direction)
				{
				case BLOWDOWN:
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
				case BLOWLEFT:
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
				case BLOWRIGHT:
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
				case BLOWUP:
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
