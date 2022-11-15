//
// Created by Kamil Kozioł on 11/11/2022.
//

#include "Matrix.h"
#include "iostream"
#include "time.h"
#include "../utils/MathUtils.h"
#include "NeuralNetwork.h"

Matrix::Matrix(int m, int n) {
    this->m = m;
    this->n = n;
    values = new float[m * n];
    for (int i = 0; i < m * n; i++) {
        values[i] = 0;
    }
}

Matrix *Matrix::dot(Matrix *other) {
    if (!canBeMultiplied(other)) return nullptr;
    Matrix *result = new Matrix(m, other->n);
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < other->n; j++) {
            float sum = 0;

            for (int k = 0; k < other->m; k++) {
                sum += get(k, i) * other->get(j, k);
            }

            result->set(j, i, sum);
        }
    }
    return result;
}

float Matrix::get(int x, int y) {
    return values[y * n + x];
}

void Matrix::set(int x, int y, float value) {
    values[y * n + x] = value;
}

void Matrix::print() {
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            std::cout << get(j, i) << " ";
        }

        std::cout << std::endl;
    }
}

void Matrix::randomize(bool negatives) {
    for (int i = 0; i < m * n; i++) {
        float value = (float) rand() / RAND_MAX;
        if (negatives) {
            if (rand() % 2 == 0) {
                value *= -1;
            }
        }
        values[i] = value;
    }
}

void Matrix::copy(Matrix *other) {
    if (!haveSameDimensions(other)) return;
    for (int i = 0; i < m * n; i++) {
        values[i] = other->values[i];
    }
}

void Matrix::add(Matrix *other) {
    if (!haveSameDimensions(other)) return;
    for (int i = 0; i < m * n; i++) {
        values[i] += other->values[i];
    }
}

bool Matrix::canBeMultiplied(Matrix *other) {
    return n == other->m;
}

bool Matrix::haveSameDimensions(Matrix *other) {
    return (n == other->n && m == other->m);
}

void Matrix::applyFunction(float (*func)(float)) {
    for (int i = 0; i < m * n; i++) {
        values[i] = func(values[i]);
    }
}

void Matrix::applyFunction(float (*func)(float, float), float arg) {
    for (int i = 0; i < m * n; i++) {
        values[i] = func(values[i], arg);
    }
}


float Matrix::get(int index) {
    return values[index];
}

void Matrix::set(int index, float value) {
    values[index] = value;
}

void Matrix::mutate(float amount) {
    for (int i = 0; i < m * n; i++) {
        float value = (float) rand() / RAND_MAX;
        if (rand() % 2 == 0) value *= -1;
        values[i] = NeuralNetwork::lerp(values[i], value, amount);
    }
}

void Matrix::appendToFile(std::ofstream *stream) {
    int size = n * m;
    stream->write((char*)(&m), sizeof(int));
    stream->write((char*)(&n), sizeof(int));

    for(int j=0; j<size; j++) {
        float weightValue = values[j];
        stream->write( (const char *)( &weightValue ), sizeof( float ));
    }
}

Matrix *Matrix::loadFromStream(std::ifstream *stream) {
    int n, m;
    stream->read((char*)(&m), sizeof(int));
    stream->read((char*)(&n), sizeof(int));

    Matrix *matrix = new Matrix(m, n);

    for(int i=0; i<n*m; i++) {
        float f_;
        stream->read((char *)&f_,sizeof(float));
        matrix->values[i] = f_;
    }

    return matrix;
}

Matrix::~Matrix() {
    delete[] values;
}

void Matrix::crossover(Matrix *other) {
    if(!haveSameDimensions(other)) return;
    for(int i=0; i<n*m; i++) {
        int r = rand()%2;
        if(r == 0) {
            values[i] = other->values[i];
        }
    }
}





