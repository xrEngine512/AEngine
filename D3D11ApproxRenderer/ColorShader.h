#pragma once

#include "AbstractShader.h"

class ColorShader : public AbstractShader
{
	bool CreateInputLayout(ID3D11Device* device, ID3DBlob* vertexShaderBuffer);
public:
	ColorShader(void);
	~ColorShader(void);
	void Initialize(ID3D11Device*)override;
	ShaderDesc GetType()override;
	void Shutdown()override;
};

