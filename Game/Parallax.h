#ifndef PARALLAX_H
#define PARALLAX_H

#include "Common.h"

#define LAYER_COUNT 2

typedef struct parallaxLayers
{
	sfSprite* layer[LAYER_COUNT];
	int offsetX[LAYER_COUNT];
	int offsetY[LAYER_COUNT];
}parallaxLayers;

void LoadParallax();
void SetParallaxLayerTexture(int _layer, sfTexture* _texture);
void SetParallaxLayerBaseOffset(int _layer, int _baseOffset, int _baseOffsetY);
void UpdateParallax(float _dt);
void SizeParallaxToCamera (void);
#endif // !PARALLAX_H