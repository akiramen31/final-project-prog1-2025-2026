#include "Box.h"

Box box[NB_BOX];

void LoadBox(void)
{
	sfTexture* texture = GetAsset("Assets/Sprites/Map/Block.png");
	float scale = 4.f;
	for (int i = 0; i < NB_BOX; i++)
	{
		box[i] = (Box){ 0 };
		box[i].down.sprite = CreateSprite(texture, (sfVector2f) { SCREEN_WIDTH, SCREEN_HEIGHT }, scale, 44.f);
		sfSprite_setTextureRect(box[i].down.sprite, (sfIntRect) { 0, 4, 16, 16 });
		box[i].down.animation = (Animation){ (sfIntRect) { 0, 4, 16, 16 }, sfFalse, 4, 0.2f, 0.f };

		box[i].up.sprite = CreateSprite(texture, (sfVector2f) { SCREEN_WIDTH, SCREEN_HEIGHT }, scale, 1.f);
		sfSprite_setTextureRect(box[i].up.sprite, (sfIntRect) { 0, 0, 16, 6 });
		sfSprite_setOrigin(box[i].up.sprite, (sfVector2f) { 0.f, 4.f });
		box[i].up.animation = (Animation){ (sfIntRect) { 0, 0, 16, 6 }, sfFalse, 4, 0.2f, 0.f };
		box[i].destroy = sfFalse;
	}
}

void UpdateBox(float _dt)
{
	for (int i = 0; i < NB_BOX; i++)
	{
		if (box[i].destroy)
		{
			if (UpdateAnimationAndGiveIfStop(box[i].down.sprite, &box[i].down.animation, _dt) || 
				UpdateAnimationAndGiveIfStop(box[i].up.sprite, &box[i].up.animation, _dt))
			{
				box[i].position = (sfVector2i){ -1,-1 };
				sfSprite_setPosition(box[i].down.sprite, (sfVector2f) { SCREEN_WIDTH, SCREEN_HEIGHT });
				sfSprite_setPosition(box[i].up.sprite, (sfVector2f) { SCREEN_WIDTH, SCREEN_HEIGHT });
				box[i].destroy = sfFalse;
			}
		}
	}
}

void SetBoxPosition(sfVector2i _position, int _index)
{
	sfVector2f position = { (float)(FIRST_CASE.left + FIRST_CASE.width * _position.x), 
		(float)(FIRST_CASE.top + FIRST_CASE.height * _position.y) };
	sfSprite_setPosition(box[_index].up.sprite, position);
	sfSprite_setPosition(box[_index].down.sprite, position);
	box[_index].position = _position;
}

void DestroyBox(sfVector2i _position)
{
	for (int i = 0; i < NB_BOX; i++)
	{
		if (box[i].position.x == _position.x && box[i].position.y == _position.y)
		{
			box[i].destroy = sfTrue;
			i = NB_BOX;
		}
	}
}

sfBool GetIfBoxIsHere(sfVector2i _position)
{
	for (int i = 0; i < NB_BOX; i++)
	{
		if (box[i].position.x == _position.x && box[i].position.y == _position.y)
		{
			return sfTrue;
		}
	}
	return sfFalse;
}