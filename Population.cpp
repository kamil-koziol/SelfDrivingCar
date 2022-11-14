//
// Created by Kamil Kozioł on 12/11/2022.
//

#include "Population.h"
#include "iostream"
#include "time.h"
#include "cmath"
#include <algorithm>

void Population::setup(sf::RenderWindow *window, Track *track) {
    this->window = window;
    this->track = track;

    for(int i=0; i<amountOfCars; i++) {
        cars[i].setup(window, track->startingPosition);
    }
}

void Population::update(Track *pTrack) {
    int carsUpdated = 0;
    for(int i=0; i<amountOfCars; i++) {
        if(cars[i].crashed) { continue;}
        carsUpdated++;

        cars[i].tick();
        cars[i].updateSensors(pTrack);
        cars[i].brain->feedForward();

        if(pTrack->carIntersects(&cars[i]) != nullptr) {
            // collision detected
            cars[i].crashed = true;
        }

        pTrack->handleCarCheckpoints(&cars[i]);
    }

    if(ticks % 10 == 0) {
        previousLeadingCarIndex = leadingCarIndex;
        setLeadingCarIndex();
    }

    if(ticks % 77 == 0 && ticks > 60*4) {
        if(previousLeadingCarIndex == leadingCarIndex) {
            if(cars[leadingCarIndex].crashed) {
                newGeneration(track);
            }
        }
    }



    ticks++;

    if(ticks > 60*30 || carsUpdated == 0) {
        newGeneration(pTrack);
    }
}

void Population::draw(sf::RenderTarget &target) {
    for(int i=0;i<amountOfCars; i++) {
        if(i == bestFitnessCarBefore || i == leadingCarIndex) continue;
        cars[i].setFillColor(sf::Color(255, 255, 255, 100));
        target.draw(cars[i]);
    }

    cars[bestFitnessCarBefore].setFillColor(sf::Color::Green);
    target.draw(cars[bestFitnessCarBefore]);

    cars[leadingCarIndex].setFillColor(sf::Color::Red);
    target.draw(cars[leadingCarIndex]);
    cars[leadingCarIndex].brain->draw(target, sf::RenderStates::Default);

}

void Population::newGeneration(Track *track) {
    // calculating fitnesses

    float sumOfFitnesses = 0;
    double bestFitness = 1.0f;
    int bestCarIndex = 0;

    Car newCars[amountOfCars];
    for(int i=0; i<amountOfCars; i++) {
        cars[i].calculateFitness(track);
        sumOfFitnesses += 1.0f/cars[i].fitness;
        if(cars[i].fitness < bestFitness) {
            bestFitness = cars[i].fitness;
            bestCarIndex = i;
        }

        newCars[i].setup(window, track->startingPosition);
        newCars[i].brain->copy(cars[i].brain);
        newCars[i].fitness = cars[i].fitness;
    }



    bestFitnessCarBefore = bestCarIndex;

//    Car previousBestCar;
//    previousBestCar.setup(window, track->startingPosition);
//    previousBestCar.brain->copy(cars[bestCarIndex].brain);

//    Car newCars[amountOfCars];
//    for(int i=0; i<amountOfCars; i++) {
//        newCars[i].setup(window, track->startingPosition);
//        double randFit = ((float)rand()/RAND_MAX) * sumOfFitnesses;
//
//        int randomWeightedIndex = 0;
//        while(randFit > 0) {
//            randFit -= 1.0f/cars[randomWeightedIndex].fitness;
//            if(randFit < 0) { break; }
//            randomWeightedIndex++;
//        }
//        newCars->brain->copy(cars[randomWeightedIndex].brain);
//        newCars->brain->mutate(0.1f);
//    }

//    for(int i=0; i<amountOfCars/2; i++) {
//        newCars[i].brain->copy(previousBestCar.brain);
//        newCars[i].brain->mutate(0.1f);
//    }
//
//    newCars[bestCarIndex].brain->copy(previousBestCar.brain);
//    newCars[bestCarIndex].fitness = previousBestCar.fitness;


    std::sort(newCars, newCars + amountOfCars,[](Car const & a, Car const & b) -> bool { return a.fitness < b.fitness; } );

    int amountOfBests = 10;
    for(int i=0; i<amountOfCars; i++) {
        cars[i] = Car();
        cars[i].setup(window, track->startingPosition);
        cars[i].brain->copy(newCars[i % amountOfBests].brain);
        cars[i].brain->mutate(0.1f);
    }


    for(int i=0; i<30; i++) {
        cars[i].brain->copy(newCars[0].brain);
        cars[i].brain->mutate(0.01f);
    }

    for(int i=0; i<amountOfBests; i++) {
        cars[i].brain->copy(newCars[i].brain);
    }

    cars[0].brain->save("brain.bin");

    ticks = 0;
}

void Population::setLeadingCarIndex() {
    int bestIndex = 0;
    int bestTotalCheckpointsReached = cars[0].totalCheckpointsReached;
    for(int i=0; i<amountOfCars; i++) {
        if(cars[i].totalCheckpointsReached > bestTotalCheckpointsReached) {
            bestIndex = i;
            bestTotalCheckpointsReached = cars[i].totalCheckpointsReached;
        }
    }

    leadingCarIndex = bestIndex;
}
