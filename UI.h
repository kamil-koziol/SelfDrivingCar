//
// Created by Kamil Kozioł on 15/11/2022.
//

#ifndef SELFDRIVINGCAR_UI_H
#define SELFDRIVINGCAR_UI_H

#include <SFML/Graphics.hpp>

class UI: public sf::Drawable {
public:
    void setup();
protected:
    sf::Font font;
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
};


#endif //SELFDRIVINGCAR_UI_H
