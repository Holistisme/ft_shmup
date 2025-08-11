/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gameplay.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aheitz <aheitz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 16:14:40 by aheitz            #+#    #+#             */
/*   Updated: 2025/08/11 17:24:26 by aheitz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/gameplay/gameplay.hpp"

/* ************************************************************************** */

using namespace std;

/* ************************************************************************** */

namespace {
    int Width  = 0, Height = 0;

    int score = 0, lives = 3, timeMs = 0;

    Entity player = {EntityKind::Player, Vector2D{1, 1}, 100};
    vector<Entity> enemies;
    vector<Entity> bullets;

    vector<Entity> views;

    bool inBounds(const Vector2D &pos) {
        return pos.x >= 0 && pos.x < Width && pos.y >= 0 && pos.y < Height;
    };

    void pushView(const Entity &entity) {
        views.push_back(entity);
    };
};

/* ************************************************************************** */

/**
 * @brief Initializes the gameplay environment with the specified width and height.
 *
 * @param width The width of the gameplay area.
 * @param height The height of the gameplay area.
 */
void initGameplay(const int width, const int height) {
    Width  = width;
    Height = height;

    score  = 0;
    lives  = 3;
    timeMs = 0;

    enemies.clear();
    bullets.clear();
    views.  clear();

    player = {EntityKind::Player, Vector2D{Width / 2, Height / 2}, 100};
    pushView(player);
};

/* ************************************************************************** */

/**
 * @brief Clears entities that are out of bounds or have zero health.
 *
 */
void clearOutOfBoundsEntities(void) {
    const auto outOfBounds = [&](const Entity &entity) { return !inBounds(entity.position); };

    enemies.erase(remove_if(enemies.begin(), enemies.end(),
        [&](const Entity &entity) {
            return outOfBounds(entity) || entity.health <= 0;
        }), enemies.end());

    bullets.erase(remove_if(bullets.begin(), bullets.end(),
        [&](const Entity &entity) {
            return outOfBounds(entity) || entity.health <= 0;
        }), bullets.end());
};

/**
 * @brief Updates the gameplay state based on the delta time and input actions.
 *
 * @param deltaTime The time elapsed since the last update in milliseconds.
 * @param input The input actions performed by the player, represented as a bitmask.
 */
void updateGameplay(const int deltaTime, const unsigned input) {
    timeMs += deltaTime;

    if (input & INPUT_W)    player.position.y -= 1;
    if (input & INPUT_S)    player.position.y += 1;
    if (input & INPUT_A)    player.position.x -= 1;
    if (input & INPUT_D)    player.position.x += 1;

    player.position.x = clamp(player.position.x, 0, Width  - 1);
    player.position.y = clamp(player.position.y, 0, Height - 1);

    clearOutOfBoundsEntities();

    views.clear();
    views.reserve(1 + enemies.size() + bullets.size());
    pushView(player);
    for (const auto &enemy : enemies) {
        pushView(enemy);
    };
    for (const auto &bullet : bullets) {
        pushView(bullet);
    };
};

/* ************************************************************************** */

/**
 * @brief Get the Score object
 *
 * @return int The current score
 */
int getScore(void) {
    return score;
};

/**
 * @brief Get the Lives object
 *
 * @return int The current lives
 */
int getLives(void) {
    return lives;
};

/**
 * @brief Get the Time object
 *
 * @return int The current time
 */
int getTime(void) {
    return timeMs;
};

/**
 * @brief Get the Views object
 *
 * @return const vector<Entity>& A reference to the vector of entities in view
 */
const vector<Entity> &getViews(void) {
    return views;
};

/* ************************************************************************** */