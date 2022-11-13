//
// Created by Kamil Kozioł on 12/11/2022.
//

#ifndef SELFDRIVINGCAR_POPULATION_H
#define SELFDRIVINGCAR_POPULATION_H


#include "Car.h"

class Population {
public:
    Track *track;
    sf::RenderWindow *window;

    static const int amountOfCars = 200;
    Car cars[amountOfCars];
    int bestFitnessCarBefore = 0;
    int leadingCarIndex = 0;
    int previousLeadingCarIndex = 0;
    int ticks = 0;


    void setup(sf::RenderWindow *window, Track *track);
    void update(Track *pTrack);
    void draw(sf::RenderTarget &target);
    void newGeneration(Track *track);
    void setLeadingCarIndex();
};


#endif //SELFDRIVINGCAR_POPULATION_H
