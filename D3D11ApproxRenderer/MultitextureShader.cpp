#include "MultitextureShader.h"

bool MultitextureShader::CreateInputLayout(ID3D11Device* device, ID3DBlob* vertexShaderBuffer)
{
    D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
    unsigned int numElements;
    HRESULT result;
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

bool MultitextureShader::CreateSamplerState(ID3D11Device* device)
{
    HRESULT result;
    D3D11_SAMPLER_DESC samplerDesc;
    // Create a texture sampler state description.
    samplerDesc.Filter = D3D11_FILTER::D3D11_FILTER_COMPARISON_MIN_MAG_MIP_POINT;
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

void MultitextureShader::Initialize(ID3D11Device* device)
{
    InitializeShader(device, L"../../Core/D3D11ApproxRenderer/Shaders/MultitextureShader/VertexShader.hlsl", L"../../Core/D3D11ApproxRenderer/Shaders/MultitextureShader/PixelShader.hlsl", 2);
}

void MultitextureShader::Shutdown()
{
    DELETE_D3D11_OBJECT(m_sampleState)
    ShutdownShader();
}

ShaderType MultitextureShader::GetType()
{
    return MULTITEXTURE_SHADER;
}

MultitextureShader::MultitextureShader()
{
}


MultitextureShader::~MultitextureShader()
{
}
