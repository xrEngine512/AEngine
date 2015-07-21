#include <DirectXMath.h>
#include <d3dcompiler.h>
#include "AbstractShader.h"
#include <fstream>
#include <ApproxSystemErrors.h>
#include <ApproxStatistics.h>

using namespace std;
using namespace DirectX;

AbstractShader::AbstractShader(void) :m_layout(nullptr), m_matrixBuffer(nullptr), m_pixelShader(nullptr), m_vertexShader(nullptr), m_sampleState(nullptr),
m_geometryShader(nullptr)
{
}

void AbstractShader::InitializeShader(ID3D11Device* device, WCHAR* vsFilename, WCHAR* psFilename, unsigned short NumberOfTextureSlots, WCHAR* gsFileName)
{
	HRESULT result;
	ID3DBlob* errorMessage ;
	ID3DBlob* vertexShaderBuffer;
	ID3DBlob* pixelShaderBuffer;
    ID3DBlob* geometryShaderBuffer = nullptr;
	
	// Initialize the pointers this function will use to null.
	errorMessage = 0;
	vertexShaderBuffer = 0;
	pixelShaderBuffer = 0;
	
	// Compile the vertex shader code.
	result = D3DCompileFromFile(vsFilename, NULL, NULL, "main", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &vertexShaderBuffer, &errorMessage);
	if(FAILED(result))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if(errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, vsFilename);
            throw ApproxException(wstring(L"Failed to compile Vertex Shader of ")+wstring(GetShaderDescWStr(GetType())),L"AbstractShader");
		}
		// If there was nothing in the error message then it simply could not find the shader file itself.

        throw ApproxException(wstring(L"Missing Vertex Shader File of ")+ wstring(GetShaderDescWStr(GetType())), L"AbstractShader");
	}

	// Compile the pixel shader code.
	result = D3DCompileFromFile(psFilename, NULL, NULL, "main", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &pixelShaderBuffer, &errorMessage);
	if(FAILED(result))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if(errorMessage)
		{
            OutputShaderErrorMessage(errorMessage, vsFilename);
            throw ApproxException(wstring(L"Failed to compile Pixel Shader of ") + wstring(GetShaderDescWStr(GetType())), L"AbstractShader");
        }
        // If there was nothing in the error message then it simply could not find the shader file itself.

        throw ApproxException(wstring(L"Missing Pixel Shader File of ") + wstring(GetShaderDescWStr(GetType())), L"AbstractShader");
	}
    if (gsFileName)
    {
        result = D3DCompileFromFile(gsFileName, NULL, NULL, "main", "gs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &geometryShaderBuffer, &errorMessage);
        if (FAILED(result))
        {
            // If the shader failed to compile it should have writen something to the error message.
            if (errorMessage)
            {
                OutputShaderErrorMessage(errorMessage, vsFilename);
                throw ApproxException(wstring(L"Failed to compile Geometry Shader of ") + wstring(GetShaderDescWStr(GetType())), L"AbstractShader");
            }
            // If there was nothing in the error message then it simply could not find the shader file itself.

            throw ApproxException(wstring(L"Missing Geometry Shader File of ") + wstring(GetShaderDescWStr(GetType())), L"AbstractShader");
        }        
    }
	// Create the vertex shader from the buffer.
	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_vertexShader);
	if(FAILED(result))
	{
        throw ApproxException(wstring(L"CreateVertexShader Failed of ") + wstring(GetShaderDescWStr(GetType())), L"AbstractShader");
	}

	// Create the pixel shader from the buffer.
	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_pixelShader);
	if(FAILED(result))
	{
        throw ApproxException(wstring(L"CreatePixelShader Failed of ") + wstring(GetShaderDescWStr(GetType())), L"AbstractShader");
	}

    if (geometryShaderBuffer)
    {
        result = device->CreateGeometryShader(geometryShaderBuffer->GetBufferPointer(), geometryShaderBuffer->GetBufferSize(),NULL, &m_geometryShader);
        if (FAILED(result))
        {
            throw ApproxException(wstring(L"CreateGeometryShader Failed of ") + wstring(GetShaderDescWStr(GetType())), L"AbstractShader");
        }
    }
	if(!CreateInputLayout(device, vertexShaderBuffer))
	{
        throw ApproxException(wstring(L"Couldn't create Input layout of ") + wstring(GetShaderDescWStr(GetType())), L"AbstractShader");
	}

    m_NumberOfTextureSlots = NumberOfTextureSlots;
    if (NumberOfTextureSlots)
        m_textures = new ID3D11ShaderResourceView*[NumberOfTextureSlots];

	// Release the vertex shader buffer and pixel shader buffer since they are no longer needed.
	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;

    DELETE_D3D11_OBJECT(geometryShaderBuffer)

	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;
	// Create a texture sampler state description.
	if(!CreateSamplerState(device))
	{
        throw ApproxException(wstring(L"Couldn't create Sampler state of ") + wstring(GetShaderDescWStr(GetType())), L"AbstractShader");
	}
	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	if(!SetupConstantBuffer(device))
	{
        throw ApproxException(wstring(L"Couldn't Setup Constant Buffer of ") + wstring(GetShaderDescWStr(GetType())), L"AbstractShader");
	}

	// Setup the description of the light dynamic constant buffer that is in the pixel shader.
	if(!SetupBuffers(device))
	{
        throw ApproxException(wstring(L"Couldn't Setup Buffers of ") + wstring(GetShaderDescWStr(GetType())), L"AbstractShader");
	}

}

