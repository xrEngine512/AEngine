#include "UnifiedShader.h"
#include <ShaderPackFile.h>
#include <ApproxStatistics.h>
#include <ApproxSystemTools.h>
#include "MaterialInterface.h"
#include "MaterialInterfaceManager.h"
#include "MemoryMultiplexer.h"
#include <RuntimeBufferInfo.h>
#include <TextureInfo.h>
#include <ShaderParamInfo.h>
#include "GenericStruct.h"
#include "GenericVariable.h"

using namespace MatInterface;
using namespace ASL;

namespace ShaderSystem
{
	UnifiedShader::UnifiedShader(const wchar_t *acs_filename, const ShaderDesc& desc) :m_desc(desc), m_vertexShader(nullptr), m_pixelShader(nullptr),
		m_geometryShader(nullptr), m_computeShader(nullptr), m_hullShader(nullptr), m_domainShader(nullptr), m_layout(nullptr), VS_buffers(),
		PS_buffers(), m_sampleState(nullptr), m_textures(nullptr)
	{
		m_reader = new ACSReader(acs_filename);
	}

	void UnifiedShader::SetParameter(const std::string& name, const floatVariant& value)
	{
		(*m_ParamsData)[name] = value;
	}

	void UnifiedShader::LoadBuffers(ID3D11Device* device, const ShaderElement& elem, BufferPack& pack)
	{
		short num = 0;
		vector<int> ParamIDs, TextureSlots;
		void* ptr;
		int size = elem.Find(SHADER_SETS)->Get(ptr);
		AbstractSaveData::Deserialize(ptr, size, ParamIDs, TextureSlots);
		for (auto buffer : elem.FindMany(RUNTIME_BUFFER_INFO))
		{
			RuntimeBufferInfo info;
			buffer->Get(info);
			
			InitializeBuffer(device, info, pack, num, ParamIDs);
			num++;
		}
	}

	void UnifiedShader::LoadShaderParams()
	{
		if (m_Settings.ParamsInfo.size())
		{
			vector<pair<GenericType, string>> TypesAndNames;
			TypesAndNames.reserve(m_Settings.ParamsInfo.size());
			for (auto info : m_Settings.ParamsInfo)
			{
				TypesAndNames.push_back(pair<GenericType, string>(MaterialInterfaceManager::NormalizeType(info.Type), info.Name));
				break;
			}
			m_ParamsData = new GenericStruct(TypesAndNames);
		}
	}


