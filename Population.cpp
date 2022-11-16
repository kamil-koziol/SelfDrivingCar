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

    font.loadFromFile("../assets/arial.ttf");
}

void Population::update(Track *pTrack) {
    carsAlive = 0;
    for(int i=0; i<amountOfCars; i++) {
        if(cars[i].crashed) { continue;}
        carsAlive++;

        cars[i].tick();
        if(ticks % 3 == 0) {
            cars[i].updateSensors(pTrack);
        }
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

    if(ticks > 60*30 || carsAlive == 0) {
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

    // texts
    sf::Text text;
    text.setFillColor(sf::Color::White);
    text.setCharacterSize(48);
    text.setFont(font);
    text.setString("GENERATION: " + std::to_string(currentGeneration));
    text.setPosition(1500, 700);
    window->draw(text);

    text.setPosition(1500, 750);
    text.setString("FITNESS: " + std::to_string(bestFitness));
    window->draw(text);

}

void Population::newGeneration(Track *track) {
    // calculating fitnesses

    double _bestFitness = 0.0f;
    int bestCarIndex = 0;

    Car newCars[amountOfCars];
    for(int i=0; i<amountOfCars; i++) {
        cars[i].calculateFitness(track);
        if(cars[i].fitness > _bestFitness) {
            _bestFitness = cars[i].fitness;
            bestCarIndex = i;
        }

        newCars[i].setup(window, track->startingPosition);
        newCars[i].brain->copy(cars[i].brain);
        newCars[i].fitness = cars[i].fitness;
    }

    bestFitness = cars[bestCarIndex].fitness;

    std::sort(newCars, newCars + amountOfCars,[](Car const & a, Car const & b) -> bool { return a.fitness > b.fitness; } );

    double sumOfExpectationFitnesses = getSumOfExpectedFitnesses(cars);
    // elites stay 0 -> amountOfElites
    for(int i=amountOfElites; i<amountOfCars; i++) {
        if(i%2 == 0) {
        // crossovers
            int parent1 = getRandomWeightedIndex(cars, sumOfExpectationFitnesses);
            int parent2;
            do {
                parent2 = getRandomWeightedIndex(cars, sumOfExpectationFitnesses);
            } while(parent1 != parent2);

            newCars[i].brain->copy(cars[parent1].brain);
            newCars[i].brain->crossover(cars[parent2].brain);
        }  else {
        // mutations
           int parentIndex = getRandomWeightedIndex(cars, sumOfExpectationFitnesses);
           newCars[i].brain->copy(cars[parentIndex].brain);
           newCars[i].brain->mutate(0.2f);
        }
    }

    for(int i=0; i<amountOfCars; i++) {
        cars[i] = Car();
        cars[i].setup(window, track->startingPosition);
        cars[i].brain->copy(newCars[i].brain);
    }

    currentGeneration++;

    cars[0].brain->save("generations/brain" + std::to_string(currentGeneration) + ".bin");
    bestFitnessCarBefore = 0;

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

double Population::getSumOfExpectedFitnesses(Car *population) {
    double sumOfExpectationFitnesses = 0;
    for(int i=0; i<amountOfCars; i++) {
        sumOfExpectationFitnesses += population[i].calculateExpectationFitness();
    }
    return sumOfExpectationFitnesses;
}

int Population::getRandomWeightedIndex(Car population[], double sumOfExpectationFitnesses) {

    double randomExceptationFitness = ((float)rand()/RAND_MAX) * sumOfExpectationFitnesses;

    int randomWeightedIndex = 0;
    while(randomExceptationFitness > 0) {
        randomExceptationFitness -= cars[randomWeightedIndex].calculateExpectationFitness();
        if(randomExceptationFitness < 0) break;
        randomWeightedIndex++;
    }

    return randomWeightedIndex;
}

void Population::loadFromFile(std::string path) {

    NeuralNetwork *nn = cars[0].brain;
    nn->load(path);

    for(int i=0; i<amountOfCars; i++) {
        cars[i].brain->copy(nn);
        if(i != 0) {
            cars[i].brain->mutate(0.01f);
        }
    }
}
