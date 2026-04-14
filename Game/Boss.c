#include "Boss.h"

void MoveBoss1(sfVector2f _move);

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
	
	sfFloatRect hitbox = { 0 };

	if (_index == 0)
	{
		boss.boss1 = Calloc(1, sizeof(Boss1));

		sfVector2f temp = { 100 ,100 };
		sfVector2f positionTurret2 = ADD_VECTOR(_position, temp);
		temp.x *= -1;
		sfVector2f positionTurret1 = ADD_VECTOR(_position, temp);

		boss.boss1->spriteTurret1Canon = CreateSprite(GetAsset("Assets/Boss/1/TurretCanon_Placeholder.png"), positionTurret1, 1.f, 1.f);
		hitbox = sfSprite_getLocalBounds(boss.boss1->spriteTurret1Canon);
		sfSprite_setOrigin(boss.boss1->spriteTurret1Canon, (sfVector2f) { hitbox.width, hitbox.height / 2.f });

		boss.boss1->spriteTurret2Canon = CreateSprite(GetAsset("Assets/Boss/1/TurretCanon_Placeholder.png"), positionTurret2, -1.f, 1.f);
		hitbox = sfSprite_getLocalBounds(boss.boss1->spriteTurret2Canon);
		sfSprite_setOrigin(boss.boss1->spriteTurret2Canon, (sfVector2f) { hitbox.width, hitbox.height / 2.f });

		boss.boss1->spriteTurret1Bis = CreateSprite(GetAsset("Assets/Boss/1/TurretBase_Placeholder.png"), positionTurret1, 1.f, 1.f);
		hitbox = sfSprite_getLocalBounds(boss.boss1->spriteTurret1Bis);
		sfSprite_setOrigin(boss.boss1->spriteTurret1Bis, (sfVector2f) { hitbox.width, hitbox.height/2.f});

		boss.boss1->spriteTurret2Bis = CreateSprite(GetAsset("Assets/Boss/1/TurretBase_Placeholder.png"), positionTurret2, -1.f, 1.f);
		hitbox = sfSprite_getLocalBounds(boss.boss1->spriteTurret2Bis);
		sfSprite_setOrigin(boss.boss1->spriteTurret2Bis, (sfVector2f) { hitbox.width, hitbox.height / 2.f });

		boss.boss1->spriteTurret1 = CreateSprite(GetAsset("Assets/Boss/1/TurretChamber_Placeholder.png"), positionTurret1, 1.f, 1.f);
		hitbox = sfSprite_getLocalBounds(boss.boss1->spriteTurret1);
		sfSprite_setOrigin(boss.boss1->spriteTurret1, (sfVector2f) { 0, hitbox.height / 2.f });

		boss.boss1->spriteTurret2 = CreateSprite(GetAsset("Assets/Boss/1/TurretChamber_Placeholder.png"), positionTurret2, -1.f, 1.f);
		hitbox = sfSprite_getLocalBounds(boss.boss1->spriteTurret2);
		sfSprite_setOrigin(boss.boss1->spriteTurret2, (sfVector2f) { 0, hitbox.height / 2.f });
	}
}


void UpdateBoss(float _dt)
{

}

void MoveBoss1(sfVector2f _move)
{
	sfSprite_move(boss.boss1->sprite1, _move);
	sfSprite_move(boss.boss1->sprite2, _move);
	sfSprite_move(boss.boss1->spriteCanon, _move);
	sfSprite_move(boss.boss1->spriteTurret1, _move);
	sfSprite_move(boss.boss1->spriteTurret1Bis, _move);
	sfSprite_move(boss.boss1->spriteTurret1Canon, _move);
	sfSprite_move(boss.boss1->spriteTurret2, _move);
	sfSprite_move(boss.boss1->spriteTurret2Bis, _move);
	sfSprite_move(boss.boss1->spriteTurret2Canon, _move);
}

void HitBoss(sfFloatRect _hitbox)
{

}
