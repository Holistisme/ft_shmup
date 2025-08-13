/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aheitz <aheitz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 07:43:13 by aheitz            #+#    #+#             */
/*   Updated: 2025/08/12 15:22:02 by aheitz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/render/render.hpp"

/* ************************************************************************** */

/**
 * @brief Clamp the time value to a specified range.
 *
 * @param value The time value to clamp.
 * @param min The minimum value for clamping.
 * @param max The maximum value for clamping.
 * @return int The clamped time value.
 */
static inline int clampTime(const int value, const int min, const int max) {
    if      (value < min) return min;
    else if (value > max) return max;
    else                  return value;
};

/* ************************************************************************** */

/**
 * @brief Draw the HUD (Heads-Up Display) for the game.
 *
 * @param game The current game state.
 * @return int The result of the drawing operation.
 */
int drawHud(const Game &game) {
    if (!stdscr) {
        return EXIT_FAILURE;
    };

    const int timeMs       = getTime(game);
    const int seconds      = timeMs / 1000;
    const int centiseconds = (timeMs % 1000) / 10;

    mvhline(0, 0, ' ', COLS);

    char display[64] = {0};
    snprintf(display, sizeof(display), "Score: %04d  Lives: %d  Time: %02d.%02ds",
        getScore(game), getLives(game), clampTime(seconds, 0, 5999), clampTime(centiseconds, 0, 99));

    mvprintw(0, COLS / 2 - strlen(display) / 2, "%s", display);
    return EXIT_SUCCESS;
};

/* ************************************************************************** */