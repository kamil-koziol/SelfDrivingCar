//
// Created by Kamil Kozioł on 11/11/2022.
//

#include "NeuralNetwork.h"
#include "utils/MathUtils.h"
#include "math.h"


NeuralNetwork::NeuralNetwork(int amountOfInputsNeurons, int amountOfHiddenLayers, int amountOfOutputNeurons, int hiddenLayerSizes[amountOfInputsNeurons]){
    amountOfLayers = amountOfHiddenLayers + 2;
    outputLayerIndex = amountOfLayers - 1;

    layers = new Layer*[amountOfLayers];

    layers[inputLayerIndex] = new Layer(amountOfInputsNeurons, hiddenLayerSizes[0]);
    layers[outputLayerIndex] = new Layer(amountOfOutputNeurons,0);

    for(int i=0; i<(amountOfHiddenLayers-1); i++) {
        layers[i + 1] = new Layer(hiddenLayerSizes[i], hiddenLayerSizes[i+1]);
    }

    layers[1 + amountOfHiddenLayers-1] = new Layer(hiddenLayerSizes[amountOfHiddenLayers-1], amountOfOutputNeurons);

    for(int i=0; i<amountOfLayers; i++) {
        if(layers[i]->size > biggestLayerSize) {
            biggestLayerSize = layers[i]->size;
        }
    }
}

void NeuralNetwork::compute() {
    for(int i=0; i<amountOfLayers-1; i++) {
        Matrix *calculationMatrix = layers[i]->weights->mult(layers[i]->neurons);
        calculationMatrix->add(layers[i]->biases);
        calculationMatrix->applyFunction(relu);
        calculationMatrix->applyFunction(normalize);
        layers[i+1]->neurons->copyFrom(calculationMatrix);
    }
}

void NeuralNetwork::randomize() {
    for(int i=0; i<amountOfLayers-1; i++) {
        layers[i]->randomize();
    }
}

void NeuralNetwork::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    float radius = 20.0f;
    sf::CircleShape neuronShape(radius);
    neuronShape.setOutlineThickness(2.0f);
    neuronShape.setOutlineColor(sf::Color::White);

    sf::Vertex line[2] = {
            sf::Vector2f(),
            sf::Vector2f()
    };

    int ox = 200, oy = 200;
    int dx = 100, dy = 50;

    for(int i=0; i<amountOfLayers; i++) {

        // draw neurons
        for(int j=0; j<layers[i]->size; j++) {
            float neuronValue = layers[i]->neurons->get(j); // from 0 to 1
            neuronValue *= 255.0f;
            uint8_t color = floor(neuronValue);
            neuronShape.setFillColor(sf::Color(255, 255-color, 255-color));

            sf::Vector2f point = sf::Vector2f(ox + i*dx, oy + j*dy);
            float offset = float(biggestLayerSize - layers[i]->size)/2.0f;
            point.y += offset * dy;
            neuronShape.setPosition(point);


            // draw weights
            line[0] = point;
            line[0].position.x += radius;
            line[0].position.y += radius;

            if(i != outputLayerIndex) {
                for (int k = 0; k < layers[i + 1]->size; k++) {

                    line[1] = line[0];
                    line[1].position.x += dx;
                    float nextLayerYOffset = float(biggestLayerSize - layers[i+1]->size)/2.0f;
                    line[1].position.y += k*dy - j*dy + (nextLayerYOffset * dy) - (offset*dy);

                    float color = layers[i]->weights->get(k); // -1 to 1
                    color += 1;
                    color *= 255.0f/2.0f;
                    uint8_t c = floor(color);
                    sf::Color col = sf::Color(255-c, 0, c, std::min(120 + c, 255));
                    line[0].color = col;
                    line[1].color = col;

                    target.draw(line, 2, sf::Lines);
                }
            }

            target.draw(neuronShape);
        }
    }
}

