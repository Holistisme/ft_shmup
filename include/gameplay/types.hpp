/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   types.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsyutkin <vsyutkin@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 15:11:48 by aheitz            #+#    #+#             */
/*   Updated: 2025/08/13 10:53:21 by vsyutkin         ###   ########.fr       */
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
    BulletPlayer,
    BulletEnemy,
    Obstacle,
	WallA,
	WallB,
};

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
	short      color{0};
	char	   ch{'@'};

    constexpr auto operator<=>(const Entity &other) const noexcept = default;
};

/**
 * @brief Ensures that the size of EntityKind is exactly 1 byte.
 *
 */
static_assert(sizeof(EntityKind) == 1, "EntityKind size must be 1 byte");

/* ************************************************************************** */