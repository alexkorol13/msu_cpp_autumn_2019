#ifndef MATRIX_H_INCLUDED
#define MATRIX_H_INCLUDED

class Matrix {
public:
    class Row {
    public:
        Row(Matrix* m, int pos);
        ~Row();
        int operator[](int col);
    private:
        Matrix* matr;
        int pos;
    };
    Matrix(const int rows, const int cols);
    ~Matrix();
    int getCols();
    int getRows();
    bool operator==(const Matrix m);
    Matrix operator*(const int n);
    Row operator[](int row);
private:
    int rows;
    int cols;
    int *matrix;
};

#endif // MATRIX_H_INCLUDED
