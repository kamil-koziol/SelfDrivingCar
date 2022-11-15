//
// Created by Kamil Kozioł on 09/11/2022.
//

#ifndef SELFDRIVINGCAR_TRACK_H
#define SELFDRIVINGCAR_TRACK_H

#include <SFML/Graphics.hpp>
#include "Car.h"
#include "string"
#include "vector"

class Car;

class Track: public sf::Drawable, public sf::Transformable {
public:
    sf::RenderWindow *window;
    int id = 1;

    int segments = 8;

    int selectedPathIndex = 0;

    // saved attributes

    int startingCheckpoint = 0;
    sf::Vector2f startingPosition;
    int startingAngle = 0;

    // paths
    int amountOfPaths = 2;
    std::vector<std::vector<sf::Vector2f>> paths;
    std::vector<std::vector<sf::Vector2f>> pathCollisionPoints;
    std::vector<sf::Vector2f> checkpoints;

    void setup(sf::RenderWindow *window);
    void update();
    void drawPoints(sf::RenderTarget &target, sf::RenderStates states) const;
    void handleEvents(sf::Event event);
    void generateCollisionPoints();

    sf::Vector2f* lineIntersects(sf::Vector2f p0, sf::Vector2f p1);
    sf::Vector2f* closestLineIntersect(sf::Vector2f origin, sf::Vector2f p0, sf::Vector2f p1) const;
    sf::Vector2f* carIntersects(Car *car);
    void setOriginAndRotation(sf::Vector2f origin, float rotation);
    void handleCarCheckpoints(Car *car);
    float distanceOfCarToNextCheckpoint(Car *car);
    void save(std::string name);
    void load(std::string name);
protected:
    sf::Vector2f* selectedPoint;
    bool inEditMode;

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
    sf::Vector2f* searchForPoint(sf::Vector2<int> mousePosition);
};


#endif //SELFDRIVINGCAR_TRACK_H
