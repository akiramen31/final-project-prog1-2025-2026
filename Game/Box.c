#include "Box.h"

Box box;

void LoadBox(void)
{
	box = (Box){ 0 };
	box.boxCount = 40;
	sfTexture* texture = GetAsset("Assets/Sprites/Map/Block.png");
	for (int i = 0; i < NB_BOX; i++)
	{
		float scale = 4.f;
		box.entity[i].spriteDown = CreateSprite(texture, (sfVector2f) { SCREEN_WIDTH, SCREEN_HEIGHT }, scale, 41.f);
		sfSprite_setTextureRect(box.entity[i].spriteDown, (sfIntRect){0, 4, 16, 16});

		box.entity[i].spriteUp = CreateSprite(texture, (sfVector2f) { SCREEN_WIDTH, SCREEN_HEIGHT }, scale, 1.f);
		sfSprite_setTextureRect(box.entity[i].spriteUp, (sfIntRect) { 0, 0, 16, 6 });
		sfSprite_setOrigin(box.entity[i].spriteUp, (sfVector2f) { 0.f, 4.f});

	}
}

void UpdateBox(float _dt)
{
	for (int i = 0; i < NB_BOX; i++)
	{
		if (box.entity[i].destroy)
		{

		}
	}
}

void SetBoxPosition(sfVector2i _position, int _index)
{
	sfVector2f position = { FIRST_CASE.left + FIRST_CASE.width * _position.x, FIRST_CASE.top + FIRST_CASE.height * _position.y };
	sfSprite_setPosition(box.entity[_index].spriteUp, position);
	sfSprite_setPosition(box.entity[_index].spriteDown, position);
}
  
void DestroyBox(sfVector2i _position)
{

}
