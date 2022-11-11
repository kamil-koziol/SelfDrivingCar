//
// Created by Kamil Kozioł on 09/11/2022.
//

#ifndef SELFDRIVINGCAR_TRACK_H
#define SELFDRIVINGCAR_TRACK_H

#include <SFML/Graphics.hpp>
#include "Car.h"

class Track: public sf::Drawable, public sf::Transformable {
public:
    sf::RenderWindow *window;

    std::vector<sf::Vector2f> points;
    std::vector<sf::Vector2f> collisionPoints;

    void setup(sf::RenderWindow *window);
    void drawPoints(sf::RenderTarget &target, sf::RenderStates states) const;
    void handleEvents(sf::Event event);
    void generateCollisionPoints();
    sf::Vector2f* lineIntersects(sf::Vector2f p0, sf::Vector2f p1);
    sf::Vector2f* closestLineIntersect(sf::Vector2f origin, sf::Vector2f p0, sf::Vector2f p1);
    sf::Vector2f* carIntersects(Car *car);
protected:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
};


#endif //SELFDRIVINGCAR_TRACK_H
