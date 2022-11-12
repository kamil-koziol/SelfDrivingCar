//
// Created by Kamil Kozioł on 12/11/2022.
//

#include "Population.h"

void Population::setup(sf::RenderWindow *window) {

    for(int i=0; i<amountOfCars; i++) {
        cars[i].setup(window);
    }

    setLeadingCar();
}

void Population::update(Track *pTrack) {
    for(int i=0; i<amountOfCars; i++) {
        if(cars[i].crashed) { continue;}
        cars[i].tick();
        cars[i].updateSensors(pTrack);
        cars[i].brain->compute();

        if(pTrack->carIntersects(&cars[i]) != nullptr) {
            // collision detected
            cars[i].crashed = true;
        }

        pTrack->handleCarCheckpoints(&cars[i]);
    }

    setLeadingCar();
    ticks++;

    if(ticks > 1200) {
        newGeneration();
    }
}

void Population::draw(sf::RenderTarget &target) {
    for(int i=0;i<amountOfCars; i++) {
        if(i == leadingCarIndex) continue;
        cars[i].setFillColor(sf::Color::White);
        target.draw(cars[i]);
    }

    cars[leadingCarIndex].setFillColor(sf::Color::Green);
    cars[leadingCarIndex].brain->draw(target, sf::RenderStates::Default);
    target.draw(cars[leadingCarIndex]);
}

void Population::setLeadingCar() {
    int bestCheckpoints = cars[0].totalCheckpointsReached;
    leadingCarIndex = 0;

    for(int i=0; i<amountOfCars; i++) {
        if(cars[i].totalCheckpointsReached > bestCheckpoints) {
            bestCheckpoints = cars[i].totalCheckpointsReached;
            leadingCarIndex = i;
        }
    }
}

void Population::newGeneration() {
    // calculating fitnesses
    for(int i=0; i<amountOfCars; i++) {
        cars[i].calculateFitness();
    }
}
