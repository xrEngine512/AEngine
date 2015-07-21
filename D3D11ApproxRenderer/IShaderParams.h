#pragma once
#include <DirectXMath.h>
#include <atomic>
#include "ShaderDesc.h"
using namespace DirectX;

struct IShaderParams
{
	virtual const ShaderDesc& GetShaderDesc() = 0;
	virtual bool ContainsLight() = 0;
	virtual bool ContainsSpecularity() = 0;
    void* operator new(size_t size)
    {
        return _aligned_malloc(size, 16);
    }
    void operator delete(void* ptr)
    {
        _aligned_free(ptr);
    }
};