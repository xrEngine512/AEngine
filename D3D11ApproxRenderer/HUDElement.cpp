#include <DirectXMath.h>
using namespace DirectX;
#include "HUDElement.h"
#include "VertexTypes.h"
#include "D3D11Common.h"
#include "ShaderParams.h"
#include "ApproxSystemTools.h"
#include "ObjMeshLoader.h"
#include "ShaderPool.h"

HUDElement::HUDElement() :m_PosX(0), m_PosY(0), m_ScaleX(100), m_ScaleY(100), m_vertexBuffer(nullptr), m_indexBuffer(nullptr), m_Width(0), m_Height(0), m_Texture(nullptr)
{
}

bool HUDElement::Initialize(ID3D11Device* device, HWND hwnd, wchar_t* texture, unsigned int width, unsigned int height)
{
    XMFLOAT4X4 tmp;
    tmp = 
    {
        -1,0,0,0,
        0,1,0,0,
        0,0,-1,0,
        0,0,5,1
    };
    m_orthoview = XMLoadFloat4x4(&tmp);
    m_Height = height;
    m_Width = width;
    m_Texture = new D3DTexture;
    m_Texture->Initialize(device, texture);
    m_Shader = g_ShaderPool->GetShader(TEXTURE_SHADER);
    m_ShaderParams = ShaderParamsFactory(TEXTURE_SHADER);    
    m_Shader->SetTexture(m_Texture->GetTexture(),0);
    /*m_MeshLoader = new ObjMeshLoader;
    m_MeshLoader->Inititalize(device, "../../Core/Engine/Resources/Meshes/HUDMesh.obj");
    m_MeshLoader->GetBuffers(m_vertexBuffer, m_indexBuffer);
    m_indexCount = m_MeshLoader->GetIndexCount();
    if (m_MeshLoader)
    {
        delete m_MeshLoader;
        m_MeshLoader = nullptr;
    }*/
    return InitializeBuffers(device);
}

bool HUDElement::InitializeBuffers(ID3D11Device* device)
{
    PosTexVertStruct *vertices = new PosTexVertStruct[6];

    float bottom, top, right, left;
    bottom = static_cast<float>(m_PosY)+static_cast<float>(m_Height) / 2;
    top = static_cast<float>(m_PosY)-static_cast<float>(m_Height) / 2;
    right = static_cast<float>(m_PosX)+static_cast<float>(m_Width) / 2;
    left = static_cast<float>(m_PosX)-static_cast<float>(m_Width) / 2;

    vertices[0].position = XMFLOAT3(left, top, 1.0f);    //Top left
    vertices[0].texture = XMFLOAT2(0.0f, 0.0f);

    vertices[1].position = XMFLOAT3(right, bottom, 1.0f);    //Bottom right
    vertices[1].texture = XMFLOAT2(1.0f, 1.0f);

    vertices[2].position = XMFLOAT3(left, bottom, 1.0f);    //Bottom left
    vertices[2].texture = XMFLOAT2(0.0f, 1.0f);

    vertices[3].position = XMFLOAT3(left, top, 1.0f);  // Top left.       
    vertices[3].texture = XMFLOAT2(0.0f, 0.0f);

    vertices[4].position = XMFLOAT3(right, top, 1.0f);  // Top right.
    vertices[4].texture = XMFLOAT2(1.0f, 0.0f);

    vertices[5].position = XMFLOAT3(right, bottom, 1.0f);  // Bottom right.
    vertices[5].texture = XMFLOAT2(1.0f, 1.0f);

    m_vertexBuffer = D3D11Common::InitializeStaticVertexBuffer(device, vertices, 6);
    delete[] vertices;
    if (!m_vertexBuffer)
        return false;

    unsigned long* indices = new unsigned long[6];
    if (!indices)
    {
        return false;
    }

    for (int i = 0; i<6; i++)
    {
        indices[i] = i;
    }

    m_indexBuffer = D3D11Common::InitializeIndexBuffer(device, indices, 6);
    delete[] indices;
    if (!m_indexBuffer)
        return false;
    return true;
}

void HUDElement::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
    unsigned int stride;
    unsigned int offset;


    // Set vertex buffer stride and offset.
    stride = sizeof(PosTexVertStruct);
    offset = 0;

    // Set the vertex buffer to active in the input assembler so it can be rendered.
    deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

    // Set the index buffer to active in the input assembler so it can be rendered.
    deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

    // Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
    deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    return;
}

void HUDElement::ShutdownBuffers()
{
    // Release the index buffer.
    if (m_indexBuffer)
    {
        m_indexBuffer->Release();
        m_indexBuffer = 0;
    }

    // Release the vertex buffer.
    if (m_vertexBuffer)
    {
        m_vertexBuffer->Release();
        m_vertexBuffer = 0;
    }

    return;
}

void HUDElement::SetCoord(int x, int y)
{
    m_PosX.store(-x);
    m_PosY.store(-y);
}



void HUDElement::SetScale(float percentScaleX, float percentScaleY)
{
    m_ScaleX.store(percentScaleX);
    m_ScaleY.store(percentScaleY);
}

bool __vectorcall HUDElement::Render(ID3D11DeviceContext* context, XMMATRIX orthoMatrix)
{
    TextureShaderParams* params = dynamic_cast<TextureShaderParams*>(m_ShaderParams);
    params->worldMatrix = XMMatrixScaling(m_ScaleX.load() / 100, m_ScaleY.load() / 100, 1.0f)*XMMatrixRotationZ(XM_PI)*XMMatrixTranslation(m_PosX.load(), m_PosY.load(), 1);
    params->indexCount = 6;
    params->projectionMatrix = orthoMatrix;
    params->viewMatrix = m_orthoview;
    RenderBuffers(context);
    return m_Shader->Render(context,6);
}

void HUDElement::Shutdown()
{
    DELETE_SYS_OBJECT(m_Texture);
    ShutdownBuffers();
}

HUDElement::~HUDElement()
{
}
