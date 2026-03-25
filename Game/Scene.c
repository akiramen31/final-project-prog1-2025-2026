#include "Scene.h"

Scene scene;

void LoadScene(void)
{
    scene.map = LoadBackground(GetAsset("Assets/Niveaux/Reduite1.png"), 60.f);
}

void SetScene(SceneState _scene)
{
    {
        char buffer[] = "Assets/Niveaux/Reduite1.png";
        buffer[23] = '1' + _scene;
        sfTexture* texture = GetAsset(buffer);
        scene.hitbox.image = sfTexture_copyToImage(texture);
        scene.hitbox.ratio = 60.f;
        sfVector2u size = sfImage_getSize(scene.hitbox.image);
        scene.hitbox.size = (sfVector2u){ size.x * scene.hitbox.ratio, size.y * scene.hitbox.ratio };
        DestroyAssetEntity(texture);
    } 
    {
        char buffer[] = "Assets/Niveaux/Reduite1.png";
        buffer[23] = '1' + _scene;
        sfSprite_setTexture(scene.map, GetAsset(buffer), sfFalse);
    }

}

HitboxMap* GetSceneImageHitbox(void)
{
    return &scene.hitbox;
}
