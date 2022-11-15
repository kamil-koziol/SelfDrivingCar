//
// Created by Kamil Kozioł on 15/11/2022.
//

#include "UI.h"

void UI::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    sf::Vector2f offset(50, 900);

    sf::Text text;
    text.setFillColor(sf::Color::White);
    text.setCharacterSize(32);
    text.setFont(font);

    text.setString("[SPACE] - New generation");
    text.setPosition(offset + sf::Vector2f(50, 0));
    target.draw(text);

    text.setString("[N] - Enter track edit mode");
    text.setPosition(offset + sf::Vector2f(50, 50));
    target.draw(text);

    text.setString("[Mouse Press] - Move point");
    text.setPosition(offset + sf::Vector2f(50, 100));
    target.draw(text);

    text.setString("[T] - Add new point");
    text.setPosition(offset + sf::Vector2f(500, 0));
    target.draw(text);

    text.setString("[D] - Delete previous point");
    text.setPosition(offset + sf::Vector2f(500, 50));
    target.draw(text);

    text.setString("[=] - Switch editing track");
    text.setPosition(offset + sf::Vector2f(500, 100));
    target.draw(text);

    text.setString("[1-9] Select save track");
    text.setPosition(offset + sf::Vector2f(1000, 0));
    target.draw(text);

    text.setString("[S] Save track");
    text.setPosition(offset + sf::Vector2f(1000, 50));
    target.draw(text);

    text.setString("[L] Load track");
    text.setPosition(offset + sf::Vector2f(1000, 100));
    target.draw(text);

    text.setString("[O] Set Origin Point on Mouse Position");
    text.setPosition(offset + sf::Vector2f(1500, 0));
    target.draw(text);

    text.setString("[P] Toggle Pause");
    text.setPosition(offset + sf::Vector2f(1500, 50));
    target.draw(text);

    text.setString("[B] Load Brain (brain.bin)");
    text.setPosition(offset + sf::Vector2f(1500, 100));
    target.draw(text);
}

void UI::setup() {
    font.loadFromFile("../assets/arial.ttf");
}
