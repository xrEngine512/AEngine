#pragma once

#include "ITexture2D.h"

#include <memory>
#include <vector>

struct ShaderDesc;

namespace ShaderSystem{
	struct ShaderSettings;

	enum ShaderErrors { SHDR_OK, SHDR_WRONG_PARAMS_TYPE, SHDR_PARAMS_IS_NULL };
	class GenericStruct;
	interface IShader
	{
		virtual const GenericStruct& ShaderParams()const = 0;
		virtual bool UpdateSceneConstantsBuffers() = 0;
		virtual bool UpdatePerFrameBuffers() = 0;
		virtual const ShaderDesc& GetDesc() = 0;
		virtual unsigned short GetNumberOfTextureSlots() = 0;
		virtual bool render(uint32_t indexCount, const GenericStruct &) = 0;
		virtual const ShaderSettings& GetSettings() = 0;
		virtual ~IShader(){};
	};

	typedef std::shared_ptr<IShader> IShaderPtr;
	typedef std::weak_ptr<IShader> IShaderRef;
}
