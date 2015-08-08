#pragma once
#include "IInternalHUDElement.h"
#include "IExternalHUDElement.h"
#include "IShader.h"
#include <atomic>
#include "D3DTexture.h"
#include "IMeshLoader.h"

class HUDElement : public IInternalHUDElement, public IExternalHUDElement
{
    struct VertexType
    {
        XMFLOAT3 position;
        XMFLOAT4 color;
        XMFLOAT2 texture;
        XMFLOAT3 normal;
    };
    XMMATRIX m_orthoview;
    std::atomic<int> m_PosX, m_PosY;
    int m_indexCount;
    unsigned int m_Width, m_Height;
    std::atomic<float> m_ScaleX, m_ScaleY;
	ShaderSystem::IShader* m_Shader;
    IShaderParams* m_ShaderParams;
    D3DTexture* m_Texture;
    IMeshLoader *m_MeshLoader;
    ID3D11Buffer* m_vertexBuffer, *m_indexBuffer;

    bool InitializeBuffers(ID3D11Device* device);
    void RenderBuffers(ID3D11DeviceContext* context);
    void ShutdownBuffers();
public:
    void SetCoord(int x, int y);
    void SetScale(float percentScaleX, float percentScaleY);

    bool Initialize(ID3D11Device* device,HWND hwnd, wchar_t* textureFile, unsigned int width, unsigned int height);
    bool __vectorcall Render(ID3D11DeviceContext *deviceContext, XMMATRIX orthoMatrix);
    void Shutdown();
    HUDElement();
    virtual ~HUDElement();
};

