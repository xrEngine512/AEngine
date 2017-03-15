#pragma once
#include "IExternalMaterial.h"

interface IExternalRenderObject
{
    virtual void SetCoord(float x, float y, float z) = 0;
    virtual void SetRotation(float x, float y, float z) = 0;
    virtual unsigned int id() = 0;
    virtual IExternalMaterialRef material(uint8_t id) = 0;
    virtual ~IExternalRenderObject(){};
};