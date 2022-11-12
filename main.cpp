#include <SFML/Graphics.hpp>
#include "Car.h"
#include "math.h"
#include "Track.h"
#include "iostream"
#include "utils/CollisionUtils.h"
#include "vector"
#include "thread"
#include "Matrix.h"
#include "time.h"
#include "NeuralNetwork.h"
#include "Population.h"

const int width = 1500;
const int height = 1000;



int main()
{
    srand(time(NULL));

    sf::RenderWindow window(sf::VideoMode(width, height), "Self Driving Car");
    window.setFramerateLimit(60);

    Track track;
    Population population;
    Car car;
    car.isHumanSteering = true;

    // setup
    car.setup(&window);
    population.setup(&window);
    track.setup(&window);
    track.load("track");

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
        car.updateSensors(&track);
        car.brain->compute();

        population.update(&track);

        track.update();

        // drawing
        window.clear();

        window.draw(car);
        car.drawVisionLines();

        window.draw(track);
        population.draw(window);

        window.display();
    }
}