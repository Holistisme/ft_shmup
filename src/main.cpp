/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aheitz <aheitz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 14:34:57 by aheitz            #+#    #+#             */
/*   Updated: 2025/08/11 17:21:32 by aheitz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/gameplay/gameplay.hpp"

// Temporary includes for testing
#include <cstdio>

/**
 * @brief Main entry point of the program
 *
 * @return int Returns 0 on successful execution
 */
int main(void) {
    initGameplay(42, 42);

    while (true) {
        const int keys[]  = {INPUT_W, INPUT_A, INPUT_S, INPUT_D, INPUT_SPACE};
        const int randKey = keys[rand() % 5];

        updateGameplay(42, randKey);
        printf("Random key pressed: %d\n", randKey);
        printf("Score: %d, Lives: %d, Time: %d\n", getScore(), getLives(), getTime());

        const auto &views = getViews();
        printf("Entities in view: %zu\n", views.size());
    };
    return (0);
};