#include "Boss.h"

Boss boss;

void LoadBoss(void)
{
	boss = (Boss){ 0 };
}

void SwitchBoss(char _index, sfVector2f _position)
{
	if (boss.boss1)
	{
		DestroyVisualEntity(boss.boss1->sprite1);
		DestroyVisualEntity(boss.boss1->sprite2);
		DestroyVisualEntity(boss.boss1->spriteCanon);
		DestroyVisualEntity(boss.boss1->spriteTurret1);
		DestroyVisualEntity(boss.boss1->spriteTurret1Bis);
		DestroyVisualEntity(boss.boss1->spriteTurret1Canon);
		DestroyVisualEntity(boss.boss1->spriteTurret2);
		DestroyVisualEntity(boss.boss1->spriteTurret2Bis);
		DestroyVisualEntity(boss.boss1->spriteTurret2Canon);
		Free(boss.boss1);
		boss.boss1 = NULL;
	}
	
	if (_index = 0)
	{
		boss.boss1 = Calloc(1, sizeof(Boss1));
		boss.boss1->sprite1 = CreateSprite(GetAsset("Boss1Part1.png"), _position, 1.f, 1.f);
	}
}


void UpdateBoss(float _dt)
{

}

void HitBoss(sfFloatRect _hitbox)
{

}
