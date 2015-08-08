#pragma once
#include "IShaderParams.h"
#include <d3d11.h>
#include "Light.h"
#include "MeshBox.h"
#include "AlignedObject.h"

namespace MatInterface
{
	class MaterialInterface;
}

struct IInternalRenderObject : AlignedObject
{
    virtual bool Initialize(ID3D11Device *device, const char *modelFileName, float PosX, float PosY, float PosZ, float RotX, float RotY, float RotZ) = 0;
    virtual bool Initialize(ID3D11Device *device, MeshBox*, float PosX, float PosY, float PosZ, float RotX, float RotY, float RotZ) = 0;
	virtual bool Render(ID3D11DeviceContext *deviceContext, MatInterface::MaterialInterface&) = 0;
    virtual MeshBox GetMeshBox() = 0;
    virtual XMVECTOR GetPoint() = 0;
    virtual XMFLOAT3 GetCoord() = 0;
    virtual unsigned int GetID() = 0;
    virtual void SetID(unsigned int) = 0;
    virtual void Shutdown() = 0;
};