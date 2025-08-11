/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gameplay.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aheitz <aheitz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 16:15:41 by aheitz            #+#    #+#             */
/*   Updated: 2025/08/11 17:21:41 by aheitz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

/* ************************************************************************** */

#include "./types.hpp"

#include <algorithm>
#include <vector>

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
};

/* ************************************************************************** */

void initGameplay  (const int width,     const int      height);
void updateGameplay(const int deltaTime, const unsigned input);

int getScore(void);
int getLives(void);
int getTime (void);

const std::vector<Entity> &getViews(void);