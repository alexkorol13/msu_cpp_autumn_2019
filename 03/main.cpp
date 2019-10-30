#include <iostream>
#include "matrix.h"

using namespace std;

int main(void)
{
    Matrix m(2, 3);
    cout << "rows = " << m.getRows() << ", 2 expected\n";
    cout << "cols = " << m.getCols() << ", 3 expected\n";
    for (int i = 0; i < m.getRows(); i++) {
        for (int j = 0; j < m.getRows(); j++) {
            m[i][j] = i * j - i - j;
        }
    }

    cout << "our matrix: ";
    for (int i = 0; i < 2; i++) {
        for (int k = 0; k < 3; k++) {
            cout << m[i][k] << " ";
        }
    }
    cout << endl;

    m *= -1;

    cout << "our matrix after m *= -1: ";
    for (int i = 0; i < 2; i++) {
        for (int k = 0; k < 3; k++) {
            cout << m[i][k] << " ";
        }
    }
    cout << endl;

    try {
        cout << m[10][1] << endl;
    } catch (const exception& e) {
        cerr << e.what();
    }

    try {
        cout << m[1][100] << endl;
    } catch (const exception& e) {
        cerr << e.what();
    }

    Matrix m2(2, 3);
    m2[0][0] = 0;
    m2[0][1] = 1;
    m2[0][2] = 0;
    m2[1][0] = 1;
    m2[1][1] = 1;
    m2[1][2] = 0;

    cout << "Matrix comparsion test, expected: 1 0\n";
    cout << (m == m2) << " ";
    cout << (m != m2) << endl;

    try {
        Matrix huge(1000000, 1000000);
    } catch (const exception& e) {
        cerr << e.what() << endl;
    }
    return 0;
}
