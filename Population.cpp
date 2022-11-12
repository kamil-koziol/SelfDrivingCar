//
// Created by Kamil Kozioł on 12/11/2022.
//

#include "Population.h"
#include "iostream"
#include "time.h"
#include "cmath"

void Population::setup(sf::RenderWindow *window) {

    for(int i=0; i<amountOfCars; i++) {
        cars[i].setup(window);
    }
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

    ticks++;

    if(ticks > 60*20) {
        newGeneration();
    }
}

void Population::draw(sf::RenderTarget &target) {
    for(int i=0;i<amountOfCars; i++) {
        if(i == leadingCarIndex) continue;
        cars[i].setFillColor(sf::Color(255, 255, 255, 100));
        target.draw(cars[i]);
    }

    cars[leadingCarIndex].setFillColor(sf::Color::Green);
    cars[leadingCarIndex].brain->draw(target, sf::RenderStates::Default);
    target.draw(cars[leadingCarIndex]);
}

void Population::newGeneration() {
    // calculating fitnesses

    double bestFitness = 1.0f;
    int bestCarIndex = 0;

    for(int i=0; i<amountOfCars; i++) {
        cars[i].calculateFitness();
        if(cars[i].fitness < bestFitness) {
            bestFitness = cars[i].fitness;
            bestCarIndex = i;
        }
    }

    leadingCarIndex = bestCarIndex;

    std::cout<<cars[bestCarIndex].fitness<<'\n';
    std::cout<<cars[bestCarIndex].totalCheckpointsReached<<'\n';

    // best car stays without mutation
    for(int i=0; i<amountOfCars; i++) {
        if(i == bestCarIndex) continue;
        cars[i].brain->copyFrom(cars[bestCarIndex].brain);
        cars[i].brain->mutate(0.03f);
    }


    for(int i=0; i<amountOfCars; i++) {
        cars[i].generationalReset();
    }

    ticks = 0;
}
