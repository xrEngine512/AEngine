#include "UnifiedShader.h"
#include "ACSReader.h"
#include <ShaderPackFile.h>
#include <ApproxStatistics.h>
#include <ApproxSystemTools.h>
#include "MaterialInterface.h"
#include "MaterialInterfaceManager.h"
#include "RGS.h"
#include <RuntimeBufferInfo.h>

using namespace MatInterface;

UnifiedShader::UnifiedShader(const wchar_t *acs_filename, const ShaderDesc& desc) :m_desc(desc), m_vertexShader(nullptr), m_pixelShader(nullptr),
m_geometryShader(nullptr), m_computeShader(nullptr), m_hullShader(nullptr), m_domainShader(nullptr), m_layout(nullptr), m_sampleState(nullptr),
m_textures(nullptr), m_NumberOfTextureSlots(0), PS_buffers(), VS_buffers()
{
    m_reader = new ACSReader(acs_filename);
}

void UnifiedShader::Initialize(ID3D11Device* device)
{
	char* sm;
	bool end_of_file(false);
	ShaderElement VS_Element;

	vector<D3D11_INPUT_ELEMENT_DESC> inputLayout;

	while (!end_of_file)
	{
		ShaderElement current_element = m_reader->NextElement();

		switch (current_element.m_MetaData.ID)
		{
		case SHADER_NAME:
			m_desc.name.assign((char*)current_element.Data);
			break;
		case COMPILED_VS:
		{
			VS_Element = current_element;
			short num = 0;
			for (auto buffer : current_element.FindMany(RUNTIME_BUFFER_INFO))
			{
				ASL::RuntimeBufferInfo info;
				info.Deserialize(buffer->Data, buffer->m_MetaData.size);
				InitializeBuffer(device, info, VS_buffers, num);
				num++;
			}
			device->CreateVertexShader(current_element.Data, current_element.m_MetaData.size, NULL, &m_vertexShader);
			break;
		}
		case COMPILED_PS:
		{
			short num = 0;
			for (auto buffer : current_element.FindMany(RUNTIME_BUFFER_INFO))
			{
				ASL::RuntimeBufferInfo info;
				info.Deserialize(buffer->Data, buffer->m_MetaData.size);
				InitializeBuffer(device, info, PS_buffers, num);
				num++;
			}
			device->CreatePixelShader(current_element.Data, current_element.m_MetaData.size, NULL, &m_pixelShader);
			break;
		}
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
		case INPUT_LAYOUT_ELEMENT:
		{			
			D3D11_INPUT_ELEMENT_DESC elementDesc;
			D3D11_INPUT_LAYOUT_ELEMENT element;
			current_element.Get(element);
			elementDesc = element.ToElemDesc();
			current_element.Find(IL_SEMANTIC_NAME)->Get(elementDesc.SemanticName);
			inputLayout.push_back(elementDesc);
			break;
		}
		case ID_NONE:
		{
			end_of_file = true;
			break;
		}
		}
	}
	delete m_reader;
	m_reader = nullptr;
	auto result = device->CreateInputLayout(inputLayout.data(), inputLayout.size(), VS_Element.Data, VS_Element.m_MetaData.size, &m_layout);
	m_textures = new ID3D11ShaderResourceView*[m_NumberOfTextureSlots];		
}

