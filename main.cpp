#include <SFML/Graphics.hpp>
#include "Car.h"
#include "math.h"
#include "Track.h"
#include "iostream"
#include "utils/CollisionUtils.h"
#include "vector"
#include "thread"
#include "neuralnetwork/Matrix.h"
#include "time.h"
#include "neuralnetwork/NeuralNetwork.h"
#include "Population.h"
#include "UI.h"

const int width = 2200;
const int height = 1080;



int main()
{

    srand(time(NULL));

    bool pause = false;

    sf::RenderWindow window(sf::VideoMode(width, height), "Self Driving Car");
    window.setFramerateLimit(60);

    Track track;
    Population population;
    UI ui;

    sf::Texture trackTexture;
    trackTexture.loadFromFile("../assets/track.jpg");
    sf::Sprite sprite;
    sprite.setTexture(trackTexture);


    // setup
    ui.setup();
    track.setup(&window);
    track.load("../assets/track" + std::to_string(track.id));

    population.setup(&window, &track);
//    population.loadFromFile("../assets/brain.bin");


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
                    if(event.key.code == sf::Keyboard::L) {
                        population.newGeneration(&track);
                    }
                    if(event.key.code == sf::Keyboard::P) {
                        pause = !pause;
                    }
                    if(event.key.code == sf::Keyboard::B) {
                        population.loadFromFile("../assets/brain.bin");
                        population.newGeneration(&track);
                    }
            }

            track.handleEvents(event);
        }

        // updating

        track.update();

        if(!pause) {
            population.update(&track);
        }

        // drawing
        window.clear();
        window.draw(sprite);

        window.draw(track);

        population.draw(window);

        window.draw(ui);

        window.display();
    }
}