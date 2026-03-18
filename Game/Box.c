#include "Box.h"

Box box;

void LoadBox(void)
{
	box = (Box){ 0 };
	box.boxCount = 40;
	sfTexture* texture = GetAsset("Assets/Sprites/Map/Block.png");
	for (int i = 0; i < NB_BOX; i++)
	{
		box.entity[i].sprite = CreateSprite(texture, (sfVector2f) { SCREEN_WIDTH, SCREEN_HEIGHT }, 4.f, 1.f);
		sfSprite_setTextureRect(box.entity[i].sprite, (sfIntRect){0, 0, 18, 20});
	}
}

void UpdateBox(float _dt)
{

}

void SetBoxPosition(sfVector2i _position, int _index)
{

	sfSprite_setPosition(box.entity[_index].sprite, _position)
}

void DestroyBox(sfVector2i _position)
{

}
