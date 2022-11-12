//
// Created by Kamil Kozioł on 12/11/2022.
//

#ifndef SELFDRIVINGCAR_POPULATION_H
#define SELFDRIVINGCAR_POPULATION_H


#include "Car.h"

class Population {
public:
    static const int amountOfCars = 200;
    Car cars[amountOfCars];
    int leadingCarIndex;
    int ticks = 0;


    void setup(sf::RenderWindow *window);
    void update(Track *pTrack);
    void setLeadingCar();
    void draw(sf::RenderTarget &target);

    void newGeneration();
};


#endif //SELFDRIVINGCAR_POPULATION_H
