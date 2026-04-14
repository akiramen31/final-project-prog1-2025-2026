#ifndef ENNEMY_H
#define ENNEMY_H

#include "Common.h"
#include "Map.h"
#include "Player.h"

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
void UpdateEnemy(float _dt, int _index);
void UpdateTotalEnemy(float _dt);
void AddEnemy(sfVector2f _position, enum EnemyType _type);
sfBool HitEnemy(unsigned _index, sfVector2f _touch, float _degat);
sfVector2u RealPositionConvertTableauPosition(sfVector2f _positionReal);
sfVector2f GetPositionEnemy(int _index);
sfFloatRect GetBounsEnemy(int _index);
int GetNumberEnemy(void);
sfColor GetColorsPixelMap(sfVector2f _position);
void SetPositionEnemy(sfVector2f _position, int _index);

void ResetEnemy(void);
int GetNearestEnemy(List* _listeIgnore, sfVector2f _position);

#endif // !GAME_H

