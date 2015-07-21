#pragma once
#include "imeshloader.h"

class ObjMeshLoader :
    public IMeshLoader
{
    struct VertexType
	{
		XMFLOAT3 position;
        //XMFLOAT4 color;
		XMFLOAT2 texture;
		XMFLOAT3 normal;
	};
    static const int buffermax = 500000;
    struct ModelBuffer
    {
        VertexType vertices[buffermax];
        DWORD  indices[buffermax];
        int    verticesI;
        int    indicesI;
    };
    ID3D11Device* m_device;
    ModelBuffer* buffer;
    //int m_vertexCount, m_indexCount,m_normalCount, m_texCoordCount;
    DWORD AddVertex(int vertexI, VertexType vertex);
    void AddIndex(DWORD index);
    MeshBox m_MeshBox;
    bool m_firstVertex = true;
public:
    int GetIndexCount();
    int GetVertexCount();
    MeshBox GetMeshBox();
    void Inititalize(ID3D11Device* device,CHAR* meshFileName);
    bool GetBuffers(ID3D11Buffer *&vertexBuffer, ID3D11Buffer *&indexBuffer);
    ObjMeshLoader(void);
    ~ObjMeshLoader(void);
};

