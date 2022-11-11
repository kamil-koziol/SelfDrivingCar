//
// Created by Kamil Kozioł on 10/11/2022.
//

#include "BezierUtils.h"

sf::Vector2f lerp(sf::Vector2f p1, sf::Vector2f p2, float t) {
    return p1 + (p2 - p1) * t;
}

sf::Vector2f bezier(sf::Vector2f p0, sf::Vector2f p1, sf::Vector2f p2, sf::Vector2f p3, float t) {
    sf::Vector2f l0 = lerp(p0, p1, t);
    sf::Vector2f l1 = lerp(p1, p2, t);
    sf::Vector2f l2 = lerp(p2, p3, t);

    sf::Vector2f q0 = lerp(l0, l1, t);
    sf::Vector2f q1 = lerp(l1, l2, t);

    sf::Vector2f c0 = lerp(q0, q1, t);

    return c0;
}

void drawBezierCurve(sf::Vector2f p1, sf::Vector2f p2, sf::Vector2f p3, sf::Vector2f p4, int segments,
                     sf::RenderTarget &target, sf::RenderStates states) {

    float step = 1.0f/((float) segments);
    float t = 0;
    sf::Vertex line[] = {
            bezier(p1, p2, p3, p4, 0),
            bezier(p1, p2, p3, p4, 0)
    };

    for(int i=0; i<=segments; i++) {
        sf::Vector2f point = bezier(p1, p2, p3, p4, t);
        sf::Vertex vertex = sf::Vertex(point);
        vertex.color = sf::Color::Red;
        line[0] = line[1];
        line[1] = vertex;
        target.draw(line, 2, sf::Lines);

        t += step;
    }
}

void addBezierCurvePoints(std::vector<sf::Vector2f> *points, sf::Vector2f p1, sf::Vector2f p2, sf::Vector2f p3,
                          sf::Vector2f p4, int segments) {
    float step = 1.0f/((float) segments);
    float t = 0;
    for(int i=0; i<=segments; i++) {
        sf::Vector2f point = bezier(p1, p2, p3, p4, t);
        points->push_back(point);
        t += step;
    }
}
