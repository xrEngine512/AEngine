#pragma once
#include "AbstractShader.h"
class SpecularShader : public AbstractShader
{
	
	struct CameraBufferType
	{
		XMFLOAT3 cameraPosition;
		float padding;
	};

	struct LightBufferType
	{
		XMFLOAT4 ambientColor;
		XMFLOAT4 diffuseColor;
		XMFLOAT3 lightDirection;
		float specularPower;
		XMFLOAT4 specularColor;
	};
	ID3D11Buffer* m_cameraBuffer;
	ID3D11Buffer* m_lightBuffer;
	SpecularShaderParams* m_params;
	bool CreateSamplerState(ID3D11Device* device);
	bool CreateInputLayout(ID3D11Device* device, ID3DBlob* vertexShaderBuffer);
	bool SetupBuffers(ID3D11Device* device);
public:
	SpecularShader();
	~SpecularShader();
	void Initialize(ID3D11Device*)override;
    ShaderErrors SetParams(IShaderParams* params)override;
    ShaderDesc GetType()override;
    bool UpdateLight(ID3D11DeviceContext* context)override;
    bool UpdateCamera(ID3D11DeviceContext* context)override;
    void Shutdown()override;
};

