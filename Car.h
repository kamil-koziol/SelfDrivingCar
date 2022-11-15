//
// Created by Kamil Kozioł on 09/11/2022.
//

#ifndef SELFDRIVINGCAR_CAR_H
#define SELFDRIVINGCAR_CAR_H

#include <SFML/Graphics.hpp>
#include "neuralnetwork/NeuralNetwork.h"
#include "Track.h"

#define TOP_LEFT 0
#define TOP_RIGHT 1
#define DOWN_RIGHT 2
#define DOWN_LEFT 3

#define AMOUNT_OF_DECISIONS 2
#define GO_RIGHT 0
#define GO_LEFT 1


class Track;

class Car: public sf::RectangleShape {
public:

    sf::RenderWindow *window;
    sf::Sprite sprite;


    static const int amountOfVisionLines = 7;
    int visionLinesDistance = 200;
    sf::Vector2f visionLines[amountOfVisionLines];
    sf::Vector2f visionLinesOrigin;

    NeuralNetwork *brain;
    float sensors[amountOfVisionLines];
    bool decisionValues[AMOUNT_OF_DECISIONS];
    double fitness = 1;

    bool crashed = false;
    bool isHumanSteering = false;
    int currentCheckpoint = 0;
    int totalCheckpointsReached = 0;

    float width = 10;
    float height = 35;

    sf::Vector2f startingPosition;

    bool isRotating = false;
    float rotatingSpeed = 3;

    bool isMoving = false;
    float velocity = 0;
    float acceleration = 0;
    float maxVelocity = 4.0f;
    float brakeAmount = 0.7f;
    float accelerationAmout = 0.5f;
    float friction = 0.2f;

    sf::Vector2f points[4];

    void setup(sf::RenderWindow *window, sf::Vector2f startingPosition);
    void tick();
    void handleEvents(sf::Event event);
    sf::Vector2f* intersectsWithLine(sf::Vector2f p0, sf::Vector2f p1);
    void updatePointPositions();
    void updateVisionLines();
    void drawVisionLines();
    void updateSensors(Track *track);
    void useBrain();
    void calculateFitness(Track *track);
    void generationalReset();

    double calculateExpectationFitness();
};


#endif //SELFDRIVINGCAR_CAR_H
