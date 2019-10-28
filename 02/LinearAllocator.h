#ifndef LINEARALLOCATOR_H_INCLUDED
#define LINEARALLOCATOR_H_INCLUDED

#include <cstddef>

class LinearAllocator
{
public:
    LinearAllocator(std::size_t maxSize);
    char* alloc(std::size_t size);
    void reset();
    ~LinearAllocator();
private:
    std::size_t maxSize;
    std::size_t ptr;
    char *buf;
};

#endif // LINEARALLOCATOR_H_INCLUDED
