//
// Created by Kamil Kozioł on 09/11/2022.
//

#include "Car.h"
#include "math.h"
#include "utils/MathUtils.h"
#include "utils/CollisionUtils.h"

void Car::setup(sf::RenderWindow *window) {
    this->window = window;

    setSize(sf::Vector2f(width, height));
    setPosition(200, 200);
    setOrigin(width/2,height/2);
}

void Car::tick() {
    if(isMoving) {
        move(movementSpeed * cos(getRotation() * DEG2RAD + M_PI / 2), movementSpeed * sin(getRotation() * DEG2RAD + M_PI / 2));
    }

    if(isRotating) {
        rotate(rotatingSpeed);
    }

    updatePointPositions();
}


void Car::handleEvents(sf::Event event) {
    if(event.type == sf::Event::KeyPressed) {
        switch (event.key.code) {
            case sf::Keyboard::Left:
                isRotating = true;
                rotatingSpeed = -abs(rotatingSpeed);
                break;
            case sf::Keyboard::Right:
                isRotating = true;
                rotatingSpeed = abs(rotatingSpeed);
                break;

            case sf::Keyboard::Up:
                isMoving = true;
                movementSpeed = -abs(movementSpeed);
                break;
            case sf::Keyboard::Down:
                isMoving = true;
                movementSpeed = abs(movementSpeed);
                break;
        }
    } else if(event.type == sf::Event::KeyReleased) {
        switch (event.key.code) {
            case sf::Keyboard::Left:
            case sf::Keyboard::Right:
                isRotating = false;
                break;

            case sf::Keyboard::Down:
            case sf::Keyboard::Up:
                isMoving = false;
                break;
        }
    }
}

sf::Vector2f *Car::intersectsWithLine(sf::Vector2f p0, sf::Vector2f p1) {
    sf::Vector2f *intersectionPoint;

    for(int i=0; i<4; i++) {
        intersectionPoint = getLineIntersection(points[i], points[(i+1)%4], p0, p1);
        if(intersectionPoint != nullptr) return intersectionPoint;
    }

    return nullptr;
}

sf::Vector2f Car::getOffsetedPointWithRotation(float offsetX, float offsetY) {
    sf::Vector2f cp1 = getPosition();
    float angle = getRotation() * DEG2RAD;
    cp1.x += cos(angle) * offsetX - sin(angle) * offsetY;
    cp1.y += sin(angle) * offsetX + cos(angle) * offsetY;
    return cp1;
}

void Car::updatePointPositions() {
    points[TOP_LEFT] = getOffsetedPointWithRotation(-width/2, -height/2);
    points[TOP_RIGHT] = getOffsetedPointWithRotation(width/2, -height/2);
    points[DOWN_LEFT] = getOffsetedPointWithRotation(-width/2, height/2);
    points[DOWN_RIGHT] = getOffsetedPointWithRotation(width/2, height/2);
}
