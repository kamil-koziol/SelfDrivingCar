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
float relu(float value);
float normalize(float value);

#endif //SELFDRIVINGCAR_MATHUTILS_H
