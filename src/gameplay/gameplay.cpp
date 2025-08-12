/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gameplay.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aheitz <aheitz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 16:14:40 by aheitz            #+#    #+#             */
/*   Updated: 2025/08/12 10:37:30 by aheitz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/gameplay/gameplay.hpp"

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
Game initGameplay(const int width, const int height) {
    Game game;

    game.Width  = width;
    game.Height = height;

    game.rng = std::mt19937(std::random_device{}());

    game.score  = 0;
    game.lives  = 3;
    game.timeMs = 0;

    game.enemyDelta         = ENEMY_DELTA;
    game.enemySpawnInterval = ENEMY_SPAWN_INTERVAL;
    game.bulletDelta        = BULLET_DELTA;
    game.bulletCooldown     = BULLET_COOLDOWN;

    game.enemies.clear();
    game.bullets.clear();
    game.views  .clear();

    game.player = {EntityKind::Player, Vector2D{game.Width / 2, game.Height / 2}, 100};
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
};

/* ************************************************************************** */

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
        [&](const Entity &enemy) { return enemy.position.x == x; });
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
            enemy.position.y++;
            if (game.player.position.x > enemy.position.x && !enemyOnX(game, enemy.position.x + 1)) {
                enemy.position.x++;
            } else if (game.player.position.x < enemy.position.x && !enemyOnX(game, enemy.position.x - 1)) {
                enemy.position.x--;
            };
        };
        game.enemyDelta = ENEMY_DELTA;
    };
};

/**
 * @brief Spawn a new enemy in the game.
 *
 * @param game The current game state.
 */
void spawnEnemy(Game &game) {
    if ((int)game.enemies.size() >= ENEMY_COUNT_MAX || game.Width <= 0 || game.Height <= 0) {
        return;
    };

    uniform_int_distribution<int> randX(0, max(0, game.Width - 1));
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
    game.bullets.push_back({EntityKind::BulletPlayer, position, 1});
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
    game.timeMs += deltaTime;

    if (input & INPUT_W)        game.player.position.y -= 1;
    if (input & INPUT_S)        game.player.position.y += 1;
    if (input & INPUT_A)        game.player.position.x -= 1;
    if (input & INPUT_D)        game.player.position.x += 1;
    if (input & INPUT_SPACE)    shootBullet(game, game.player.position);

    game.player.position.x = clamp(game.player.position.x, 0, game.Width  - 1);
    game.player.position.y = clamp(game.player.position.y, 0, game.Height - 1);

    clearOutOfBoundsEntities(game);
    handleEnemySpawn(game, deltaTime);
    moveEnemies(game, deltaTime);
    moveBullets(game, deltaTime);
    hitEntities(game);

    game.views.clear();
    game.views.reserve(1 + game.enemies.size() + game.bullets.size());
    game.pushView(game.player);
    for (const auto &enemy : game.enemies) {
        game.pushView(enemy);
    };
    for (const auto &bullet : game.bullets) {
        game.pushView(bullet);
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