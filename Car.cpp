//
// Created by Kamil Kozioł on 09/11/2022.
//

#include "Car.h"
#include "math.h"
#include "utils/MathUtils.h"
#include "utils/CollisionUtils.h"
#include "iostream"

void Car::setup(sf::RenderWindow *window, sf::Vector2f startingPosition) {
    this->window = window;
    this->startingPosition = startingPosition;

    setSize(sf::Vector2f(width, height));
    setPosition(startingPosition);
    setOrigin(width/2,height/2);

    updateVisionLines();

    int sizes[] = { 5 };
    brain = new NeuralNetwork(amountOfVisionLines + 1, 1, AMOUNT_OF_DECISIONS, sizes);
    brain->randomize();
}

void Car::tick() {
    if(crashed) return;


    // movement handling
    velocity -= friction;
    velocity += acceleration;
    velocity = std::max(velocity, 0.0f);

    if(velocity > maxVelocity) {
        velocity = maxVelocity;
    }

    move(-velocity * cos(getRotation() * DEG2RAD + M_PI / 2), -velocity * sin(getRotation() * DEG2RAD + M_PI / 2));


    // rotation handling
    if(isRotating) {
        rotate(rotatingSpeed);
    }

    updatePointPositions();
    updateVisionLines();

    // input sensors
    for(int i=0; i<amountOfVisionLines; i++) {
        brain->layers[brain->inputLayerIndex]->neurons->set(i, sensors[i]);
    }

    brain->layers[brain->inputLayerIndex]->neurons->set(amountOfVisionLines, velocity/maxVelocity);


    if(!isHumanSteering) {
        useBrain();
    }
}


void Car::handleEvents(sf::Event event) {
    if(!isHumanSteering) return;
    if(event.type == sf::Event::KeyPressed) {
        switch (event.key.code) {
            case sf::Keyboard::Left:
                isRotating = true;
                rotatingSpeed = -abs(rotatingSpeed);
                break;
            case sf::Keyboard::Right:
                isRotating = true;
                rotatingSpeed = abs(rotatingSpeed);
                break;

            case sf::Keyboard::Up:
                acceleration = accelerationAmout;
                break;
            case sf::Keyboard::Down:
                acceleration = -brakeAmount;
                break;
        }
    } else if(event.type == sf::Event::KeyReleased) {
        switch (event.key.code) {
            case sf::Keyboard::Left:
            case sf::Keyboard::Right:
                isRotating = false;
                break;

            case sf::Keyboard::Down:
            case sf::Keyboard::Up:
                isMoving = false;
                acceleration = 0;
                break;
        }
    }
}

sf::Vector2f *Car::intersectsWithLine(sf::Vector2f p0, sf::Vector2f p1) {
    sf::Vector2f *intersectionPoint;

    for(int i=0; i<4; i++) {
        intersectionPoint = getLineIntersection(points[i], points[(i+1)%4], p0, p1);
        if(intersectionPoint != nullptr) return intersectionPoint;
    }

    return nullptr;
}

void Car::updatePointPositions() {
    float angle = getRotation() * DEG2RAD;
    points[TOP_LEFT] = getOffsetedPointWithRotation(getPosition(), -width/2, -height/2, angle);
    points[TOP_RIGHT] = getOffsetedPointWithRotation(getPosition(), width/2, -height/2, angle);
    points[DOWN_LEFT] = getOffsetedPointWithRotation(getPosition(), -width/2, height/2, angle);
    points[DOWN_RIGHT] = getOffsetedPointWithRotation(getPosition(), width/2, height/2, angle);
}

void Car::updateVisionLines() {

    float angleStep = M_PI/(amountOfVisionLines - 1);
    float angle = -M_PI + getRotation()*DEG2RAD;
    visionLinesOrigin = getOffsetedPointWithRotation(getPosition(), 0, -height/2, getRotation() * DEG2RAD);

    for(int i=0; i<amountOfVisionLines; i++) {
        sf::Vector2f newPoint = visionLinesOrigin;
        newPoint.x += cos(angle) * visionLinesDistance;
        newPoint.y += sin(angle) * visionLinesDistance;
        visionLines[i] = newPoint;
        angle += angleStep;
    }
}

void Car::drawVisionLines() {
    sf::Vertex line[2];
    line[0] = visionLinesOrigin;

    for(int i=0; i<amountOfVisionLines; i++) {
        line[1] = visionLines[i];
        window->draw(line, 2, sf::Lines);
    }
}

void Car::updateSensors(Track *track) {
    for (int i = 0; i < amountOfVisionLines; i++) {
        sf::Vector2f *colPoint = track->closestLineIntersect(visionLinesOrigin, visionLinesOrigin,
                                                            visionLines[i]);
        if (colPoint != nullptr) {

            sf::Vector2f diff = visionLinesOrigin - *colPoint;

            float distance = diff.x * diff.x + diff.y * diff.y;

            sensors[i] = (distance/(visionLinesDistance*visionLinesDistance*1.0f));
        } else {
            sensors[i] = 0.0f;
        }
    }

}

void Car::useBrain() {
    Layer* outputLayer = brain->layers[brain->outputLayerIndex];
    for(int i=0; i<outputLayer->size; i++) {
        decisionValues[i] = outputLayer->neurons->get(i) == 1.0f;
    }

    if(decisionValues[GO_LEFT]) {
        isRotating = true;
        rotatingSpeed = -abs(rotatingSpeed);
    } else if(decisionValues[GO_RIGHT]) {
        isRotating = true;
        rotatingSpeed = abs(rotatingSpeed);
    } else {
        isRotating = false;
    }

    if(decisionValues[ACCELERATE]) {
        acceleration = accelerationAmout;
    } else if(decisionValues[BRAKE]) {
        acceleration = -brakeAmount;
    }


}

void Car::calculateFitness(Track *track) {
    float distanceToNextCheckpoint = track->distanceOfCarToNextCheckpoint(this); // cannot be 0
    fitness = 1.0/((double) (totalCheckpointsReached+1 + (1.0/(distanceToNextCheckpoint+1))));
//    if(crashed) {
//        fitness = std::min(1.0, fitness + 0.1);
//    }
}

void Car::generationalReset() {
    setPosition(startingPosition);

    fitness = 1;
    totalCheckpointsReached = 0;
    currentCheckpoint = 0;

    crashed = false;
    isMoving = false;
    isRotating = false;

    velocity = 0;
    acceleration = 0;

    setRotation(0);
    updatePointPositions();
    updateVisionLines();
}
