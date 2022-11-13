//
// Created by Kamil Kozioł on 11/11/2022.
//

#ifndef SELFDRIVINGCAR_NEURALNETWORK_H
#define SELFDRIVINGCAR_NEURALNETWORK_H

#include <SFML/Graphics.hpp>
#include "Layer.h"

#define INPUT_LAYER 0
#include "string"

class NeuralNetwork: public sf::Drawable {
public:

    int amountOfLayers;
    const int inputLayerIndex = 0;
    int outputLayerIndex;
    int biggestLayerSize = 0;
    Layer **layers;
    NeuralNetwork(int amountOfInputsNeurons, int amountOfHiddenLayers, int amountOfOutputNeurons, int hiddenLayerSizes[amountOfInputsNeurons]);
    void compute();
    void randomize();
    void mutate(float amount);
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
    void copyFrom(NeuralNetwork *other);
    void save(std::string name);
    void load(std::string name);
};


#endif //SELFDRIVINGCAR_NEURALNETWORK_H
