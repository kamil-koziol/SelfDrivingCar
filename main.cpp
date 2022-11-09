#include <SFML/Graphics.hpp>

const int width = 640;
const int height = 480;

int main()
{
    sf::RenderWindow window(sf::VideoMode(width, height), "SFML Application");
    sf::CircleShape shape;

    shape.setRadius(40.f);
    shape.setPosition(100.f, 100.f);
    shape.setFillColor(sf::Color::Cyan);

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
                    printf("%i", event.key.code);
                    break;
            }
        }

        window.clear();
        window.draw(shape);
        window.display();
    }

//    sf::Clock clock; // starts the clock
//    ...
//    sf::Time elapsed1 = clock.getElapsedTime();
//    std::cout << elapsed1.asSeconds() << std::endl;
//    clock.restart();
//    ...
//    sf::Time elapsed2 = clock.getElapsedTime();
//    std::cout << elapsed2.asSeconds() << std::endl;
}