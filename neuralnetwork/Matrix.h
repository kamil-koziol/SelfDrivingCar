//
// Created by Kamil Kozioł on 11/11/2022.
//

#ifndef SELFDRIVINGCAR_MATRIX_H
#define SELFDRIVINGCAR_MATRIX_H

#include "fstream"


class Matrix {
public:
    int m, n;
    float *values; // MxN
    Matrix(int m, int n);

    Matrix *dot(Matrix *other);

    void add(Matrix *other);

    float get(int x, int y);

    float get(int index);

    void set(int x, int y, float value);

    void set(int index, float value);

    void print();

    void randomize(bool negatives = false);

    void mutate(float amount);

    void copyFrom(Matrix *other);

    void applyFunction(float (*func)(float));

    void applyFunction(float (*func)(float, float), float);

    bool canBeMultiplied(Matrix *other);

    bool haveSameDimensions(Matrix *other);

    void appendToFile(std::ofstream *stream);

    static Matrix *loadFromStream(std::ifstream *stream);

    virtual ~Matrix();
};


#endif //SELFDRIVINGCAR_MATRIX_H
