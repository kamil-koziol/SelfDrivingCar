//
// Created by Kamil Kozioł on 11/11/2022.
//

#ifndef SELFDRIVINGCAR_NEURALNETWORK_H
#define SELFDRIVINGCAR_NEURALNETWORK_H

#include <SFML/Graphics.hpp>
#include "Layer.h"
#include "string"

class NeuralNetwork : public sf::Drawable {
public:

    int amountOfLayers;
    const int inputLayerIndex = 0;
    int outputLayerIndex;
    int biggestLayerSize = 0;

    Layer **layers;

    NeuralNetwork(int amountOfInputsNeurons, int amountOfHiddenLayers, int amountOfOutputNeurons,
                  int hiddenLayerSizes[amountOfInputsNeurons]);

    void feedForward();

    void randomize();

    void copy(NeuralNetwork *other);

    void mutate(float amount);

    void save(std::string path);

    void load(std::string path);

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    static float binarize(float value, float threshhold = 0.5f);

    static float lerp(float value1, float value2, float t);

    static float relu(float value);

    static float normalize(float value);

    void crossover(NeuralNetwork *other);
    bool haveSameSizes(NeuralNetwork *other);
};

#endif //SELFDRIVINGCAR_NEURALNETWORK_H
