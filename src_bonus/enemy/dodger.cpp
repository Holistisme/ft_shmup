/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dodger.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsyutkin <vsyutkin@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 07:06:15 by aheitz            #+#    #+#             */
/*   Updated: 2025/08/13 10:42:06 by vsyutkin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/gameplay/gameplay.hpp"

/* ************************************************************************** */

using namespace std;

/* ************************************************************************** */

/**
 * @brief Promote an enemy to a dodger.
 *
 * @param enemy The enemy entity.
 */
void promoteDodger(Entity &enemy) {
    enemy.kind = EntityKind::Dodger;
	enemy.ch = ENTITY_SYM_DODGER;
	enemy.color = ENTITY_COLOR_RED;
};

/**
 * @brief Jams the player's gun, preventing shooting for a period of time.
 *
 * @param game The current game state.
 */
void jamsGun(Game &game) {
    game.shootCooldown = SHOOT_COOLDOWN * 5;
};