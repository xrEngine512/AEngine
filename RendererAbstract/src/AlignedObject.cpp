//
// Created by islam on 22.02.17.
//
#include "AlignedObject.h"

#include <OS.h>

void* AlignedObject::operator new(size_t size)
{
    return OS::Memory::aligned_alloc(size, 16);
}

void AlignedObject::operator delete(void* ptr)
{
    OS::Memory::aligned_free(ptr);
}

AlignedObject::~AlignedObject() {

}
