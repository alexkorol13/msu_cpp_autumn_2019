#include <iostream>
#include "LinearAllocator.h"


int main(void)
{
    LinearAllocator A(100);
    std::cout << "LinearAllocator A has been created, maxSize = 100\n";
    char *buf;

    buf = A.alloc(10);
    std::cout << "A: maxSize = 100, trying to allocate 10 bytes, 100 bytes are available\n";
    if (buf) {
        std::cout << "test has been passed\n";
    } else {
        std::cout << "Incorrect allocation\n";
    }
    buf[0] = 'c';
    buf[1] = 'c';
    buf[1] += 1;
    if (buf[0] != 'c' || buf[1] != 'd') {
        std::cout << "we can't use allocated memory\n";
    } else {
        std::cout << "we can use allocated memory, ok\n";
    }
    buf = A.alloc(90);
    std::cout << "A: maxSize = 100, trying to allocate 90 bytes, 90 bytes are available\n";
    if (buf) {
        std::cout << "test has been passed\n";
    } else {
        std::cout << "Incorrect allocation\n";
    }

    std::cout << "A: maxSize = 100, trying to allocate 10 bytes, 0 bytes are available\n";
    buf = A.alloc(10);
    if (!buf) {
        std::cout << "test has been passed\n";
    } else {
        std::cout << "Incorrect allocation\n";
    }

    A.reset();
    std::cout << "LinearAllocator A has been reseted, maxSize = 100\n";
    std::cout << "A: maxSize = 100, trying to allocate 100 bytes, 100 bytes are available\n";
    buf = A.alloc(100);
    if (buf) {
        std::cout << "test has been passed\n";
    } else {
        std::cout << "Incorrect allocation\n";
    }

    A.reset();
    std::cout << "A has been reseted\n";
    buf = A.alloc(101);
    std::cout << "A: maxSize = 100, trying to allocate 101 bytes, 100 bytes are available\n";
    if (!buf) {
        std::cout << "test has been passed\n";
    } else {
        std::cout << "Incorrect allocation\n";
    }

    LinearAllocator B(0);
    std::cout << "LinearAllocator B has been created, maxSize = 0\n";

    buf = B.alloc(0);
    std::cout << "B: maxSize = 0, trying to allocate 0 bytes, 0 bytes are available\n";
    if (!buf) {
        std::cout << "test has been passed\n";
    } else {
        std::cout << "Incorrect allocation\n";
    }

    LinearAllocator C(1);
    std::cout << "LinearAllocator C has been created, maxSize = 1\n";

    buf = C.alloc(1);
    std::cout << "C: maxSize = 1, trying to allocate 1 byte, 1 byte is available\n";
    if (buf) {
        std::cout << "test has been passed\n";
    } else {
        std::cout << "Incorrect allocation\n";
    }
    return 0;
}
