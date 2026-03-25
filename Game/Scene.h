#ifndef SCENE_H
#define SCENE_H

#include "Common.h"

typedef enum SceneState
{
	LEVEL1,
	LEVEL2,
	LEVEL3
}SceneState;

typedef struct HitboxMap
{
	sfImage* image;
	sfVector2u size;
	int ratio;
}HitboxMap;

typedef struct Scene
{
	HitboxMap hitbox;
	sfSprite* map;
	SceneState state;

}Scene;

void LoadScene(void);
void SetScene(SceneState _scene);
HitboxMap* GetSceneImageHitbox(void);


#endif // !SCENE_H