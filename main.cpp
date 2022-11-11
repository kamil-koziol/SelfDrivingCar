#include <SFML/Graphics.hpp>
#include "Car.h"
#include "math.h"
#include "Track.h"
#include "iostream"
#include "utils/CollisionUtils.h"

const int width = 1000;

const int height = 1000;


int main()
{
    sf::RenderWindow window(sf::VideoMode(width, height), "SFML Application");
    window.setFramerateLimit(60);

    Car car;
    Track track;


    // setup

    car.setup(&window);
    track.setup(&window);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;
            }

            car.handleEvents(event);
            track.handleEvents(event);
        }

        // updating

        car.tick();
        track.update();

        // drawing
        window.clear();

        window.draw(car);
        window.draw(track);

        sf::Vertex line1[] = {
                sf::Vertex(sf::Vector2f(100, 100)),
                sf::Vertex(sf::Vector2f(500, 500))
        };

        sf::Vertex line2[] = {
                sf::Vertex(sf::Vector2f(300, 100)),
                sf::Vertex(sf::Vector2f(sf::Mouse::getPosition(window)))
        };

        window.draw(line1, 2, sf::Lines);
        window.draw(line2, 2, sf::Lines);

        sf::Vector2f *intersectionPoint = getLineIntersection(
                line1[0].position,
                line1[1].position,
                line2[0].position,
                line2[1].position
        );

        sf::CircleShape shape(10.0f);
        if(intersectionPoint != nullptr) {
            shape.setPosition(intersectionPoint->x - 10, intersectionPoint->y - 10);
            window.draw(shape);
        }

        float r = 5;

        shape.setRadius(r);
        shape.setFillColor(sf::Color::Red);
        for(int i=0; i<car.getPointCount(); i++) {
            shape.setPosition(car.points[i]);
            shape.move(-r, -r);
            window.draw(shape);
        }

        sf::Vector2f *ip = car.intersectsWithLine(line2[0].position, line2[1].position);
        if(ip != nullptr) {
            shape.setPosition(*ip);
            shape.move(-r, -r);
            window.draw(shape);
        }

        sf::Vector2f *isp = track.carIntersects(&car);
        if(isp != nullptr) {
            shape.setFillColor(sf::Color::Green);
            shape.setRadius(15.0f);
            shape.setPosition(*isp);
            shape.move(-15.0f, -15.0f);
            window.draw(shape);
        }

        window.display();
    }
}