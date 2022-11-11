//
// Created by Kamil Kozioł on 10/11/2022.
//

#ifndef SELFDRIVINGCAR_COLLISIONUTILS_H
#define SELFDRIVINGCAR_COLLISIONUTILS_H
#include <SFML/Graphics.hpp>

sf::Vector2f* getLineIntersection(sf::Vector2f p0, sf::Vector2f p1,
                                  sf::Vector2f p2, sf::Vector2f p3);

#endif //SELFDRIVINGCAR_COLLISIONUTILS_H
