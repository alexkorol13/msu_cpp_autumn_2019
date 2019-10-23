#ifndef LINEARALLOCATOR_H_INCLUDED
#define LINEARALLOCATOR_H_INCLUDED
#include <stdlib.h>

class LinearAllocator
{
public:
    LinearAllocator(size_t maxSize);
    char* alloc(size_t size);
    void reset();
    ~LinearAllocator();
private:
    size_t maxSize;
    size_t ptr;
    char *buf;
};

#endif // LINEARALLOCATOR_H_INCLUDED
