#ifndef ENNEMY_H
#define ENNEMY_H

#include "Common.h"
#include "Player.h"
#include "Box.h"

#define MAX_FALL_SPEED_ENEMY 1000

typedef struct EnemyData
{
	sfTexture* texture;
	float speed;
	float maxEnegie;
	float jumpForce;
	float weight;
}EnemyData;

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
	ENEMY_TYPE_COUNT
}EnemyType;

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
	EnemyData data[ENEMY_TYPE_COUNT];
	int count;

}Enemy;

void LoadEnemy(void);
void UpdateEnemy(float _dt);
void HitEnemy(sfFloatRect* _hitbox);
void ReloadEnemy(void);
void AddEnemy(sfVector2f _position, EnemyType _type, sfFloatRect _region);

/*

#ifndef ENNEMY_H
#define ENNEMY_H

#include "Common.h"
#include "Map.h"
#include "Player.h"

#define TIMER_ASTAR 0.1f
#define JUMP_FORCE 5
#define MAX_ENRGIE 300

typedef enum Type
{
	SOLDIER,
	FLYER,
	ALEATORY
}Type;

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
	enum Type type;
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
	float Résultat;
	float energie;
	enum Direction direction;
	sfBool jetPackActive;
}Case;

void LoadEnnemy(void);
void UpdateEnnemy(float _dt, int _index);
void AddEnnemy(sfVector2f _position, enum Type _type);
sfBool HitEnnemy(unsigned _index, sfVector2f _touch, float _degat);
sfVector2u RealPositionConvertTableauPosition(sfVector2f _positionReal);
sfVector2f GetPositionEnnemy(int _index);
sfFloatRect GetBounsEnnemy(int _index);
int GetNumberEnnemy();
sfColor GetColorsPixelMap(sfVector2f _position);
void SetPositionEnnemy(sfVector2f _position, int _index);

void ResetEnnemy(void);
int GetNearestEnnemy(List* _listeIgnore, sfVector2f _position);

#endif // !GAME_H


*/

#endif // !GAME_H
