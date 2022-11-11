//
// Created by Kamil Kozioł on 11/11/2022.
//

#include "Matrix.h"

Matrix::Matrix(int n) {
    this->n = n;
    values = new float[n];
}
