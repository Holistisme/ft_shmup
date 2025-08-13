/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gameplay.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsyutkin <vsyutkin@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 16:14:40 by aheitz            #+#    #+#             */
/*   Updated: 2025/08/13 10:48:32 by vsyutkin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/gameplay/gameplay.hpp"
#include "../../include/gameplay/obstacle.hpp"

/* ************************************************************************** */

using namespace std;

/* ************************************************************************** */

//TODO: Parse this fucking file, it's a mess

/**
 * @brief Initialize the game state.
 *
 * @param width The width of the game area.
 * @param height The height of the game area.
 * @return Game The initialized game state.
 */
Game initGameplay(void) {
    Game game;

    game.rng = std::mt19937(std::random_device{}());

    game.score  = 0;
    game.lives  = 3;
    game.timeMs = 0;

    game.enemyDelta         = ENEMY_DELTA;
    game.enemySpawnInterval = ENEMY_SPAWN_INTERVAL;
    game.shootCooldown      = SHOOT_COOLDOWN;
    game.bulletDelta        = BULLET_DELTA;

    game.shooterShootCooldown = SHOOTER_COOLDOWN;
    game.shooterSpawnInterval = SHOOTER_INTERVAL;

    game.bomberSpawnInterval = BOMBER_INTERVAL;
    game.bomberDelta         = ENEMY_DELTA * 2;

    game.dodgerSpawnInterval = DODGER_INTERVAL;
    game.dodgerDelta         = ENEMY_DELTA / 2;

    game.enemies.clear();
    game.bullets.clear();
    game.views  .clear();
	game.walls  .clear();

    game.obstacleDelta         = OBSTACLE_DELTA;
    game.obstacleSpawnInterval = OBSTACLE_SPAWN_INTERVAL;

	buildWalls(game);

    game.player = {EntityKind::Player, Vector2D{COLS / 2, LINES - 3}, 100, 0, 0, ENTITY_COLOR_BLUE, ENTITY_SYM_PLAYER};
    game.pushView(game.player);

    return game;
};

/* ************************************************************************** */

/**
 * @brief Clear entities that are out of bounds.
 *
 * @param game The current game state.
 */
void clearOutOfBoundsEntities(Game &game) {
    const auto outOfBounds = [&](const Entity &entity) { return !game.inBounds(entity.position); };

    game.enemies.erase(remove_if(game.enemies.begin(), game.enemies.end(),
        [&](const Entity &entity) {
            return outOfBounds(entity) || entity.health <= 0;
        }), game.enemies.end());

    game.bullets.erase(remove_if(game.bullets.begin(), game.bullets.end(),
        [&](const Entity &entity) {
            return outOfBounds(entity) || entity.health <= 0;
        }), game.bullets.end());

    game.obstacles.erase(remove_if(game.obstacles.begin(), game.obstacles.end(),
        [&](const Entity &entity) {
            return outOfBounds(entity) || entity.health <= 0;
        }), game.obstacles.end());

    game.shootersBullets.erase(remove_if(game.shootersBullets.begin(), game.shootersBullets.end(),
        [&](const Entity &bullet) {
            return outOfBounds(bullet) || bullet.health <= 0;
        }), game.shootersBullets.end());

    game.fires.erase(remove_if(game.fires.begin(), game.fires.end(),
        [&](const Entity &fire) {
            return outOfBounds(fire) || fire.health <= 0;
        }), game.fires.end());
};

/* ************************************************************************** */

/**
 * @brief Apply damage to the player if an enemy collides with them.
 *
 * @param game The current game state.
 */
void enemyDamage(Game &game) {
    for (auto &enemy : game.enemies) {
        if (enemy.position == game.player.position) {
            if (enemy.kind == EntityKind::Dodger) {
                jamsGun(game);
                enemy.health = 0;
                continue;
            };
            --game.lives;
            enemy.health = 0;
            if (enemy.kind == EntityKind::Bomber) {
                bomberExplode(game, enemy);
            };
        };
    };
};

/**
 * @brief
 * @brief Check if a bullet is on the way to the enemy.
 *
 * @param game The current game state.
 * @param enemy The enemy entity.
 * @param x The X coordinate to check.
 * @return true If a bullet is on the way to the enemy.
 * @return false If no bullet is on the way to the enemy.
 */
