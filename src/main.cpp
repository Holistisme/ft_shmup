/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aheitz <aheitz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 14:34:57 by aheitz            #+#    #+#             */
/*   Updated: 2025/08/12 17:02:18 by aheitz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/gameplay/gameplay.hpp"
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
	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_BLUE, COLOR_RED);
	init_pair(3, COLOR_GREEN, COLOR_BLACK); // carré vert
    init_pair(4, COLOR_WHITE, COLOR_BLACK);
	std::srand(std::time(nullptr));

    nodelay(stdscr, TRUE);
	timeout(0);
}

//TODO: Did some things here, but not my work, please refactor this.
#include <thread>

bool	display(void)
{
	auto last_scroll = std::chrono::steady_clock::now();
	int scroll_interval = DEFAULT_SCROLL_INTERVAL; // ms

    // Initialisation du motif des bords
    const int bord_height = LINES;
    char bord_pattern[bord_height];
    for (int i = 0; i < bord_height; ++i)
	bord_pattern[i] = (i % 2 == 0) ? '\\' : '/';

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

        updateGameplay(game, delta, input);

        if (game.lives <= 0) {
            break;
        };

        clear();
        drawHud(game);
        const auto& vs = getViews(game);
        for (const auto& e : vs) {
            int x = e.position.x;
            int y = e.position.y + 1;
            if (x < 0 || x >= COLS || y < 1 || y >= LINES) continue;

            short cp = 0;
            switch (e.kind) {
                case EntityKind::Player:       cp = 1; break;
                case EntityKind::Enemy:        cp = 2; break;
                case EntityKind::BulletPlayer: cp = 3; break;
                case EntityKind::Obstacle:     cp = 4; break;
                default: break;
            };
            if (cp) attron(COLOR_PAIR(cp));
            mvaddch(y, x, ' ' | A_REVERSE);
            if (cp) attroff(COLOR_PAIR(cp));
        };

        // [AFFICHAGE] Affiche les bords verticaux
        for (int i = 0; i < LINES; ++i) {
            mvaddch(i, 0, bord_pattern[i % bord_height]); // [AFFICHAGE] Bord gauche
            mvaddch(i, COLS - 1, bord_pattern[i % bord_height]); // [AFFICHAGE] Bord droit
        }

        refresh();

		if (std::chrono::duration_cast<std::chrono::milliseconds>(now - last_scroll).count() > scroll_interval) {
            // Décale le motif vers le bas
            char last = bord_pattern[bord_height - 1];
            for (int i = bord_height - 1; i > 0; --i)
                bord_pattern[i] = bord_pattern[i - 1];
            bord_pattern[0] = last;
            last_scroll = now;
        }

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
	std::string opt2 = "2 Players";
	nodelay(stdscr, FALSE);
	while (true)
	{
        clear();
        mvprintw(LINES / 2 - 2, (COLS - title.size()) / 2, "%s", title.c_str());
        mvprintw(LINES / 2, (COLS - opt1.size()) / 2, "%s", opt1.c_str());
        mvprintw(LINES / 2 + 1, (COLS - opt2.size()) / 2, "%s", opt2.c_str());
        mvprintw(LINES / 2 + 3, (COLS - 30) / 2, "Appuyez sur 1 ou 2 pour commencer");
        refresh();
		
        ch = getch();
		mvprintw(0, 0, "ch = %d", ch);
        if (ch == '1' || ch == '2')
			break;
		if (ch == 'q')
		{
			endwin();
			exit(0);
		}
    }
	nodelay(stdscr, TRUE);
	if (ch == '1' || ch == '2')
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
		mvprintw(LINES / 2 - 2, (COLS - title.size()) / 2, "%s", title.c_str());
		mvprintw(LINES / 2, (COLS - score_str.size()) / 2, "%s", score_str.c_str());
		mvprintw(LINES / 2 + 1, (COLS - time_str.size()) / 2, "%s", time_str.c_str());
		mvprintw(LINES / 2 + 3, (COLS - 30) / 2, "Appuyez sur une touche pour quitter");
		refresh();
		ch = getch();
		if (ch != 'q' && ch != 410)
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