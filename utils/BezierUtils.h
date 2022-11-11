//
// Created by Kamil Kozioł on 10/11/2022.
//

#ifndef SELFDRIVINGCAR_BEZIERUTILS_H
#define SELFDRIVINGCAR_BEZIERUTILS_H

#include <SFML/Graphics.hpp>

sf::Vector2f lerp(sf::Vector2f p1, sf::Vector2f p2, float t);
sf::Vector2f bezier(sf::Vector2f p1, sf::Vector2f p2, sf::Vector2f p3, sf::Vector2f p4, float t);
void drawBezierCurve(sf::Vector2f p1, sf::Vector2f p2, sf::Vector2f p3, sf::Vector2f p4, int segments, sf::RenderTarget &target, sf::RenderStates states);
void addBezierCurvePoints(std::vector<sf::Vector2f> *points, sf::Vector2f p1, sf::Vector2f p2, sf::Vector2f p3, sf::Vector2f p4, int segments);




#endif //SELFDRIVINGCAR_BEZIERUTILS_H
