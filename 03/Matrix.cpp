#include "matrix.h"
#include <iostream>

Matrix::Matrix(const int rows, const int cols)
{
    this->rows = rows;
    this->cols = cols;
    int *m = (int *) malloc(sizeof(int) * rows * cols);
    if (m != nullptr) {
        this->matrix = m;
    } else {
        throw std::bad_alloc();
    }
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

bool Matrix::operator==(const Matrix& m) const
{
    if (this == &m) {
        return true;
    }
    if (m.rows != this->rows || m.cols != this->cols) {
        return false;
    }
    int w = cols * rows;
    for (int i = 0; i < w; i++) {
        if (*(m.matrix + i) != *(this->matrix + i)) {
            return false;
        }
    }
    return true;
}

bool Matrix::operator!=(const Matrix& m) const
{
    return !(*this == m);
}

Matrix& Matrix::operator*=(int m)
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
        throw std::out_of_range("row index out of range\n");
    }
    return Row(this, row, cols);
}

Matrix::Row::Row(Matrix* m, int row, int len)
{
    if (row >= m->getRows()) {
        throw std::out_of_range("row index out of range\n");
    }
    this->row_ptr = m->matrix + len * row;
    this->len = len;
}

Matrix::Row::~Row() {}

int& Matrix::Row::operator[](int col)
{
    if (col >= this->len) {
        throw std::out_of_range("column index out of range\n");
    }
    return this->row_ptr[col];
}

const int& Matrix::Row::operator[](int col) const
{
    if (col >= this->len) {
        throw std::out_of_range("column index out of range\n");
    }
    return this->row_ptr[col];
}
