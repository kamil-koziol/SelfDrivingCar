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

const int width = 2200;
const int height = 1080;



int main()
{
    srand(time(NULL));

    sf::RenderWindow window(sf::VideoMode(width, height), "Self Driving Car");
    window.setFramerateLimit(60);

    Track track;
    Population population;
    Car car;
    car.isHumanSteering = true;

    sf::Texture trackTexture;
    trackTexture.loadFromFile("../assets/track.jpg");
//    sf::Image image = trackTexture.copyToImage();
//
//    int iHeight = image.getSize().y, iWidth = image.getSize().x;
//
//    for(int y=0; y<iHeight; y++) {
//        for(int x=0; x<iWidth; x++) {
//            sf::Color imagecolor = image.getPixel(x, y);
//            imagecolor.a -= 100;
//            image.setPixel(x, y, imagecolor);
//        }
//    }
//    trackTexture.loadFromImage(image);

    sf::Sprite sprite;
    sprite.setTexture(trackTexture);


    // setup
    track.setup(&window);
    track.load("../assets/track");

    car.setup(&window, track.startingPosition);

    population.setup(&window, &track);


    while (window.isOpen())
    {

        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::KeyPressed:
                    if(event.key.code == sf::Keyboard::Space) {
                        population.newGeneration(&track);
                    }
            }

            car.handleEvents(event);
            track.handleEvents(event);
        }

        // updating

        car.tick();
        car.updateSensors(&track);
        car.brain->compute();
//        track.handleCarCheckpoints(&car);


        population.update(&track);

        track.update();

        // drawing
        window.clear();
        window.draw(sprite);

        window.draw(car);
        car.drawVisionLines();

        window.draw(track);
        population.draw(window);

        window.display();
    }
}