/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gameplay_player2.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsyutkin <vsyutkin@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 11:22:41 by vsyutkin          #+#    #+#             */
/*   Updated: 2025/08/13 12:49:58 by vsyutkin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gameplay/gameplay.hpp"
#include "gameplay/obstacle.hpp"

#include <climits>

using namespace std;

void moveEnemies2(Game &game, const int delta);
Entity nearestPlayer(Game &game, const Vector2D &pos);

/**
 * @brief Initialize the game state.
 *
 * @param width The width of the game area.
 * @param height The height of the game area.
 * @return Game The initialized game state.
 */
Game initGameplay2(void) {
    Game game;

    game.rng = std::mt19937(std::random_device{}());

    game.score  = 0;
    game.lives  = 6;
    game.timeMs = 0;

    game.enemyDelta         = ENEMY_DELTA;
    game.enemySpawnInterval = ENEMY_SPAWN_INTERVAL;
    game.shootCooldown      = SHOOT_COOLDOWN;
    game.bulletDelta        = BULLET_DELTA;

    game.shooterShootCooldown = SHOOTER_COOLDOWN;
    game.shooterSpawnInterval = SHOOTER_INTERVAL;

    game.bomberSpawnInterval = BOMBER_INTERVAL;
    game.bomberDelta         = ENEMY_DELTA * 2;

    game.dodgerSpawnInterval = DODGER_INTERVAL;
    game.dodgerDelta         = ENEMY_DELTA / 2;

    game.enemies.clear();
    game.bullets.clear();
    game.views  .clear();
	game.walls  .clear();

    game.obstacleDelta         = OBSTACLE_DELTA;
    game.obstacleSpawnInterval = OBSTACLE_SPAWN_INTERVAL;

	buildWalls(game);

    game.player = {EntityKind::Player, Vector2D{COLS / 3, LINES - 3}, 100, 0, 0, ENTITY_COLOR_BLUE, '1'};
	game.player2 = {EntityKind::Player2, Vector2D{COLS * 2 / 3, LINES - 3}, 100, 0, 0, ENTITY_COLOR_BLUE, '2'};
    game.pushView(game.player);
	game.pushView(game.player2);

    return game;
};


void enemyDamage2(Game &game) {
    for (auto &enemy : game.enemies) {
        if (enemy.position == game.player.position) {
            if (enemy.kind == EntityKind::Dodger) {
                jamsGun(game);
                enemy.health = 0;
                continue;
            };
            --game.lives;
            enemy.health = 0;
            if (enemy.kind == EntityKind::Bomber) {
                bomberExplode(game, enemy);
            };
        };
    };
	for (auto &enemy : game.enemies) {
        if (enemy.position == game.player2.position) {
            if (enemy.kind == EntityKind::Dodger) {
                jamsGun(game);
                enemy.health = 0;
                continue;
            };
            --game.lives;
            enemy.health = 0;
            if (enemy.kind == EntityKind::Bomber) {
                bomberExplode(game, enemy);
            };
        };
    };
};


/**
 * @brief Update the gameplay state.
 *
 * @param game The current game state.
 * @param deltaTime The time elapsed since the last update.
 * @param input The current player input.
 */
void updateGameplay2(Game &game, const int deltaTime, const unsigned long input) {
     game.timeMs        += deltaTime;
    game.shootCooldown -= deltaTime;

    if (input & INPUT_W && !obstacleOnXY(game, {game.player.position.x, game.player.position.y - 1})) {
        game.player.position.y -= 1;
    };
    if (input & INPUT_S && !obstacleOnXY(game, {game.player.position.x, game.player.position.y + 1})) {
        game.player.position.y += 1;
    };
    if (input & INPUT_A && !obstacleOnXY(game, {game.player.position.x - 1, game.player.position.y})) {
        game.player.position.x -= 1;
    };
    if (input & INPUT_D && !obstacleOnXY(game, {game.player.position.x + 1, game.player.position.y})) {
        game.player.position.x += 1;
    };

    if (input & INPUT_SPACE)    shootBullet(game, game.player.position);

    game.player.position.x = clamp(game.player.position.x, 1, COLS - 2);
    game.player.position.y = clamp(game.player.position.y, 0, LINES - 2);

	if (input & INPUT_UP && !obstacleOnXY(game, {game.player2.position.x, game.player2.position.y - 1})) {
        game.player2.position.y -= 1;
    };
	if (input & INPUT_DOWN && !obstacleOnXY(game, {game.player2.position.x, game.player2.position.y + 1})) {
		game.player2.position.y += 1;
	};
	if (input & INPUT_LEFT && !obstacleOnXY(game, {game.player2.position.x - 1, game.player2.position.y})) {
		game.player2.position.x -= 1;
	};
	if (input & INPUT_RIGHT && !obstacleOnXY(game, {game.player2.position.x + 1, game.player2.position.y})) {
		game.player2.position.x += 1;
	};
	
	if (input & INPUT_0)		shootBullet(game, game.player2.position);

	game.player2.position.x = clamp(game.player2.position.x, 1, COLS - 2);
	game.player2.position.y = clamp(game.player2.position.y, 0, LINES - 2);

    moveBullets(game, deltaTime);
    shootFromShooter(game, deltaTime);

    handleEnemySpawn(game, deltaTime);
    moveEnemies2(game, deltaTime);
    hitEntities(game);
    enemyDamage(game);

    spawnObstacle(game, deltaTime);
    moveObstacles(game, deltaTime);

    clearOutOfBoundsEntities(game);

    game.views.clear();
    game.views.reserve(1 + game.enemies.size() + game.bullets.size());
    game.pushView(game.player);
	game.pushView(game.player2);
    for (const auto &enemy : game.enemies) {
        game.pushView(enemy);
    };
    for (const auto &bullet : game.bullets) {
        game.pushView(bullet);
    };
    for (const auto &obstacle : game.obstacles) {
        game.pushView(obstacle);
    };
    for (const auto &shooterBullet : game.shootersBullets) {
        game.pushView(shooterBullet);
    };
    for (const auto &fire : game.fires) {
        game.pushView(fire);
    };
	for (const auto &wall : game.walls) {
		game.pushView(wall);
	}
};

