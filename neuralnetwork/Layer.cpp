//
// Created by Kamil Kozioł on 11/11/2022.
//

#include "Layer.h"

Layer::Layer(int size, int nextLayerSize) : size(size) {
    neurons = new Matrix(size, 1);
    weights = new Matrix(nextLayerSize, size);
    biases = new Matrix(nextLayerSize, 1);
}

void Layer::randomize() {
    weights->randomize(true);
    biases->randomize(true);
}

void Layer::mutate(float amount) {
    weights->mutate(amount);
    biases->mutate(amount);
}

void Layer::copy(Layer *pLayer) {
    weights->copy(pLayer->weights);
    biases->copy(pLayer->biases);
}

void Layer::crossover(Layer *other) {
    weights->crossover(other->weights);
    biases->crossover(other->biases);
}
