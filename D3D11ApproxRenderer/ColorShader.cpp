#include <d3dcompiler.h>

#include "ColorShader.h"


ColorShader::ColorShader(void)
{
}


ColorShader::~ColorShader(void)
{
}

void ColorShader::Initialize(ID3D11Device* device)
{
	InitializeShader(device, L"../../Core/D3D11ApproxRenderer/Shaders/ColorShader/VertexShader.hlsl", L"../../Core/D3D11ApproxRenderer/Shaders/ColorShader/PixelShader.hlsl",0);
}


bool ColorShader::CreateInputLayout(ID3D11Device* device, ID3DBlob* vertexShaderBuffer)
{
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	unsigned int numElements;

	HRESULT result;
	
	// Now setup the layout of the data that goes into the shader.
	// This setup needs to match the VertexType stucture in the ModelClass and in the shader.
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "COLOR";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = SVO_COLOR;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;
	// Get a count of the elements in the layout.
	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// Create the vertex input layout.
	result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), 
					   vertexShaderBuffer->GetBufferSize(), &m_layout);
	if(FAILED(result))
	{
		return false;
	}
	return true;
}

ShaderDesc ColorShader::GetType()
{
	return COLOR_SHADER;
}

void ColorShader::Shutdown()
{
	ShutdownShader();
}


