//
// Created by Kamil Kozioł on 09/11/2022.
//

#include "Car.h"
#include "math.h"
#include "utils/MathUtils.h"
#include "utils/CollisionUtils.h"
#include "iostream"

void Car::setup(sf::RenderWindow *window) {
    this->window = window;

    setSize(sf::Vector2f(width, height));
    setPosition(200, 200);
    setOrigin(width/2,height/2);

    updateVisionLines();

    int sizes[] = { 8 };
    brain = new NeuralNetwork(amountOfVisionLines, 1, AMOUNT_OF_DECISIONS, sizes);
    brain->randomize();
}

void Car::tick() {
    if(crashed) return;

    if(isMoving) {
        move(movementSpeed * cos(getRotation() * DEG2RAD + M_PI / 2), movementSpeed * sin(getRotation() * DEG2RAD + M_PI / 2));
    }

    if(isRotating) {
        rotate(rotatingSpeed);
    }

    updatePointPositions();
    updateVisionLines();

    // input sensors
    for(int i=0; i<amountOfVisionLines; i++) {
        brain->layers[brain->inputLayerIndex]->neurons->set(i, sensors[i]);
    }

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
                isMoving = true;
                movementSpeed = -abs(movementSpeed);
                break;
            case sf::Keyboard::Down:
                isMoving = true;
                movementSpeed = abs(movementSpeed);
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
            sensors[i] = 1-(distance/float(visionLinesDistance*visionLinesDistance));
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

    isMoving = true;

    if(decisionValues[GO_LEFT]) {
        isRotating = true;
        rotatingSpeed = -abs(rotatingSpeed);
    } else if(decisionValues[GO_RIGHT]) {
        isRotating = true;
        rotatingSpeed = abs(rotatingSpeed);
    } else {
        isRotating = false;
    }
}

void Car::toggleHumanSteering() {
    isHumanSteering = !isHumanSteering;
}

void Car::calculateFitness() {
    fitness = 1.0/(double) (totalCheckpointsReached+1);
    if(crashed) {
        fitness = std::min(1.0, fitness + 0.1);
    }
}

void Car::generationalReset() {
    setPosition(200, 200);
    fitness = 1;
    totalCheckpointsReached = 0;
    currentCheckpoint = 0;
    crashed = false;
    setRotation(0);
    updatePointPositions();
    updateVisionLines();
}
