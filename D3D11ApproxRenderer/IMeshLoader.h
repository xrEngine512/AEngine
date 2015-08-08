#pragma once
#include "MeshBox.h"

struct ID3D11Device;
struct ID3D11Buffer;

__interface IMeshLoader
{
    void Inititalize(ID3D11Device* device, const char* meshFileName);
    bool GetBuffers(ID3D11Buffer *&vertexBuffer, ID3D11Buffer *&indexBuffer);
    MeshBox GetMeshBox();
    int GetIndexCount();
    int GetVertexCount();
};