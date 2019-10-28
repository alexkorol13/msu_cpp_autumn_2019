#include <iostream>
#include "LinearAllocator.h"

using namespace std;

LinearAllocator::LinearAllocator(size_t maxSize)
{
    ptr = 0;
    this->maxSize = maxSize;
    buf = (char *) malloc(maxSize);
    if (!buf) {
        this->maxSize = 0;
        std::cerr << "Unable to allocate memory\n";
        // allocation won't happen if maxSize == 0, (line 18, col 9)
    }
}

char *LinearAllocator::alloc(size_t size)
{
    if (this->maxSize && (ptr + size <= maxSize)) {
        size_t old_ptr = ptr;
        ptr += size;
        return (char *) (buf + old_ptr);
    }
    return nullptr;
}

void LinearAllocator::reset()
{
    ptr = 0;
}

LinearAllocator::~LinearAllocator()
{
    free(buf);
}
