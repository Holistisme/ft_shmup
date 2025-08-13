/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obstacle.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsyutkin <vsyutkin@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 15:34:24 by aheitz            #+#    #+#             */
/*   Updated: 2025/08/13 06:24:25 by vsyutkin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/gameplay/obstacle.hpp"

/* ************************************************************************** */

using namespace std;

/* ************************************************************************** */

/**
 * @brief Check if there is an obstacle coming on the specified X and Y coordinates.
 *
 * @param game The current game state.
 * @param x The X coordinate to check.
 * @param y The Y coordinate to check.
 * @return true If there is an obstacle coming on the X and Y coordinates.
 * @return false If there is no obstacle coming on the X and Y coordinates.
 */
bool obstacleComingOnXY(const Game &game, const int x, const int y) {
    return any_of(game.obstacles.begin(), game.obstacles.end(), [x, y](const Entity &obstacle) {
        return obstacle.position.x == x && obstacle.position.y > y;
    });
};

/**
 * @brief
 * @brief Check if there is an obstacle coming on the specified Y coordinate.
 *
 * @param game The current game state.
 * @param y The Y coordinate to check.
 * @return true If there is an obstacle coming on the Y coordinate.
 * @return false If there is no obstacle coming on the Y coordinate.
 */
bool obstacleComingOnY(const Game &game, const int y) {
    return any_of(game.obstacles.begin(), game.obstacles.end(), [y, &game](const Entity &obstacle) {
        return obstacle.position.y == y && obstacle.position.x > game.player.position.x;
    });
};

/**
 * @brief
 * @brief Check if there is an obstacle on the specified X coordinate.
 *
 * @param game The current game state.
 * @param x The X coordinate to check.
 * @return true If there is an obstacle on the X coordinate.
 * @return false If there is no obstacle on the X coordinate.
 */
static bool obstacleOnX(const Game &game, const int x) {
    return any_of(game.obstacles.begin(), game.obstacles.end(), [x](const Entity &obstacle) {
        return obstacle.position.x == x;
    });
};

/**
 * @brief
 * @brief Check if there is an obstacle on the specified X and Y coordinates.
 *
 * @param game The current game state.
 * @param pos The position to check.
 * @return true If there is an obstacle on the X and Y coordinates.
 * @return false If there is no obstacle on the X and Y coordinates.
 */
bool obstacleOnXY(const Game &game, const Vector2D &pos) {
    return any_of(game.obstacles.begin(), game.obstacles.end(), [pos](const Entity &obstacle) {
        return obstacle.position == pos;
    });
};

//yeah don't care about that ty
/* 
 * @brief Build the walls of the game.
 *
 * @param game The current game state.
 */
void	buildWalls(Game &game)
{
	(void)game;
	for (auto i = 0; i < LINES; i++)
	{
		if (i % 2 == 0 && game.wallType)
		{
			game.walls.push_back({EntityKind::WallA, Vector2D{0, i}, 1});
			game.walls.push_back({EntityKind::WallB, Vector2D{COLS - 1, i}, 1});
		}
		else if (game.wallType)
		{
			game.walls.push_back({EntityKind::WallB, Vector2D{0, i}, 1});
			game.walls.push_back({EntityKind::WallA, Vector2D{COLS - 1, i}, 1});
		}
		else if (i % 2 == 0)
		{
			game.walls.push_back({EntityKind::WallB, Vector2D{0, i}, 1});
			game.walls.push_back({EntityKind::WallA, Vector2D{COLS - 1, i}, 1});
		}
		else
		{
			game.walls.push_back({EntityKind::WallA, Vector2D{0, i}, 1});
			game.walls.push_back({EntityKind::WallB, Vector2D{COLS - 1, i}, 1});
		}
	}
	game.wallType = !game.wallType;
}

/**
 * @brief Move the obstacles down the screen.
 *
 * @param game The current game state.
 * @param delta The time delta since the last frame.
 */
void moveObstacles(Game &game, const int delta) {
    game.obstacleDelta -= delta;
    if (game.obstacleDelta <= 0) {
        for (auto &obstacle : game.obstacles) {
            obstacle.position.y++;
            if (obstacle.position == game.player.position) {
                --game.lives;
                if (game.player.position.y == LINES - 2) {
                    game.lives = 0;
                } else {
                    game.player.position.y++;
                };
            };
            for (auto &enemy : game.enemies) {
                if (obstacle.position == enemy.position) {
                    enemy.health = 0;
                };
            };
        };
		// for (auto &wall : game.walls) {
        game.obstacleDelta = max(50, OBSTACLE_DELTA - game.timeMs / 316);
		buildWalls(game);
    };
};

/**
 * @brief Spawn a new obstacle.
 *
 * @param game The current game state.
 * @param delta The time delta since the last frame.
 */
void spawnObstacle(Game &game, const int delta) {
    game.obstacleSpawnInterval -= delta;

    if ((int)game.obstacles.size() >= COLS / 3 || COLS <= 0 || LINES <= 0 || game.obstacleSpawnInterval > 0) {
        return;
    };

    int x{0};
    do {
        uniform_int_distribution<int> randX(0, max(0, COLS - 1));
        x = randX(game.rng);
    } while (obstacleOnX(game, x));
    game.obstacleSpawnInterval = OBSTACLE_SPAWN_INTERVAL;
    game.obstacles.push_back({EntityKind::Obstacle, Vector2D{x, 1}, 1});
};

/* ************************************************************************** */