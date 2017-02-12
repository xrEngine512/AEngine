#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include "Mesh.h"
#include "ObjMeshLoader.h"
#include "VertexTypes.h"//пока не используется
#include "D3D11Common.h"

Mesh::Mesh(void) :m_vertexBuffer(nullptr), m_indexBuffer(nullptr), m_MeshLoader(nullptr)
{
}

//UINT currVertCount;

bool Mesh::Initialize(ID3D11Device* device, const char* MeshFileName, bool dynamic_shader)
{
    m_MeshLoader = new ObjMeshLoader;
	// Initialize the vertex and index buffer that hold the geometry for the triangle.
    m_MeshLoader->Inititalize(device,MeshFileName);
    m_vertexCount = m_MeshLoader->GetVertexCount();
    m_indexCount = m_MeshLoader->GetIndexCount();
    if(!m_MeshLoader->GetBuffers(m_vertexBuffer,m_indexBuffer))
        return false;
	/*result = InitializeBuffers(device);
	if(!result)
	{
		return false;
	}*/

	return true;
}

bool Mesh::Initialize(ID3D11Device* device, MeshBox* box, bool dynamic_shader)
{
    VertexType* BoxVerts = static_cast<VertexType*>(calloc(8, sizeof(VertexType)));

    BoxVerts[0].position.x = box->left;
    BoxVerts[0].position.y = box->bottom;
    BoxVerts[0].position.z = box->front;

    BoxVerts[1].position.x = box->left;
    BoxVerts[1].position.y = box->bottom;
    BoxVerts[1].position.z = box->back;

    BoxVerts[2].position.x = box->left;
    BoxVerts[2].position.y = box->top;
    BoxVerts[2].position.z = box->front;

    BoxVerts[3].position.x = box->left;
    BoxVerts[3].position.y = box->top;
    BoxVerts[3].position.z = box->back;

    BoxVerts[4].position.x = box->right;
    BoxVerts[4].position.y = box->bottom;
    BoxVerts[4].position.z = box->front;

    BoxVerts[5].position.x = box->right;
    BoxVerts[5].position.y = box->bottom;
    BoxVerts[5].position.z = box->back;

    BoxVerts[6].position.x = box->right;
    BoxVerts[6].position.y = box->top;
    BoxVerts[6].position.z = box->front;

    BoxVerts[7].position.x = box->right;
    BoxVerts[7].position.y = box->top;
    BoxVerts[7].position.z = box->back;
    m_vertexCount = 8;

    DWORD indices[] = 
    { 
        0, 1, 2,              
        0, 2, 3 ,                
        0, 4, 5 ,
        0, 5, 1 ,
        1, 5, 6 ,
        1, 6, 2 ,
        2, 6, 7 ,
        2, 7, 3 ,
        3, 7, 4 ,
        3, 4, 0 ,
        4, 7, 6 ,
        4, 6, 5 
    };
    m_indexCount = 12;

    m_vertexBuffer = D3D11Common::InitializeStaticVertexBuffer(device, BoxVerts, 8);
    if (!m_vertexBuffer)
        return false;
    m_indexBuffer = D3D11Common::InitializeIndexBuffer(device, indices, 12);
    if (!m_indexBuffer)
        return false;

    return true;

}

void Mesh::Shutdown()
{
	// Release the vertex and index buffers.
	ShutdownBuffers();

	return;
}

MeshBox Mesh::GetMeshBox()
{
    return m_MeshLoader->GetMeshBox();
}

void Mesh::Render(ID3D11DeviceContext* deviceContext)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(deviceContext);
	return;
}

int Mesh::GetIndexCount()
{
	return m_indexCount;
}

bool Mesh::InitializeBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	// Set the number of vertices in the vertex array.
	m_vertexCount = 6;

	// Set the number of indices in the index array.
	m_indexCount = 6;

	// Create the vertex array.
	vertices = new VertexType[m_vertexCount];
	if(!vertices)
	{
		return false;
	}

	// Create the index array.
	indices = new unsigned long[m_indexCount];
	if(!indices)
	{
		return false;
	}
	// Load the vertex array with data.
	vertices[0].position = XMFLOAT4(-1.0f, -1.0f, 0.0f, 1.0f);  // Bottom left.
	vertices[0].texture = XMFLOAT2(0.0f, 1.0f);
	//vertices[0].color = XMFLOAT4(1.0f,0.0f,0.0f,1.0f);
	vertices[0].normal = XMFLOAT3(0.0f,0.0f,-1.0f);

	vertices[1].position = XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);  // Top right.
	vertices[1].texture = XMFLOAT2(1.0f, 0.0f);
	//vertices[1].color = XMFLOAT4(0.0f,1.0f,0.0f,1.0f);
	vertices[1].normal = XMFLOAT3(0.0f,0.0f,-1.0f);

	vertices[2].position = XMFLOAT4(1.0f, -1.0f, 0.0f, 1.0f);  // Bottom right.
	vertices[2].texture = XMFLOAT2(1.0f, 1.0f);
	//vertices[2].color = XMFLOAT4(0.0f,0.0f,1.0f,1.0f);
	vertices[2].normal = XMFLOAT3(0.0f,0.0f,-1.0f);

	vertices[3].position = XMFLOAT4(-1.0f, -1.0f, 0.0f, 1.0f);  // Bottom left.
	vertices[3].texture = XMFLOAT2(0.0f, 1.0f);
	//vertices[3].color = XMFLOAT4(1.0f,0.0f,0.0f,1.0f);
	vertices[3].normal = XMFLOAT3(0.0f,0.0f,-1.0f);

	vertices[4].position = XMFLOAT4(-1.0f, 1.0f, 0.0f, 1.0f);  // Top left.
	vertices[4].texture = XMFLOAT2(0.0f, 0.0f);
	//vertices[4].color = XMFLOAT4(1.0f,0.0f,0.0f,1.0f);
	vertices[4].normal = XMFLOAT3(0.0f,0.0f,-1.0f);

	vertices[5].position = XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);  // Top right.
	vertices[5].texture = XMFLOAT2(1.0f, 0.0f);
	//vertices[5].color = XMFLOAT4(0.0f,1.0f,0.0f,1.0f);
	vertices[5].normal = XMFLOAT3(0.0f,0.0f,-1.0f);

	
	// Load the index array with data.
	indices[0] = 0;  // Bottom left.
	indices[1] = 1;  // Top middle.
	indices[2] = 2;  // Bottom right.
	indices[3] = 3;  // Bottom left.
	indices[4] = 4;  // Top middle.
	indices[5] = 5;  // Bottom right.

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if(FAILED(result))
	{
		return false;
	}

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if(FAILED(result))
	{
		return false;
	}
	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete [] vertices;
	vertices = 0;

	delete [] indices;
	indices = 0;

	return true;
}

void Mesh::ShutdownBuffers()
{
	// Release the index buffer.
	if(m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	// Release the vertex buffer.
	if(m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	return;
}

void Mesh::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;


	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType); 
	offset = 0;
    
	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

Mesh::~Mesh(void)
{
}
