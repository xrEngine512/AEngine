#include "AbstractShader.h"
#include "GenericStruct.h"
#include "GenericVariable.h"
#include "MaterialInterface.h"
#include "ACSReader.h"

#include <ApproxDebug.h>
#include <RuntimeBufferInfo.h>
#include <TextureInfo.h>
#include <ShaderParamInfo.h>

using namespace MatInterface;
using namespace ASL;
using namespace std;

namespace ShaderSystem
{
	AbstractShader::AbstractShader(const string& acs_filename, const ShaderDesc& desc):
		m_desc(desc),
		VS_buffers(),
		PS_buffers()
	{
		parse_asc(acs_filename);
	}

	void AbstractShader::parse_asc(const string& acs_filename) {
		ACSReader reader(acs_filename);

		bool end_of_file(false);

		while (!end_of_file)
		{
			const ShaderElement& current_element = reader.NextElement();

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
				case COMPILED_PS:
				case COMPILED_GS:
				case COMPILED_CS:
				case COMPILED_HS:
				case COMPILED_DS:
					handle_compiled_shader(current_element);
					break;

				case INPUT_LAYOUT_ELEMENT:
					handle_input_layout(current_element);
					break;

				default:
				case ID_NONE:
					end_of_file = true;
					break;
			}
		}

		m_desc.TextureSlots = static_cast<uint8_t>(m_Settings.TexturesInfo.size());
	}

	void AbstractShader::LoadShaderParams()
	{
		if (m_Settings.ParamsInfo.size())
		{
			vector<pair<GenericType, string>> TypesAndNames;
			TypesAndNames.reserve(m_Settings.ParamsInfo.size());
			for (auto info : m_Settings.ParamsInfo)
			{
				TypesAndNames.push_back({MaterialInterfaceManager::NormalizeType(info.Type), info.Name});
			}
			m_ParamsData = new GenericStruct(TypesAndNames);
		}
	}

	const GenericStruct& AbstractShader::ShaderParams() const
	{
		return *m_ParamsData;
	}

	const ShaderDesc& AbstractShader::GetDesc()
	{
		return m_desc;
	}

	const ShaderSettings& AbstractShader::GetSettings()
	{
		return m_Settings;
	}

	unsigned short AbstractShader::GetNumberOfTextureSlots()
	{
		return m_desc.TextureSlots;
	}

	AbstractShader::BufferDescription AbstractShader::InitializeBuffer(const ASL::RuntimeBufferInfo& info, BufferPack& pack, short BufferNum, const vector<int>& ParamIDs)
	{
		auto vars = g_materialInterfaceManager->GetMaterialVariablesPtr(info.IDs);

        BufferDescription description;

		switch (info.group)
		{
            case SCENE_CONSTANT:
            {
                pack.m_sceneConstantsData = new MemoryMultiplexer;
                pack.m_sceneConstantsData->setInput(vars);
                pack.m_sceneConstants.num = BufferNum;
                description.size = pack.m_sceneConstantsData->size();
                break;
            }
            case PER_FRAME:
            {
                pack.m_perFrameData = new MemoryMultiplexer;
                pack.m_perFrameData->setInput(vars);
                pack.m_perFrameBuffer.num = BufferNum;
                description.size = pack.m_perFrameData->size();
                break;
            }
            case PER_OBJECT:
            {
                for (auto ID : ParamIDs)
                {
                    for (auto pInfo : m_Settings.ParamsInfo)
                    {
                        if (pInfo.ID == ID)
                            vars.push_back(pair<void*, GenericType>((*m_ParamsData)[pInfo.Name].Get()));
                    }
                }
                pack.m_perObjectData = new MemoryMultiplexer;
                pack.m_perObjectData->setInput(vars);
                pack.m_perObjectBuffer.num = BufferNum;
                description.size = pack.m_perObjectData->size();
                break;
            }
            default:
                description.size = 0;
                break;
		}
        return description;
	}

	AbstractShader::~AbstractShader()
	{
        delete m_ParamsData;
		APPROX_DEBUG("AbstractShader Destroy")
	}

	AbstractShader::BufferPack::Buffer::~Buffer()
	{
		APPROX_DEBUG("Buffer Destroy")
	}

	AbstractShader::BufferPack::~BufferPack()
	{
		APPROX_DEBUG("BufferPack Destroy");
		delete m_perObjectData;
		delete m_sceneConstantsData;
		delete m_perFrameData;
	}
}