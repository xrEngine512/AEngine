#include "UnifiedShader.h"
#include "ACSReader.h"
#include <ShaderPackFile.h>
#include <ApproxStatistics.h>
#include <ApproxSystemTools.h>
#include "MaterialInterface.h"
#include "RGS.h"

using namespace MatInterface;

UnifiedShader::UnifiedShader(wchar_t *acs_filename) :m_computeShader(nullptr), m_NumberOfTextureSlots(0),
m_vertexShader(nullptr), m_pixelShader(nullptr), m_geometryShader(nullptr), m_hullShader(nullptr), m_domainShader(nullptr), m_layout(nullptr), m_perObjectBuffer(nullptr),
m_sampleState(nullptr), m_textures(nullptr)
{
    m_reader = new ACSReader(acs_filename);
}

void UnifiedShader::Initialize(ID3D11Device* device)
{
	char* sm;
	bool end_of_file(false);
	bool t(false);
	ShaderElement VS_Element;
	while (!end_of_file)
	{
		ShaderElement current_element = m_reader->NextElement();

		switch (current_element.m_MetaData.ID)
		{
		case SHADER_NAME:
			m_ShaderName.assign((char*)current_element.Data);
			break;
		case COMPILED_VS:
			VS_Element = current_element;
			device->CreateVertexShader(current_element.Data, current_element.m_MetaData.size, NULL, &m_vertexShader);
			break;
		case COMPILED_PS:
			if (!t)		//# Костыль начинается
				device->CreatePixelShader(current_element.Data, current_element.m_MetaData.size, NULL, &m_pixelShader);
			t = true;	//Костыль кончается
			break;
		case COMPILED_GS:
			device->CreateGeometryShader(current_element.Data, current_element.m_MetaData.size, NULL, &m_geometryShader);
			break;
		case COMPILED_CS:
			device->CreateComputeShader(current_element.Data, current_element.m_MetaData.size, NULL, &m_computeShader);
			break;
		case COMPILED_HS:
			device->CreateHullShader(current_element.Data, current_element.m_MetaData.size, NULL, &m_hullShader);
			break;
		case COMPILED_DS:
			device->CreateDomainShader(current_element.Data, current_element.m_MetaData.size, NULL, &m_domainShader);
			break;
		case IL_SEMANTIC_NAME:
			sm = new char[current_element.m_MetaData.size];
			memcpy(sm, current_element.Data, current_element.m_MetaData.size);
			m_semanticNames.push_back(sm);
			break;
		case INPUT_LAYOUT:
		{
			HRESULT result;
			int numOfElems = current_element.m_MetaData.size / sizeof(D3D11_INPUT_ELEMENT_DESC);
			D3D11_INPUT_ELEMENT_DESC* inputLayout = new D3D11_INPUT_ELEMENT_DESC[numOfElems];

			D3D11_INPUT_LAYOUT_ELEMENT* pElement = static_cast<D3D11_INPUT_LAYOUT_ELEMENT*>(current_element.Data);
			for (int i = 0; i < numOfElems; i++)
			{
				inputLayout[i] = pElement[i].ToElemDesc();
			}
			int i(0);
			for (auto semName : m_semanticNames)
			{
				inputLayout[i++].SemanticName = semName;
			}
			result = device->CreateInputLayout(inputLayout, m_semanticNames.size(), VS_Element.Data, VS_Element.m_MetaData.size, &m_layout);

			break;
		}
		case ID_NONE:
		{
			end_of_file = true;
			break;
		}
		}
	}
	D3D11_BUFFER_DESC matrixBufferDesc;
	HRESULT result;

	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	//result = device->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer);

	m_textures = new ID3D11ShaderResourceView*[m_NumberOfTextureSlots];
}

void UnifiedShader::Shutdown()
{
	DELETE_D3D11_OBJECT(m_perObjectBuffer);
    DELETE_D3D11_OBJECT(m_layout);
    DELETE_D3D11_OBJECT(m_sampleState);
    DELETE_D3D11_OBJECT(m_pixelShader);
    DELETE_D3D11_OBJECT(m_vertexShader);
    DELETE_D3D11_OBJECT(m_geometryShader);
    DELETE_D3D11_OBJECT(m_computeShader);
    DELETE_D3D11_OBJECT(m_hullShader);
    DELETE_D3D11_OBJECT(m_domainShader);
    for (int i = 0; i < m_NumberOfTextureSlots; i++)
    {
        DELETE_D3D11_OBJECT(m_textures[i]);
    }
}

ShaderType UnifiedShader::GetType()
{
    return UNIFIED_SHADER;
}

unsigned short UnifiedShader::GetNumberOfTextureSlots()
{
    return m_NumberOfTextureSlots;
}

bool UnifiedShader::SetTexture(ID3D11ShaderResourceView* texture, unsigned slot)
{
    if (slot < m_NumberOfTextureSlots)
    {
        m_textures[slot] = texture;
        return true;
    }
    return false;
}

bool UnifiedShader::UpdateBuffer(ID3D11DeviceContext* context, ID3D11Buffer* buffer, RGS *data)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT result = context->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}
	*data = mappedResource.pData;
	data->Update();
	context->Unmap(buffer, 0);
	return true;
}

bool UnifiedShader::UpdateSceneConstants(ID3D11DeviceContext* context)
{
	if (m_sceneConstants)
	{
		if (!UpdateBuffer(context, m_sceneConstants, m_sceneConstantsData))
			return false;
	}
	return true;
}

bool UnifiedShader::UpdatePerFrameBuffer(ID3D11DeviceContext* context)
{
	if (m_perFrameBuffer)
	{
		if (!UpdateBuffer(context, m_perFrameBuffer, m_perFrameData))
			return false;
	}
	return true;
}

bool UnifiedShader::UpdatePerObjectBuffer(ID3D11DeviceContext* context)
{
	if (m_perObjectBuffer)
	{
		if (!UpdateBuffer(context, m_perObjectBuffer, m_perObjectData))
			return false;
	}
	return true;
}

bool UnifiedShader::Render(ID3D11DeviceContext* context, unsigned int indexCount)
{
    HRESULT result;
    
    MatrixBufferType* dataPtr;
    unsigned int bufferNumber;
    // Transpose the matrices to prepare them for the shader.
   /* m_basicParams->worldMatrix = XMMatrixTranspose(m_basicParams->worldMatrix);
    m_basicParams->viewMatrix = XMMatrixTranspose(m_basicParams->viewMatrix);
    m_basicParams->projectionMatrix = XMMatrixTranspose(m_basicParams->projectionMatrix);*/
    // Lock the constant buffer so it can be written to.
	if (!UpdatePerObjectBuffer(context))
		return false;
    // Set the position of the constant buffer in the vertex shader.
    bufferNumber = 0;

    // Finanly set the constant buffer in the vertex shader with the updated values.
	if (m_perObjectBuffer)
	{
		context->VSSetConstantBuffers(bufferNumber, 1, &m_perObjectBuffer);
		bufferNumber++;
	}
	if (m_perFrameBuffer)
	{
		context->VSSetConstantBuffers(bufferNumber, 1, &m_perFrameBuffer);
		bufferNumber++;
	}
	if (m_sceneConstants)
	{
		context->VSSetConstantBuffers(bufferNumber, 1, &m_sceneConstants);
	}

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
    context->DrawIndexed(indexCount, 0, 0);

    g_Statistics->IncDrawCalls();
    return true;
}

UnifiedShader::~UnifiedShader()
{
}
