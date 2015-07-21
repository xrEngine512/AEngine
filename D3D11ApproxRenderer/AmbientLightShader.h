#pragma once
#include "AbstractShader.h"
class AmbientLightShader :
	public AbstractShader
{
	

	struct LightBufferType
	{
		XMFLOAT4 ambientColor;
		XMFLOAT4 diffuseColor;
		XMFLOAT3 lightDirection;
		float padding;  // Added extra padding so structure is a multiple of 16 for CreateBuffer function requirements.
	};

	ID3D11Buffer* m_lightBuffer;
	AmbientLightShaderParams* m_params;

	bool CreateSamplerState(ID3D11Device* device);
	bool CreateInputLayout(ID3D11Device* device, ID3DBlob* vertexShaderBuffer);
	bool SetupBuffers(ID3D11Device* device);
public:
	AmbientLightShader();
	~AmbientLightShader();
	void Initialize(ID3D11Device*) override;
    ShaderErrors SetParams(IShaderParams* params)override;
	ShaderDesc GetType()override;
    bool UpdateLight(ID3D11DeviceContext* context)override;
	void Shutdown()override;
};