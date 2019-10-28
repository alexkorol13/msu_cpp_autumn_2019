#include "matrix.h"
#include <stdlib.h>
#include <iostream>

Matrix::Matrix(const int rows, const int cols)
{
    this->rows = rows;
    this->cols = cols;
    int *m = (int *) malloc(sizeof(int) * rows * cols);
    if (!m) {
        this->matrix = m;
    } else {
        std::cerr << "Unable to allocate memory\n";
        exit(1);
    }
}

Matrix::Row::Row(Matrix* m, int row)
{
    this->pos = row;
    this->matr = m;
}

Matrix::~Matrix()
{
    free(this->matrix);
}

int Matrix::getCols()
{
    return this->cols;
}

int Matrix::getRows()
{
    return this->rows;
}

bool Matrix::operator==(Matrix m)
{
    if (m.getRows() != this->getRows() || m.getCols() != this->getCols()) {
        return false;
    }
    int w = this->getCols() * this->getRows();
    for (int i = 0; i < w; i++) {
        if (*(m.matrix + i) != *(this->matrix + i)) {
            return false;
        }
    }
    return true;
}

Matrix Matrix::operator*(int m)
{
    int n = this->getCols() * this->getRows();
    for (int i = 0; i < n; i++) {
        this->matrix[i] *= m;
    }
    return *this;
}

Matrix::Row Matrix::operator[](int row)
{
    if (row >= this->getCols()) {
        throw std::out_of_range("");
    } else {
        return Row(this, row);
    }
}

Matrix::Row::~Row() {}

int Matrix::Row::operator[](int col)
{
    if (col >= matr->getCols()) {
        throw std::out_of_range("");
    } else {
        return *((int *)this->matr + this->pos * this->matr->getCols() + col);
    }
}
