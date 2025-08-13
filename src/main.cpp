/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aheitz <aheitz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 14:34:57 by aheitz            #+#    #+#             */
/*   Updated: 2025/08/13 13:35:55 by aheitz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/gameplay/gameplay.hpp"
#include "../include/gameplay/obstacle.hpp"
#include "../include/render/render.hpp"
#include "../include/defines.hpp"

// Temporary includes for testing
#include <cstdio>

// benpicar
#include <ncurses.h>
#include <iostream>
#include <cstring>
#include <chrono>
#include <algorithm>
#include <vector>
#include <cstdlib>
#include <ctime>

static inline void	showGameOverScreen(int score, double elapsed_seconds);

void	init_display(void)
{
	initscr();
	noecho();
	curs_set(0);
	keypad(stdscr, TRUE);
	start_color();
	init_pair(1, COLOR_BLACK, COLOR_BLUE);
	init_pair(2, COLOR_BLACK, COLOR_RED);
	init_pair(3, COLOR_BLACK, COLOR_GREEN); // carré vert
    init_pair(4, COLOR_BLACK, COLOR_WHITE);
    init_pair(5, COLOR_BLACK, COLOR_YELLOW);
	std::srand(std::time(nullptr));

    nodelay(stdscr, TRUE);
	timeout(0);
}

//TODO: Did some things here, but not my work, please refactor this.
#include <thread>

bool	display(void)
{
    Game game = initGameplay();
    bool running = true;
    auto prev_frame = std::chrono::steady_clock::now();
	while (running)
	{
        auto now = std::chrono::steady_clock::now();
        int delta = (int)std::chrono::duration_cast<std::chrono::milliseconds>(now - prev_frame).count();
        if (delta < 0)   delta = 0;
        if (delta > 50)  delta = 50;
        prev_frame = now;

        unsigned input = 0;
        int ch;
        while ((ch = getch()) != ERR) {
            switch (ch) {
                case 'q': running = false; break;
                case 'w': case KEY_UP:    input |= INPUT_W;     break;
                case 's': case KEY_DOWN:  input |= INPUT_S;     break;
                case 'a': case KEY_LEFT:  input |= INPUT_A;     break;
                case 'd': case KEY_RIGHT: input |= INPUT_D;     break;
                case ' ':                 input |= INPUT_SPACE; break;
                default: break;
            }
        }
        if (!running) break;
		
		erase();

        updateGameplay(game, delta, input);

        if (game.lives <= 0) {
            break;
        };

        drawHud(game);
        const auto& vs = getViews(game);
        for (const auto& e : vs) {
            int x = e.position.x;
            int y = e.position.y + 1;
            if (x < 0 || x >= COLS || y < 1 || y >= LINES) continue;

            short cp = e.color;
            char  ch = e.ch;
            if (cp) attron(COLOR_PAIR(cp));
            mvaddch(y, x, ch | A_REVERSE);
            if (cp) attroff(COLOR_PAIR(cp));
        };
		
        refresh();

        auto end = std::chrono::steady_clock::now();
        int used = (int)std::chrono::duration_cast<std::chrono::milliseconds>(end - now).count();
        if (used < 16) std::this_thread::sleep_for(std::chrono::milliseconds(16 - used));
    }
	if (game.lives <= 0)
	{
		showGameOverScreen(game.score, game.timeMs / 1000.0);
	}
	return (false);
}

static inline void	Display_menu(void)
{
	int ch;
	std::string title = "FT_SHMUP";
	std::string opt1 = "1 Player";
	nodelay(stdscr, FALSE);
	while (true)
	{
        clear();
        mvprintw(LINES / 2 - 2, (COLS - title.size()) / 2, "%s", title.c_str());
        mvprintw(LINES / 2, (COLS - opt1.size()) / 2, "%s", opt1.c_str());
        mvprintw(LINES / 2 + 3, (COLS - 30) / 2, "Appuyez sur 1 pour commencer");
        refresh();
		
        ch = getch();
		mvprintw(0, 0, "ch = %d", ch);
        if (ch == '1')
			break;
		if (ch == 'q')
		{
			endwin();
			exit(0);
		}
    }
	nodelay(stdscr, TRUE);
	if (ch == '1')
	{
		display();
	}
}

static inline void	showGameOverScreen(int score, double elapsed_seconds)
{
	std::string	title = "GAME OVER";
	std::string	score_str = "Score : " + std::to_string(score);
	char		time_buf[64];
	snprintf(time_buf, sizeof(time_buf), "Temps : %.2f s", elapsed_seconds);
	std::string	time_str = time_buf;
	int			ch;
	nodelay(stdscr, FALSE);
	while (true)
	{
		clear();
		attron(COLOR_PAIR(2) | A_REVERSE);
		mvprintw(LINES / 2 - 2, (COLS - title.size()) / 2, "%s", title.c_str());
		attroff(COLOR_PAIR(2) | A_REVERSE);
		attron(COLOR_PAIR(3) | A_REVERSE);
		mvprintw(LINES / 2, (COLS - score_str.size()) / 2, "%s", score_str.c_str());
		mvprintw(LINES / 2 + 1, (COLS - time_str.size()) / 2, "%s", time_str.c_str());
		mvprintw(LINES / 2 + 3, (COLS - 30) / 2, "Appuyez sur 'n' pour retourner au menu");
		attroff(COLOR_PAIR(3) | A_REVERSE);
		refresh();
		ch = getch();
		if (ch == 'n')
			break;
		else if (ch == 'q')
		{
			endwin();
			exit(0);
		}
	}
	nodelay(stdscr, TRUE);
	Display_menu();
}

/**
 * @brief Main entry point of the program
 *
 * @return int Returns 0 on successful execution
 */
int	main(void)
{
    init_display();

    // Menu principal
    Display_menu();

    endwin();
    return (0);
}