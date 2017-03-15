#pragma once

#include "AlignedObject.h"

struct ShaderDesc;

struct IShaderParams: AlignedObject
{
	virtual const ShaderDesc& GetShaderDesc() = 0;
	virtual bool ContainsLight() = 0;
	virtual bool ContainsSpecularity() = 0;
    virtual ~IShaderParams(){};
};