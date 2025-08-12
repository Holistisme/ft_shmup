/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gameplay.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: benpicar <benpicar@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 16:15:41 by aheitz            #+#    #+#             */
/*   Updated: 2025/08/12 18:43:02 by benpicar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "./types.hpp"

#include <algorithm>
#include <ncurses.h>
#include <random>
#include <vector>

/* ************************************************************************** */

#define ENEMY_DELTA          200
#define ENEMY_SPAWN_INTERVAL 500

#define SHOOT_COOLDOWN  250

#define BULLET_DELTA    50
#define BULLET_COOLDOWN 500

#define ENEMY_COUNT_MAX 50

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

    Entity player;
	Entity player2;
    std::vector<Entity> enemies;
    std::vector<Entity> bullets;
    std::vector<Entity> obstacles;
    std::vector<Entity> views;

    /**
     * @brief Checks if a position is within the game bounds.
     *
     * @param pos The position to check.
     * @return true if the position is within bounds, false otherwise.
     */
    bool inBounds(const Vector2D &pos) {
        return pos.x >= 0 && pos.x < COLS && pos.y >= 0 && pos.y < LINES;
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
	INPUT_UP	= 1 << 5,
	INPUT_DOWN	= 1 << 6,
	INPUT_LEFT	= 1 << 7,
	INPUT_RIGHT	= 1 << 8,
	INPUT_0		= 1 << 9,
};

/* ************************************************************************** */

Game initGameplay  (void);
Game initGameplay2  (void);
void updateGameplay(Game &game, const int deltaTime, const unsigned input);
void updateGameplay2(Game &game, const int deltaTime, const unsigned input);

int getScore(const Game &game);
int getLives(const Game &game);
int getTime (const Game &game);

const std::vector<Entity> &getViews(const Game &game);

/* ************************************************************************** */