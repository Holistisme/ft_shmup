/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gameplay_player2.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsyutkin <vsyutkin@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 11:22:41 by vsyutkin          #+#    #+#             */
/*   Updated: 2025/08/13 12:17:27 by vsyutkin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/gameplay/gameplay.hpp"
#include "../../include/gameplay/obstacle.hpp"

/* ************************************************************************** */

using namespace std;

/* ************************************************************************** */

/**
 * @brief Initialize the game state.
 *
 * @param width The width of the game area.
 * @param height The height of the game area.
 * @return Game The initialized game state.
 */
Game initGameplay(void) {
    Game game;

    game.rng = std::mt19937(std::random_device{}());

    game.score  = 0;
    game.lives  = 3;
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
	
    game.player = {EntityKind::Player, Vector2D{COLS / 3, LINES - 3}, 100};
	game.player2 = {EntityKind::Player2, Vector2D{COLS * 2 / 3, LINES - 3}, 100};
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
void updateGameplay2(Game &game, const int deltaTime, const unsigned input) {
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

    game.player.position.x = clamp(game.player.position.x, 1, COLS - 2);
    game.player.position.y = clamp(game.player.position.y, 0, LINES - 2);

    game.player2.position.x = clamp(game.player2.position.x, 1, COLS - 2);
    game.player2.position.y = clamp(game.player2.position.y, 0, LINES - 2);

    moveBullets(game, deltaTime);
    shootFromShooter(game, deltaTime);

    handleEnemySpawn(game, deltaTime);
    moveEnemies(game, deltaTime);
    hitEntities(game);
    enemyDamage(game);

    spawnObstacle(game, deltaTime);
    moveObstacles(game, deltaTime);

    clearOutOfBoundsEntities(game);

    game.views.clear();
    game.views.reserve(1 + game.enemies.size() + game.bullets.size());
    game.pushView(game.player);
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

