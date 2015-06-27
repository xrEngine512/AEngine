#pragma once
#include "AbstractShader.h"
class LightShader :
	public AbstractShader
{
	struct LightBufferType
	{
		XMFLOAT4 diffuseColor;
		XMFLOAT3 lightDirection;
		float padding;  // Added extra padding so structure is a multiple of 16 for CreateBuffer function requirements.
	};
	bool CreateInputLayout(ID3D11Device* device, ID3DBlob* vertexShaderBuffer);
	bool CreateSamplerState(ID3D11Device* device);
	bool SetupBuffers(ID3D11Device* device);
	ID3D11Buffer* m_lightBuffer;
public:
    void SetTexture(ID3D11DeviceContext* context, ID3D11ShaderResourceView *texture);
	ShaderErrors Initialize(ID3D11Device* device, HWND hwnd, IShaderParams* params);
    bool UpdateLight(ID3D11DeviceContext* context);
	void Shutdown();
	ShaderType GetType();
	LightShader(void);
	~LightShader(void);
};

