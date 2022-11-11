//
// Created by Kamil Kozioł on 09/11/2022.
//

#ifndef SELFDRIVINGCAR_CAR_H
#define SELFDRIVINGCAR_CAR_H

#include <SFML/Graphics.hpp>
#define TOP_LEFT 0
#define TOP_RIGHT 1
#define DOWN_RIGHT 2
#define DOWN_LEFT 3

class Car: public sf::RectangleShape {
public:

    sf::RenderWindow *window;

    float width = 100; // 10
    float height = 200; // 35

    bool isRotating = false;
    float rotatingSpeed = 3;

    bool isMoving = false;
    float movementSpeed = 5;

    void setup(sf::RenderWindow *window);
    void tick();
    void handleEvents(sf::Event event);

    sf::Vector2f points[4];

    sf::Vector2f* intersectsWithLine(sf::Vector2f p0, sf::Vector2f p1);
    sf::Vector2f getOffsetedPointWithRotation(float offsetX, float offsetY);
    void updatePointPositions();
};


#endif //SELFDRIVINGCAR_CAR_H
