//
// Created by Kamil Kozioł on 10/11/2022.
//

#include "CollisionUtils.h"

sf::Vector2f *getLineIntersection(sf::Vector2f p0, sf::Vector2f p1, sf::Vector2f p2, sf::Vector2f p3) {
    sf::Vector2f s1, s2;
    s1.x = p1.x - p0.x;     s1.y = p1.y - p0.y;
    s2.x = p3.x - p2.x;     s2.y = p3.y - p2.y;

    float s, t;
    s = (-s1.y * (p0.x - p2.x) + s1.x * (p0.y - p2.y)) / (-s2.x * s1.y + s1.x * s2.y);
    t = ( s2.x * (p0.y - p2.y) - s2.y * (p0.x - p2.x)) / (-s2.x * s1.y + s1.x * s2.y);

    if (s >= 0 && s <= 1 && t >= 0 && t <= 1)
    {
        float iax = p0.x + (t * s1.x);
        float iay = p0.y + (t * s1.y);
        return new sf::Vector2f(iax, iay);
    }

    return nullptr;
}
