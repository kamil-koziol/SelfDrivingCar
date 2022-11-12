//
// Created by Kamil Kozioł on 12/11/2022.
//

#ifndef SELFDRIVINGCAR_POPULATION_H
#define SELFDRIVINGCAR_POPULATION_H


#include "Car.h"

class Population {
public:
    static const int amountOfCars = 500;
    Car cars[amountOfCars];
    int leadingCarIndex = 0;
    int ticks = 0;


    void setup(sf::RenderWindow *window);
    void update(Track *pTrack);
    void draw(sf::RenderTarget &target);

    void newGeneration();
};


#endif //SELFDRIVINGCAR_POPULATION_H