ShaderErrors AbstractShader::SetParams(IShaderParams* params)
{
    if (!params)
        return SHDR_PARAMS_IS_NULL;
    m_basicParams = dynamic_cast<AbstractShaderParams*>(params);
    return SHDR_OK;
}

void AbstractShader::RenderShader(ID3D11DeviceContext* context)
{
    if (m_NumberOfTextureSlots)
        context->PSSetShaderResources(0, m_NumberOfTextureSlots, m_textures);
	// Set the vertex input layout.
    context->IASetInputLayout(m_layout);

	// Set the vertex and pixel shaders that will be used to render this triangle.
    context->VSSetShader(m_vertexShader, NULL, 0);
    
    context->GSSetShader(m_geometryShader, NULL, 0);

    context->PSSetShader(m_pixelShader, NULL, 0);
	if (m_sampleState)
        context->PSSetSamplers(0, 1, &m_sampleState);

	// Render the triangles.
    context->DrawIndexed(m_basicParams->indexCount, 0, 0);
    
    g_Statistics->IncDrawCalls();
	return;
}

bool AbstractShader::UpdateLight(ID3D11DeviceContext* context)
{
	return false;
}

bool AbstractShader::UpdateCamera(ID3D11DeviceContext* context)
{
	return false;
}

unsigned short AbstractShader::GetNumberOfTextureSlots()
{
    return m_NumberOfTextureSlots;
}

bool AbstractShader::SetupBuffers(ID3D11Device* device)
{
	return true;
}

bool AbstractShader::SetupConstantBuffer(ID3D11Device* device)
{
	D3D11_BUFFER_DESC matrixBufferDesc;
	HRESULT result;

	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	result = device->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer);
	if(FAILED(result))
	{
		return false;
	}
	return true;
}

bool AbstractShader::CreateSamplerState(ID3D11Device* device)
{
	return true;
}

bool AbstractShader::CreateInputLayout(ID3D11Device* device, ID3DBlob* vertexShaderBuffer)
{
	D3D11_INPUT_ELEMENT_DESC polygonLayout[4];
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

	polygonLayout[1].SemanticName = "COLOR";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	polygonLayout[2].SemanticName = "TEXCOORD";
	polygonLayout[2].SemanticIndex = 0;
	polygonLayout[2].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[2].InputSlot = 0;
	polygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[2].InstanceDataStepRate = 0;

	polygonLayout[3].SemanticName = "NORMAL";
	polygonLayout[3].SemanticIndex = 0;
	polygonLayout[3].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[3].InputSlot = 0;
	polygonLayout[3].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[3].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[3].InstanceDataStepRate = 0;

	// Get a count of the elements in the layout.
	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// Create the vertex input layout.
	result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), 
					   &m_layout);
    
	if(FAILED(result))
	{
		return false;
	}
	return true;
}

bool __vectorcall AbstractShader::SetShaderParameters(ID3D11DeviceContext* context,XMMATRIX& worldMatrix, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	unsigned int bufferNumber;
	// Transpose the matrices to prepare them for the shader.
	worldMatrix = XMMatrixTranspose(worldMatrix);
	viewMatrix = XMMatrixTranspose(viewMatrix);
	projectionMatrix = XMMatrixTranspose(projectionMatrix);
	// Lock the constant buffer so it can be written to.
	result = context->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if(FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the constant buffer.
	dataPtr = reinterpret_cast<MatrixBufferType*>(mappedResource.pData);

	// Copy the matrices into the constant buffer.
	dataPtr->world = worldMatrix;
	dataPtr->view = viewMatrix;
	dataPtr->projection = projectionMatrix;

	// Unlock the constant buffer.
    context->Unmap(m_matrixBuffer, 0);
	// Set the position of the constant buffer in the vertex shader.
	bufferNumber = 0;

	// Finanly set the constant buffer in the vertex shader with the updated values.
    context->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);

	return true;
}

bool AbstractShader::Render(ID3D11DeviceContext* context)
{
	bool result;


	// Set the shader parameters that it will use for rendering.
    result = SetShaderParameters(context, m_basicParams->worldMatrix, m_basicParams->viewMatrix, m_basicParams->projectionMatrix);
	if(!result)
	{
		return false;
	}

	// Now render the prepared buffers with the shader.
    RenderShader(context);

	return true;
}


bool AbstractShader::SetTexture(ID3D11ShaderResourceView* texture, unsigned int slot)
{
    if (slot < m_NumberOfTextureSlots)
    {
        m_textures[slot] = texture;
        return true;
    }
    return false;
}

void AbstractShader::ShutdownShader()
{
	// Release the matrix constant buffer.
    DELETE_D3D11_OBJECT(m_matrixBuffer)
    DELETE_D3D11_OBJECT(m_layout)
    DELETE_D3D11_OBJECT(m_sampleState)
    DELETE_D3D11_OBJECT(m_pixelShader)
    DELETE_D3D11_OBJECT(m_vertexShader)
	return;
}

void AbstractShader::OutputShaderErrorMessage(ID3DBlob* errorMessage, WCHAR* shaderFilename)
{
	char* compileErrors;
	unsigned long bufferSize, i;
	ofstream fout;


	// Get a pointer to the error message text buffer.
	compileErrors = (char*)(errorMessage->GetBufferPointer());

	// Get the length of the message.
	bufferSize = errorMessage->GetBufferSize();

	// Open a file to write the error message to.
	fout.open("shader-error.txt");

	// Write out the error message.
	for(i=0; i<bufferSize; i++)
	{
		fout << compileErrors[i];
	}

	// Close the file.
	fout.close();

	// Release the error message.
	errorMessage->Release();
	errorMessage = 0;
	return;
}

AbstractShader::~AbstractShader(void)
{
}
