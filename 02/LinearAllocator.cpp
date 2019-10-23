#include "LinearAllocator.h"

LinearAllocator::LinearAllocator(size_t maxSize)
{
    ptr = 0;
    this->maxSize = maxSize;
    buf = (char *) malloc(maxSize);
}

char *LinearAllocator::alloc(size_t size)
{
    if (this->maxSize && (ptr + size <= maxSize)) {
        size_t old_ptr = ptr;
        ptr += size;
        return (char *) (buf + old_ptr);
    }
    return NULL;
}

void LinearAllocator::reset()
{
    ptr = 0;
}

LinearAllocator::~LinearAllocator()
{
    free(buf);
}