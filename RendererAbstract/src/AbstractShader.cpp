#include "AbstractShader.h"
#include "GenericStruct.h"
#include "GenericVariable.h"
#include "MaterialInterface.h"
#include "ACSReader.h"

#include <ApproxDebug.h>
#include <serialization/RuntimeBufferInfo.h>
#include <serialization/TextureInfo.h>
#include <serialization/ShaderParameterInfo.h>

using namespace MatInterface;
using namespace ASL;
using namespace std;

namespace ShaderSystem
{
	AbstractShader::AbstractShader(const ShaderDesc& desc):
		m_desc(desc),
		VS_buffers(),
		PS_buffers()
	{
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

	AbstractShader::BufferDescription AbstractShader::initialize_buffer_pack(const ASL::RuntimeBufferInfo & info,
                                                                             BufferPack & pack, short buffer_num,
                                                                             const vector<int> & ParamIDs)
	{
		auto vars = g_materialInterfaceManager->GetMaterialVariablesPtr(info.IDs);

        BufferDescription description;

		switch (info.group)
		{
            case SCENE_CONSTANT:
            {
                description.size = pack.scene_constants_buffer.initialize(vars, buffer_num);
                break;
            }
            case PER_FRAME:
            {
                description.size = pack.per_frame_buffer.initialize(vars, buffer_num);
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
                description.size = pack.per_object_buffer.initialize(vars, buffer_num);
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

    void AbstractShader::initialize_buffers(const ShaderElement & elem, AbstractShader::BufferPack & pack) {
        short num = 0;
        vector<int> ParamIDs, TextureSlots;
        void* ptr;
        auto size = elem.Find(SHADER_SETS)->Get(ptr);
        AbstractSaveData::Deserialize(ptr, size, ParamIDs, TextureSlots);
        for (auto buffer : elem.FindMany(RUNTIME_BUFFER_INFO))
        {
            RuntimeBufferInfo info;
            buffer->Get(info);

            initialize_buffer_pack(info, pack, num++, ParamIDs);
        }
    }

    AbstractShader::BufferPack::Buffer::~Buffer()
	{
		APPROX_DEBUG("Buffer Destroy");
		delete memory;
	}

	uint64_t AbstractShader::BufferPack::Buffer::initialize(const std::vector<std::pair<void *, GenericType>> & data,
															short buffer_num) {
		memory = new MemoryMultiplexer;
		memory->set_input(data);
		num = buffer_num;
		return memory->get_size();
	}

	AbstractShader::BufferPack::~BufferPack()
	{
		APPROX_DEBUG("BufferPack Destroy");
	}
}