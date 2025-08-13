/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   boss.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aheitz <aheitz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 10:01:31 by aheitz            #+#    #+#             */
/*   Updated: 2025/08/13 13:16:39 by aheitz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/gameplay/gameplay.hpp"
#include "../../../include/gameplay/obstacle.hpp"

/* ************************************************************************** */

using namespace std;

/* ************************************************************************** */

/**
 * @brief Update the trajectory of the boss's bullets.
 *
 * @param game The current game state.
 * @param delta The time elapsed since the last frame.
 */
static void bossBulletsTrajectory(Game &game, const int delta) {
    game.bossBulletDelta -= delta;

    if (game.bossBulletDelta <= 0) {
        for (auto &bullet : game.bossBullets) {
            bullet.position.x += bullet.velocity.x;
            bullet.position.y += bullet.velocity.y;
        };
        game.bossBulletDelta = BULLET_DELTA;
    };
};

/**
 * @brief Update the boss's view.
 *
 * @param game The current game state.
 */
static void bossView(Game &game) {
    auto boss = find_if(game.enemies.begin(), game.enemies.end(),
        [](const Entity &enemy) { return enemy.kind == EntityKind::Boss; });

    if (boss == game.enemies.end()) {
        game.bossSides.clear();
        return;
    };

    const Vector2D bossPositions[9] = {
        {boss->position.x - 1, boss->position.y - 1},
        {boss->position.x,     boss->position.y - 1},
        {boss->position.x + 1, boss->position.y - 1},

        {boss->position.x - 1, boss->position.y},
        {boss->position.x,     boss->position.y},
        {boss->position.x + 1, boss->position.y},

        {boss->position.x - 1, boss->position.y + 1},
        {boss->position.x,     boss->position.y + 1},
        {boss->position.x + 1, boss->position.y + 1},
    };

    game.bossSides.clear();
    for (const auto &pos : bossPositions) {
        game.bossSides.push_back({EntityKind::BossSide, pos, boss->health});
    };
};

/**
 * @brief Update the boss's incoming attack.
 *
 * @param game The current game state.
 * @param delta The time elapsed since the last frame.
 */
static void bossIncoming(Game &game, const int delta) {
    game.bossSpawnInterval -= delta;

    auto boss = find_if(game.enemies.begin(), game.enemies.end(),
        [](const Entity &enemy) { return enemy.kind == EntityKind::Boss; });

    if (boss != game.enemies.end()) {
        return;
    };

    if (game.bossSpawnInterval <= 0) {
        const Vector2D bossPositions[9] = {
            {COLS / 2 - 1, 0},
            {COLS / 2,     0},
            {COLS / 2 + 1, 0},

            {COLS / 2 - 1, 1},
            {COLS / 2,     1},
            {COLS / 2 + 1, 1},

            {COLS / 2 + 1, 2},
            {COLS / 2,     2},
            {COLS / 2 - 1, 2},
        };

        for (const auto &pos : bossPositions) {
            if (obstacleOnXY(game, pos)) {
                return;
            };
        };

        game.enemies.push_back({EntityKind::Boss, {COLS / 2, 1}, 100});
        game.bossSpawnInterval = BOSS_INTERVAL;
    };
};

/**
 * @brief Update the boss's movement.
 *
 * @param game The current game state.
 * @param delta The time elapsed since the last frame.
 */
static void bossMovement(Game &game, const int delta) {
    auto boss = find_if(game.enemies.begin(), game.enemies.end(),
        [](const Entity &enemy) { return enemy.kind == EntityKind::Boss; });

    if (boss != game.enemies.end()) {
        game.bossDelta -= delta;

        if (game.bossDelta <= 0) {
            if (!obstacleOnXY(game, {boss->position.x, boss->position.y + 1})
                && !obstacleOnXY(game, {boss->position.x - 1, boss->position.y + 1})
                && !obstacleOnXY(game, {boss->position.x + 1, boss->position.y + 1})) {
                boss->position.y += 1;
            };

            if (!game.inBounds(boss->position)) {
                game.lives = 0;
                return;
            };
            game.bossDelta = max(50, BOSS_DELTA - game.timeMs * 950 / 300000);
        };
    };
};

/**
 * @brief Update the boss's mega shot.
 *
 * @param game The current game state.
 * @param delta The time elapsed since the last frame.
 */
static void bossMegaShot(Game &game, const int delta) {
    game.bossShootCooldown -= delta;

    if (game.bossShootCooldown <= 0) {
        auto boss = find_if(game.enemies.begin(), game.enemies.end(),
            [](const Entity &enemy) { return enemy.kind == EntityKind::Boss; });

        if (boss == game.enemies.end()) {
            return;
        };

        Vector2D bossSides[8] = {
            {boss->position.x - 1, boss->position.y - 1},
            {boss->position.x, boss->position.y - 1},
            {boss->position.x + 1, boss->position.y - 1},
            {boss->position.x - 1, boss->position.y},
            {boss->position.x + 1, boss->position.y},
            {boss->position.x - 1, boss->position.y + 1},
            {boss->position.x, boss->position.y + 1},
            {boss->position.x + 1, boss->position.y + 1},
        };

        for (const auto& side : bossSides) {
            game.bossBullets.push_back({EntityKind::BulletBoss, side, 1});

            if (side.y == boss->position.y - 1) {
                game.bossBullets.back().velocity = {0, -1};
            } else if (side.y == boss->position.y + 1) {
                game.bossBullets.back().velocity = {0, 1};
            } else if (side.x == boss->position.x - 1) {
                game.bossBullets.back().velocity = {-1, 0};
            } else if (side.x == boss->position.x + 1) {
                game.bossBullets.back().velocity = {1, 0};
            };

            if (side == bossSides[0] || side == bossSides[5]) {
                game.bossBullets.push_back({EntityKind::BulletBoss, {side}, 1});
                game.bossBullets.back().velocity = {-1, 0};
            } else if (side == bossSides[2] || side == bossSides[7]) {
                game.bossBullets.push_back({EntityKind::BulletBoss, {side}, 1});
                game.bossBullets.back().velocity = {1, 0};
            };
        };

        game.bossShootCooldown = SHOOT_COOLDOWN * 3;
    };
};

/**
 * @brief Update the boss's position.
 *
 * @param game The current game state.
 * @param position The new position of the boss.
 */
void bossOnPosition(Game &game, Vector2D position) {
    for (auto &bossSide : game.bossSides) {
        if (position == bossSide.position) {
            game.lives = 0;
        };
    };
};

/**
 * @brief Update the boss's battle state.
 *
 * @param game The current game state.
 * @param delta The time elapsed since the last frame.
 */
void bossBattle(Game &game, const int delta) {
    bossIncoming(game, delta);
    bossMovement(game, delta);
    bossMegaShot(game, delta);
    bossBulletsTrajectory(game, delta);
    bossView(game);

    bossOnPosition(game, game.player.position);

    auto boss = find_if(game.enemies.begin(), game.enemies.end(),
        [](const Entity &enemy) { return enemy.kind == EntityKind::Boss; });

    if (boss != game.enemies.end() && boss->health <= 0) {
        game.enemies.erase(boss);
        game.bossSpawnInterval = BOSS_INTERVAL;
        game.bossSides.clear();
    };
};