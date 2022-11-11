//
// Created by Kamil Kozioł on 11/11/2022.
//

#include "Matrix.h"
#include "iostream"
#include "time.h"

Matrix::Matrix(int m, int n) {
    this->m = m;
    this->n = n;
    values = new float[m*n];
    for(int i=0; i<m*n; i++) {
        values[i] = 0;
    }
}

Matrix* Matrix::mult(Matrix *other) {
    if(!canBeMultiplied(other)) return nullptr;
    Matrix *result = new Matrix(m, other->n);
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < other->n; j++) {
            float sum  = 0;

            for (int k = 0; k < other->m; k++) {
                sum += get(k, i) * other->get(j,k);
            }

            result->set(j, i, sum);
        }
    }
    return result;
}

float Matrix::get(int x, int y) {
    return values[y*n + x];
}

void Matrix::set(int x, int y, float value) {
    values[y*n + x] = value;
}

void Matrix::print() {
    for(int i=0; i<m; i++) {
        for(int j=0; j<n; j++) {
            std::cout<<get(j, i) << " ";
        }

        std::cout<<std::endl;
    }
}

void Matrix::randomize(bool negatives) {
    for(int i=0; i<m; i++) {
        for(int j=0; j<n; j++) {
            float value = (float) rand()/RAND_MAX;
            if(negatives) {
                if(rand()%2==0) {
                    value *= -1;
                }
            }
            set(j,i, value);
        }
    }
}

void Matrix::copyFrom(Matrix *other) {
    if(!haveSameDimensions(other)) return;
    for(int i=0; i<m*n; i++) {
        values[i] = other->values[i];
    }
}

void Matrix::add(Matrix *other) {
    if(!haveSameDimensions(other)) return;
    for(int i=0; i<m*n; i++) {
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
    for(int i=0; i<m*n; i++) {
        values[i] = func(values[i]);
    }
}

float Matrix::get(int index) {
    return values[index];
}




