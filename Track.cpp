//
// Created by Kamil Kozioł on 09/11/2022.
//

#include "Track.h"
#include "utils/BezierUtils.h"
#include "utils/CollisionUtils.h"
#include "iostream"
#include "fstream"
#include "vector"
#include "utils/MathUtils.h"

#define POINT_RADIUS 10

void Track::draw(sf::RenderTarget &target, sf::RenderStates states) const {

    // drawing dots
    if(this->inEditMode) {
        drawPoints(target, states);
    }

    // drawingBezierCurves

    for(auto & points : paths) {
        if (points.size() < 4) {
            continue;
        }

        for (int i = 0; i < points.size(); i += 3) {
            if (i + 4 > points.size()) break;
            drawBezierCurve(points[i], points[i + 1], points[i + 2], points[i + 3], 100, target, states);
        }
    }


    // drawing collisionPoints
    if (this->inEditMode) {
        for(auto & collisionPoints : pathCollisionPoints) {
            sf::Vertex line[2];
            if(collisionPoints.empty()) { continue; }
            for (int i = 0; i < collisionPoints.size() - 1; i++) {
                line[0] = collisionPoints[i];
                line[1] = collisionPoints[i + 1];
                target.draw(line, 2, sf::Lines);
            }
        }
    }

    // draw checkpoints
    if(inEditMode) {
        sf::Vertex line[2];
        for (int i = 0; i < checkpoints.size(); i += 2) {
            line[0] = checkpoints[i];
            line[0].color = sf::Color::Green;
            line[1] = checkpoints[i + 1];
            line[1].color = sf::Color::Green;

            if(i == startingCheckpoint * 2) {
                line[0].color = sf::Color::Yellow;
                line[1].color = sf::Color::Yellow;
            }

            target.draw(line, 2, sf::LineStrip);
        }
    }
}

void Track::setup(sf::RenderWindow *window) {
    this->window = window;
    this->selectedPoint = nullptr;


    // creating paths
    for(int i=0; i<amountOfPaths; i++) {
        paths.push_back(std::vector<sf::Vector2f>());
        pathCollisionPoints.push_back(std::vector<sf::Vector2f>());
    }

    generateCollisionPoints();
}

void Track::drawPoints(sf::RenderTarget &target, sf::RenderStates states) const {

    sf::CircleShape shape(0.0f);
    sf::Vertex line[2];

    float smallRadius = POINT_RADIUS;
    float bigRadius = 2 * POINT_RADIUS;

    for(auto & points : paths) {
        for (int i = 0; i < points.size(); i++) {

            // connecting tangent dots
            if (i % 3 == 0 || i % 3 == 2) {
                if (i + 1 < points.size()) {
                    line[0] = points[i];
                    line[1] = points[i + 1];
                    target.draw(line, 2, sf::Lines);
                }
            }

            shape.setPosition(points[i]);

            if (i % 3 == 0) {
                shape.setFillColor(sf::Color::Red);
                shape.setRadius(bigRadius);
                shape.move(-bigRadius, -bigRadius);
            } else {
                shape.setFillColor(sf::Color::White);
                shape.setRadius(smallRadius);
                shape.move(-smallRadius, -smallRadius);
            }

            target.draw(shape);
        }
    }
}

void Track::handleEvents(sf::Event event) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::N) {
            this->inEditMode = !this->inEditMode;
        }

        if(inEditMode) {
            if (event.key.code == sf::Keyboard::T) {
                paths[selectedPathIndex].push_back(sf::Vector2f(sf::Mouse::getPosition(*window)));
                generateCollisionPoints();
            }

            else if (event.key.code == sf::Keyboard::D) {

                paths[selectedPathIndex].pop_back();
                generateCollisionPoints();
            }

            else if (event.key.code == sf::Keyboard::Equal) {
                selectedPathIndex = (selectedPathIndex + 1) % amountOfPaths;
            }

            else if (event.key.code == sf::Keyboard::C) {
                checkpoints.push_back(sf::Vector2f(sf::Mouse::getPosition(*window)));
            }
            else if(event.key.code == sf::Keyboard::O) {
                sf::Vector2f mouse = sf::Vector2f(sf::Mouse::getPosition(*window));
                setOriginAndRotation(mouse, 0);
            }




        }


        if(event.type == sf::Event::KeyPressed) {
            if(event.key.code >= sf::Keyboard::Num1 && event.key.code <= sf::Keyboard::Num9) {
                id = event.key.code - sf::Keyboard::Num1 + 1;
            }
        }

        if (event.key.code == sf::Keyboard::S) {
            save("../assets/track" + std::to_string(id));
        }

        else if (event.key.code == sf::Keyboard::L) {
            load("../assets/track" + std::to_string(id));
        }

    }

    if(inEditMode) {
        if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                selectedPoint = searchForPoint(sf::Mouse::getPosition(*window));
            }
        }

        if (event.type == sf::Event::MouseButtonReleased) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                selectedPoint = nullptr;
            }
        }
    }
}

void Track::generateCollisionPoints() {
    for(int p=0; p<paths.size(); p++) {
        pathCollisionPoints[p].clear();
        for (int i = 0; i < paths[p].size(); i += 3) {
            if (i + 4 > paths[p].size()) break;
            addBezierCurvePoints(&pathCollisionPoints[p], paths[p][i], paths[p][i + 1], paths[p][i + 2], paths[p][i + 3], segments);
        }
    }
}