void UnifiedShader::Shutdown()
{
	DELETE_D3D11_OBJECT(VS_buffers.m_perObjectBuffer.m_Buffer);
	DELETE_D3D11_OBJECT(VS_buffers.m_perFrameBuffer.m_Buffer);
	DELETE_D3D11_OBJECT(VS_buffers.m_sceneConstants.m_Buffer);

	//Delete RGS

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

const ShaderDesc& UnifiedShader::GetDesc()
{
    return m_desc;
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

void UnifiedShader::InitializeBuffer(ID3D11Device* device, const ASL::RuntimeBufferInfo& info, BufferPack& pack, short BufferNum)
{
	auto vars = g_materialInterfaceManager->GetMaterialVariablesPtr(info.IDs);
	
	D3D11_BUFFER_DESC desc;

	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;

	HRESULT res;
	
	switch (info.group)
	{
	case SCENE_CONSTANT:
	{		
		pack.m_sceneConstantsData = new RGS(vars);
		pack.m_sceneConstants.num = BufferNum;
		desc.ByteWidth = pack.m_sceneConstantsData->Size();
		res = device->CreateBuffer(&desc, nullptr, &pack.m_sceneConstants.m_Buffer);
		break;
	}
	case PER_FRAME:
	{
		pack.m_perFrameData = new RGS(vars);
		pack.m_perFrameBuffer.num = BufferNum;
		desc.ByteWidth = pack.m_perFrameData->Size();
		res = device->CreateBuffer(&desc, nullptr, &pack.m_perFrameBuffer.m_Buffer);
		break;
	}
	case PER_OBJECT:
	{
		pack.m_perObjectData = new RGS(vars);
		pack.m_perObjectBuffer.num = BufferNum;
		desc.ByteWidth = pack.m_perObjectData->Size();
		res = device->CreateBuffer(&desc, nullptr, &pack.m_perObjectBuffer.m_Buffer);
		break;
	}
	default:
		break;
	}
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

bool UnifiedShader::UpdateSceneConstantsBuffers(ID3D11DeviceContext* context)
{
	if (VS_buffers.m_sceneConstants.m_Buffer)
		if (!UpdateBuffer(context, VS_buffers.m_sceneConstants.m_Buffer, VS_buffers.m_sceneConstantsData))
			return false;
	if (PS_buffers.m_sceneConstants.m_Buffer)
		if (!UpdateBuffer(context, PS_buffers.m_sceneConstants.m_Buffer, PS_buffers.m_sceneConstantsData))
			return false;
	return true;
}

bool UnifiedShader::UpdatePerFrameBuffers(ID3D11DeviceContext* context)
{
	if (VS_buffers.m_perFrameBuffer.m_Buffer)
		if (!UpdateBuffer(context, VS_buffers.m_perFrameBuffer.m_Buffer, VS_buffers.m_perFrameData))
			return false;
	if (PS_buffers.m_perFrameBuffer.m_Buffer)
		if (!UpdateBuffer(context, PS_buffers.m_perFrameBuffer.m_Buffer, PS_buffers.m_perFrameData))
			return false;
	return true;
}

bool UnifiedShader::UpdatePerObjectBuffers(ID3D11DeviceContext* context)
{
	if (VS_buffers.m_perObjectBuffer.m_Buffer)
		if (!UpdateBuffer(context, VS_buffers.m_perObjectBuffer.m_Buffer, VS_buffers.m_perObjectData))
			return false;
	if (PS_buffers.m_perObjectBuffer.m_Buffer)
		if (!UpdateBuffer(context, PS_buffers.m_perObjectBuffer.m_Buffer, PS_buffers.m_perObjectData))
			return false;
	return true;
}

bool UnifiedShader::Render(ID3D11DeviceContext* context, unsigned int indexCount)
{
    HRESULT result;    
   
    unsigned int bufferNumber;

	if (!UpdatePerObjectBuffers(context))
		return false;	

	if (VS_buffers.m_sceneConstants.m_Buffer)
		context->VSSetConstantBuffers(VS_buffers.m_sceneConstants.num, 1, &VS_buffers.m_sceneConstants.m_Buffer);
	if (PS_buffers.m_sceneConstants.m_Buffer)
		context->PSSetConstantBuffers(PS_buffers.m_sceneConstants.num, 1, &PS_buffers.m_sceneConstants.m_Buffer);
	if (VS_buffers.m_perFrameBuffer.m_Buffer)
		context->VSSetConstantBuffers(VS_buffers.m_perFrameBuffer.num, 1, &VS_buffers.m_perFrameBuffer.m_Buffer);
	if (PS_buffers.m_perFrameBuffer.m_Buffer)
		context->PSSetConstantBuffers(PS_buffers.m_perFrameBuffer.num, 1, &PS_buffers.m_perFrameBuffer.m_Buffer);
	if (VS_buffers.m_perObjectBuffer.m_Buffer)
		context->VSSetConstantBuffers(VS_buffers.m_perObjectBuffer.num, 1, &VS_buffers.m_perObjectBuffer.m_Buffer);
	if (PS_buffers.m_perObjectBuffer.m_Buffer)
		context->PSSetConstantBuffers(PS_buffers.m_perObjectBuffer.num, 1, &PS_buffers.m_perObjectBuffer.m_Buffer);

    if (m_NumberOfTextureSlots)
        context->PSSetShaderResources(0, m_NumberOfTextureSlots, m_textures);
    // Set the vertex input layout.
	context->IASetInputLayout(m_layout);

    // Set the vertex and pixel shaders that will be used to render this triangle.
    context->VSSetShader(m_vertexShader, NULL, 0);

	if (m_geometryShader)
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
