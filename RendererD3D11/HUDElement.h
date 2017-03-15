#pragma once
#include "IInternalHUDElement.h"
#include "IExternalHUDElement.h"
#include "IShader.h"
#include "IMeshLoader.h"

#include <ApproxMath.h>
#include <atomic>
#include <string>

interface IShaderParams;
interface ITexture2D;

class HUDElement : public IInternalHUDElement, public IExternalHUDElement
{
    struct DefaultVertex
    {
        Math::AVector3 position;
        Math::AVector color;
        Math::AVector2 texture;
        Math::AVector3 normal;
    };
    Math::AMatrix m_orthoview;
    std::atomic<int> m_PosX, m_PosY;
    int m_indexCount;
    unsigned int m_Width, m_Height;
    std::atomic<float> m_ScaleX, m_ScaleY;
	ShaderSystem::IShader* m_Shader;
    IShaderParams* m_ShaderParams;
    ITexture2D* m_Texture;
    ID3D11Buffer* m_vertexBuffer, *m_indexBuffer;

    bool InitializeBuffers();
    void RenderBuffers();
    void ShutdownBuffers();
public:
    void SetCoord(int x, int y);
    void SetScale(float percentScaleX, float percentScaleY);

    bool Initialize(const std::string& textureFile, unsigned int width, unsigned int height);
    bool simdcall Render(AMatrix orthoMatrix);
    void Shutdown();
    HUDElement();
    virtual ~HUDElement();
};

