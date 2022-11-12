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

float normalize(float value) {
    if(value > 1.0f) {
        return 1.0f;
    } else {
        return value;
    }
}

float binarize(float value, float threshhold) {
    if(value > threshhold) {
        return 1.0f;
    } else {
        return 0.0f;
    }
}

float lerp1d(float value1, float value2, float t) {
    return value1 + (value2-value1)*t;
}
