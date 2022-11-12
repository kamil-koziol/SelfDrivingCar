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
    int segments = 8;
    int amountOfPaths = 2;
    int selectedPathIndex = 0;

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
    void handleCarCheckpoints(Car *car);
    void save(std::string name);
    void load(std::string name);
protected:
    sf::Vector2f* selectedPoint;
    bool isShowingGizmos;

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
    sf::Vector2f* searchForPoint(sf::Vector2<int> mousePosition);
};


#endif //SELFDRIVINGCAR_TRACK_H
