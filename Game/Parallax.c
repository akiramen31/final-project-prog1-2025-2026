#include "Parallax.h"
#include "Camera.h"
#include "Map.h"

parallaxLayers parallax;

void LoadParallax()
{
	parallax.offsetX[0] = 64;
	parallax.offsetX[1] = 256;
	parallax.offsetY[0] = 64;
	parallax.offsetY[1] = 128;
	for (int i = 0; i < LAYER_COUNT; i++)
	{
		sfVector2u tempSize;
		switch (i)
		{
		case 0:
			parallax.layer[0] = LoadBackground(GetAsset("Assets/Maps/game_background.png"), 1.f);
			break;
		case 1:
			parallax.layer[i] = CreateSprite(GetAsset("Assets/Maps/parallax_lv1_la1.png"), (sfVector2f) { 0 }, 1.f, 1000 + (10 * i));
			break;
		}
		tempSize = sfTexture_getSize(sfSprite_getTexture(parallax.layer[i]));
		sfSprite_setTextureRect(parallax.layer[i], (sfIntRect) { parallax.offsetX[i], parallax.offsetY[i], tempSize.x - parallax.offsetX[i], tempSize.y - parallax.offsetY[i] });
		sfSprite_setOrigin(parallax.layer[i], (sfVector2f) { tempSize.x / 2, tempSize.y / 2 });
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
	sfVector2f cameraPos = GetViewPosition();
	sfVector2f cameraSize = GetViewSize();
	sfVector2f cameraCenterPos = { cameraPos.x + (cameraSize.x / 2) ,cameraPos.y + (cameraSize.y / 2) };
	float ratioX = cameraCenterPos.x / ((mapSize.x*16) - (cameraSize.x / 2));
	float ratioY = cameraCenterPos.y / ((mapSize.y*16) - (cameraSize.y / 2));
	printf("All vars:\nCameraCenterPos    x: %f    y: %f\nmapSize    x: %u    y: %u\ncameraSize    x: %f    y: %f\nRatio    x: %f    y: %f\n", cameraCenterPos.x, cameraCenterPos.y, mapSize.x, mapSize.y, cameraSize.y, cameraSize.y, ratioX, ratioY);

	for (int i = 0; i < LAYER_COUNT; i++)
	{
		sfIntRect textureRect = sfSprite_getTextureRect(parallax.layer[i]);
		textureRect.left = parallax.offsetX[i] * ratioX;
		textureRect.top = parallax.offsetY[i] * ratioY;
		sfSprite_setPosition(parallax.layer[i], cameraCenterPos);
		sfSprite_setTextureRect(parallax.layer[i], textureRect);
	}
}