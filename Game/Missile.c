#include "Missile.h"

Missile missileList[MISSILE_MAX] = { 0 };
sfTexture* missileTexture;
void MoveMissile(unsigned _index, sfVector2f _playerPos, float _dt);
void CheckCollisionMissileScreen(unsigned _index);

void LoadMissile(void)
{
	missileTexture = GetAsset("Assets/Sprites/Drone_Placeholder.png");
	for (unsigned i = 0; i < MISSILE_MAX; i++)
	{
		missileList[i].sprite = CreateSprite(missileTexture, (sfVector2f) { 0, 0 }, 1.f, 39);
		SetSpriteOriginMiddel(missileList[i].sprite);

		sfVector2f missilePosition = { 0 };

		missileList[i].lifetime = 0;

		missileList[i].rotation = 0;

		missileList[i].isAlive = sfFalse;

		missileList[i].speed = 30.f;
	}
}

void AddMissile(sfVector2f _pos)
{
	for (unsigned i = 0; i < MISSILE_MAX; i++)
	{
		if (missileList[i].isAlive == sfFalse)
		{
			missileList[i].isAlive = sfTrue;
			missileList[i].lifetime = 0;
			missileList[i].rotation = 0;
			sfSprite_setPosition(missileList[i].sprite, (sfVector2f) { _pos.x, _pos.y - WEAPON_ORIGIN });

			return;
		}
	}
}

void UpdateMissile(sfVector2f _posAim, float _dt)
{
	for (unsigned i = 0; i < MISSILE_MAX; i++)
	{
		if (missileList[i].isAlive == sfTrue)
		{
			missileList[i].lifetime += _dt;
			if (missileList[i].lifetime <= MISSILE_DURATION)
			{
				CheckCollisionMissilesList();
				CheckCollisionMissileScreen(i);
				MoveMissile(i, _posAim, _dt);
			}
			else
			{
				missileList[i].isAlive = sfFalse;
				sfSprite_setPosition(missileList[i].sprite, (sfVector2f) { 0, 0 });
				continue;
			}
			sfVector2f reaction = Colision(sfSprite_getGlobalBounds(missileList[i].sprite), AXIS_BOTH);
			sfVector2f reactionBox = ColisionBox(sfSprite_getGlobalBounds(missileList[i].sprite), sfTrue, AXIS_BOTH);
			reaction.x += reactionBox.x;
			reaction.y += reactionBox.y;
			if (reaction.x != 0 || reaction.y != 0)
			{
				missileList[i].isAlive = sfFalse;
				sfSprite_setPosition(missileList[i].sprite, (sfVector2f) { 0, 0 });
				continue;
			}
		}
	}
}

void MoveMissile(unsigned _index, sfVector2f _playerPos, float _dt)
{
	if (!missileList[_index].isAlive) return;

	sfVector2f currentPos = sfSprite_getPosition(missileList[_index].sprite);

	float dx = _playerPos.x - currentPos.x;
	float dy = _playerPos.y - currentPos.y;

	float targetAngle = atan2f(dy, dx) * 180.0f / (float)PI;

	float angleDiff = targetAngle - missileList[_index].rotation;

	while (angleDiff > 180.0f)  angleDiff -= 360.0f;
	while (angleDiff < -180.0f) angleDiff += 360.0f;

	float maxRotationThisFrame = DEGRE_ROTATION * _dt;

	if (fabsf(angleDiff) <= maxRotationThisFrame)
	{
		missileList[_index].rotation = targetAngle;
	}
	else
	{
		if (angleDiff > 0)
			missileList[_index].rotation += maxRotationThisFrame;
		else
			missileList[_index].rotation -= maxRotationThisFrame;
	}

	float angleInRadians = missileList[_index].rotation * (float)PI / 180.0f;

	sfVector2f movement;
	movement.x = cosf(angleInRadians) * SPEED_MISSILE * _dt;
	movement.y = sinf(angleInRadians) * SPEED_MISSILE * _dt;

	// 7. Application à la forme SFML
	sfSprite_setRotation(missileList[_index].sprite, missileList[_index].rotation);
	sfSprite_move(missileList[_index].sprite, movement);
}

void CheckCollisionMissileScreen(unsigned _index)
{
	sfFloatRect missileHitbox = sfSprite_getGlobalBounds(missileList[_index].sprite);

	if (missileHitbox.left < 0)
	{
		sfSprite_move(missileList[_index].sprite, (sfVector2f) { 0 - missileHitbox.left, 0 });
		missileList[_index].speed *= -1;
	}
	else if (missileHitbox.left + missileHitbox.width > SCREEN_WIDTH)
	{
		sfSprite_move(missileList[_index].sprite, (sfVector2f) { SCREEN_WIDTH - (missileHitbox.left + missileHitbox.width), 0 });
		missileList[_index].speed *= -1;
	}

	if (missileHitbox.top < 0)
	{
		sfSprite_move(missileList[_index].sprite, (sfVector2f) { 0, 0 - missileHitbox.top });
		missileList[_index].speed *= -1;
	}
	else if (missileHitbox.top + missileHitbox.height > SCREEN_HEIGHT)
	{
		sfSprite_move(missileList[_index].sprite, (sfVector2f) { 0, SCREEN_HEIGHT - (missileHitbox.top + missileHitbox.height) });
		missileList[_index].speed *= -1;
	}
}

void CheckCollisionMissilesList(void)
{
	for (unsigned i = 0; i < MISSILE_MAX; i++)
	{
		if (!missileList[i].isAlive) continue;

		// On commence à j = i + 1 pour ne pas tester deux fois la même paire
		// et ne pas tester le missile contre lui-même
		for (unsigned j = i + 1; j < MISSILE_MAX; j++)
		{
			if (!missileList[j].isAlive) continue;

			sfVector2f posA = sfSprite_getPosition(missileList[i].sprite);
			sfVector2f posB = sfSprite_getPosition(missileList[j].sprite);

			float dx = posB.x - posA.x;
			float dy = posB.y - posA.y;
			float distance = sqrtf(dx * dx + dy * dy);
			float minDistance = MISSILE_WEIGHT * 2.0f; // Somme des deux rayons

			if (distance < minDistance)
			{
				// 1. Calcul du décalage (pénétration)
				float overlap = minDistance - distance;

				// 2. Normalisation du vecteur de direction (pour savoir où pousser)
				// On évite la division par zéro si les missiles sont exactement au même point
				if (distance == 0) distance = 0.1f;
				float nx = dx / distance;
				float ny = dy / distance;

				// 3. On décale chaque missile de la moitié de l'overlap dans des directions opposées
				sfVector2f moveA = { -nx * overlap / 2.0f, -ny * overlap / 2.0f };
				sfVector2f moveB = { nx * overlap / 2.0f,  ny * overlap / 2.0f };

				sfSprite_move(missileList[i].sprite, moveA);
				sfSprite_move(missileList[j].sprite, moveB);

				// 4. Inversion des vitesses (ton code actuel)
				missileList[i].speed *= -0.5f; // Un peu d'amorti rend le choc plus naturel
				missileList[j].speed *= -0.5f;
			}
		}
	}
}
