/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bomber.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: benpicar <benpicar@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 06:28:43 by aheitz            #+#    #+#             */
/*   Updated: 2025/08/13 11:59:13 by benpicar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gameplay/gameplay.hpp"

/* ************************************************************************** */

using namespace std;

/* ************************************************************************** */

/**
 * @brief Promote an enemy to a bomber.
 *
 * @param enemy The enemy entity to promote.
 */
void promoteBomber(Entity &enemy) {
    enemy.kind     = EntityKind::Bomber;
	enemy.ch       = ENTITY_SYM_BOMBER;
	enemy.color    = ENTITY_COLOR_RED;
};

/**
 * @brief Handle the explosion of a bomber enemy.
 *
 * @param game The current game state.
 * @param bomber The bomber enemy entity.
 */
void bomberExplode(Game &game, const Entity &bomber) {
    Vector2D fireCercle[11] = {
        {bomber.position.x, bomber.position.y},
        {bomber.position.x, bomber.position.y - 1},
        {bomber.position.x, bomber.position.y + 1},
        {bomber.position.x - 1, bomber.position.y},
        {bomber.position.x + 1, bomber.position.y},
        {bomber.position.x - 1, bomber.position.y - 1},
        {bomber.position.x - 1, bomber.position.y + 1},
        {bomber.position.x + 1, bomber.position.y - 1},
        {bomber.position.x + 1, bomber.position.y + 1},
        {bomber.position.x - 2, bomber.position.y},
        {bomber.position.x + 2, bomber.position.y},
    };

    for (const auto &pos : fireCercle) {
        game.fires.push_back({EntityKind::Fire, pos, 100, 0, 0, ENTITY_COLOR_YELLOW, ENTITY_SYM_FIRE});
    };
};

/**
 * @brief Check if a fire is coming towards a specific position.
 *
 * @param game The current game state.
 * @param pos The position to check.
 * @return true If a fire is coming towards the position.
 * @return false If no fire is coming towards the position.
 */
bool fireComing(const Game &game, const Vector2D &pos) {
    for (const auto &fire : game.fires) {
        if (fire.position.x == pos.x
            && fire.position.y >= pos.y
            && fire.position.y <= game.player.position.y) {
            return true;
        };
    };
    return false;
};