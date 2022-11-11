//
// Created by Kamil Kozioł on 09/11/2022.
//

#include "Track.h"
#include "utils/BezierUtils.h"
#include "utils/CollisionUtils.h"
#include "iostream"


void Track::draw(sf::RenderTarget &target, sf::RenderStates states) const {

    drawPoints(target, states);

    float step = 0.01f;

    if(points.size() < 4) {
        return;
    }

    sf::Vertex line[] = {
            bezier(points[0], points[1], points[2], points[3], 0),
            bezier(points[0], points[1], points[2], points[3], 0)
    };


    for(int i=0; i<points.size(); i+=3) {
        if(i + 4 > points.size()) break;
        drawBezierCurve(points[i], points[i+1], points[i+2], points[i+3], 100, target, states);
    }

    for(int i=0; i<collisionPoints.size()-1; i++) {
        line[0] = collisionPoints[i];
        line[1] = collisionPoints[i+1];
        target.draw(line, 2, sf::Lines);
    }
}

void Track::setup(sf::RenderWindow *window) {
    this->window = window;
    generateCollisionPoints();
}

void Track::drawPoints(sf::RenderTarget &target, sf::RenderStates states) const {

    sf::CircleShape shape(0.0f);
    sf::Vertex line[2];

    float smallRadius = 10;
    float bigRadius = 20;

    for(int i=0; i<points.size(); i++) {

        // connecting tangent dots
        if(i%3 == 0 || i%3 == 2) {
            if(i+1 < points.size()) {
                line[0] = points[i];
                line[1] = points[i + 1];
                target.draw(line, 2, sf::Lines);
            }
        }

        shape.setPosition(points[i]);

        if(i%3 == 0) {
            shape.setFillColor(sf::Color::Red);
            shape.setRadius(bigRadius);
            shape.move(-bigRadius, -bigRadius);
        }
        else  {
            shape.setFillColor(sf::Color::White);
            shape.setRadius(smallRadius);
            shape.move(-smallRadius, -smallRadius);
        }

        target.draw(shape);



    }
}

void Track::handleEvents(sf::Event event) {
    if(event.type == sf::Event::KeyPressed) {
        if(event.key.code == sf::Keyboard::T) {
            points.push_back(sf::Vector2f(sf::Mouse::getPosition(*window)));
            generateCollisionPoints();
        }
    }
}

void Track::generateCollisionPoints() {
    collisionPoints.clear();
    for(int i=0; i<points.size(); i+=3) {
        if(i + 4 > points.size()) break;
        addBezierCurvePoints(&collisionPoints, points[i], points[i+1], points[i+2], points[i+3], 8);
    }
}

sf::Vector2f *Track::lineIntersects(sf::Vector2f p0, sf::Vector2f p1) {
    if(collisionPoints.empty()) { return nullptr; }
    for(int i=0; i<collisionPoints.size()-1; i++) {
        sf::Vector2f *collisionPoint = getLineIntersection(p0, p1, collisionPoints[i], collisionPoints[i+1]);
        if(collisionPoint != nullptr) return collisionPoint;
    }

    return nullptr;
}

sf::Vector2f *Track::carIntersects(Car *car) {
    if(collisionPoints.empty()) { return nullptr; }
    for(int i=0; i<collisionPoints.size()-1; i++) {
        sf::Vector2f *collisionPoint = car->intersectsWithLine(collisionPoints[i], collisionPoints[i+1]);
        if(collisionPoint != nullptr) return collisionPoint;
    }

    return nullptr;
}

sf::Vector2f *Track::closestLineIntersect(sf::Vector2f origin, sf::Vector2f p0, sf::Vector2f p1) {
    if(collisionPoints.empty()) { return nullptr; }

    sf::Vector2f *closestCollisionPoint = nullptr;
    int recordDistance = INT32_MAX;

    for(int i=0; i<collisionPoints.size()-1; i++) {
        sf::Vector2f *collisionPoint = getLineIntersection(p0, p1, collisionPoints[i], collisionPoints[i+1]);
        if(collisionPoint == nullptr) continue;

        sf::Vector2f diff = *collisionPoint - origin;
        int distance = diff.x*diff.x + diff.y*diff.y;
        if(distance < recordDistance) {
            closestCollisionPoint = collisionPoint;
        }
    }

    return closestCollisionPoint;
}
