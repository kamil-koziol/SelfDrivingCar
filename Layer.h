//
// Created by Kamil Kozioł on 11/11/2022.
//

#ifndef SELFDRIVINGCAR_LAYER_H
#define SELFDRIVINGCAR_LAYER_H


#include "Matrix.h"

class Layer {
public:
    const int size;

    Matrix *neurons;
    Matrix *weights;
    Matrix *biases;

    Layer(int size, int nextLayerSize);
    void randomize();
};


#endif //SELFDRIVINGCAR_LAYER_H
