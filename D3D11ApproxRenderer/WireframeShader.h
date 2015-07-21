#pragma once
#include "AbstractShader.h"

class WireframeShader : public AbstractShader
{
    bool CreateSamplerState(ID3D11Device* device)override;
    bool CreateInputLayout(ID3D11Device* device, ID3DBlob* vertexShaderBuffer)override;
public:

    WireframeShader();
    void Initialize(ID3D11Device* device)override;
    ShaderDesc GetType()override;
    void Shutdown()override;
    ~WireframeShader();
};