bool bulletOnWay(const Game &game, const Entity &enemy, const int x) {
    return any_of(game.bullets.begin(), game.bullets.end(),
        [&](const Entity &bullet) {
            return bullet.position.x == x
                && bullet.position.y >= enemy.position.y
                && bullet.position.y <= game.player.position.y;
        });
};

/**
 * @brief Check if there is an enemy on the specified X coordinate.
 *
 * @param game The current game state.
 * @param x The X coordinate to check.
 * @return true If there is an enemy on the X coordinate.
 * @return false If there is no enemy on the X coordinate.
 */
bool enemyOnX(const Game &game, const int x, const int y) {
    return any_of(game.enemies.begin(), game.enemies.end(),
        [&](const Entity &enemy) {
            return enemy.position.x == x
                && enemy.position.y < game.player.position.y
                && abs(enemy.position.y - y) <= 5;
        });
};

/**
 * @brief 
 * @brief Check if there is an enemy on the specified position.
 *
 * @param game The current game state.
 * @param pos The position to check.
 * @return true If there is an enemy on the position.
 * @return false If there is no enemy on the position.
 */
bool enemyOnPos(const Game &game, const Vector2D &pos) {
    return any_of(game.enemies.begin(), game.enemies.end(),
        [&](const Entity &enemy) {
            return enemy.position == pos;
        });
};

/**
 * @brief Move enemies in the game.
 *
 * @param game The current game state.
 * @param delta The time elapsed since the last frame.
 */
void moveEnemies(Game &game, const int delta) {
    Vector2D newPos;
    game.enemyDelta  -= delta;
    game.dodgerDelta -= delta;
    game.bomberDelta -= delta;

    for (auto &enemy : game.enemies) {
        if ((game.enemyDelta <= 0
            || (enemy.kind == EntityKind::Dodger && game.dodgerDelta <= 0)
            || (enemy.kind == EntityKind::Bomber && game.bomberDelta <= 0))
            && !enemy.canShoot) {

            if ((enemy.manual == 1 && !game.inBounds(Vector2D{enemy.position.x + 1, enemy.position.y}))
                || (enemy.manual == -1 && !game.inBounds(Vector2D{enemy.position.x - 1, enemy.position.y}))) {
                enemy.manual = -enemy.manual;
            };

            if (fireComing(game, Vector2D{enemy.position.x, enemy.position.y})) {
                newPos = {enemy.position.x + enemy.manual, enemy.position.y};

                if (!obstacleOnXY(game, newPos) && !enemyOnPos(game, newPos)) {
                    enemy.position = newPos;
                };
            } else if (obstacleComingOnXY(game, enemy.position.x, enemy.position.y)) {
                newPos = {enemy.position.x + enemy.manual, enemy.position.y};

                if (!obstacleOnXY(game, newPos) && !enemyOnPos(game, newPos)) {
                    enemy.position = newPos;
                };
            } else {
                newPos = {enemy.position.x, enemy.position.y + 1};

                if (!enemyOnPos(game, newPos)) {
                    enemy.position.y++;
                } else {
                    newPos = {enemy.position.x + enemy.manual, enemy.position.y};
                    if (!enemyOnPos(game, newPos) && !obstacleOnXY(game, newPos)) {
                        enemy.position = newPos;
                    };
                };

                if (enemy.position.y <= game.player.position.y) {
                    newPos = enemy.position;

                    if (game.player.position.x > enemy.position.x) {
                        newPos.x++;
                    } else if (game.player.position.x < enemy.position.x) {
                        newPos.x--;
                    };

                    if (!enemyOnPos(game, newPos)
                        && !obstacleOnXY(game, newPos)
                        && !fireComing(game, newPos)
                        && ((!enemyOnX(game, newPos.x, newPos.y)
                            && !bulletOnWay(game, enemy, newPos.x))
                            || game.player.position.y == enemy.position.y)) {
                        enemy.position = newPos;
                    };
                }

                if (enemy.kind == EntityKind::Dodger) {
                    if (bulletOnWay(game, enemy, enemy.position.x)) {
                        newPos = {enemy.position.x + enemy.manual, enemy.position.y};

                        if (!bulletOnWay(game, enemy, newPos.x) && !enemyOnPos(game, newPos) && !obstacleOnXY(game, newPos)) {
                            enemy.position = newPos;
                        };
                    };
                };
            };
        };
    };

    if (game.enemyDelta <= 0) {
        game.enemyDelta = max(50, ENEMY_DELTA - game.timeMs / 2000);
    };

    if (game.dodgerDelta <= 0) {
        game.dodgerDelta = max(50, ENEMY_DELTA * 2 - game.timeMs / 2000);
    };

    if (game.bomberDelta <= 0) {
        game.bomberDelta = max(50, ENEMY_DELTA * 2 - game.timeMs / 2000);
    };
};

