/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obstacle.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: benpicar <benpicar@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 15:34:59 by aheitz            #+#    #+#             */
/*   Updated: 2025/08/12 18:41:27 by benpicar         ###   ########.fr       */
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
void moveObstacles2(Game &game, const int delta);
void spawnObstacle(Game &game, const int delta);

bool obstacleOnXY(const Game &game, const Vector2D &pos);

bool obstacleComingOnY(const Game &game, const int y);
bool obstacleComingOnXY(const Game &game, const int x, const int y);