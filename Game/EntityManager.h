#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include "Common.h"

typedef enum VisualEntityType
{
	SPRITE,
	TEXT
}VisualEntityType;

typedef enum SoundEntityType
{
	SOUND,
	MUSIC
}SoundEntityType;

typedef struct VisualEntity
{
	VisualEntityType type;
	void* ptr;
	float drawPlan;
	struct VisualEntity* next;
}VisualEntity;

typedef struct SoundEntity
{
	SoundEntityType type;
	void* ptr;
}SoundEntity;

typedef struct AssetEntity
{
	void* ptr;
	char* file;
}AssetEntity;

typedef struct EntityManager
{
	AssetEntity* asset;
	int assetCount;
	int generalAssetCount;
	VisualEntity* visual;
	SoundEntity* sound;
	int soundCount;

}EntityManager;

void LoadEntityManager(void);
void Draw(sfRenderWindow* _renderWindow);
void CleanupEntityManager(void);
void CleanupTempEntity(void);

void* GetAsset(char* _file);

sfSprite* LoadBackground(sfTexture* _texture);
sfSprite* CreateSprite(sfTexture* _texture, sfVector2f _position, float _scale, float _drawPlan);
sfText* CreateText(sfFont* _font, sfVector2f _position, float _scale, float _drawPlan);
sfSound* CreateSound(sfSoundBuffer* _buffer, float _volume, sfBool _play);
sfMusic* CreateMusic(char* _fileMusic, float _volume, sfBool _play);

void DestroyVisualEntity(void* _entity);
void DestroySoundEntity(void* _entity);
void DestroyAssetEntity(void* _entity);
#endif // !ENTITY_MANAGER_H