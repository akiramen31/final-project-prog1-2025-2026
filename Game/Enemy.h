#ifndef ENEMY_H
#define ENEMY_H

#include "Common.h"

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

#endif // !GAME_H