sf::Vector2f *Track::lineIntersects(sf::Vector2f p0, sf::Vector2f p1) {
    for(auto & collisionPoints : pathCollisionPoints) {
        if (collisionPoints.empty()) { continue; }
        for (int j = 0; j < collisionPoints.size() - 1; j++) {
            sf::Vector2f *collisionPoint = getLineIntersection(p0, p1, collisionPoints[j], collisionPoints[j + 1]);
            if (collisionPoint != nullptr) return collisionPoint;
        }
    }
    return nullptr;
}

sf::Vector2f *Track::carIntersects(Car *car) {
    for(auto & collisionPoints : pathCollisionPoints) {
        if (collisionPoints.empty()) { continue; }
        for (int i = 0; i < collisionPoints.size() - 1; i++) {
            sf::Vector2f *collisionPoint = car->intersectsWithLine(collisionPoints[i], collisionPoints[i + 1]);
            if (collisionPoint != nullptr) return collisionPoint;
        }

    }

    return nullptr;
}

sf::Vector2f *Track::searchForPoint(sf::Vector2<int> mousePosition) {
    for(auto & points : paths) {
        int i = 0;
        for (auto point: points) {
            int detectionRadius = POINT_RADIUS;
            int distX = point.x - mousePosition.x;
            int distY = point.y - mousePosition.y;
            if ((distX * distX + distY * distY) <= 2 * detectionRadius * detectionRadius) {
                return &points[i];
            }
            i++;
        }
    }

    return nullptr;
}

void Track::update() {
    if (selectedPoint != nullptr) {
        *selectedPoint = sf::Vector2f(sf::Mouse::getPosition(*window));
        generateCollisionPoints();
    }
}

sf::Vector2f *Track::closestLineIntersect(sf::Vector2f origin, sf::Vector2f p0, sf::Vector2f p1) const {

    sf::Vector2f *closestCollisionPoint = nullptr;
    int recordDistance = INT32_MAX;

    for(auto & collisionPoints : pathCollisionPoints) {
        if (collisionPoints.empty()) { continue; }

        for (int i = 0; i < collisionPoints.size() - 1; i++) {
            sf::Vector2f *collisionPoint = getLineIntersection(p0, p1, collisionPoints[i], collisionPoints[i + 1]);
            if (collisionPoint == nullptr) continue;

            sf::Vector2f diff = *collisionPoint - origin;
            int distance = diff.x * diff.x + diff.y * diff.y;
            if (distance < recordDistance) {
                closestCollisionPoint = collisionPoint;
                recordDistance = distance;
            }
        }
    }

    return closestCollisionPoint;
}

void Track::save(std::string name) {

    std::ofstream file;
    file.open(name);

    // paths
    file << paths.size() << "\n";

    for(auto & points : paths) {
        file << points.size() << "\n";
        for (int i = 0; i < points.size(); i++) {
            file << points[i].x << " " << points[i].y << "\n";
        }
    }

    // checkpoints
    file << checkpoints.size() << "\n";
    for(auto & point: checkpoints) {
        file << point.x << " " << point.y << "\n";
    }

    // starting position and starting Angle
    file << startingPosition.x << " " << startingPosition.y << "\n";
    file << startingAngle << "\n";

    // starting Checkpoint
    file << startingCheckpoint << "\n";

    file.close();
}

void Track::load(std::string name) {

    paths.clear();

    std::ifstream  file;
    file.open(name);

    // paths
    file >> amountOfPaths;
    setup(window);

    for(int i=0; i<amountOfPaths; i++) {
        int amountOfPoints;
        file >> amountOfPoints;

        int px, py;
        for(int j=0; j<amountOfPoints; j++) {
            file >> px >> py;
            sf::Vector2f point = sf::Vector2f(px, py);
            paths[i].push_back(point);
        }
    }

    generateCollisionPoints();

    // checkpoints
    checkpoints.clear();
    int amountOfCheckpoints;
    file >> amountOfCheckpoints;
    int px, py;
    for(int i=0; i<amountOfCheckpoints; i++) {
        file >> px >> py;
        sf::Vector2f point = sf::Vector2f(px, py);
        checkpoints.push_back(point);
    }

    // starting position and starting Angle
    file >> startingPosition.x >> startingPosition.y;
    file >> startingAngle;

    // starting checkpoint
    file >> startingCheckpoint;

    file.close();
}

void Track::handleCarCheckpoints(Car *car) {
    if(checkpoints.size() < 2) { return; }
    int cpindex = (car->currentCheckpoint*2 + startingCheckpoint*2) % (checkpoints.size());
    if(car->intersectsWithLine(checkpoints[cpindex], checkpoints[cpindex+1]) != nullptr) {
        car->currentCheckpoint = (car->currentCheckpoint+1) % (checkpoints.size()/2);
        car->totalCheckpointsReached++;
    }
}

void Track::setOriginAndRotation(sf::Vector2f origin, float rotation) {
    startingPosition = origin;
    startingAngle = rotation;
}

float Track::distanceOfCarToNextCheckpoint(Car *car) {
    if(checkpoints.size() < 2) { return 0; }
    int cpindex = (car->currentCheckpoint*2 + startingCheckpoint*2) % (checkpoints.size());
    float distance = getDistanceFromPointToLine(car->getPosition(), checkpoints[cpindex], checkpoints[cpindex+1]);
    return distance;
}
