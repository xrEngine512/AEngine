#pragma once

struct ShaderDesc;

__interface IExternalMaterial
{
    bool LoadTexture(wchar_t* diffTexfilename, unsigned short slot);
    void ChangeShaderAndSaveTextures(const ShaderDesc& type);
};