/**
 * @brief Spawn a new enemy in the game.
 *
 * @param game The current game state.
 * @param kind The kind of enemy to spawn.
 */
void spawnEnemy(Game &game, const EntityKind kind) {
    uniform_int_distribution<int> randX(0, max(0, COLS - 2));
    game.enemies.push_back({EntityKind::Enemy, Vector2D{randX(game.rng), 0}, 1, 0, 0, ENTITY_COLOR_RED, ENTITY_SYM_ENEMY});

    if (kind == EntityKind::Shooter) {
        promoteShooter(game.enemies.back());
    };

    if (kind == EntityKind::Bomber) {
        promoteBomber(game.enemies.back());
    };

    if (kind == EntityKind::Dodger) {
        promoteDodger(game.enemies.back());
    };

    if (uniform_int_distribution<>(0, 1)(game.rng) == 0) {
        game.enemies.back().manual *= -1;
    };
};

/**
 * @brief Handle enemy spawning in the game.
 *
 * @param game The current game state.
 * @param delta The time elapsed since the last update.
 */
void handleEnemySpawn(Game &game, const int delta) {
    game.enemySpawnInterval   -= delta;
    game.shooterSpawnInterval -= delta;
    game.bomberSpawnInterval  -= delta;
    game.dodgerSpawnInterval  -= delta;

    if (game.enemySpawnInterval <= 0) {
        if (game.bomberSpawnInterval <= 0) {
            spawnEnemy(game, EntityKind::Bomber);
            game.bomberSpawnInterval = BOMBER_INTERVAL;
        } else if (game.shooterSpawnInterval <= 0) {
            spawnEnemy(game, EntityKind::Shooter);
            game.shooterSpawnInterval = SHOOTER_INTERVAL;
        } else if (game.dodgerSpawnInterval <= 0) {
            spawnEnemy(game, EntityKind::Dodger);
            game.dodgerSpawnInterval = DODGER_INTERVAL;
        } else {
            spawnEnemy(game, EntityKind::Enemy);
        };
        game.enemySpawnInterval = ENEMY_SPAWN_INTERVAL;
    };
};

/* ************************************************************************** */

/**
 * @brief Check for collisions between bullets and enemies.
 *
 * @param game The current game state.
 */
void hitEntities(Game &game) {
    for (auto &bullet : game.bullets) {
        for (auto &enemy : game.enemies) {
            if (bullet.position == enemy.position) {
                bullet.health  = 0;
                enemy.health   = 0;
                game.score     += 1;
                if (enemy.kind == EntityKind::Bomber) {
                    bomberExplode(game, enemy);
                };
            };
        };
        for (auto &obstacle : game.obstacles) {
            if (bullet.position == obstacle.position) {
                bullet.health = 0;
            };
        };

        for (auto &shooterBullet : game.shootersBullets) {
            if (bullet.position == shooterBullet.position) {
                bullet.health         = 0;
                shooterBullet.health  = 0;
                game.score           += 1;
            };
        };
    };

    for (auto &shooterBullet : game.shootersBullets) {
        shooterBullet.position.y += 1;
        for (auto &enemy : game.enemies) {
            if (shooterBullet.position == enemy.position) {
                enemy.health = 0;
                if (enemy.kind == EntityKind::Bomber) {
                    bomberExplode(game, enemy);
                };
            };
        };

        for (auto &obstacle : game.obstacles) {
            if (shooterBullet.position == obstacle.position) {
                shooterBullet.health = 0;
            };
        };

        if (game.player.position == shooterBullet.position) {
            shooterBullet.health = 0;
            --game.lives;
        };
    };

    for (auto &fire : game.fires) {
        if (uniform_int_distribution<>(0, 1)(game.rng) == 0) {
            fire.health -= 1;
        };

        for (auto &enemy : game.enemies) {
            if (fire.position == enemy.position) {
                enemy.health = 0;
            };
        };

        for (auto &obstacle : game.obstacles) {
            if (fire.position == obstacle.position) {
                fire.health = 0;
            };
        };

        if (game.player.position == fire.position) {
            fire.health = 0;
            game.lives  = 0;
        };
    };
};

