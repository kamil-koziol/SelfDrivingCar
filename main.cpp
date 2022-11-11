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

        // drawing
        window.clear();

        window.draw(car);
        car.drawVisionLines();

        window.draw(track);

        sf::CircleShape shape(10.0f);

        sf::Vector2f *isp = track.carIntersects(&car);
        if(isp != nullptr) {
            shape.setFillColor(sf::Color::Green);
            shape.setRadius(15.0f);
            shape.setPosition(*isp);
            shape.move(-15.0f, -15.0f);
            window.draw(shape);
        }

        for(int i=0; i<car.amountOfVisionLines; i++) {
            sf::Vector2f *colPoint = track.closestLineIntersect(car.visionLinesOrigin ,car.visionLinesOrigin, car.visionLines[i]);
            if(colPoint != nullptr) {
                shape.setFillColor(sf::Color::Green);
                shape.setRadius(15.0f);
                shape.setPosition(*colPoint);
                shape.move(-15.0f, -15.0f);
                window.draw(shape);
            }
        }


        window.display();
    }
}