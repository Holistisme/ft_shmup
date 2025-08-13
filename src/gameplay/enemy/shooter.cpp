/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shooter.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aheitz <aheitz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 17:28:23 by aheitz            #+#    #+#             */
/*   Updated: 2025/08/13 09:36:53 by aheitz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/gameplay/gameplay.hpp"

/* ************************************************************************** */

using namespace std;

/* ************************************************************************** */

/**
 * @brief Promote an enemy to a shooter.
 *
 * @param enemy The enemy entity.
 */
void promoteShooter(Entity &enemy) {
    enemy.kind     = EntityKind::Shooter;
    enemy.canShoot = true;
};

/**
 * @brief Handle shooting from a shooter enemy.
 *
 * @param game The current game state.
 * @param deltaTime The time elapsed since the last frame.
 */
void shootFromShooter(Game &game, const int deltaTime) {
    game.shooterShootCooldown -= deltaTime;

    if (game.shooterShootCooldown <= 0) {
        for (auto &enemy : game.enemies) {
            if (enemy.kind == EntityKind::Shooter)
                enemy.canShoot = true;
        };
        game.shooterShootCooldown = SHOOTER_COOLDOWN;
    };

    for (Entity &enemy : game.enemies) {
        if (enemy.kind == EntityKind::Shooter && enemy.canShoot && enemy.position.x == game.player.position.x) {
            const Vector2D shootPos = {enemy.position.x, enemy.position.y + 1};
            game.shootersBullets.push_back({EntityKind::BulletEnemy, shootPos, 1});
            enemy.canShoot = false;
        };
    };
};