#pragma once

#include <d3d11.h>
#include "IMeshLoader.h"
#include "IShader.h"

//#include "IVertexType.h"

using namespace DirectX;



class Mesh
{
	struct VertexType
	{
		XMFLOAT3 position;
		//XMFLOAT4 color;
		XMFLOAT2 texture;
		XMFLOAT3 normal;
	};
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;
    IMeshLoader* m_MeshLoader;
	IShader *m_bindedShader;
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

public:
	Mesh(void);
	~Mesh(void);
	bool Initialize(ID3D11Device* , char* MeshFileName , bool dynamic_shader);
    bool Initialize(ID3D11Device*, MeshBox*, bool dynamic_shader);
	void Shutdown();
	void Render(ID3D11DeviceContext*);
    MeshBox GetMeshBox();
	void BindShader(IShader *);
	int GetIndexCount();
};

