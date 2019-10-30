#ifndef MATRIX_H_INCLUDED
#define MATRIX_H_INCLUDED

class Matrix {
public:
    class Row {
    public:
        Row(Matrix* m, int pos, int len);
        const int& operator[](int col) const;
        int& operator[](int col);
        ~Row();
    private:
        int *row_ptr;
        int len;
    };
    Matrix(const int rows, const int cols);
    ~Matrix();
    int getCols();
    int getRows();
    bool operator==(const Matrix& m) const;
    bool operator!=(const Matrix& m) const;
    Matrix& operator*=(const int n);
    Row operator[](int row);
    const Row operator[](int row) const;
private:
    int rows;
    int cols;
    int *matrix;
};

#endif // MATRIX_H_INCLUDED
