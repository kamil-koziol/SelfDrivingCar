//
// Created by Kamil Kozioł on 12/11/2022.
//

#ifndef SELFDRIVINGCAR_POPULATION_H
#define SELFDRIVINGCAR_POPULATION_H


#include "Car.h"
#include "string"


class Population {
public:
    Track *track;
    sf::RenderWindow *window;

    static const int amountOfCars = 350;
    Car cars[amountOfCars];
    int bestFitnessCarBefore = 0;
    int leadingCarIndex = 0;
    int previousLeadingCarIndex = 0;
    int ticks = 0;


    int currentGeneration = 0;
    int carsAlive = 0;
    float bestFitness = 0.0f;

    // UI
    sf::Font font;

    int amountOfElites = 10;

    void setup(sf::RenderWindow *window, Track *track);
    void update(Track *pTrack);
    void draw(sf::RenderTarget &target);
    void newGeneration(Track *track);
    void setLeadingCarIndex();

    int getRandomWeightedIndex(Car population[], double sumOfExpectedFitnesses);
    double getSumOfExpectedFitnesses(Car population[]);
    void loadFromFile(std::string path);
};


#endif //SELFDRIVINGCAR_POPULATION_H
