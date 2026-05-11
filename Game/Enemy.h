#ifndef ENEMY_H
#define ENEMY_H

#include "Common.h"

#if DEV_PIERRE_ENEMY


void LoadEnemy(void);
void UpdateEnemy(float _dt);
void AddEnemy(sfVector2f _position, enum EnemyType _type, sfFloatRect _region);;
sfBool HitEnemy(float _degat, sfFloatRect _hitbox, AttackType _attaque);
sfVector2f GetPositionEnemy(unsigned _index);
sfFloatRect GetBounsEnemy(unsigned _index);
int GetEnemyCount(void);
void SetPositionEnemy(sfVector2f _position, unsigned _index);
sfVector2u RealPositionConvertTableauPosition(sfVector2f _positionReal);
int GetEnemyZone(void);

#else
#define MAX_FALL_SPEED_ENEMY 1000

typedef enum EnemyType
{
	SOLDIER_SMALL,
	SOLDIER_MEDIUM,
	SOLDIER_HEAVY,
	DRONE_SMALL,
	DRONE_MEDIUM,
	DRONE_HEAVY,
	CROWLER_SMALL,
	CROWLER_MEDIUM,
	CROWLER_HEAVY,
	ALEATORY,
}EnemyType;

typedef struct EnemyData
{
	sfTexture* texture;
	float speed;
	float maxEnegie;
	float jumpForce;
	float weight;
	float lifeMax;
}EnemyData;

typedef struct EnemyEntity
{
	sfSprite* sprite;
	sfVector2f acceleration;
	EnemyType type;
	State state;
	float life;
	float energy;
	sfFloatRect region;
	sfVector2f velocity;
	char actif;
}EnemyEntity;

typedef struct IAData
{
	sfVector2u mapSize;
	char** moveMap[ALEATORY];
	char** coliderMap;
	sfFloatRect region;
}IAData;

typedef struct Enemy
{
	EnemyData data[ALEATORY];
	IAData IAData;
	EnemyEntity* entity;
	int count;
}Enemy;

void LoadEnemy(void);
void UpdateEnemy(float _dt); 
sfBool HitEnemy(float _degat, sfFloatRect _hitbox);
void ResetEnemy(void);
void AddEnemy(sfVector2f _position, EnemyType _type, sfFloatRect _region);

#endif //  DEV_PIERRE_ENEMY == 1


#endif // !GAME_H

