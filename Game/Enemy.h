#ifndef ENEMY_H
#define ENEMY_H

#include "Common.h"

#define TIMER_ASTAR 0.5f
#define JUMP_FORCE 5
#define MAX_ENRGIE 300

#pragma region Struct
typedef enum EnemyType
{
	DRONE_SMALL,
	GROUND_HEAVY,
	SOLDIER_SMALL,
	ALEATORY,
}EnemyType;

#pragma region AStar
typedef struct ActionDemander
{
	char gauche;
	char droite;
	char Saut;
	char jetPack;
	char bas;
}ActionDemander;

typedef struct Case2
{
	int jumpForce;
	Direction direction;
	char jetPackActive;
	int compteur;
}Case2;

typedef struct Tableau
{
	Case2** grid[ALEATORY];
	sfFloatRect region;
	char** collision;
	sfBool new[ALEATORY];
}Tableau;
#pragma endregion

typedef struct EnemyDataByType
{
	sfTexture* texture;
	ArmorType armure;
	float trust;
	float consomation;
	float lifeMax;
	float energyMax;
	float energyRegen;
	float speedMax;
	float jumForce;
	float shootCooldown;
}EnemyDataByType;

typedef struct EnemyEntity
{
	sfSprite* sprite;
	EnemyType type;
	sfVector2f velocity;
	ActionDemander action;
	sfFloatRect region;
	float life;
	float freezeTimer;
	float freezePower;
	float shootTimer;
	float aStarTimer;
	float energy;
}EnemyEntity;

typedef struct Enemy
{
#pragma region AStar
	List* listeWait;
	Tableau tableau;
#pragma endregion

	EnemyDataByType dataByType[ALEATORY];
	EnemyEntity* entity;
	int activeCount;
	unsigned count;
}Enemy;
#pragma endregion

void LoadEnemy(void);
void UpdateEnemy(float _dt);
void AddEnemy(sfVector2f _position, enum EnemyType _type, sfFloatRect _region);;
sfBool HitEnemy(float _degat, sfFloatRect _hitbox, AttackType _attaque);
sfVector2f GetPositionEnemy(unsigned _index);
sfFloatRect GetBounsEnemy(unsigned _index);
int GetEnemyCount(void);
void SetPositionEnemy(sfVector2f _position, unsigned _index);
int GetEnemyZone(void);

#endif // !GAME_H

