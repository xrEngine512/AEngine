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

    protected:
		ShaderDesc m_desc;

		struct BufferPack
		{
			struct Buffer{
				uint16_t num = 0;
				MemoryMultiplexer * memory = nullptr;
				uint64_t initialize(const std::vector<std::pair<void *, GenericType>> & data, short num);
				~Buffer();
			} scene_constants_buffer, per_frame_buffer, per_object_buffer;
			~BufferPack();
		} VS_buffers, PS_buffers;

        struct BufferDescription {
            uint64_t size = 0;
        };

		GenericStruct * m_ParamsData = nullptr;
		ShaderSettings  m_Settings;

	public:
		const GenericStruct& ShaderParams()const final;
		const ShaderDesc& GetDesc() final;
		const ShaderSettings& GetSettings() final;
		unsigned short GetNumberOfTextureSlots() final;
		~AbstractShader() final;

    protected:
        explicit AbstractShader(const ShaderDesc& desc);

		virtual void LoadShaderParams();
        AbstractShader::BufferDescription initialize_buffer_pack(const ASL::RuntimeBufferInfo & info,
                                                                         BufferPack & pack, short buffer_num,
                                                                         const vector<int> & ParamIDs);
        void initialize_buffers(const ShaderElement& elem, BufferPack& pack);
	};
}