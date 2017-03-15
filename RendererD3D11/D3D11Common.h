#pragma once
#include <d3d11.h>
namespace D3D11Common
{
    template < class T > 
    inline ID3D11Buffer* InitializeStaticVertexBuffer(ID3D11Device* device,T* vertices, int vertexCount)
    {
        D3D11_BUFFER_DESC vertexBufferDesc;
        D3D11_SUBRESOURCE_DATA vertexData;
        HRESULT result;
        ID3D11Buffer* resBuffer;
        if (!vertices)
        {
            vertices = new T[vertexCount];
            if (!vertices)
            {
                return false;
            }
        }

        vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
        vertexBufferDesc.ByteWidth = sizeof(T) * vertexCount;
        vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        vertexBufferDesc.CPUAccessFlags = 0;
        vertexBufferDesc.MiscFlags = 0;
        vertexBufferDesc.StructureByteStride = 0;

        // Give the subresource structure a pointer to the vertex data.
        vertexData.pSysMem = vertices;
        vertexData.SysMemPitch = 0;
        vertexData.SysMemSlicePitch = 0;

        // Now create the vertex buffer.
        result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &resBuffer);

        if (FAILED(result))
        {
            return nullptr;
        }
        return resBuffer;
    }
    inline ID3D11Buffer* InitializeIndexBuffer(ID3D11Device* device, unsigned long* indices, int indexCount)
    {
        D3D11_BUFFER_DESC indexBufferDesc;
        D3D11_SUBRESOURCE_DATA indexData;
        HRESULT result;
        ID3D11Buffer* resBuffer;

        if (!indices)
        {
            // Create the index array.
            indices = new unsigned long[indexCount];
            if (!indices)
            {
                return nullptr;
            }

            // Load the index array with data.
            for (int i = 0; i < indexCount; i++)
            {
                indices[i] = i;
            }
        }

        // Set up the description of the static index buffer.
        indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
        indexBufferDesc.ByteWidth = sizeof(unsigned long) * indexCount;
        indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
        indexBufferDesc.CPUAccessFlags = 0;
        indexBufferDesc.MiscFlags = 0;
        indexBufferDesc.StructureByteStride = 0;

        // Give the subresource structure a pointer to the index data.
        indexData.pSysMem = indices;
        indexData.SysMemPitch = 0;
        indexData.SysMemSlicePitch = 0;

        // Create the index buffer.
        result = device->CreateBuffer(&indexBufferDesc, &indexData, &resBuffer);
        if (FAILED(result))
        {
            return nullptr;
        }
        return resBuffer;
    }
}

