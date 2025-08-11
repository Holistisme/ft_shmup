/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mainb.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: benpicar <benpicar@student.42mulhouse.fr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 14:18:43 by benpicar          #+#    #+#             */
/*   Updated: 2025/08/11 16:23:00 by benpicar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ncurses.h>
#include <iostream>
#include <cstring>
#include <chrono>
#include <algorithm>
#include <vector>
#include <cstdlib>
#include <ctime>
    
int	main(void)
{
	initscr();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_BLUE, COLOR_BLACK);
    std::srand(std::time(nullptr));
    init_pair(3, COLOR_GREEN, COLOR_BLACK); // carré vert

    int x = COLS / 2, y = LINES / 2;
    int ch;
    timeout(50); // getch() attend 50ms max

    auto last_scroll = std::chrono::steady_clock::now();
    int scroll_interval = 500; // ms

    // Initialisation du motif des bords
    const int bord_height = LINES;
    char bord_pattern[bord_height];
    for (int i = 0; i < bord_height; ++i)
        bord_pattern[i] = (i % 2 == 0) ? '|' : ' ';

    // Vecteur pour stocker les positions des carrés bleus
    std::vector<std::pair<int, int>> blue_squares;
    auto last_blue_gen = std::chrono::steady_clock::now();
    int blue_gen_interval = 1000; // ms

    // Vecteur pour stocker les tirs verts
    std::vector<std::pair<int, int>> green_shots;
    auto last_shot = std::chrono::steady_clock::now();
    int shot_delay = 200; // ms
        int green_move_interval = 100; // ms (vitesse des tirs verts)
        auto last_green_move = std::chrono::steady_clock::now();

    while (true)
    {
        clear();
        // Affiche les bords verticaux
        for (int i = 0; i < LINES; ++i) {
            mvaddch(i, 0, bord_pattern[i % bord_height]);
            mvaddch(i, COLS - 1, bord_pattern[i % bord_height]);
        }
        // Affiche les carrés bleus
        attron(COLOR_PAIR(2));
        for (auto& sq : blue_squares) {
            if (sq.second >= 0 && sq.second < LINES && sq.first > 0 && sq.first < COLS - 1)
                mvaddch(sq.second, sq.first, ' ' | A_REVERSE);
        }
        attroff(COLOR_PAIR(2));
        // Affiche les tirs verts
        attron(COLOR_PAIR(3));
        for (auto& shot : green_shots) {
            if (shot.second >= 0 && shot.second < LINES && shot.first > 0 && shot.first < COLS - 1)
                mvaddch(shot.second, shot.first, ' ' | A_REVERSE);
        }
        attroff(COLOR_PAIR(3));
        // Affiche le carré rouge
        attron(COLOR_PAIR(1));
        mvaddch(y, x, ' ' | A_REVERSE);
        attroff(COLOR_PAIR(1));
        refresh();

        ch = getch();
        if (ch == 'q') break;
        // Déplacement du carré avec WASD
        if (ch == 'w' && y > 0) y--;
        if (ch == 's' && y < LINES - 1) y++;
        if (ch == 'a' && x > 1) x--;
        if (ch == 'd' && x < COLS - 2) x++;
        // Tir vers le haut avec espace
        auto now = std::chrono::steady_clock::now();
        if (ch == ' ' && std::chrono::duration_cast<std::chrono::milliseconds>(now - last_shot).count() > shot_delay) {
            green_shots.push_back({x, y - 1});
            last_shot = now;
        }

        // GAME OVER si le carré sort de l'écran
        if (x <= 0 || x >= COLS - 1 || y < 0 || y >= LINES) {
            attron(A_BOLD);
            while (1)
            {
                clear();
                mvprintw(LINES / 2, (COLS - 9) / 2, "GAME OVER");
                if (getch() == 'q')
                    break;
            }
            attroff(A_BOLD);
            refresh();
            break;
        }


        // Déplacement des tirs verts à leur propre rythme
        if (std::chrono::duration_cast<std::chrono::milliseconds>(now - last_green_move).count() > green_move_interval) {
            for (auto& shot : green_shots) {
                shot.second--;
            }
            // Supprime les tirs verts hors écran
            green_shots.erase(
                std::remove_if(green_shots.begin(), green_shots.end(), [](const std::pair<int,int>& shot) {
                    return shot.second < 0;
                }),
                green_shots.end()
            );
            last_green_move = now;
        }
		
		if (std::chrono::duration_cast<std::chrono::milliseconds>(now - last_scroll).count() > scroll_interval) {
            // Décale le motif vers le bas
            char last = bord_pattern[bord_height - 1];
            for (int i = bord_height - 1; i > 0; --i)
                bord_pattern[i] = bord_pattern[i - 1];
            bord_pattern[0] = last;
            last_scroll = now;
            // Déplace les carrés bleus vers le bas
            for (auto& sq : blue_squares) {
                sq.second++;
            }
            // Supprime les carrés bleus hors écran
            blue_squares.erase(
                std::remove_if(blue_squares.begin(), blue_squares.end(), [](const std::pair<int,int>& sq) {
                    return sq.second >= LINES;
                }),
                blue_squares.end()
            );
            
            // (Suppression déjà gérée dans le timer des tirs verts)
        }
        // Génération aléatoire de nouveaux carrés bleus
        if (std::chrono::duration_cast<std::chrono::milliseconds>(now - last_blue_gen).count() > blue_gen_interval) {
            int bx = 1 + std::rand() % (COLS - 2);
            blue_squares.push_back({bx, 0});
            last_blue_gen = now;
        }
    }

    endwin();
	return (0);
}
