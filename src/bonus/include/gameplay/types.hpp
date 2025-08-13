/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   types.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aheitz <aheitz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 15:11:48 by aheitz            #+#    #+#             */
/*   Updated: 2025/08/13 13:27:05 by aheitz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

/* ************************************************************************** */

#include <cassert>
#include <compare>
#include <cstdint>

/* ************************************************************************** */

/**
 * @brief A 2D vector type used for various gameplay mechanics.
 *
 */
struct Vector2D {
    int x{};
    int y{};

    [[nodiscard]] constexpr std::int64_t lengthSquared() const noexcept {
        using i64 = std::int64_t;
        return i64(x) * i64(x) + i64(y) * i64(y);
    };

    constexpr auto operator<=>(const Vector2D &other) const noexcept {
        if (auto d = lengthSquared() <=> other.lengthSquared(); d != 0) {
            return d;
        } else if (auto dx = x <=> other.x; dx != 0) {
            return dx;
        } else {
            return y <=> other.y;
        };
    };

    constexpr bool operator==(const Vector2D &other) const noexcept = default;
};

/* ************************************************************************** */

/**
 * @brief Enumeration representing different types of entities in the game.
 *
 */
enum class EntityKind: std::uint8_t {
    Player,
    Enemy,
    Shooter,
    Bomber,
    Fire,
    Dodger,
    Boss,
    BossSide,
    BulletPlayer,
    BulletEnemy,
    BulletBoss,
    Obstacle,
	WallA,
	WallB,
};

#define ENTITY_SYM_PLAYER       '^'
#define ENTITY_SYM_ENEMY        'v'
#define ENTITY_SYM_SHOOTER     'O'
#define ENTITY_SYM_BOMBER      '*'
#define ENTITY_SYM_DODGER      '~'
#define ENTITY_SYM_BULLET_PLAYER '|'
#define ENTITY_SYM_BULLET_ENEMY '|'
#define ENTITY_SYM_BULLET_BOSS   '|'
#define ENTITY_SYM_OBSTACLE    '#'
#define ENTITY_SYM_FIRE        '&'
#define ENTITY_SYM_WALL_A     '\\'
#define ENTITY_SYM_WALL_B     '/'

#define ENTITY_COLOR_BLUE  1
#define ENTITY_COLOR_RED   2
#define ENTITY_COLOR_GREEN 3
#define ENTITY_COLOR_WHITE 4
#define ENTITY_COLOR_YELLOW 5

/**
 * @brief Structure representing an entity in the game world.
 *
 */
struct Entity {
    EntityKind kind{};
    Vector2D   position{};
    int        health{100};
    bool       canShoot{false};
    int        manual{1};
    Vector2D   velocity{0, 0};
    char       ch{' '};
    short      color{0};

    constexpr auto operator<=>(const Entity &other) const noexcept = default;
};

/**
 * @brief Ensures that the size of EntityKind is exactly 1 byte.
 *
 */
static_assert(sizeof(EntityKind) == 1, "EntityKind size must be 1 byte");

/* ************************************************************************** */