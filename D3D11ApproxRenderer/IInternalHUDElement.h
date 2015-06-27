#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "AlignedObject.h"

struct IInternalHUDElement : AlignedObject
{
    virtual bool Initialize(ID3D11Device* device, HWND hwnd, wchar_t* textureFile, unsigned int width, unsigned int height) = 0;
    virtual bool __vectorcall Render(ID3D11DeviceContext* context, XMMATRIX orthoMatrix) = 0;
    virtual void Shutdown() = 0;
};