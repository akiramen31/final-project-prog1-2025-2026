#include "Parallax.h"
#include "Camera.h"
#include "Map.h"

parallaxLayers parallax;

void LoadParallax()
{
	parallax.offsetX[0] = 64;
	parallax.offsetX[1] = 256;
	parallax.offsetY[0] = 64;
	parallax.offsetY[1] = 256;
	for (int i = 0; i < LAYER_COUNT; i++)
	{
		switch (i)
		{
		case 0:
			sfTexture_setRepeated(GetAsset("Assets/Maps/game_background.png"), sfTrue);
			parallax.layer[0] = LoadBackground(GetAsset("Assets/Maps/game_background.png"), 1.f);
			break;
		case 1:
			sfTexture_setRepeated(GetAsset("Assets/Maps/parallax_lv1_la1.png"), sfTrue);
			parallax.layer[i] = CreateSprite(GetAsset("Assets/Maps/parallax_lv1_la1.png"), (sfVector2f) { 0 }, 1.f, 1000 + (10 * i));
			break;
		}
		sfSprite_setTextureRect(parallax.layer[i], (sfIntRect) {0, 0, 384, 216});
		sfSprite_setOrigin(parallax.layer[i], (sfVector2f) { 384 / 2, 216 / 2 });
		sfSprite_setPosition(parallax.layer[i], GetViewPosition());
		sfSprite_setColor(parallax.layer[i], sfColor_fromRGB(200 + 30 * i, 200 + 30 * i,200+30*i));
	}
}

void SetParallaxLayerTexture(int _layer, sfTexture* _texture)
{
	sfSprite_setTexture(parallax.layer[_layer], _texture, sfFalse);
}

void SetParallaxLayerBaseOffset(int _layer, int _baseOffsetX, int _baseOffsetY)
{
	parallax.offsetX[_layer] = _baseOffsetX;
	parallax.offsetY[_layer] = _baseOffsetY;
}

void UpdateParallax(float _dt)
{
	sfVector2u mapSize = GetMapData()->size;
	sfVector2f viewPos = GetViewPosition();
	sfVector2f viewSize = GetViewSize();
	sfVector2f viewCenterPos = { viewPos.x + (viewSize.x / 2) ,viewPos.y + (viewSize.y / 2) };
	float ratioX = viewCenterPos.x / ((mapSize.x*16) - (viewSize.x / 2));
	float ratioY = viewCenterPos.y / ((mapSize.y * 16) - (viewSize.y / 2));
	for (int i = 0; i < LAYER_COUNT; i++)
	{
		sfIntRect textureRect = sfSprite_getTextureRect(parallax.layer[i]);
		textureRect.left = parallax.offsetX[i] * ratioX;
		textureRect.top = parallax.offsetY[i] * ratioY;
		sfSprite_setPosition(parallax.layer[i], viewCenterPos);
		sfSprite_setTextureRect(parallax.layer[i], textureRect);
	}
}

void SizeParallaxToCamera()
{
	for (int i = 0; i < LAYER_COUNT; i++)
	{
		sfIntRect textureRect = sfSprite_getTextureRect(parallax.layer[i]);
		sfVector2f viewSize = GetViewSize();
		textureRect.width = viewSize.x;
		textureRect.height = viewSize.y;
		sfSprite_setOrigin(parallax.layer[i], (sfVector2f) {viewSize.x/2, viewSize.y/2});
		sfSprite_setTextureRect(parallax.layer[i], textureRect);
	}
}