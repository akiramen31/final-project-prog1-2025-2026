#ifndef ENNEMY_H
#define ENNEMY_H

#include "Common.h"
#include "Map.h"
#include "Player.h"

#if DEV_PIERRE_ENEMY == 1

#define TIMER_ASTAR 0.1f
#define JUMP_FORCE 5
#define MAX_ENRGIE 300

typedef enum EnemyType
{
	DRONE_SMALL,
	DRONE_MEDIUM,
	DRONE_HEAVY,
	CROWLER_SMALL,
	CROWLER_MEDIUM,
	CROWLER_HEAVY,
	SOLDIER_SMALL,
	SOLDIER_MEDIUM,
	SOLDIER_HEAVY,
	ENEMY_TYPE_COUNT,
	ALEATORY,
}EnemyType;


typedef struct Jetpack
{
	float trust;
	float consomation;
	float life;
} Jetpack;

typedef struct EnnemyData
{
	float life;
	float energyMax;
	float energy;
	float energyRegen;
	float accelerationMax;
	float speedMax;
	float jumForce;
}EnnemyData;

typedef struct EnnemyEntity
{
	enum EnemyType type;
	struct EnnemyData ennemydata;
	sfBool isJetpack;
	struct Jetpack jetpack;
	sfVector2f acceleration;
	sfVector2f move;
	enum State state;
	float timer;
}EnnemyEntity;

typedef struct ActionDemander
{
	sfBool gauche;
	sfBool droite;
	sfBool Saut;
	sfBool jetPack;
}ActionDemander;

typedef struct Ennemy
{
	EnnemyEntity ennemyEntity;
	ActionDemander actiondemander;
	sfSprite* sprite;
	sfImage* imageColideur;
}Ennemy;

typedef struct Case
{
	int jumpForce;
	float rangeToDestination;
	float action;
	float resultat;
	float energie;
	enum Direction direction;
	sfBool jetPackActive;
}Case;

void LoadEnemy(void);
void UpdateEnemy(float _dt);
void AddEnemy(sfVector2f _position, enum EnemyType _type);
sfBool HitEnemy(unsigned _index, sfVector2f _touch, float _degat, sfFloatRect* _hitbox);
sfVector2f GetPositionEnemy(int _index);
sfFloatRect GetBounsEnemy(int _index);
int GetEnemyCount(void);
void SetPositionEnemy(sfVector2f _position, int _index);

void ResetEnemy(void);
sfBool HitEnemy(float _degat, sfFloatRect _hitbox);

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
}EnemyEntity;

typedef struct Enemy
{
	EnemyEntity* entity;
	EnemyData data[ALEATORY];
	int count;

}Enemy;

void LoadEnemy(void);
void UpdateEnemy(float _dt); 
sfBool HitEnemy(float _degat, sfFloatRect _hitbox);
void ResetEnemy(void);
void AddEnemy(sfVector2f _position, EnemyType _type, sfFloatRect _region);

#endif //  DEV_PIERRE_ENEMY == 1


#endif // !GAME_H

