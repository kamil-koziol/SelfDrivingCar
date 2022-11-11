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

float relu(float value) {
    return std::max(0.0f, value);
}
