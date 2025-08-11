/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aheitz <aheitz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 14:34:57 by aheitz            #+#    #+#             */
/*   Updated: 2025/08/11 16:01:58 by aheitz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/gameplay/types.hpp"

/**
 * @brief Main entry point of the program
 *
 * @return int Returns 0 on successful execution
 */
int main(void) {
    Entity UselessEntity = {EntityKind::Player, Vector2D{0, 0}};

    UselessEntity.position = Vector2D{42, 42};
    return (0);
};