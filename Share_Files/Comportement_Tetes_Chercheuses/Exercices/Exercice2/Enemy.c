#include "Enemy.h"

Enemy enemyList[ENEMY_COUNT] = { 0 };

void MoveEnemy(unsigned _index,sfVector2f _playerPos, float _dt);
void CheckCollisionEnemyScreen(unsigned _index);

void LoadEnemy(void)
{
	for (unsigned i = 0; i < ENEMY_COUNT; i++)
	{
		enemyList[i].circle = sfCircleShape_create();
		sfCircleShape_setRadius(enemyList[i].circle, ENEMY_RADIUS);
		sfCircleShape_setOrigin(enemyList[i].circle, (sfVector2f) { ENEMY_RADIUS, ENEMY_RADIUS });
		sfCircleShape_setFillColor(enemyList[i].circle, sfTransparent);
		sfCircleShape_setOutlineColor(enemyList[i].circle, sfGreen);
		sfCircleShape_setOutlineThickness(enemyList[i].circle, 2);
		sfCircleShape_setPointCount(enemyList[i].circle, 50);

		sfVector2f enemyPosition = { 0 };
		enemyPosition.x = (float)GetRandom(ENEMY_RADIUS, SCREEN_WIDTH - ENEMY_RADIUS - 1);
		enemyPosition.y = (float)GetRandom(ENEMY_RADIUS, SCREEN_HEIGHT - ENEMY_RADIUS - 1);
		sfCircleShape_setPosition(enemyList[i].circle, enemyPosition);

		enemyList[i].rotation = 0;

		enemyList[i].isAlive = sfTrue;

		enemyList[i].lifetime = 40;

		enemyList[i].speed = 30.f;
	}
}

void UpdateEnemy(sfRenderWindow* const _renderWindow, sfVector2f _playerPos ,float _dt)
{
	for (unsigned i = 0; i < ENEMY_COUNT; i++)
	{
		MoveEnemy(i, _playerPos, _dt);
		CheckCollisionEnemyScreen(i);
	}
}

void DrawEnemy(sfRenderWindow* const _renderWindow)
{
	for (unsigned i = 0; i < ENEMY_COUNT; i++)
	{
		sfRenderWindow_drawCircleShape(_renderWindow, enemyList[i].circle, NULL);
	}
}

void CleanupEnemy(void)
{
	for (unsigned i = 0; i < ENEMY_COUNT; i++)
	{
		sfCircleShape_destroy(enemyList[i].circle);
		enemyList[i] = (Enemy){ 0 };
	}
}

sfVector2f GetNearestEnemyPosition(sfVector2f _position)
{
	unsigned nearestEnemyIndex = 0;
	float minDistance = GetDistance(_position, sfCircleShape_getPosition(enemyList[0].circle));

	for (unsigned i = 1; i < ENEMY_COUNT; i++)
	{
		float distanceEnemy = GetDistance(_position, sfCircleShape_getPosition(enemyList[i].circle));
		if (distanceEnemy < minDistance)
		{
			minDistance = distanceEnemy;
			nearestEnemyIndex = i;
		}
	}

	return sfCircleShape_getPosition(enemyList[nearestEnemyIndex].circle);
}

void MoveEnemy(unsigned _index, sfVector2f _playerPos, float _dt)
{
	// 1. Vérification si l'ennemi est actif
	if (!enemyList[_index].isAlive) return;

	// 2. Récupération de la position actuelle
	sfVector2f currentPos = sfCircleShape_getPosition(enemyList[_index].circle);

	// 3. Calcul de l'angle vers le joueur (Cible - Moi)
	// On calcule la direction, puis l'angle en degrés
	float dx = _playerPos.x - currentPos.x;
	float dy = _playerPos.y - currentPos.y;

	// atan2f renvoie des radians, on multiplie par 180/PI pour avoir des degrés
	float targetAngle = atan2f(dy, dx) * 180.0f / (float)PI;

	// 4. Calcul de la différence d'angle
	// On veut savoir de combien de degrés le missile doit tourner
	float angleDiff = targetAngle - enemyList[_index].rotation;

	// Normalisation de l'angle entre -180 et 180
	// (Pour que le missile choisisse toujours le chemin le plus court pour tourner)
	while (angleDiff > 180.0f)  angleDiff -= 360.0f;
	while (angleDiff < -180.0f) angleDiff += 360.0f;

	// 5. Rotation progressive (Le "Turn Speed")
	// On définit une vitesse de rotation (ex: 200 degrés par seconde)
	float maxRotationThisFrame = DEGRE_ROTATION * _dt;

	if (fabsf(angleDiff) <= maxRotationThisFrame)
	{
		// Si l'angle est très petit, on s'aligne directement
		enemyList[_index].rotation = targetAngle;
	}
	else
	{
		// Sinon, on tourne vers la gauche ou la droite au maximum autorisé
		if (angleDiff > 0)
			enemyList[_index].rotation += maxRotationThisFrame;
		else
			enemyList[_index].rotation -= maxRotationThisFrame;
	}

	// 6. Déplacement à vitesse constante
	// On repasse en radians pour les fonctions cos/sin
	float angleInRadians = enemyList[_index].rotation * (float)PI / 180.0f;

	sfVector2f movement;
	movement.x = cosf(angleInRadians) * SPEED_MISSILE * _dt;
	movement.y = sinf(angleInRadians) * SPEED_MISSILE * _dt;

	// 7. Application à la forme SFML
	sfCircleShape_setRotation(enemyList[_index].circle, enemyList[_index].rotation);
	sfCircleShape_move(enemyList[_index].circle, movement);
}

void CheckCollisionEnemyScreen(unsigned _index)
{
	sfFloatRect enemyHitbox = sfCircleShape_getGlobalBounds(enemyList[_index].circle);

	if (enemyHitbox.left < 0)
	{
		sfCircleShape_move(enemyList[_index].circle, (sfVector2f) { 0 - enemyHitbox.left, 0 });
		enemyList[_index].speed *= -1;
	}
	else if (enemyHitbox.left + enemyHitbox.width > SCREEN_WIDTH)
	{
		sfCircleShape_move(enemyList[_index].circle, (sfVector2f) { SCREEN_WIDTH - (enemyHitbox.left + enemyHitbox.width), 0 });
		enemyList[_index].speed *= -1;
	}

	if (enemyHitbox.top < 0)
	{
		sfCircleShape_move(enemyList[_index].circle, (sfVector2f) { 0, 0 - enemyHitbox.top });
		enemyList[_index].speed *= -1;
	}
	else if (enemyHitbox.top + enemyHitbox.height > SCREEN_HEIGHT)
	{
		sfCircleShape_move(enemyList[_index].circle, (sfVector2f) { 0, SCREEN_HEIGHT - (enemyHitbox.top + enemyHitbox.height) });
		enemyList[_index].speed *= -1;
	}
}
