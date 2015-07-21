#include "SpecularShader.h"


SpecularShader::SpecularShader() :m_cameraBuffer(nullptr), m_lightBuffer(nullptr)
{
}

void SpecularShader::Initialize(ID3D11Device* device)
{
    InitializeShader(device, L"../../Core/D3D11ApproxRenderer/Shaders/SpecularShader/VertexShader.hlsl", L"../../Core/D3D11ApproxRenderer/Shaders/SpecularShader/PixelShader.hlsl", 1);
}

ShaderErrors SpecularShader::SetParams(IShaderParams* params)
{
    auto res = AbstractShader::SetParams(params);
    if (res != SHDR_OK)
        return res;
    m_params = shader_params_cast<SpecularShaderParams*>(params);
    if (!m_params)
        return SHDR_WRONG_PARAMS_TYPE;
    else
        return SHDR_OK;
}

bool SpecularShader::UpdateLight(ID3D11DeviceContext* context)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	unsigned int bufferNumber;
	LightBufferType* dataPtr;

	// Lock the light constant buffer so it can be written to.
	result = context->Map(m_lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the constant buffer.
	dataPtr = (LightBufferType*)mappedResource.pData;

	// Copy the lighting variables into the constant buffer.
	dataPtr->diffuseColor = m_params->lighting.diffuseColor;
	dataPtr->ambientColor = m_params->lighting.ambientColor;
	dataPtr->lightDirection = m_params->lighting.lightDirection;
	dataPtr->specularColor = m_params->specular.specularColor;
	dataPtr->specularPower = m_params->specular.specularPower;

	// Unlock the constant buffer.
	context->Unmap(m_lightBuffer, 0);

	// Set the position of the light constant buffer in the pixel shader.
	bufferNumber = 0;

	// Finally set the light constant buffer in the pixel shader with the updated values.
	context->PSSetConstantBuffers(bufferNumber, 1, &m_lightBuffer);

	return true;
}

bool SpecularShader::UpdateCamera(ID3D11DeviceContext* context)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	unsigned int bufferNumber;
	CameraBufferType* dataPtr;

	result = context->Map(m_cameraBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}
	dataPtr = (CameraBufferType*)mappedResource.pData;
	dataPtr->cameraPosition = m_params->cameraPosition;
	dataPtr->padding = 0.0f;

	context->Unmap(m_cameraBuffer, 0);

	bufferNumber = 1;
	context->PSSetConstantBuffers(bufferNumber, 1, &m_cameraBuffer);
	return true;
}

bool SpecularShader::CreateSamplerState(ID3D11Device* device)
{
	HRESULT result;
	D3D11_SAMPLER_DESC samplerDesc;
	// Create a texture sampler state description.
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// Create the texture sampler state.
	result = device->CreateSamplerState(&samplerDesc, &m_sampleState);
	if (FAILED(result))
	{
		return false;
	}
	return true;
}

bool SpecularShader::CreateInputLayout(ID3D11Device* device, ID3DBlob* vertexShaderBuffer)
{
	D3D11_INPUT_ELEMENT_DESC polygonLayout[3];
	HRESULT result;
	unsigned int numElements;
	// Create the vertex input layout description.
	// This setup needs to match the VertexType stucture in the ModelClass and in the shader.
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "TEXCOORD";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = SVO_TEXCOORD;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	polygonLayout[2].SemanticName = "NORMAL";
	polygonLayout[2].SemanticIndex = 0;
	polygonLayout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[2].InputSlot = 0;
	polygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[2].InstanceDataStepRate = 0;

	// Get a count of the elements in the layout.
	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// Create the vertex input layout.
	result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(),
		&m_layout);
	if (FAILED(result))
	{
		return false;
	}
	return true;
}

bool SpecularShader::SetupBuffers(ID3D11Device* device)
{
	D3D11_BUFFER_DESC BufferDesc;
	HRESULT result;
	// Setup the description of the light dynamic constant buffer that is in the pixel shader.
	// Note that ByteWidth always needs to be a multiple of 16 if using D3D11_BIND_CONSTANT_BUFFER or CreateBuffer will fail.
	BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	BufferDesc.ByteWidth = sizeof(LightBufferType);
	BufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	BufferDesc.MiscFlags = 0;
	BufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	result = device->CreateBuffer(&BufferDesc, NULL, &m_lightBuffer);
	if (FAILED(result))
	{
		return false;
	}

	BufferDesc.ByteWidth = sizeof(CameraBufferType);
	result = device->CreateBuffer(&BufferDesc, NULL, &m_cameraBuffer);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void SpecularShader::Shutdown()
{
	if (m_cameraBuffer)
	{
		m_cameraBuffer->Release();
		m_cameraBuffer = nullptr;
	}

	if (m_lightBuffer)
	{
		m_lightBuffer->Release();
		m_lightBuffer = nullptr;
	}
	ShutdownShader();
}

ShaderDesc SpecularShader::GetType()
{
	return SPECULAR_SHADER;
}

SpecularShader::~SpecularShader()
{
}
