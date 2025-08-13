/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gameplay_player2.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsyutkin <vsyutkin@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 11:22:41 by vsyutkin          #+#    #+#             */
/*   Updated: 2025/08/13 11:23:44 by vsyutkin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @brief Initialize the game state.
 *
 * @param width The width of the game area.
 * @param height The height of the game area.
 * @return Game The initialized game state.
 */
Game initGameplay2(void) {
    Game game;

    game.rng = std::mt19937(std::random_device{}());

    game.score  = 0;
    game.lives  = 3;
    game.timeMs = 0;

    game.enemyDelta         = ENEMY_DELTA;
    game.enemySpawnInterval = ENEMY_SPAWN_INTERVAL;
    game.shootCooldown      = SHOOT_COOLDOWN;
    game.bulletDelta        = BULLET_DELTA;
    game.bulletCooldown     = BULLET_COOLDOWN;

    game.enemies.clear();
    game.bullets.clear();
    game.views  .clear();

	game.obstacleDelta         = OBSTACLE_DELTA;
    game.obstacleSpawnInterval = OBSTACLE_SPAWN_INTERVAL;

    game.player = {EntityKind::Player, Vector2D{COLS / 3, LINES - 3}, 100};
	game.player2 = {EntityKind::Player2, Vector2D{COLS * 2 / 3, LINES - 3}, 100};
    game.pushView(game.player);
	game.pushView(game.player2);

    return game;
};


void enemyDamage2(Game &game) {
    for (auto &enemy : game.enemies) {
        if (enemy.position == game.player.position) {
            --game.lives;
            enemy.health = 0;
        };
		if (enemy.position == game.player2.position) {
            --game.lives;
            enemy.health = 0;
        };
    };
};


/**
 * @brief Update the gameplay state.
 *
 * @param game The current game state.
 * @param deltaTime The time elapsed since the last update.
 * @param input The current player input.
 */
void updateGameplay2(Game &game, const int deltaTime, const unsigned input) {
    game.timeMs        += deltaTime;
    game.shootCooldown -= deltaTime;

    if (input & INPUT_W)        game.player.position.y -= 1;
    if (input & INPUT_S)        game.player.position.y += 1;
    if (input & INPUT_A)        game.player.position.x -= 1;
    if (input & INPUT_D)        game.player.position.x += 1;

    if (input & INPUT_SPACE)    shootBullet(game, game.player.position);

    game.player.position.x = clamp(game.player.position.x, 1, COLS - 2);
    game.player.position.y = clamp(game.player.position.y, 0, LINES - 2);

	if (input & INPUT_UP)		game.player2.position.y -= 1;
	if (input & INPUT_DOWN)		game.player2.position.y += 1;
	if (input & INPUT_LEFT)		game.player2.position.x -= 1;
	if (input & INPUT_RIGHT)	game.player2.position.x += 1;

	if (input & INPUT_0)		shootBullet(game, game.player2.position);

	game.player2.position.x = clamp(game.player2.position.x, 1, COLS - 2);
	game.player2.position.y = clamp(game.player2.position.y, 0, LINES - 2);

    clearOutOfBoundsEntities(game);
    handleEnemySpawn(game, deltaTime);
    moveEnemies(game, deltaTime);
    moveBullets(game, deltaTime);
    hitEntities(game);
    enemyDamage(game);

	spawnObstacle(game, deltaTime);
    moveObstacles2(game, deltaTime);

    game.views.clear();
    game.views.reserve(1 + game.enemies.size() + game.bullets.size());
    game.pushView(game.player);
	game.pushView(game.player2);
    for (const auto &enemy : game.enemies) {
        game.pushView(enemy);
    };
    for (const auto &bullet : game.bullets) {
        game.pushView(bullet);
    };
	for (const auto &obstacle : game.obstacles) {
        game.pushView(obstacle);
    };
};

