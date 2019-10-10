#include <iostream>
#include <string>
#include <cstdlib>
#include "Timer.h"

using namespace std;

int main()
{
    int N = 10000;
    int *arr = new int [N * N];
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            arr[N * i + j] = rand() & 1;
        }
    }
    volatile long long sum = 0;
    Timer t1;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            sum += arr[N * i + j];
        }
    }
    sum = 0;
    Timer t2;
    for (int j = 0; j < N; j++) {
        for (int i = 0; i < N; i++) {
            sum += arr[N * i + j];
        }
    }
    delete [] arr;
    return 0;
}
