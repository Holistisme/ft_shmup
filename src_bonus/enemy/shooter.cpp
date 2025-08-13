/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shooter.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsyutkin <vsyutkin@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 17:28:23 by aheitz            #+#    #+#             */
/*   Updated: 2025/08/13 10:42:39 by vsyutkin         ###   ########.fr       */
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
	enemy.color   = ENTITY_COLOR_RED;
	enemy.ch      = ENTITY_SYM_SHOOTER;
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
            game.shootersBullets.push_back({EntityKind::BulletEnemy, shootPos, 1, 0, 0, ENTITY_COLOR_YELLOW, ENTITY_SYM_BULLET_ENEMY});
            enemy.canShoot = false;
        };
    };
};