#pragma once
#include <malloc.h>

struct AlignedObject
{
    void* operator new(size_t size)
    {
        return _aligned_malloc(size, 16);
    }
        void operator delete(void* ptr)
    {
        _aligned_free(ptr);
    }
    virtual ~AlignedObject(){}
};