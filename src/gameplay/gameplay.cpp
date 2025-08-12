/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gameplay.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aheitz <aheitz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 16:14:40 by aheitz            #+#    #+#             */
/*   Updated: 2025/08/12 16:58:03 by aheitz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/gameplay/gameplay.hpp"
#include "../../include/gameplay/obstacle.hpp"

/* ************************************************************************** */

using namespace std;

/* ************************************************************************** */

//TODO: Parse this fucking file, it's a mess

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
    game.bulletCooldown     = BULLET_COOLDOWN;

    game.enemies.clear();
    game.bullets.clear();
    game.views  .clear();

    game.obstacleDelta         = OBSTACLE_DELTA;
    game.obstacleSpawnInterval = OBSTACLE_SPAWN_INTERVAL;

    game.player = {EntityKind::Player, Vector2D{COLS / 2, LINES - 3}, 100};
    game.pushView(game.player);

    return game;
};

/* ************************************************************************** */

/**
 * @brief Clear entities that are out of bounds.
 *
 * @param game The current game state.
 */
void clearOutOfBoundsEntities(Game &game) {
    const auto outOfBounds = [&](const Entity &entity) { return !game.inBounds(entity.position); };

    game.enemies.erase(remove_if(game.enemies.begin(), game.enemies.end(),
        [&](const Entity &entity) {
            return outOfBounds(entity) || entity.health <= 0;
        }), game.enemies.end());

    game.bullets.erase(remove_if(game.bullets.begin(), game.bullets.end(),
        [&](const Entity &entity) {
            return outOfBounds(entity) || entity.health <= 0;
        }), game.bullets.end());

    game.obstacles.erase(remove_if(game.obstacles.begin(), game.obstacles.end(),
        [&](const Entity &entity) {
            return outOfBounds(entity) || entity.health <= 0;
        }), game.obstacles.end());
};

/* ************************************************************************** */

void enemyDamage(Game &game) {
    for (auto &enemy : game.enemies) {
        if (enemy.position == game.player.position) {
            --game.lives;
            enemy.health = 0;
        };
    };
};

bool bulletOnWay(const Game &game, const Entity &enemy, const int x) {
    return any_of(game.bullets.begin(), game.bullets.end(),
        [&](const Entity &bullet) { return bullet.position.x == x && bullet.position.y < enemy.position.y;});
};

/**
 * @brief Check if there is an enemy on the specified X coordinate.
 *
 * @param game The current game state.
 * @param x The X coordinate to check.
 * @return true If there is an enemy on the X coordinate.
 * @return false If there is no enemy on the X coordinate.
 */
bool enemyOnX(const Game &game, const int x) {
    return any_of(game.enemies.begin(), game.enemies.end(),
        [&](const Entity &enemy) { return enemy.position.x == x && enemy.position.y < game.player.position.y; });
};

/**
 * @brief Move the enemies in the game.
 *
 * @param game The current game state.
 * @param delta The time elapsed since the last update.
 */
void moveEnemies(Game &game, const int delta) {
    game.enemyDelta -= delta;

    if (game.enemyDelta <= 0) {
        for (auto &enemy : game.enemies) {
            if (!obstacleOnXY(game, Vector2D{enemy.position.x, enemy.position.y + 1})) {
                enemy.position.y++;
            };
            if (obstacleComingOnY(game, enemy.position.y)) {
                if (!obstacleComingOnXY(game, enemy.position.x - 1, enemy.position.y)) {
                    enemy.position.x--;
                } else if (!obstacleComingOnXY(game, enemy.position.x + 1, enemy.position.y)) {
                    enemy.position.x++;
                } else {
                    if (uniform_int_distribution<>(0, 1)(game.rng) == 0) {
                        enemy.position.x--;
                    } else {
                        enemy.position.x++;
                    };
                };
            } else if (enemy.position.y < game.player.position.y) {
                if (game.player.position.x > enemy.position.x && !enemyOnX(game, enemy.position.x + 1) && !bulletOnWay(game, enemy, enemy.position.x + 1)) {
                    enemy.position.x++;
                } else if (game.player.position.x < enemy.position.x && !enemyOnX(game, enemy.position.x - 1) && !bulletOnWay(game, enemy, enemy.position.x - 1)) {
                    enemy.position.x--;
                };
            };
        };
        game.enemyDelta = max(50, ENEMY_DELTA - game.timeMs / 2000);
    };
};

