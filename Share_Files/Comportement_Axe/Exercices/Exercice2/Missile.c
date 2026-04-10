#include "Missile.h"

Missile missileList[MISSILE_MAX] = { 0 };

void MoveMissile(unsigned _index,sfVector2f _playerPos, float _dt);
void CheckCollisionMissileScreen(unsigned _index);

void LoadMissile(void)
{
	for (unsigned i = 0; i < MISSILE_MAX; i++)
	{
		missileList[i].circle = sfCircleShape_create();
		sfCircleShape_setRadius(missileList[i].circle, MISSILE_RADIUS);
		sfCircleShape_setOrigin(missileList[i].circle, (sfVector2f) { MISSILE_RADIUS, MISSILE_RADIUS });
		sfCircleShape_setFillColor(missileList[i].circle, sfTransparent);
		sfCircleShape_setOutlineColor(missileList[i].circle, sfGreen);
		sfCircleShape_setOutlineThickness(missileList[i].circle, 2);
		sfCircleShape_setPointCount(missileList[i].circle, 50);

		sfVector2f missilePosition = { 0 };

		sfCircleShape_setPosition(missileList[i].circle, missilePosition);

		missileList[i].lifetime = 0;

		missileList[i].rotation = 0;

		missileList[i].isAlive = sfFalse;

		missileList[i].speed = 30.f;
	}
}

void AddMissile(sfVector2f _playerPos)
{
	for (unsigned i = 0; i < MISSILE_MAX; i++)
	{
		if (missileList[i].isAlive == sfFalse)
		{
			missileList[i].isAlive = sfTrue;
			missileList[i].lifetime = 0; 
			missileList[i].rotation = 0; 
			sfCircleShape_setPosition(missileList[i].circle, _playerPos);

			return;
		}
	}
}

void UpdateMissile(sfRenderWindow* const _renderWindow, sfVector2f _playerPos ,float _dt)
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
				MoveMissile(i, _playerPos, _dt);
			}
			else
			{
				missileList[i].isAlive = sfFalse;
			}
		}
	}
}

void DrawMissile(sfRenderWindow* const _renderWindow)
{
	for (unsigned i = 0; i < MISSILE_MAX; i++) 
	{
		if (missileList[i].isAlive)
		{
			sfRenderWindow_drawCircleShape(_renderWindow, missileList[i].circle, NULL);
		}
	}
}

void CleanupMissile(void)
{
	for (unsigned i = 0; i < MISSILE_MAX; i++)
	{
		sfCircleShape_destroy(missileList[i].circle);
		missileList[i] = (Missile){ 0 };
	}
}

void MoveMissile(unsigned _index, sfVector2f _playerPos, float _dt)
{
	if (!missileList[_index].isAlive) return;

	sfVector2f currentPos = sfCircleShape_getPosition(missileList[_index].circle);

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
	sfCircleShape_setRotation(missileList[_index].circle, missileList[_index].rotation);
	sfCircleShape_move(missileList[_index].circle, movement);
}

void CheckCollisionMissileScreen(unsigned _index)
{
	sfFloatRect missileHitbox = sfCircleShape_getGlobalBounds(missileList[_index].circle);

	if (missileHitbox.left < 0)
	{
		sfCircleShape_move(missileList[_index].circle, (sfVector2f) { 0 - missileHitbox.left, 0 });
		missileList[_index].speed *= -1;
	}
	else if (missileHitbox.left + missileHitbox.width > SCREEN_WIDTH)
	{
		sfCircleShape_move(missileList[_index].circle, (sfVector2f) { SCREEN_WIDTH - (missileHitbox.left + missileHitbox.width), 0 });
		missileList[_index].speed *= -1;
	}

	if (missileHitbox.top < 0)
	{
		sfCircleShape_move(missileList[_index].circle, (sfVector2f) { 0, 0 - missileHitbox.top });
		missileList[_index].speed *= -1;
	}
	else if (missileHitbox.top + missileHitbox.height > SCREEN_HEIGHT)
	{
		sfCircleShape_move(missileList[_index].circle, (sfVector2f) { 0, SCREEN_HEIGHT - (missileHitbox.top + missileHitbox.height) });
		missileList[_index].speed *= -1;
	}
}

CircleHitbox GetMissileCircleHitbox(int _index)
{
	CircleHitbox missileHitbox = { 0 };

	missileHitbox.position = sfCircleShape_getPosition(missileList[_index].circle);
	missileHitbox.radius = sfCircleShape_getRadius(missileList[_index].circle);

	return missileHitbox;
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

			sfVector2f posA = sfCircleShape_getPosition(missileList[i].circle);
			sfVector2f posB = sfCircleShape_getPosition(missileList[j].circle);

			float dx = posB.x - posA.x;
			float dy = posB.y - posA.y;
			float distance = sqrtf(dx * dx + dy * dy);
			float minDistance = MISSILE_RADIUS * 2.0f; // Somme des deux rayons

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

				sfCircleShape_move(missileList[i].circle, moveA);
				sfCircleShape_move(missileList[j].circle, moveB);

				// 4. Inversion des vitesses (ton code actuel)
				missileList[i].speed *= -0.5f; // Un peu d'amorti rend le choc plus naturel
				missileList[j].speed *= -0.5f;
			}
		}
	}
}
