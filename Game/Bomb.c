#include "Bomb.h"

sfTexture* bombTexture;
Bomb bombList[NUM_MAX_BOMB];

sfTexture* ExplosionTexture;

int bombCount = 0;

void SortBombList(int _index);

void LoadBomb(void)
{
	bombCount = 0;

	for (int i = 0; i < NUM_MAX_BOMB; i++)
	{
		bombList[i] = (Bomb){ 0 };
	}

	bombTexture = GetAsset("Assets/Sprites/Bomb/Bomb.png");

	ExplosionTexture = GetAsset("Assets/Sprites/Bomb/Explosion.png");
}

void SpawnBomb(sfVector2i _bombPos)
{
	bombCount++;
	//if (bombCount > GetIntToSave(bombCount))
	//{
	//	bombCount--;
	//	return;
	//}
	for (int i = 0; i < bombCount; i++)
	{
		if (bombList[i].placed == sfFalse)
		{
			printf("spawn bomb num %d\n", i + 1);
			bombList[i].position = _bombPos;

			bombList[i].sprite = CreateSprite(bombTexture, (sfVector2f) { 200, 200 }, 4.f, 41);

			sfSprite_setTextureRect(bombList[i].sprite, (sfIntRect) { 0, 0, 16, 16 });

			sfSprite_setOrigin(bombList[i].sprite, (sfVector2f) { 8, 16 });
			sfSprite_setPosition(bombList[i].sprite, TransformVector2iToVector2f(_bombPos));

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
		CreateDeflagration(DOWN, _colision.down);
	}
	if (_colision.right)
	{
		bombList[_num].blowDirectionCode += BLOWRIGHT;
		CreateDeflagration(RIGHT, _colision.right);
	}
	if (_colision.up)
	{
		bombList[_num].blowDirectionCode += BLOWUP;
		CreateDeflagration(UP, _colision.up);
	}
	if (_colision.left)
	{
		bombList[_num].blowDirectionCode += BLOWLEFT;
		CreateDeflagration(LEFT, _colision.left);
	}

	printf("blown direction %d\n", bombList[_num].blowDirectionCode);
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

void CreateDeflagration(Direction _direction, int _length)
{
	switch (_direction)
	{
	case DOWN:
		if (_length = 1)
		{

		}
		break;
	case LEFT:

		break;
	case RIGHT:

		break;
	case UP:

		break;
	default:
		break;
	}
}
