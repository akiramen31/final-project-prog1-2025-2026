#ifndef ENNEMY_H
#define ENNEMY_H

#include "Common.h"
#include "Map.h"
#include "Player.h"

#if DEV_PIERRE_ENEMY == 1

#define TIMER_ASTAR 0.5f
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
	sfFloatRect region;
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

typedef struct Case2
{
	int jumpForce;
	enum Direction direction;
	sfBool jetPackActive;
	int compteur;
}Case2;

typedef struct Tableau
{
	Case2** grid[ALEATORY];
	sfFloatRect region[ALEATORY];
	char** collision;
	sfBool new[ALEATORY];
}Tableau;

void LoadEnemy(void);
void UpdateEnemy(float _dt);
void AddEnemy(sfVector2f _position, enum EnemyType _type, sfFloatRect _region);
sfBool HitEnemyI(unsigned _index, sfVector2f _touch, float _degat);
sfBool HitEnemy(float _degat, sfFloatRect _hitbox);
sfVector2f GetPositionEnemy(int _index);
sfFloatRect GetBounsEnemy(int _index);
int GetEnemyCount(void);
void SetPositionEnemy(sfVector2f _position, int _index);
sfVector2u RealPositionConvertTableauPosition(sfVector2f _positionReal);
void ResetEnemy(void);
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
	char* gridColider;
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

