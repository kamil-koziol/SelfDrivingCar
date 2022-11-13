//
// Created by Kamil Kozioł on 09/11/2022.
//

#ifndef SELFDRIVINGCAR_MATHUTILS_H
#define SELFDRIVINGCAR_MATHUTILS_H

#include "math.h"
#include <SFML/Graphics.hpp>

#define DEG2RAD (M_PI/180)
#define RAD2DEG (180/M_PI)

sf::Vector2f getOffsetedPointWithRotation(sf::Vector2f origin, float offsetX, float offsetY, float rotation);
float getDistanceFromPointToLine(sf::Vector2f point, sf::Vector2f p1, sf::Vector2f p2);
float relu(float value);
float normalize(float value);
float binarize(float value, float threshhold=0.5f);
float lerp1d(float value1, float value2, float t);

#endif //SELFDRIVINGCAR_MATHUTILS_H
