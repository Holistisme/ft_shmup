/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   defines.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsyutkin <vsyutkin@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 15:14:09 by vsyutkin          #+#    #+#             */
/*   Updated: 2025/08/13 14:01:09 by vsyutkin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define DEFAULT_WIDTH 100
#define DEFAULT_HEIGHT 25

#define DEFAULT_BULLET_SPEED 100 		// in ms
#define DEFAULT_GETCH_DELAY 50
#define DEFAULT_SCROLL_INTERVAL 100
#define DEFAULT_SHOT_DELAY 200			// in ms
#define DEFAULT_ENEMY_SPAWN_INTERVAL 1000 // in ms

#define MAGIC_NUMBER_DELTA_TIME 42

#define ENTITY_COLOR_BLUE 1
#define ENTITY_COLOR_RED 2
#define ENTITY_COLOR_GREEN 3
#define ENTITY_COLOR_WHITE 4
#define ENTITY_COLOR_YELLOW 5

#define ENTITY_SYM_PLAYER '^'
#define ENTITY_SYM_ENEMY 'v'
#define ENTITY_SYM_OBSTACLE '#'
#define ENTITY_SYM_BULLET_PLAYER '|'
#define ENTITY_SYM_BULLET_ENEMY '|'
#define ENTITY_SYM_BULLET_BOSS   '|'
#define ENTITY_SYM_WALL_A '\\'
#define ENTITY_SYM_WALL_B '/'
#define ENTITY_SYM_BOMBER '*'
#define ENTITY_SYM_DODGER '~'
#define ENTITY_SYM_SHOOTER 'O'
#define ENTITY_SYM_FIRE '&'