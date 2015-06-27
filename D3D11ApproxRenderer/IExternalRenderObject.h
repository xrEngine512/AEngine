#pragma once
#include "IExternalMaterial.h"

__interface IExternalRenderObject
{
    void SetCoord(float x, float y, float z);
    void SetRotation(float x, float y, float z);
    unsigned int GetID();
    IExternalMaterial* GetMaterial();
};