/**
 * @brief Move the bullets in the game.
 *
 * @param game The current game state.
 * @param delta The time elapsed since the last update.
 */
void moveBullets(Game &game, const int delta) {
    game.bulletDelta -= delta;

    if (game.bulletDelta <= 0) {
        for (auto &bullet : game.bullets) {
            bullet.position.y -= 1;
        };
        game.bulletDelta = BULLET_DELTA;
    };
};

/**
 * @brief Shoot a bullet from the player's position.
 *
 * @param game The current game state.
 * @param position The position to shoot the bullet from.
 */
void shootBullet(Game &game, const Vector2D &position) {
    if (game.shootCooldown <= 0) {
        const Vector2D shootPos = {position.x, position.y - 1};
        game.bullets.push_back({EntityKind::BulletPlayer, shootPos, 1, 0, 0, ENTITY_COLOR_GREEN, ENTITY_SYM_BULLET_PLAYER});
        game.shootCooldown = SHOOT_COOLDOWN;
    };
};

/* ************************************************************************** */

/**
 * @brief Update the gameplay state.
 *
 * @param game The current game state.
 * @param deltaTime The time elapsed since the last update.
 * @param input The current player input.
 */
void updateGameplay(Game &game, const int deltaTime, const unsigned input) {
    game.timeMs        += deltaTime;
    game.shootCooldown -= deltaTime;

    if (input & INPUT_W && !obstacleOnXY(game, {game.player.position.x, game.player.position.y - 1})) {
        game.player.position.y -= 1;
    };
    if (input & INPUT_S && !obstacleOnXY(game, {game.player.position.x, game.player.position.y + 1})) {
        game.player.position.y += 1;
    };
    if (input & INPUT_A && !obstacleOnXY(game, {game.player.position.x - 1, game.player.position.y})) {
        game.player.position.x -= 1;
    };
    if (input & INPUT_D && !obstacleOnXY(game, {game.player.position.x + 1, game.player.position.y})) {
        game.player.position.x += 1;
    };

    if (input & INPUT_SPACE)    shootBullet(game, game.player.position);

    game.player.position.x = clamp(game.player.position.x, 1, COLS - 2);
    game.player.position.y = clamp(game.player.position.y, 0, LINES - 2);

    moveBullets(game, deltaTime);
    shootFromShooter(game, deltaTime);

    handleEnemySpawn(game, deltaTime);
    moveEnemies(game, deltaTime);
    hitEntities(game);
    enemyDamage(game);

    spawnObstacle(game, deltaTime);
    moveObstacles(game, deltaTime);

    clearOutOfBoundsEntities(game);

    game.views.clear();
    game.views.reserve(1 + game.enemies.size() + game.bullets.size());
    game.pushView(game.player);
    for (const auto &enemy : game.enemies) {
        game.pushView(enemy);
    };
    for (const auto &bullet : game.bullets) {
        game.pushView(bullet);
    };
    for (const auto &obstacle : game.obstacles) {
        game.pushView(obstacle);
    };
    for (const auto &shooterBullet : game.shootersBullets) {
        game.pushView(shooterBullet);
    };
    for (const auto &fire : game.fires) {
        game.pushView(fire);
    };
	for (const auto &wall : game.walls) {
		game.pushView(wall);
	}
};

/* ************************************************************************** */

/**
 * @brief Get the Score object
 *
 * @param game The current game state.
 * @return int The current score.
 */
int getScore(const Game &game) {
    return game.score;
};

/**
 * @brief Get the Lives object
 *
 * @param game The current game state.
 * @return int The current lives.
 */
int getLives(const Game &game) {
    return game.lives;
};

/**
 * @brief Get the Time object
 *
 * @param game The current game state.
 * @return int The current time.
 */
int getTime(const Game &game) {
    return game.timeMs;
};

/**
 * @brief Get the Views object
 *
 * @param game The current game state.
 * @return const vector<Entity>& The current views.
 */
const vector<Entity> &getViews(const Game &game) {
    return game.views;
};

/* ************************************************************************** */