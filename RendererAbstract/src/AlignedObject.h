#pragma once

#include <cstddef>

struct AlignedObject
{
    void* operator new(size_t size);
    void operator delete(void* ptr);
    virtual ~AlignedObject();
};