	void UnifiedShader::Initialize(ID3D11Device* device)
	{
		bool end_of_file(false);
		ShaderElement VS_Element, PS_Element;

		vector<D3D11_INPUT_ELEMENT_DESC> inputLayout;

		while (!end_of_file)
		{
			const ShaderElement& current_element = m_reader->NextElement();

			switch (current_element.m_MetaData.ID)
			{
			case SHADER_NAME:
				m_desc.name.assign((char*)current_element.Data);
				break;
			case TEXTURE_DESC:
			{
				TextureInfo info;
				current_element.Get(info);
				m_Settings.TexturesInfo.push_back(info);
				break;
			}
			case PARAM_DESC:
			{
				ShaderParamInfo info;
				current_element.Get(info);
				m_Settings.ParamsInfo.push_back(info);
				break;
			}
			case COMPILED_VS:
			{
				VS_Element = current_element;
				auto res = device->CreateVertexShader(current_element.Data, current_element.m_MetaData.size, NULL, &m_vertexShader);
				break;
			}
			case COMPILED_PS:
			{
				PS_Element = current_element;
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
			default:
			case ID_NONE:
			{
				end_of_file = true;
				break;
			}
			}
		}
		//Init shader parameters
	{
		LoadShaderParams();
	}

	LoadBuffers(device, VS_Element, VS_buffers);
	LoadBuffers(device, PS_Element, PS_buffers);

	m_desc.TextureSlots = m_Settings.TexturesInfo.size();
	if (m_desc.TextureSlots)
	{
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
		device->CreateSamplerState(&samplerDesc, &m_sampleState);
	}
	delete m_reader;
	m_reader = nullptr;
	auto result = device->CreateInputLayout(inputLayout.data(), inputLayout.size(), VS_Element.Data, VS_Element.m_MetaData.size, &m_layout);
	if (FAILED(result)) throw ApproxException(L"Ошибка при создании Input layout");
	m_textures = static_cast<ID3D11ShaderResourceView**>(calloc(1, sizeof(void*)*m_desc.TextureSlots));
	}

	void UnifiedShader::Shutdown()
	{
		DELETE_D3D11_OBJECT(m_layout);
		DELETE_D3D11_OBJECT(m_sampleState);
		DELETE_D3D11_OBJECT(m_pixelShader);
		DELETE_D3D11_OBJECT(m_vertexShader);
		DELETE_D3D11_OBJECT(m_geometryShader);
		DELETE_D3D11_OBJECT(m_computeShader);
		DELETE_D3D11_OBJECT(m_hullShader);
		DELETE_D3D11_OBJECT(m_domainShader);
		for (int i = 0; i < m_desc.TextureSlots; i++)
		{
			DELETE_D3D11_OBJECT(m_textures[i]);
		}
		delete m_ParamsData;
	}

	const ShaderDesc& UnifiedShader::GetDesc()
	{
		return m_desc;
	}

	const ShaderSettings& UnifiedShader::GetSettings()
	{
		return m_Settings;
	}

	unsigned short UnifiedShader::GetNumberOfTextureSlots()
	{
		return m_desc.TextureSlots;
	}

	bool UnifiedShader::SetTexture(ID3D11ShaderResourceView* texture, unsigned slot)
	{
		if (slot < m_desc.TextureSlots)
		{
			m_textures[slot] = texture;
			return true;
		}
		return false;
	}

	void UnifiedShader::InitializeBuffer(ID3D11Device* device, const ASL::RuntimeBufferInfo& info, BufferPack& pack, short BufferNum, const vector<int>& ParamIDs)
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
			for (auto ID : ParamIDs)
			{
				for (auto pInfo : m_Settings.ParamsInfo)
				{
					if (pInfo.ID == ID)
						vars.push_back(pair<void*, GenericType>((*m_ParamsData)[pInfo.Name].Get()));
				}
			}
			pack.m_sceneConstantsData = new MemoryMultiplexer;
			pack.m_sceneConstantsData->setInput(vars);
			pack.m_sceneConstants.num = BufferNum;
			desc.ByteWidth = pack.m_sceneConstantsData->size();
			res = device->CreateBuffer(&desc, nullptr, &pack.m_sceneConstants.m_Buffer);
			break;
		}
		case PER_FRAME:
		{
			pack.m_perFrameData = new MemoryMultiplexer;
			pack.m_perFrameData->setInput(vars);
			pack.m_perFrameBuffer.num = BufferNum;
			desc.ByteWidth = pack.m_perFrameData->size();
			res = device->CreateBuffer(&desc, nullptr, &pack.m_perFrameBuffer.m_Buffer);
			break;
		}
		case PER_OBJECT:
		{
			pack.m_perObjectData = new MemoryMultiplexer;
			pack.m_perObjectData->setInput(vars);
			pack.m_perObjectBuffer.num = BufferNum;
			desc.ByteWidth = pack.m_perObjectData->size();
			res = device->CreateBuffer(&desc, nullptr, &pack.m_perObjectBuffer.m_Buffer);
			break;
		}
		default:
			break;
		}
	}

	bool UnifiedShader::UpdateBuffer(ID3D11DeviceContext* context, ID3D11Buffer* buffer, MemoryMultiplexer *data)
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		HRESULT result = context->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		if (FAILED(result))
		{
			return false;
		}
		data->setOutput(mappedResource.pData);
		data->process();
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
		if (!UpdateSceneConstantsBuffers(context))
			return false;
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

		if (m_desc.TextureSlots)
			context->PSSetShaderResources(0, m_desc.TextureSlots, m_textures);
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
		OutputDebugStringW(L"UnifiedShader Destroy\n");
	}

	UnifiedShader::BufferPack::Buffer::~Buffer()
	{
		OutputDebugStringW(L"D3D11Buffer Destroy\n");
		DELETE_D3D11_OBJECT(m_Buffer);
	}

	UnifiedShader::BufferPack::~BufferPack()
	{
		OutputDebugStringW(L"BufferPack Destroy\n");
		delete m_perObjectData;
		delete m_sceneConstantsData;
		delete m_perFrameData;
	}
}