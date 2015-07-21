#pragma once

#include <d3d11.h>
#include "Light.h"
#include "IShader.h"

namespace MatInterface
{
	class MaterialInterface;
}

__interface IInternalMaterial
{
	bool Initialize(ID3D11Device *device, ShaderDesc type);
    bool Render(ID3D11DeviceContext *deviceContext, int indexCount);
    void Shutdown();
};