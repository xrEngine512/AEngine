#pragma once
#include <d3d11.h>
#include "MeshBox.h"


__interface IMeshLoader
{
    void Inititalize(ID3D11Device* device, CHAR* meshFileName);
    bool GetBuffers(ID3D11Buffer *&vertexBuffer, ID3D11Buffer *&indexBuffer);
    MeshBox GetMeshBox();
    int GetIndexCount();
    int GetVertexCount();
};