/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obstacle.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsyutkin <vsyutkin@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 15:34:59 by aheitz            #+#    #+#             */
/*   Updated: 2025/08/13 06:32:16 by vsyutkin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

/* ************************************************************************** */

#include "./gameplay.hpp"

/* ************************************************************************** */

#define OBSTACLE_DELTA          1000
#define OBSTACLE_SPAWN_INTERVAL 2000

/* ************************************************************************** */

void moveObstacles(Game &game, const int delta);
void spawnObstacle(Game &game, const int delta);

bool obstacleOnXY(const Game &game, const Vector2D &pos);

void buildWalls(Game &game);

bool obstacleComingOnY(const Game &game, const int y);
bool obstacleComingOnXY(const Game &game, const int x, const int y);