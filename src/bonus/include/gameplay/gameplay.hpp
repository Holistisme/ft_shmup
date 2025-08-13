/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gameplay.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aheitz <aheitz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 16:15:41 by aheitz            #+#    #+#             */
/*   Updated: 2025/08/13 12:05:43 by aheitz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

/* ************************************************************************** */

#include "./types.hpp"

#include <algorithm>
#include <ncurses.h>
#include <random>
#include <vector>

/* ************************************************************************** */

#define ENEMY_DELTA          200
#define ENEMY_SPAWN_INTERVAL 1000

#define SHOOT_COOLDOWN  250

#define BULLET_DELTA    50

#define ENEMY_COUNT_MAX 50

#define SHOOTER_INTERVAL 5000
#define SHOOTER_COOLDOWN 2000

#define BOMBER_INTERVAL 10000
#define FIRE_DURATION   2000

#define DODGER_INTERVAL 3000

#define BOSS_DELTA    1000
#define BOSS_INTERVAL 30000

/* ************************************************************************** */

/**
 * @brief Represents the game state and logic.
 *
 */
struct Game {
    std::mt19937 rng;

    int score, lives, timeMs;

    int enemyDelta, enemySpawnInterval;
    int obstacleDelta, obstacleSpawnInterval;
    int bulletDelta, bulletCooldown;
    int shootCooldown;

    int shooterSpawnInterval, shooterShootCooldown;
    int bomberSpawnInterval, bomberDelta;
    int dodgerSpawnInterval, dodgerDelta;
	bool wallType;

    int bossSpawnInterval, bossDelta, bossShootCooldown, bossBulletDelta;

    Entity player;
    std::vector<Entity> enemies;
    std::vector<Entity> fires;
    std::vector<Entity> bullets;
    std::vector<Entity> shootersBullets;
    std::vector<Entity> bossBullets;
    std::vector<Entity> bossSides;
    std::vector<Entity> obstacles;
    std::vector<Entity> views;
	std::vector<Entity> walls;

    /**
     * @brief Checks if a position is within the game bounds.
     *
     * @param pos The position to check.
     * @return true if the position is within bounds, false otherwise.
     */
    bool inBounds(const Vector2D &pos) {
        return pos.x >= 1 && pos.x <= COLS - 2 && pos.y >= 0 && pos.y <= LINES - 2;
    };

    /**
     * @brief Pushes an entity to the views vector.
     *
     * @param entity The entity to push.
     */
    void pushView(const Entity &entity) {
        views.push_back(entity);
    };
};

/* ************************************************************************** */

/**
 * @brief Input actions for the player
 *
 */
enum : unsigned {
    INPUT_W     = 1 << 0,
    INPUT_A     = 1 << 1,
    INPUT_S     = 1 << 2,
    INPUT_D     = 1 << 3,
    INPUT_SPACE = 1 << 4,
};

/* ************************************************************************** */

Game initGameplay  (void);
void updateGameplay(Game &game, const int deltaTime, const unsigned input);

int getScore(const Game &game);
int getLives(const Game &game);
int getTime (const Game &game);

const std::vector<Entity> &getViews(const Game &game);

/* ************************************************************************** */

void promoteShooter(Entity &enemy);
void shootFromShooter(Game &game, const int deltaTime);

void promoteBomber(Entity &enemy);
void bomberExplode(Game &game, const Entity &bomber);

bool fireComing(const Game &game, const Vector2D &pos);

void promoteDodger(Entity &enemy);
void jamsGun(Game &game);

void bossBattle(Game &game, const int delta);