#pragma once
#include "IShader.h"
#include "ShaderSystemTypes.h"
#include "ShaderDesc.h"
#include "ACSReader.h"

#include <vector>
#include <string>


class ShaderPool;

namespace ShaderSystem
{
	class MemoryMultiplexer;
	class GenericStruct;
	
	class AbstractShader : public IShader
	{
		friend class ShaderPool;
		ShaderDesc m_desc;

		struct BufferPack
		{
			struct Buffer{
				short num = 0;
				~Buffer();
			}m_sceneConstants, m_perFrameBuffer, m_perObjectBuffer;
			MemoryMultiplexer *m_perObjectData = nullptr, *m_perFrameData = nullptr, *m_sceneConstantsData = nullptr;
			~BufferPack();
		}VS_buffers, PS_buffers;

        struct BufferDescription {
            int size = 0;
        };

		GenericStruct *m_ParamsData = nullptr;
		ShaderSettings m_Settings;

		void parse_asc(const string& acs_filename);
	public:
		const GenericStruct& ShaderParams()const override final;
		const ShaderDesc& GetDesc()override final;
		const ShaderSettings& GetSettings()override final;
		unsigned short GetNumberOfTextureSlots()override final;
		virtual ~AbstractShader();

    protected:
		AbstractShader(const std::string& acs_filename, const ShaderDesc& desc);

		virtual void LoadShaderParams();
        virtual AbstractShader::BufferDescription InitializeBuffer(const ASL::RuntimeBufferInfo& info, BufferPack& pack, short BufferNum, const vector<int>& ParamIDs);
        virtual void handle_input_layout(const ShaderElement&) = 0;
        virtual void handle_compiled_shader(const ShaderElement&) = 0;
	};
}