/**
 * @brief Spawn a new enemy in the game.
 *
 * @param game The current game state.
 */
void spawnEnemy(Game &game) {
    if ((int)game.enemies.size() >= ENEMY_COUNT_MAX || COLS <= 0 || LINES <= 0) {
        return;
    };

    uniform_int_distribution<int> randX(0, max(0, COLS - 1));
    game.enemies.push_back({EntityKind::Enemy, Vector2D{randX(game.rng), 1}, 1});
};

/**
 * @brief Handle enemy spawning in the game.
 *
 * @param game The current game state.
 * @param delta The time elapsed since the last update.
 */
void handleEnemySpawn(Game &game, const int delta) {
    game.enemySpawnInterval -= delta;

    if (game.enemySpawnInterval <= 0) {
        spawnEnemy(game);
        game.enemySpawnInterval = ENEMY_SPAWN_INTERVAL;
    };
};

/* ************************************************************************** */

/**
 * @brief Check for collisions between bullets and enemies.
 *
 * @param game The current game state.
 */
void hitEntities(Game &game) {
    for (auto &bullet : game.bullets) {
        for (auto &enemy : game.enemies) {
            if (bullet.position == enemy.position) {
                bullet.health  = 0;
                enemy.health   = 0;
                game.score     += 1;
            };
        };

        for (auto &obstacle : game.obstacles) {
            if (bullet.position == obstacle.position) {
                bullet.health = 0;
            };
        };
    };
};

/**
 * @brief Move the bullets in the game.
 *
 * @param game The current game state.
 * @param delta The time elapsed since the last update.
 */
void moveBullets(Game &game, const int delta) {
    game.bulletDelta -= delta;

    if (game.bulletDelta <= 0) {
        for (auto &bullet : game.bullets) {
            bullet.position.y -= 1;
        };
        game.bulletDelta = BULLET_DELTA;
    };
};

/**
 * @brief Shoot a bullet from the player's position.
 *
 * @param game The current game state.
 * @param position The position to shoot the bullet from.
 */
void shootBullet(Game &game, const Vector2D &position) {
    if (game.shootCooldown <= 0) {
        const Vector2D shootPos = {position.x, position.y - 1};
        game.bullets.push_back({EntityKind::BulletPlayer, shootPos, 1});
        game.shootCooldown = SHOOT_COOLDOWN;
    };
};

/* ************************************************************************** */

/**
 * @brief Update the gameplay state.
 *
 * @param game The current game state.
 * @param deltaTime The time elapsed since the last update.
 * @param input The current player input.
 */
void updateGameplay(Game &game, const int deltaTime, const unsigned input) {
    game.timeMs        += deltaTime;
    game.shootCooldown -= deltaTime;

    if (input & INPUT_W)        game.player.position.y -= 1;
    if (input & INPUT_S)        game.player.position.y += 1;
    if (input & INPUT_A)        game.player.position.x -= 1;
    if (input & INPUT_D)        game.player.position.x += 1;

    if (input & INPUT_SPACE)    shootBullet(game, game.player.position);

    game.player.position.x = clamp(game.player.position.x, 1, COLS - 2);
    game.player.position.y = clamp(game.player.position.y, 0, LINES - 2);

    clearOutOfBoundsEntities(game);
    handleEnemySpawn(game, deltaTime);
    moveEnemies(game, deltaTime);
    moveBullets(game, deltaTime);
    hitEntities(game);
    enemyDamage(game);

    spawnObstacle(game, deltaTime);
    moveObstacles(game, deltaTime);

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
};

/* ************************************************************************** */

/**
 * @brief Get the Score object
 *
 * @param game The current game state.
 * @return int The current score.
 */
int getScore(const Game &game) {
    return game.score;
};

/**
 * @brief Get the Lives object
 *
 * @param game The current game state.
 * @return int The current lives.
 */
int getLives(const Game &game) {
    return game.lives;
};

/**
 * @brief Get the Time object
 *
 * @param game The current game state.
 * @return int The current time.
 */
int getTime(const Game &game) {
    return game.timeMs;
};

/**
 * @brief Get the Views object
 *
 * @param game The current game state.
 * @return const vector<Entity>& The current views.
 */
const vector<Entity> &getViews(const Game &game) {
    return game.views;
};

/* ************************************************************************** */