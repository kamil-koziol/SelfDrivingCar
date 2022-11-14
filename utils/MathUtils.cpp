//
// Created by Kamil Kozioł on 09/11/2022.
//

#include "MathUtils.h"
#include "cmath"

sf::Vector2f getOffsetedPointWithRotation(sf::Vector2f origin, float offsetX, float offsetY, float rotation) {
    sf::Vector2f cp1 = origin;
    float cosr = cos(rotation);
    float sinr = sin(rotation);
    cp1.x += cosr * offsetX - sinr * offsetY;
    cp1.y += sinr * offsetX + cosr * offsetY;
    return cp1;
}

float getDistanceFromPointToLine(sf::Vector2f point, sf::Vector2f p1, sf::Vector2f p2) {
    // https://en.wikipedia.org/wiki/Distance_from_a_point_to_a_line
    return abs((p2.x - p1.x)*(p1.y - point.y) - (p1.x - point.x)*(p2.y - p1.y))/(sqrt(pow(p2.x-p1.x,2) + pow(p2.y - p1.y,2)));
}
