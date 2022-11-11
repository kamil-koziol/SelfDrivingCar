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

    static const int amountOfVisionLines = 8;
    int visionLinesDistance = 1000;
    sf::Vector2f visionLines[amountOfVisionLines];
    sf::Vector2f visionLinesOrigin;

    float width = 10;
    float height = 35;

    bool isRotating = false;
    float rotatingSpeed = 3;

    bool isMoving = false;
    float movementSpeed = 5;

    void setup(sf::RenderWindow *window);
    void tick();
    void handleEvents(sf::Event event);

    sf::Vector2f points[4];

    sf::Vector2f* intersectsWithLine(sf::Vector2f p0, sf::Vector2f p1);
    void updatePointPositions();
    void updateVisionLines();
    void drawVisionLines();
};


#endif //SELFDRIVINGCAR_CAR_H
