#ifndef ENEMY_H
#define ENEMY_H

#include "Common.h"
#include "Map.h"
#include "Player.h"

#if DEV_PIERRE_ENEMY

#define TIMER_ASTAR 0.05f
#define JUMP_FORCE 5
#define MAX_ENRGIE 300

typedef enum EnemyType
{
	DRONE_SMALL,
	GROUND_HEAVY,
	SOLDIER_SMALL,
	ALEATORY,
}EnemyType;


typedef struct Jetpack
{
	float trust;
	float consomation;
	float life;
} Jetpack;

typedef struct EnemyData
{
	float life;
	float energyMax;
	float energy;
	float energyRegen;
	float accelerationMax;
	float speedMax;
	float jumForce;
	int nbCaseJump;
	ArmorType armure;
}EnemyData;

typedef struct EnemyEntity
{
	EnemyType type;
	EnemyData enemydata;
	sfBool isJetpack;
	Jetpack jetpack;
	sfVector2f move;
	int state;
	float timer;
	float timerTir;
	int powerGel;
	float timerGel;
	sfFloatRect region;
}EnemyEntity;

typedef struct ActionDemander
{
	sfBool gauche;
	sfBool droite;
	sfBool Saut;
	sfBool jetPack;
}ActionDemander;

typedef struct Enemy
{
	EnemyEntity enemyEntity;
	ActionDemander actiondemander;
	sfSprite* sprite;
	sfImage* imageColideur;
}Enemy;

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
void AddEnemy(sfVector2f _position, enum EnemyType _type, sfFloatRect _region);;
sfBool HitEnemy(float _degat, sfFloatRect _hitbox, AttackType _attaque);
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

