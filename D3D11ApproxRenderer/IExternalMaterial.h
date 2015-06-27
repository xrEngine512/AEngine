#pragma once
#include "ShaderTypes.h"

__interface IExternalMaterial
{
    void SetSpecularity(float R, float G, float B, float Power);
    bool LoadTexture(wchar_t* diffTexfilename, unsigned short slot);
    void ChangeShaderAndSaveTextures(ShaderType type);
};