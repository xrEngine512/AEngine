#pragma once
#include "AbstractShader.h"
class TextureShader :
	public AbstractShader
{
    bool CreateInputLayout(ID3D11Device* device, ID3DBlob* vertexShaderBuffer)override;
    bool CreateSamplerState(ID3D11Device* device)override;
public:
    void Initialize(ID3D11Device* device)override;
    void Shutdown()override;
    ShaderDesc GetType()override;
	TextureShader(void);
	~TextureShader(void);
};