/**
 * @brief Move enemies in the game.
 *
 * @param game The current game state.
 * @param delta The time elapsed since the last frame.
 */
void moveEnemies2(Game &game, const int delta) {
    Vector2D newPos;
	Entity nearest;
    game.enemyDelta  -= delta;
    game.dodgerDelta -= delta;
    game.bomberDelta -= delta;

    for (auto &enemy : game.enemies) {
		nearest = nearestPlayer(game, enemy.position);
        if ((game.enemyDelta <= 0
            || (enemy.kind == EntityKind::Dodger && game.dodgerDelta <= 0)
            || (enemy.kind == EntityKind::Bomber && game.bomberDelta <= 0))
            && !enemy.canShoot) {

            if ((enemy.manual == 1 && !game.inBounds(Vector2D{enemy.position.x + 1, enemy.position.y}))
                || (enemy.manual == -1 && !game.inBounds(Vector2D{enemy.position.x - 1, enemy.position.y}))) {
                enemy.manual = -enemy.manual;
            };

            if (fireComing(game, Vector2D{enemy.position.x, enemy.position.y})) {
                newPos = {enemy.position.x + enemy.manual, enemy.position.y};

                if (!obstacleOnXY(game, newPos) && !enemyOnPos(game, newPos)) {
                    enemy.position = newPos;
                };
            } else if (obstacleComingOnXY(game, enemy.position.x, enemy.position.y)) {
                newPos = {enemy.position.x + enemy.manual, enemy.position.y};

                if (!obstacleOnXY(game, newPos) && !enemyOnPos(game, newPos)) {
                    enemy.position = newPos;
                };
            } else {
                newPos = {enemy.position.x, enemy.position.y + 1};

                if (!enemyOnPos(game, newPos)) {
                    enemy.position.y++;
                } else {
                    newPos = {enemy.position.x + enemy.manual, enemy.position.y};
                    if (!enemyOnPos(game, newPos) && !obstacleOnXY(game, newPos)) {
                        enemy.position = newPos;
                    };
                };

                if (enemy.position.y <= nearest.position.y) {
                    newPos = enemy.position;

                    if (nearest.position.x > enemy.position.x) {
                        newPos.x++;
                    } else if (nearest.position.x < enemy.position.x) {
                        newPos.x--;
                    };

                    if (!enemyOnPos(game, newPos)
                        && !obstacleOnXY(game, newPos)
                        && !fireComing(game, newPos)
                        && ((!enemyOnX(game, newPos.x, newPos.y)
                            && !bulletOnWay(game, enemy, newPos.x))
                            || nearest.position.y == enemy.position.y)) {
                        enemy.position = newPos;
                    };
                }

                if (enemy.kind == EntityKind::Dodger) {
                    if (bulletOnWay(game, enemy, enemy.position.x)) {
                        newPos = {enemy.position.x + enemy.manual, enemy.position.y};

                        if (!bulletOnWay(game, enemy, newPos.x) && !enemyOnPos(game, newPos) && !obstacleOnXY(game, newPos)) {
                            enemy.position = newPos;
                        };
                    };
                };
            };
        };
    };

    if (game.enemyDelta <= 0) {
        game.enemyDelta = max(50, ENEMY_DELTA - game.timeMs / 2000);
    };

    if (game.dodgerDelta <= 0) {
        game.dodgerDelta = max(50, ENEMY_DELTA * 2 - game.timeMs / 2000);
    };

    if (game.bomberDelta <= 0) {
        game.bomberDelta = max(50, ENEMY_DELTA * 2 - game.timeMs / 2000);
    };
};

Entity nearestPlayer(Game &game, const Vector2D &pos) 
{
	auto distPlayer1 = abs(game.player.position.x - pos.x) + abs(game.player.position.y - pos.y);
	auto distPlayer2 = abs(game.player2.position.x - pos.x) + abs(game.player2.position.y - pos.y);

	if (distPlayer1 < distPlayer2) {
		return game.player;
	} else {
		return game.player2;
	}
}