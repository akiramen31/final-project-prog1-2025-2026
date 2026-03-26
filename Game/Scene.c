#include "Scene.h"

Scene scene;

void LoadScene(void)
{
    scene.map = LoadBackground(GetAsset("Assets/Maps/Reduite1.png"), 60.f);
    SetScene(LEVEL1);
}

void SetScene(SceneState _scene)
{
    {
        char buffer[] = "Assets/Maps/Reduite1.png";
        //buffer[22] = '1' + _scene;
        sfTexture* texture = GetAsset(buffer);
        scene.hitbox.image = sfTexture_copyToImage(texture);
        scene.hitbox.ratio = TILE_SIZE;
        sfVector2u size = sfImage_getSize(scene.hitbox.image);
        scene.hitbox.size = (sfVector2u){ size.x, size.y};
        DestroyAssetEntity(texture);
    } 
    {
        char buffer[] = "Assets/Maps/Reduite1.png";
        //buffer[22] = '1' + _scene;
        sfSprite_setTexture(scene.map, GetAsset(buffer), sfFalse);
    }
}

HitboxMap* GetSceneImageHitbox(void)
{
    return &scene.hitbox;
}
